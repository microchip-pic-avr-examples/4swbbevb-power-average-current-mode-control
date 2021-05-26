/**
  @Company
    Microchip Technology Inc.

  @File Name
    drv_adc.h

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

#ifndef _DRV_ADC_H_
#define	_DRV_ADC_H_

//======================================================================================================================
// @brief structure for variables for analog ADC values 
//======================================================================================================================

typedef struct
{
    volatile uint16_t drv_adc_val_FB_Vdd;
    volatile uint16_t drv_adc_val_FB_Vout;
    volatile uint16_t drv_adc_val_FB_Iin; 
    volatile uint16_t drv_adc_val_FB_Iout;
    volatile uint16_t drv_adc_val_AN15;   
    volatile uint16_t drv_adc_val_FB_Vaux;
}PWR_CTRL_DATA_t;                       // power control soft-start settings and variables
extern PWR_CTRL_DATA_t pwr_ctrl_adc_data;


#endif
