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
// @file os.h
//
// @brief includes all the other header files that are necessary for the project
//
// @note adjust the options of the operating system according your needs in main/project_settings.h
// @note call the Init function in your main.c
// @note call the OS_Scheduler_Run in your main loop or call the RunForever-Function in your main.c
//
// @version v1.0
// @date 2019-08-02
// @author M52409
//
//=======================================================================================================
#ifndef _OS_H_
#define _OS_H_

#include "project_settings.h"
#include "os_scheduler.h"



#ifdef __cplusplus  // Provide C++ Compatibility
    extern "C" {
#endif

        
//=======================================================================================================
//  @brief  Initializes all Operating System Components
//  @note   call this function in your main routine before calling the RunForever function
//=======================================================================================================
extern void OS_Init(void);


#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif
#endif              // _OS_H_
