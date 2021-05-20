/**
  @Company
    Microchip Technology Inc.

  @File Name
    main_tasks.c

  @Summary
    This is the generated driver implementation file using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
 
    This source file contains the application specific tasks that are called regularly by the os_scheduler
        two different timings priorities are available:
          1. 1ms Tasks called from the os_scheduler interrupt
              the jitter that you will have in the 1ms realtime tasks called by the interrupt depends
              on other interrupts that have a higher interrupt priority
          2. 1ms, 10ms, 100ms, 1s Tasks called from the main loop
              these tasks are for soft realtime and not for hard realtime
              so in average they are called with the required timing but the jitter can be very huge,
              depending on the calls before.
              use this for your non-timing critical application state machines

 @note    you might consider to implement a watchdog and put the watchdog triggering into some Task
 
 
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.167.0
        Device            :  dsPIC33CK256MP506      
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70
        MPLAB 	          :  MPLAB X v5.45
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include <stdint.h>
#include <xc.h>
#include "project_settings.h"

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "device/dev_gui_comm.h"
#include "device/dev_button.h"
#include "app/app_HMI.h"
#include "app/app_GUI.h"
#include "driver/drv_led.h"

#include "driver/drv_adc.h"
#include "driver/power_controller/drv_pwrctrl_4SWBB.h"
#include "driver/power_controller/drv_pwrctrl_4SWBB_fault.h"
#include "driver/power_controller/drv_pwrctrl_4SWBB_internal.h"
#include "driver/power_controller/Controller_pwrctrl_4SWBB.h"

extern uint16_t os_resetCause;  //@sw??

//=======================================================================================================
//
//                          put your application specific code in the following functions:
//                              choose wisely between real-time and non-realtime!
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


//=======================================================================================================
//  @brief  Tasks_Realtime_100us gets called directly from the timer interrupt every 100 µs
//  @note   keep this routine as short as possible
//=======================================================================================================

void Tasks_Realtime_100us(void)
{
    // put your application specific code here that needs to be called every 100 micro seconds from the interrupt
    // example: Drv_TestPin_Toggle(DBG_PIN1);
    Drv_PwrCtrl_4SWBB_Task_100us();
}

//=======================================================================================================
//  @brief  Tasks_Realtime_1ms gets called directly from the timer interrupt every millisecond
//  @note   keep this routine as short as possible
//=======================================================================================================
void Tasks_Realtime_1ms(void)
{
    //Scope record function
}

//=======================================================================================================
//  @brief  Tasks_100us gets called every 100µs, put your things in it that need to be called regularly
//  @note   there could be some jitter here because it is not called directly by a timer interrupt
//=======================================================================================================
void Tasks_100us(void)
{
    Dev_GuiComm_Task_100us();
}

//=======================================================================================================
//  @brief  Tasks_1ms gets called every millisecond, put your things in it that need to be called regularly
//  @note   there could be some jitter here because it is not called directly by a timer interrupt
//=======================================================================================================
void Tasks_1ms(void)
{
    App_GUI_Task_1ms();
}

//=======================================================================================================
//  @brief  Tasks_10ms gets called every 10ms, put your things in it that need to be called regularly
//  @note   there could be some jitter here because it is not called directly by a timer interrupt
//=======================================================================================================
void Tasks_10ms(void)
{
  // put your application specific code here that needs to be called every 10 milliseconds
  Dev_Button_Task_10ms();
}

//=======================================================================================================
//  @brief  Tasks_100ms gets called every 100 ms, put your things in it that need to be called regularly
//  @note   there could be some jitter here because it is not called directly by a timer interrupt
//=======================================================================================================

void Tasks_100ms(void)
{
  // put your application specific code here that needs to be called every 100 milliseconds
  App_HMI_Task_100ms();
  Drv_LED_Task_100ms();
}

//=======================================================================================================
//  @brief  Tasks_1s gets called every second, put your things in it that need to be called regularly
//  @note   there could be some jitter here because it is not called directly by a timer interrupt
//=======================================================================================================

void Tasks_1s(void)
{
    // put your application specific code here that needs to be called every second
//    Dev_GuiComm_Scope_TriggerNow();
}

//=======================================================================================================
//  @brief  Tasks_Background gets called all the time when no other of the above tasks are being called
//  @note   call this function when you want to implement your own timing or get code called as often
//          as possible. You can also put your timing variables into Tasks_Realtime_100us or
//          Tasks_Realtime_1ms. This way you get accurate timing variables that you can use here.
//=======================================================================================================
void Tasks_Background(void)
{
    // put your application specific code here that needs to be called in the background.
    // your application needs to take care of it's timing.
}

