/**
  @Company
    Microchip Technology Inc.

  @File Name
    Controller_pwrctrl_4SWBB.h

  @Summary
    This is the generated driver implementation file using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides Controller settings for average current mode and voltage mode controller.
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


#ifndef _CONTROLLER_PWRCTRL_4SWBB_H
#define	_CONTROLLER_PWRCTRL_4SWBB_H

#include <xc.h> 
#include "smps_control.h"   //interfaces toSwitch Mode Power Supply compensator library.
#include "dcdt/avgcm/dcdt_generated_code/avgcm_dcdt.h"   //coefficients for average current controller
#include "dcdt/vmc/dcdt_generated_code/vmc_dcdt.h"       //coefficients for voltage controller

//======================================================================================================================
// @brief extern declarations
//======================================================================================================================

extern SMPS_2P2Z_T AVG_CM2p2z, VMC2p2z;

//======================================================================================================================
// @brief structure of variables for controller (compensator)
//======================================================================================================================

typedef volatile struct
{
    uint16_t _VDCref;
    uint16_t _IACref;
    uint16_t _DCBuckset;
    uint16_t _DCBoostset;
    uint16_t _CurrentControllerResult;
    uint16_t _VoltageControllerResult;    
} t_Controller;
extern t_Controller Controller_4SWBB;

//======================================================================================================================
// @brief function prototypes
//======================================================================================================================

void CurrentController_PwrCtrl_4SWBB_Init(void);
void VoltageController_PwrCtrl_4SWBB_Init(void);

#endif	/* XC_HEADER_TEMPLATE_H */

