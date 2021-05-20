//=======================================================================================================
// Copyright(c) 2018 Microchip Technology Inc. and its subsidiaries.
// Subject to your compliance with these terms, you may use Microchip software and any derivatives
// exclusively with Microchip products. It is your responsibility to comply with third party license
// terms applicable to your use of third-party software (including open source software) that may
// accompany Microchip software.
// THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY,
// APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND
// FITNESS FOR A PARTICULAR PURPOSE.
// IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
// LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF
// MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT
// ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT
// EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
//=======================================================================================================

//=======================================================================================================
// @file os/os_scheduler_1ms.c
//
// @brief contains the main scheduler that calls all the tasks that need to be called regularly
//        two different timings priorities are available:
//          1. 1ms Tasks called from the scheduler interrupt
//              the jitter that you will have in the 1ms realtime tasks called by the interrupt depends
//              on other interrupts that have a higher interrupt priority
//          2. 1ms, 10ms, 100ms, 1s Tasks called from the main loop
//              these tasks are for soft realtime and not for hard realtime
//              so in average they are called with the required timing but the jitter can be very huge,
//              depending on the calls before.
//              use this for your non-timing critical application state machines
//
// @note    put your application specific code in main/main_tasks.c
// @note    you might consider to implement a watchdog and put the watchdog triggering into some Task
//
// @version v1.0
// @date 2019-08-09
// @author M52409
//
//=======================================================================================================

#include <stdint.h>
#include <xc.h>
#include "project_settings.h"
#if OS_FEATURE_WATCHDOG_ENABLED == 1
#include "os_watchdog.h"
#endif

#if OS_USE_SCHEDULER_1ms == 1

#ifndef OS_TIMER_NUMBER_OF_TIMERS
#warning OS_TIMER_NUMBER_OF_TIMERS needs to be defined in main/project_setting.h
#endif
#ifndef OS_USE_SYSTIME
#warning OS_USE_SYSTIME needs to be defined in main/project_setting.h
#endif


//=======================================================================================================
//  The following extern Tasks_... functions should be placed in the file main/main_tasks.c
//  call OS_Scheduler_Run() in the main loop or call OS_Scheduler_RunForever() in main.c
//=======================================================================================================
extern void Tasks_Realtime_1ms(void);
extern void Tasks_1ms(void);
extern void Tasks_10ms(void);
extern void Tasks_100ms(void);
extern void Tasks_1s(void);
extern void Tasks_Background(void);

//=======================================================================================================
//
//          put your application specific code in the following functions in main/main_tasks.c
//                          choose wisely between real-time and non-realtime!
//
//  Interrupt Realtime Functions:
//  Tasks_Realtime_1ms  :   is called by the interrupt every ms - for time critical low jitter stuff
//
//
//  Mainloop Non-Realtime Functions:
//  Tasks_1ms           :   function is called by the main loop in average every 1ms
//  Tasks_10ms          :   function is called by the main loop in average every 10ms
//  Tasks_100ms         :   function is called by the main loop in average every 100ms
//  Tasks_1s            :   function is called by the main loop in average every second
//
//  @note there could be some jitter here because it is not called directly by a timer interrupt
//        the timing in average is exact (keep in mind: in average), the jitter depends on the
//        called functions before
//=======================================================================================================

#if OS_TIMER_NUMBER_OF_TIMERS > 0
    void OS_Timer_Tick(void);
#endif

static volatile uint16_t    scheduler_interrupt_leader_1ms = 0;
static volatile uint16_t    scheduler_interrupt_follower_1ms = 0;
volatile static uint16_t    scheduler_1ms_timer = 0;       // local counter for 1ms tasks
volatile static uint16_t    scheduler_10ms_timer = 0;      // local counter for 10ms tasks
volatile static uint16_t    scheduler_100ms_timer = 0;     // local counter for 100ms tasks
volatile static uint16_t    scheduler_1s_timer = 0;        // local counter for 1s tasks

//                  Timer 1 bit field settings
#define T1ON        1       // 1 = En, 0 = disabled
#define T1TCS       0       // 1 = External Clock, 0 = Peripheral Clock
#define T1TCKPS     1       // 3 = 1:256, 2 = 1:64, 1 = 1:8, 0 = 1:1

