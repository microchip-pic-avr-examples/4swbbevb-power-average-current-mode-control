/**
  @Company
    Microchip Technology Inc.

  @File Name
    project_settings.h

  @Summary
    This is the generated driver implementation file using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file contains some project specific settings regarding the operating system parts and application.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.167.0
        Device            :  dsPIC33CK256MP506      
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70
        MPLAB 	          :  MPLAB X v5.45
*/

/*
    (c) 2021 Microchip Technology Inc. and its subsidiaries. You may use this
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

#ifndef _PROJECT_SETTINGS_H_
#define	_PROJECT_SETTINGS_H_

//=======================================================================================================
//sometimes the software runs on different hardware versions, so we have defines for different board settings

#define PROJECT_SETTINGS_BOARD_DPPIM_CK 1       // dsPic33CK single core on DPPIM module

#define PROJECT_SETTINGS_BOARD_EXPANDER_CK 0    // BOARD_EXPANDER_CK single core dsPic33CK for testing basic stuff

//=======================================================================================================
//  OS Settings for the OS_Scheduler
//
//  Decide which timing precision you need for your project and choose the right scheduler
//-------------------------------------------------------------------------------------------------------
#define OS_USE_SCHEDULER_100us 1
#define OS_USE_SCHEDULER_1ms   0
//=======================================================================================================

#define FCY     100000000ULL  // 100 MHz - Define instruction rate for delay routine

//=======================================================================================================
//  OS Settings for the OS_Scheduler
//
//  Decide if you want to use OS_Systime in your project
//-------------------------------------------------------------------------------------------------------
    #define OS_USE_SYSTIME 0
//=======================================================================================================

//=======================================================================================================
//  OS Settings for OS_Timer
//-------------------------------------------------------------------------------------------------------
    #define OS_TIMER_NUMBER_OF_TIMERS       0
//-------------------------------------------------------------------------------------------------------
//  Application specific Timers:
    #define OS_TIMER_COUNTDOWN_COILSWITCH   0
//=======================================================================================================

//=======================================================================================================
// USE MCC driver layer if available
//-------------------------------------------------------------------------------------------------------
#define OS_USE_MCC_TIMER1 1
#define OS_USE_MCC_WATCHDOG 0
#define OS_USE_MCC_RESET 0
//=======================================================================================================

#endif	//_PROJECT_SETTINGS_H_

