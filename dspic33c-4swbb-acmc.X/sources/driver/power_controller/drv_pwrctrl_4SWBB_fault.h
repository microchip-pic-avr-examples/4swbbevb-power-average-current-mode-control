/**
  @Company
    Microchip Technology Inc.

  @File Name
    drv_pwrctrl_4SWBB_fault.h

  @Summary
    This is the generated driver implementation file using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides the 4SWBB dedicated fault handling based on generic fault function in fault_common.
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

#ifndef DRV_FLT_HANDLER_H
#define	DRV_FLT_HANDLER_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <stdbool.h>

//======================================================================================================================
// @brief structure of variables for fault handling
//======================================================================================================================

typedef union
{
	struct
	{
		uint16_t inputOverVoltage       : 1;
		uint16_t inputUnderVoltage      : 1;
		uint16_t inputVoltageCMP        : 1;
        
		uint16_t outputOverVoltage      : 1;
		uint16_t outputVoltageCMP       : 1;
		
        uint16_t inputOverCurrent       : 1;
        uint16_t inputCurrentCMP        : 1;
        
        uint16_t outputOverCurrent      : 1;
        
        uint16_t auxVoltageOor          : 1;
        
		uint16_t                        : 7;
	};
	uint16_t CollectiveFaults;
} PWR_CTRL_FAULTS;
extern PWR_CTRL_FAULTS FourSWBBFaults;		//TODO: should not be exported this way

//======================================================================================================================
// @brief function prototypes
//======================================================================================================================

PWR_CTRL_FAULTS Drv_PwrCtrl_4SWBB_GetFaults(void);
void Drv_PwrCtrl_4SWBB_Init_Fault(void);
void Drv_PwrCtrl_4SWBB_Fault_Check(void);
void Drv_PwrCtrl_4SWBB_SetFaultCounters(void);

//------------------------------------------------------------------------------

#endif	/* TASK_FAULT_HANDLER_H */