#if OS_USE_MCC_TIMER1 == 0
static inline void OS_Scheduler_Init_Timer1(void)
{
    //Switch off Timer 1
    T1CONbits.TON = 0;      // Disable Timer1
    IEC0bits.T1IE = 0;      // Disable Timer1 interrupt

    //Configure Timer 1
    T1CONbits.TSIDL = 0;    // Timer1 Stop in Idle Mode: Continues module operation in Idle mode
    T1CONbits.TMWDIS = 0;   // Asynchronous Timer1 Write Disable: Back-to-back writes are enabled in Asynchronous mode
    T1CONbits.TMWIP = 0;    // Asynchronous Timer1 Write in Progress: Write to the timer in Asynchronous mode is complete
    T1CONbits.PRWIP = 0;    // Asynchronous Period Write in Progress: Write to the Period register in Asynchronous mode is complete
    //T1CONbits.TECS = 0b11;  // Timer1 Extended Clock Select: FRC clock
    T1CONbits.TECS = 0b01;  //FCY
    //T1CONbits.TGATE = 0;    // Timer1 Gated Time Accumulation Enable: Gated time accumulation is disabled when TCS = 0
    //T1CONbits.TCKPS = 0;    // Timer1 Input Clock Prescale Select: 1:1
    T1CONbits.TCKPS = 0b01; // Timer1 Input Clock Prescale Select: 1:8
    T1CONbits.TSYNC = 0;    // Timer1 External Clock Input Synchronization Select: Does not synchronize the External Clock input
    //T1CONbits.TCS = 0;      // Timer1 Clock Source Select: Internal peripheral clock
    T1CONbits.TCS = 1;      // Timer1 Clock Source Select: based on TECS value

    TMR1 = 0x00;            // Reset Timer Counter Register TMR to Zero; 
    PR1 = 9999;             // Period = 0.0001 s; Frequency = 100000000 Hz; PR 9999

    //TODO: these two lines are from the qi initialization: check/synchronize with the above init
    // Initialize Timer 1 - Period: 1 ms
    PR1 = (FCY / 1000 / 8);
    T1CON = ((T1ON << 15) | (T1TCKPS << 4) | (T1TCS));

    // in the master/slave version the slave had priority 4
    IPC0bits.T1IP = 1;      // Set interrupt priority to one (cpu is running on ip zero)
    IFS0bits.T1IF = 0;      // Reset interrupt flag bit

    //Switch on Timer 1
    IEC0bits.T1IE = 1;      // Enable Timer1 interrupt
    T1CONbits.TON = 1;      // Enable Timer1
}
#endif

//=======================================================================================================
//  @brief  Initializes Scheduler
//  @note   call this function in your main routine before calling the RunForever function
//=======================================================================================================
void OS_Scheduler_Init(void)
{
#if OS_USE_MCC_TIMER1 == 0
    OS_Scheduler_Init_Timer1();
#endif

#if OS_USE_SYSTIME == 1
    OS_SysTime_ResetTime();
#endif  //OS_USE_SYSTIME
    scheduler_interrupt_leader_1ms = 0;   // reset directly before calling the Scheduler Loop
    scheduler_interrupt_follower_1ms = 0; // reset directly before calling the Scheduler Loop
}

//=======================================================================================================
//  @brief  1ms Timer 1 interrupt routine for generating the timing for the scheduler
//  @note   with this simple implementation we do not lose any tick from the timer, even when the tasks
//          in the main loop take longer than 1ms
//=======================================================================================================
#if OS_USE_MCC_TIMER1 == 1
void TMR1_CallBack(void)
#else
void __attribute__((__interrupt__,no_auto_psv)) _T1Interrupt(void)
#endif
{
    scheduler_interrupt_leader_1ms++;   //increment our counter for the scheduler, no tick gets lost
    _T1IF = 0;                          //clear Timer1 interrupt flag
#if OS_TIMER_NUMBER_OF_TIMERS > 0
    OS_Timer_Tick();
#endif
#if OS_USE_SYSTIME == 1
    OS_SysTime_IncrementTime_1ms();
#endif  //OS_USE_SYSTIME
    Tasks_Realtime_1ms();
}

//=======================================================================================================
//  @brief  Scheduler function for calling all the Tasks regularly ( 1ms, 10ms, 100ms, 1s )
//  @note   call this function in your main loop in main.c after calling the Init-function
//          if you have nothing else to call in your main loop then you can call the function
//          Main_Scheduler_RunForever() instead.
//          please consider that the timing of the calls are dependent on the duration of the last call
//          the resulting jitter therefore depends on the timing of the calls before
//=======================================================================================================
void OS_Scheduler_RunOnce(void)
{
    if (scheduler_interrupt_follower_1ms != scheduler_interrupt_leader_1ms)
    {
        scheduler_interrupt_follower_1ms++;
        Tasks_1ms();                            //call 1 ms tasks
        if (++scheduler_10ms_timer >= 10)
        {
            scheduler_10ms_timer = 0;           //reset 10 ms timer
            Tasks_10ms();                       //call 10 ms tasks
            if (++scheduler_100ms_timer >= 10)
            {
                scheduler_100ms_timer = 0;      //reset 100 ms timer
                Tasks_100ms();                  //call 100 ms tasks
                if (++scheduler_1s_timer >= 10)
                {
					scheduler_1s_timer = 0;
					#if OS_FEATURE_WATCHDOG_ENABLED == 1
						OS_Watchdog_KeepAlivePing();
					#endif
                    Tasks_1s();                  //call 1 s tasks
                }                            
            }
        }
    }
    else
    {
        Tasks_Background();                         // run the background tasks with their own timing
    }
}

//=======================================================================================================
//  @brief  Scheduler function for calling all the Tasks regularly ( 1ms, 10ms, 100ms, 1s )
//  @note   call this function as last function in main.c after calling the Init-function
//          please consider that the timing of the calls are dependent on the duration of the last call
//          the resulting jitter therefore depends on the timing of the calls before
//=======================================================================================================
void OS_Scheduler_RunForever(void)
{
    // do some initialization
    scheduler_interrupt_leader_1ms = 0;   // reset directly before calling the Scheduler Loop
    scheduler_interrupt_follower_1ms = 0; // reset directly before calling the Scheduler Loop

    while (1)                   // run that loop forever
        OS_Scheduler_RunOnce();
}

#endif  //OS_USE_SCHEDULER_1ms
