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
// @file os_timer.c
//
// @brief contains the functions for using software timers
//
// @note    in this file you will find functions for using timers and countdowns for your application
//          These software timers are updated by the hardware timer interrupt of the os_scheduler
//          The number of timers need to be defined in main/project_settings.h :
//              #define OS_TIMER_NUMBER_OF_TIMERS 3
//          You should also put your your individual Timer defines with the corresponding index there
//              #define OS_TIMER_COUNTDOWN_EXAMPLE_SWITCHOFFTHELIGHT        0
//              #define OS_TIMER_COUNTDOWN_EXAMPLE_SWITCHOFFCOFFEEMACHINE   1
//              #define OS_TIMER_COUNTDOWN_EXAMPLE_IAMGOINGHOMENOW          2
//
//
// @version v1.0
// @date 2019-08-30
// @author M52409
//
//=======================================================================================================

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "./project_settings.h"

#ifndef OS_TIMER_NUMBER_OF_TIMERS
    #error OS_TIMER_NUMBER_OF_TIMERS needs to be defined in the file project_settings.h
#endif  //OS_TIMER_NUMBER_OF_TIMERS

#if OS_TIMER_NUMBER_OF_TIMERS > 0

typedef struct
{
    uint16_t timerval;
    uint8_t countdown:1;
    uint8_t countup:1;
    uint8_t expired:1;
    uint8_t reserved:5;
} OS_TIMER_DATA_t;

#if OS_TIMER_NUMBER_OF_TIMERS > 0
OS_TIMER_DATA_t os_timer_data[OS_TIMER_NUMBER_OF_TIMERS];
#endif

//=======================================================================================================
// @brief   OS_Timer_Init initializes the Software Timers
// @note    call this function at boot up to initialize the Software Timers
//=======================================================================================================
void OS_Timer_Init(void)
{
#if OS_TIMER_NUMBER_OF_TIMERS > 0
    uint8_t i;

    memset(os_timer_data, 0, sizeof(os_timer_data));
    for (i=0; i < OS_TIMER_NUMBER_OF_TIMERS; i++)
    {
        os_timer_data[i].expired = true;
    }
#endif
}


//=======================================================================================================
// @brief   OS_Timer_StartCountdown_100us/1ms starts a new Countdown
// @note    call this function if you want to use a new countdown
// @param   timer_index - index of the timer that should be used for that countdown
// @param   numOfTicks - depending on the scheduler timing, the number of 100µs or 1ms ticks to count
//=======================================================================================================
#if OS_USE_SCHEDULER_100us == 1
void OS_Timer_StartCountdown_100us(uint8_t timer_index, uint16_t numOfTicks)
#elif OS_USE_SCHEDULER_1ms == 1
void OS_Timer_StartCountdown_1ms(uint8_t timer_index, uint16_t numOfTicks)
#else
#error OS_USE_SCHEDULER_100us or OS_USE_SCHEDULER_1ms needs to be definied as 1 to get the right timer functions
#endif
{
    if (timer_index < OS_TIMER_NUMBER_OF_TIMERS)
    {
        os_timer_data[timer_index].countup = false;
        os_timer_data[timer_index].countdown = true;
        os_timer_data[timer_index].expired = false;
        os_timer_data[timer_index].timerval = numOfTicks;
    }
}

bool OS_Timer_IsCountdownExpired(uint8_t timer_index)
{
    if (timer_index >= OS_TIMER_NUMBER_OF_TIMERS)
        return false;
    if (os_timer_data[timer_index].expired == false)
        return false;
    return true;
}

void OS_Timer_Tick(void)
{
    uint8_t i;
    for (i=0; i < OS_TIMER_NUMBER_OF_TIMERS; i++)
    {
        if (os_timer_data[i].countdown)
        {
            if (os_timer_data[i].timerval != 0)
            {
                if (--os_timer_data[i].timerval == 0)
                    os_timer_data[i].expired = true;
            }
        }
    }
}

#endif