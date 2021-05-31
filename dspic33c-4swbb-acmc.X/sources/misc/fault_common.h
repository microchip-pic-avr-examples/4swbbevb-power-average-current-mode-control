/**
  @Company
    Microchip Technology Inc.

  @File Name
    fault_common.h

  @Summary
    This is the generated driver implementation file using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides all the generic/general fault functions.
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

#ifndef FAULT_H
#define FAULT_H

//#include <xc.h>

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
   int16_t  val1_Threshold;     // Fault threshold limit: Range of -32768 to 32767
   int16_t  val1_Hysteresis;    // Fault hysteresis limit. If hysteresis is not needed, fltThreshold = fltHysLimit
   int16_t  val2_Threshold;     // Fault threshold limit: Range of -32768 to 32767
   int16_t  val2_Hysteresis;    // Fault hysteresis limit. If hysteresis is not needed, fltThreshold = fltHysLimit
   uint16_t  CounterSet;        // Number of consecutive fault events before fault becomes active.  
   uint32_t  CounterReset;      // Number of consecutive events when input outside hysteresis limit in order to remove fault, set to 0 to disable Hys 
   uint16_t  fltCounter;        // Internal counter for activating/removing fault 
   uint16_t  fltCode;           // Code that can be used to display fault (1st fault occurred) to global variable
   uint8_t   faultActive;       // Set/Cleared inside flt check loop        
}FAULT_OBJ_T;   

typedef void(*FAULT_CALLBACK)(FAULT_OBJ_T* faultInput);

void FAULT_Init(FAULT_OBJ_T *faultInput,
                int16_t  threshold, int16_t  hysLimit,
                uint16_t thresholdCnt, uint32_t hysCnt);

void FAULT_InitRange(FAULT_OBJ_T *faultInput,
                int16_t  thresholdMin, int16_t  hysLimitMin,
                int16_t  thresholdMax, int16_t  hysLimitMax,
                uint16_t thresholdCnt, uint32_t hysCnt);

void FAULT_SetCounters(FAULT_OBJ_T *faultInput, uint16_t fltThresholdCnt, uint32_t fltHysCnt);
void FAULT_SetThresholds(FAULT_OBJ_T *faultInput, int16_t fltThreshold, int16_t fltHysLimit);
bool FAULT_CheckMax(FAULT_OBJ_T *faultInput, int16_t faultSource, FAULT_CALLBACK callback);
bool FAULT_CheckMin(FAULT_OBJ_T *faultInput, int16_t faultSource, FAULT_CALLBACK callback);
bool FAULT_CheckBit(FAULT_OBJ_T *faultInput, bool faultBit, FAULT_CALLBACK callback);
bool FAULT_CheckRange(FAULT_OBJ_T *faultInput, int16_t inputValue, FAULT_CALLBACK callback);

#endif 

