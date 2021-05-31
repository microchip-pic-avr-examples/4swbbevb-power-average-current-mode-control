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

#include "Controller_pwrctrl_4SWBB.h"

SMPS_2P2Z_T AVG_CM2p2z, VMC2p2z;
t_Controller Controller_4SWBB;

//======================================================================================================================
//@brief SMPS_2P2Z_T AVG_CM2p2z //typedef from smps_control.h to var
//======================================================================================================================

int16_t AVG_CM2p2zErrorHistory[3] __attribute__((space(ymemory), far));
int16_t AVG_CM2p2zACoefficients[2]__attribute__((space(xmemory)));
int16_t AVG_CM2p2zControlHistory[2] __attribute__((space(ymemory), far));
int16_t AVG_CM2p2zBCoefficients[3] __attribute__ ((space(xmemory)));
uint16_t AVG_CM2p2zReferenceSet;

//======================================================================================================================
//@brief SMPS_2P2Z_T VMC2p2z //typedef from smps_control.h to var
//======================================================================================================================

int16_t VMC2p2zErrorHistory[3] __attribute__((space(ymemory), far));
int16_t VMC2p2zACoefficients[2]__attribute__((space(xmemory)));
int16_t VMC2p2zControlHistory[2] __attribute__((space(ymemory), far));
int16_t VMC2p2zBCoefficients[3] __attribute__ ((space(xmemory)));
uint16_t VMC2p2zReferenceSet;

//======================================================================================================================
//@brief current controller initialization with value calculated by DCDT and SMPS library
//======================================================================================================================

void CurrentController_PwrCtrl_4SWBB_Init(void)
{
  AVG_CM2p2z.aCoefficients = &AVG_CM2p2zACoefficients[0]; // Set up pointer to derived coefficients
  AVG_CM2p2z.bCoefficients = &AVG_CM2p2zBCoefficients[0]; // Set up pointer to derived coefficients
  AVG_CM2p2z.controlHistory = &AVG_CM2p2zControlHistory[0]; // Set up pointer to controller history
  AVG_CM2p2z.errorHistory = &AVG_CM2p2zErrorHistory[0]; // Set up pointer to error history

  AVG_CM2p2z.preShift = AVGCM_COMP_2P2Z_PRESHIFT; // Normalization shift for error amplifier results
  AVG_CM2p2z.postScaler = AVGCM_COMP_2P2Z_POSTSCALER;
  AVG_CM2p2z.postShift = AVGCM_COMP_2P2Z_POSTSHIFT; // Normalization shift for control loop results to peripheral
  AVG_CM2p2z.minOutput = AVGCM_COMP_2P2Z_MIN_CLAMP; //;PG1DCmin;
  AVG_CM2p2z.maxOutput = AVGCM_COMP_2P2Z_MAX_CLAMP; //PG1DCmax; 0;//FTX FTX
  AVG_CM2p2zReferenceSet = 0;

  SMPS_Controller2P2ZInitialize(&AVG_CM2p2z); // Clear histories

  //Set coefficients
  AVG_CM2p2zACoefficients[0] = AVGCM_COMP_2P2Z_COEFF_A1;
  AVG_CM2p2zACoefficients[1] = AVGCM_COMP_2P2Z_COEFF_A2;

  AVG_CM2p2zBCoefficients[0] = AVGCM_COMP_2P2Z_COEFF_B0;
  AVG_CM2p2zBCoefficients[1] = AVGCM_COMP_2P2Z_COEFF_B1;
  AVG_CM2p2zBCoefficients[2] = AVGCM_COMP_2P2Z_COEFF_B2;
}

//======================================================================================================================
//@brief outer voltage controller initialization with value calculated by DCDT and SMPS library
//======================================================================================================================

void VoltageController_PwrCtrl_4SWBB_Init(void)
{
  VMC2p2z.aCoefficients = &VMC2p2zACoefficients[0]; // Set up pointer to derived coefficientsG_CM2p2zACoefficients[0]; // Set up pointer to derived coefficients
  VMC2p2z.bCoefficients = &VMC2p2zBCoefficients[0]; // Set up pointer to derived coefficients
  VMC2p2z.controlHistory = &VMC2p2zControlHistory[0]; // Set up pointer to controller history
  VMC2p2z.errorHistory = &VMC2p2zErrorHistory[0]; // Set up pointer to error history

  VMC2p2z.preShift = VMC_COMP_2P2Z_PRESHIFT; // Normalization shift for error amplifier results
  VMC2p2z.postScaler = VMC_COMP_2P2Z_POSTSCALER;
  VMC2p2z.postShift = VMC_COMP_2P2Z_POSTSHIFT; // Normalization shift for control loop results to peripheral
  VMC2p2z.minOutput = VMC_COMP_2P2Z_MIN_CLAMP; //PG1DCmin;
  VMC2p2z.maxOutput = VMC_COMP_2P2Z_MAX_CLAMP; //PG1DCmax;

  VMC2p2zReferenceSet = 0;

  SMPS_Controller2P2ZInitialize(&VMC2p2z); // Clear histories

  //Set coefficients
  VMC2p2zACoefficients[0] = VMC_COMP_2P2Z_COEFF_A1;
  VMC2p2zACoefficients[1] = VMC_COMP_2P2Z_COEFF_A2;

  VMC2p2zBCoefficients[0] = VMC_COMP_2P2Z_COEFF_B0;
  VMC2p2zBCoefficients[1] = VMC_COMP_2P2Z_COEFF_B1;
  VMC2p2zBCoefficients[2] = VMC_COMP_2P2Z_COEFF_B2;
}
//======================================================================================================================
//======================================================================================================================
