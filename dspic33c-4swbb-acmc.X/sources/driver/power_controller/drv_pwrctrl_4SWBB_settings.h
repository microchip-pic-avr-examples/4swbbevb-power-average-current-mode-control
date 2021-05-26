/**
  @Company
    Microchip Technology Inc.

  @File Name
    drv_pwrctrl_4SWBB_settings.h

  @Summary
    This is the generated driver implementation file using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides all value definitions for fault values other voltage and current definitions.
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

//======================================================================================================================
// @brief Defines for thresholds and counters for faults
//======================================================================================================================

//Vin Under Voltage ADC
#define VIN_UV_FLOAT        (float)((4096/26.4)*7)   //7V
#define VIN_UV_THRESHOLD     (uint16_t) VIN_UV_FLOAT     

#define VIN_UV_HYS_FLOAT 		(float)((4096/8/3.3)*7.5)  //7.5V
#define VIN_UV_HYSLIMIT      (uint16_t) VIN_UV_HYS_FLOAT 

#define VIN_UV_THRESHOLD_CNT  30
#define VIN_UV_HYSCNT        10000

//Vin Over Voltage ADC
#define VIN_OV_FLOAT        (float)((4096/26.4)*18)   //18V
#define VIN_OV_THRESHOLD     (uint16_t) VIN_OV_FLOAT     

#define VIN_OV_HYS_FLOAT 		 (float)((4096/26.4)*17)  //17V
#define VIN_OV_HYSLIMIT      (uint16_t) VIN_OV_HYS_FLOAT 

#define VIN_OV_THRESHOLD_CNT  30
#define VIN_OV_HYS_CNT        10000

//Vout Over Voltage ADC
#define VOUT_OV_FLOAT        (float)((4096/26.4)*22)   //22V
#define VOUT_OV_THRESHOLD     (uint16_t) VOUT_OV_FLOAT   //  

#define VOUT_OV_HYS_FLOAT 		(float)((4096/8/3.3)*5)   //5V
#define VOUT_OV_HYS_LIMIT      (uint16_t) VOUT_OV_HYS_FLOAT //

#define VOUT_OV_THRESHOLD_CNT  30
#define VOUT_OV_HYS_CNT        10000

//Vout Over Voltage CMP
#define VOUT_OV_CMP_FLOAT        (float)((4096/26.4)*25)   //25V
#define VOUT_OV_CMP_THRESHOLD    (uint16_t) VOUT_OV_CMP_FLOAT   // 

#define VOUT_OV_CMP_HYS_FLOAT    (float)((4096/8/3.3)*5)   //5V
#define VOUT_OV_CMP_HYS_LIMIT    (uint16_t) VOUT_OV_CMP_HYS_FLOAT //

#define VoutOVCMPThresholdCnt   30
#define VoutOVCMPHysCnt         10000

//Vaux out of range
#define VAUX_OOR1_FLOAT          (float)((4096/2/3.3)*5)   //5V
#define VAUX_OOR_THRESHOLD1      (uint16_t) VAUX_OOR1_FLOAT 
#define VAUX_OOR2_FLOAT          (float)((4096/2/3.3)*6)   //6V
#define VAUX_OOR_THRESHOLD2      (uint16_t) VAUX_OOR2_FLOAT  

#define VAUX_OOR_HYS1_FLOAT      (float)((4096/2/3.3)*5)   //5V
#define VAUX_OOR_HYS_LIMIT1      (uint16_t) VAUX_OOR_HYS1_FLOAT 
#define VAUX_OOR_HYS2_FLOAT      (float)((4096/2/3.3)*6)   //6V
#define VAUX_OOR_HYS_LIMIT2      (uint16_t) VAUX_OOR_HYS2_FLOAT 

#define VAUX_OOR_THRESHOLD_CNT  30
#define VAUX_OOR_HYS_CNT        10000

//Iout Over Current ADC
#define IOUT_OC_FLOAT           (float)((((0.4*2.2)+1.65)*4096)/3.3) //2.2A
#define IOUT_OC_THRESHOLD       (uint16_t) IOUT_OC_FLOAT    

#define IOUT_OC_HYS_FLOAT       (float)((((0.4*2)+1.65)*4096)/3.3) //2A
#define IOUT_OC_HYS_LIMIT       (uint16_t) IOUT_OC_HYS_FLOAT 

#define IOUT_OC_THRESHOLD_CNT  30
#define IOUT_OC_HYS_CNT        10000

//Iin Over Current CMP
#define IIN_OC_CMP_FLOAT        (float)((((0.47*5)+0.447)*4096)/3.3) //5A
#define IIN_OC_CMP_THRESHOLD    (uint16_t) IIN_OC_CMP_FLOAT    

#define IIN_OC_CMP_HYS_FLOAT 	(float)((((0.47*4)+0.447)*4096)/3.3) //4A
#define IIN_OC_CMP_HYS_LIMIT    (uint16_t) IIN_OC_CMP_HYS_FLOAT 

#define IIN_OC_CMP_THRESHOLD_CNT  30
#define IIN_OC_CMP_HYS_CNT        10000

//======================================================================================================================
//@brief Defines for voltage and current settings
//======================================================================================================================

#define VOUT_PREBIAS_FLOAT    (float)((4096/26.4)*3)            //3V
#define VOUT_PREBIAS          (uint16_t) VOUT_PREBIAS_FLOAT     //  

#define VOUT_10V_FLOAT      (float)((4096/8/3.3)*10)   //10V
#define VOUT_10V            (uint16_t)VOUT_10V_FLOAT

#define IOUT_CCM_FLOAT 		(float)((((0.4*0.5)+1.65)*4096)/3.3) //0.5A
#define IOUT_CCM            (uint16_t) IOUT_CCM_FLOAT 

#define IOUT_DCM_FLOAT 		(float)((((0.4*0.4)+1.65)*4096)/3.3) //0.4A
#define IOUT_DCM            (uint16_t) IOUT_DCM_FLOAT 

#define VREF_FIXED12V_FLOAT (float)((4096/8/3.3)*12)   //12V
#define VREF_FIXED12V       (uint16_t) VREF_FIXED12V_FLOAT

//======================================================================================================================
//@brief Defines for PWM Portpins
//======================================================================================================================

#define BuckPWM_H           PORTBbits.RB14