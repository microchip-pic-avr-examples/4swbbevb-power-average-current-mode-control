//=======================================================================================================
// Copyright(c) 2019 Microchip Technology Inc. and its subsidiaries.
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
// @file os_sys_time.h
//
// @brief contains the function for the system time
//
// @note    counting up the time can/should be done in the interrupt routine of the scheduler
//          reading the time can also be done outside of the interrupt without any conflicts
//
// @author M52409
//
// @date 2019-08-09
//=======================================================================================================

#ifndef _OS_SYS_TIME_H_
#define	_OS_SYS_TIME_H_

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */


typedef struct
{
    uint16_t    millisecond;
    uint8_t     second;
    uint8_t     minute;
    uint8_t     hour;
} OS_SYSTIME_t;


//=======================================================================================================
// @brief   call this function every millisecond from the interrupt in your scheduler
//=======================================================================================================
void OS_SysTime_IncrementTime_1ms(void);

//=======================================================================================================
// @brief   function to reset the time to zero
// @ note   use this function at boot up time or in the init function of your scheduler
//=======================================================================================================
void OS_SysTime_ResetTime(void);

//=======================================================================================================
// @brief   function to reset the time to zero
// @ note   use this function at boot up time or in the init function of your scheduler
//=======================================================================================================
void OS_SysTime_GetTime(OS_SYSTIME_t* retVal);






#ifdef	__cplusplus
}
#endif  // __cplusplus
#endif	// _OS_SYS_TIME_H_

