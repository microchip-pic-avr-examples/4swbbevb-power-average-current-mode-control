/**
  @Company
    Microchip Technology Inc.

  @File Name
    drv_adc.c

  @Summary
    adc driver

  @Description
    This source file provides Interrupt Service Routine for ADC Core 0 with all analog channel handling.
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

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "drv_adc.h"
#include "power_controller/drv_pwrctrl_4SWBB.h"
#include "power_controller/drv_pwrctrl_4SWBB_internal.h"

#define ADC_CHANNEL_AN06_VAUX		6
#define ADC_CHANNEL_AN10_VDD		10
#define ADC_CHANNEL_AN15_TESTREF	15
#define ADC_CHANNEL_AN17_IOUT		17
#define ADC_CHANNEL_AN19_IOUT		19

PWR_CTRL_DATA_t pwr_ctrl_adc_data;

//======================================================================================================================
// @brief ADC Core 0 ISR for analog ADC values; Overwrite the weak ISR function in MMC generated file adc1.c
// ISR is called every second PWM cycle. Trigger prescaler is set in PWM configuration
// Every second cyle is choosen to get enough time for other debuging tasks
//======================================================================================================================

void __attribute__((__interrupt__, auto_psv)) _ADCAN0Interrupt(void)
{
  
//  TP35_TX_SetHigh();
  
  pwr_ctrl_adc_data.drv_adc_val_FB_Iin =  ADCBUF0;
  pwr_ctrl_adc_data.drv_adc_val_FB_Vout = ADCBUF1;

  Drv_PwrCtrl_4SWBB_CtrlLoop(); 

  switch (ADCON3Lbits.CNVCHSEL)
  {
    case ADC_CHANNEL_AN15_TESTREF: //Read the ADC value from the ADCBUF15
      if (ADSTATLbits.AN15RDY)
      {
        pwr_ctrl_adc_data.drv_adc_val_AN15 = ADCBUF15;
        ADCON3Lbits.CNVCHSEL = ADC_CHANNEL_AN17_IOUT;
      }
      break;

    case ADC_CHANNEL_AN17_IOUT: //Read the ADC value from the ADCBUF19
      if (ADSTATHbits.AN17RDY)
      {
        pwr_ctrl_adc_data.drv_adc_val_FB_Iout = ADCBUF17;
        ADCON3Lbits.CNVCHSEL = ADC_CHANNEL_AN10_VDD;
      }
      break;

    case ADC_CHANNEL_AN10_VDD: //Read the ADC value from the ADCBUF10
      if (ADSTATLbits.AN10RDY)
      {
        pwr_ctrl_adc_data.drv_adc_val_FB_Vdd = ADCBUF10;
        ADCON3Lbits.CNVCHSEL = ADC_CHANNEL_AN06_VAUX;
      }
      break;

    case ADC_CHANNEL_AN06_VAUX: //Read the ADC value from the ADCBUF6
      if (ADSTATLbits.AN6RDY)
      {
        pwr_ctrl_adc_data.drv_adc_val_FB_Vaux = ADCBUF6;
        ADCON3Lbits.CNVCHSEL = ADC_CHANNEL_AN15_TESTREF;
      }
      break;

    default:
      ADCON3Lbits.CNVCHSEL = ADC_CHANNEL_AN15_TESTREF;
      break;
  }

  IFS5bits.ADCAN0IF = 0; //clear the channel_ANA0 interrupt flag
  
//  TP35_TX_SetLow();
}

//======================================================================================================================
//======================================================================================================================

