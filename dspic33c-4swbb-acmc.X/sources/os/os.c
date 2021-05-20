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
// @file os/os.c
//
// @brief contains the init function of the operating system and calls all the other Init functions
//
// @version v1.0
// @date 2019-23-12
// @author M52409
//
//=======================================================================================================


#include "../../mcc_generated_files/system.h"
#include "../../mcc_generated_files/watchdog.h"
#include "project_settings.h"

#include "os_scheduler.h"
#include "stdint.h"

#ifndef OS_TIMER_NUMBER_OF_TIMERS
#warning OS_TIMER_NUMBER_OF_TIMERS needs to be defined in main/project_setting.h
#endif
#ifndef OS_USE_SYSTIME
#warning OS_USE_SYSTIME needs to be defined in main/project_setting.h
#endif

#if OS_TIMER_NUMBER_OF_TIMERS > 0
#include"os/os_timer.h"
#endif

#define OS_RESETCAUSE_WATCHDOG	1
uint8_t os_resetCause = 0;

//=======================================================================================================
//  @brief  Initializes Scheduler
//  @note   call this function in your main routine before calling the RunForever function
//=======================================================================================================
void OS_Init(void)
{
#if OS_USE_MCC_WATCHDOG == 1
	if (RCONbits.WDTO)
		os_resetCause |= 1 << OS_RESETCAUSE_WATCHDOG;
	RCON = 0;                                         //FTX all Resets are cleared !!!
	WATCHDOG_TimerSoftwareEnable();
#endif
    OS_Scheduler_Init();
#if OS_TIMER_NUMBER_OF_TIMERS > 0
	OS_Timer_Init();
#endif
}

