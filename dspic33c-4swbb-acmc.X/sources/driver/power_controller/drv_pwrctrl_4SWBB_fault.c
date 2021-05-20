/**
  @Company
    Microchip Technology Inc.

  @File Name
   drv_pwrctrl_4SWBB_fault.c  

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
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
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

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "mcc_generated_files/system.h"

#include "misc/fault_common.h"
#include "driver/power_controller/drv_pwrctrl_4SWBB.h"
#include "driver/drv_adc.h"
#include "../mcc_generated_files/cmp3.h"
#include "../mcc_generated_files/cmp1.h"
#include "drv_pwrctrl_4SWBB_fault.h"
#include "drv_pwrctrl_4SWBB_settings.h"

PWR_CTRL_FAULTS FourSWBBFaults;

FAULT_OBJ_T fltobj_4SWBB_VinUv;     // Input Under Voltage Protection (UVP) handled by ADC
FAULT_OBJ_T fltobj_4SWBB_VinOv;     // Input Over Voltage Protection (OVP) handled by ADC

FAULT_OBJ_T fltobj_4SWBB_VoutOv;		// Output Over Voltage Protection (OVP) handled by ADC
FAULT_OBJ_T fltobj_4SWBB_VoutOvCMP; // Output Over Voltage Protection (OVP) handled by CMP

FAULT_OBJ_T fltobj_4SWBB_VauxOOR;   // Auxilliary Voltage out of range Protection handled by ADC 

FAULT_OBJ_T fltobj_4SWBB_IoutOC;    // Output Over Current Protection handled by ADC 

FAULT_OBJ_T fltobj_4SWBB_IinOCCMP;  // Input Over Current Protection handled by CMP

		
//======================================================================================================================
// @brief this function contains 
//======================================================================================================================

void Drv_PwrCtrl_4SWBB_Init_Fault(void)
{
    FAULT_Init( &fltobj_4SWBB_VinUv, VIN_UV_THRESHOLD, VIN_UV_HYSLIMIT, 0, 0 ); //initialize with 0's that fault is immediately activ
    FAULT_Init( &fltobj_4SWBB_VinOv, VIN_OV_THRESHOLD, VIN_OV_HYSLIMIT, 0, 0 ); //initialize with 0's that fault is immediately activ

    FAULT_Init( &fltobj_4SWBB_VoutOv, VOUT_OV_THRESHOLD, VOUT_OV_HYS_LIMIT, VOUT_OV_THRESHOLD_CNT, VOUT_OV_HYS_CNT );
    FAULT_Init( &fltobj_4SWBB_VoutOvCMP, VOUT_OV_CMP_THRESHOLD, VOUT_OV_CMP_HYS_LIMIT, VoutOVCMPThresholdCnt, VoutOVCMPHysCnt  );  
    CMP3_SetDACDataHighValue(VOUT_OV_CMP_THRESHOLD);
  
    FAULT_InitRange(&fltobj_4SWBB_VauxOOR, VAUX_OOR_THRESHOLD1, VAUX_OOR_HYS_LIMIT1,VAUX_OOR_THRESHOLD2, VAUX_OOR_HYS_LIMIT2, VAUX_OOR_THRESHOLD_CNT, VAUX_OOR_HYS_CNT);

    FAULT_Init(&fltobj_4SWBB_IoutOC, IOUT_OC_THRESHOLD, IOUT_OC_HYS_LIMIT, IOUT_OC_THRESHOLD_CNT, IOUT_OC_HYS_CNT );
  
    FAULT_Init(&fltobj_4SWBB_IinOCCMP, IIN_OC_CMP_THRESHOLD, IIN_OC_CMP_HYS_LIMIT, IIN_OC_CMP_THRESHOLD_CNT, IIN_OC_CMP_HYS_CNT  );  
    CMP1_SetDACDataHighValue(IIN_OC_CMP_THRESHOLD);
}

//======================================================================================================================
// @brief this function contains 
//======================================================================================================================

void Drv_PwrCtrl_4SWBB_SetFaultCounters(void)
{
    FAULT_SetCounters(&fltobj_4SWBB_VinUv, VIN_UV_THRESHOLD_CNT, VIN_UV_HYSCNT);  
    FAULT_SetCounters(&fltobj_4SWBB_VinOv, VIN_OV_THRESHOLD_CNT, VIN_OV_HYS_CNT);
}

//======================================================================================================================
// @brief this function contains 
//======================================================================================================================

void Drv_PwrCtrl_4SWBB_Fault_Check()
{
  //  Vin uvervoltage check
  FourSWBBFaults.inputUnderVoltage = FAULT_CheckMin(&fltobj_4SWBB_VinUv, pwr_ctrl_adc_data.drv_adc_val_FB_Vdd, NULL);
  //  Vin overvoltage check
  FourSWBBFaults.inputOverVoltage = FAULT_CheckMax(&fltobj_4SWBB_VinOv, pwr_ctrl_adc_data.drv_adc_val_FB_Vdd, NULL);

  //  Vout overvoltage check
  FourSWBBFaults.outputOverVoltage = FAULT_CheckMax(&fltobj_4SWBB_VoutOv, pwr_ctrl_adc_data.drv_adc_val_FB_Vout, NULL);

  //  Vout Comparator HW overvoltage check
  FourSWBBFaults.outputVoltageCMP = FAULT_CheckBit(&fltobj_4SWBB_VoutOvCMP, PG1STATbits.FLTEVT, NULL);
  // Iin overcurrent check
  FourSWBBFaults.inputCurrentCMP = FAULT_CheckBit(&fltobj_4SWBB_IinOCCMP, PG1STATbits.FLTEVT, NULL);
  if ((FourSWBBFaults.outputVoltageCMP || FourSWBBFaults.inputCurrentCMP) && (PG1STATbits.FLTEVT || PG2STATbits.FLTEVT))
  {
    PG1FPCILbits.SWTERM = 1;
    PG1STATbits.FLTEVT = 0;
    PG2FPCILbits.SWTERM = 1;
    PG2STATbits.FLTEVT = 0;
  }
  // Vaux out of range check
  FourSWBBFaults.auxVoltageOor = FAULT_CheckRange(&fltobj_4SWBB_VauxOOR, pwr_ctrl_adc_data.drv_adc_val_FB_Vaux, NULL);

  // Iout overcurrent check
  FourSWBBFaults.outputOverCurrent = FAULT_CheckMax(&fltobj_4SWBB_IoutOC, pwr_ctrl_adc_data.drv_adc_val_FB_Iout, NULL);

}

//======================================================================================================================
// @brief this function contains 
//======================================================================================================================

PWR_CTRL_FAULTS Drv_PwrCtrl_4SWBB_GetFaults(void)
{
	return FourSWBBFaults;
}

//====================================================================================================================== 
//======================================================================================================================


