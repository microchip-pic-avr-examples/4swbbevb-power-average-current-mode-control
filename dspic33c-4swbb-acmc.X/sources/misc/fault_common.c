/**
  @Company
    Microchip Technology Inc.

  @File Name
    fault_common.c

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

#include <stdbool.h>
#include <stddef.h>

#include "misc/fault_common.h"

/*******************************************************************************
 * Function:       FAULT_Init
 * Inputs:         Pointer to faultStructure, 
 * 
 *  
 * Description:	   
 * 
 * 
 *******************************************************************************/

void FAULT_Init(FAULT_OBJ_T *faultInput,
                int16_t  threshold, int16_t  hysLimit,
                uint16_t thresholdCnt, uint32_t hysCnt)
{
  faultInput->val1_Threshold = threshold;
  faultInput->val1_Hysteresis = hysLimit;
  faultInput->CounterSet = thresholdCnt;
  faultInput->CounterReset = hysCnt;
  faultInput->fltCode = 0;
  faultInput->faultActive = 0;
}

void FAULT_InitRange(FAULT_OBJ_T *faultInput,
                int16_t  thresholdMin, int16_t  hysLimitMin,
                int16_t  thresholdMax, int16_t  hysLimitMax,
                uint16_t thresholdCnt, uint32_t hysCnt)
{
  faultInput->val1_Threshold = thresholdMin;
  faultInput->val1_Hysteresis = hysLimitMin;
  faultInput->val2_Threshold = thresholdMax;
  faultInput->val2_Hysteresis = hysLimitMax;
  faultInput->CounterSet = thresholdCnt;
  faultInput->CounterReset = hysCnt;
  faultInput->fltCode = 0;
  faultInput->faultActive = 0;
}


/*******************************************************************************
 * Function:       FAULT_SetCounters
 * Inputs:         Pointer to faultStructure, 
 * 
 *  
 * Description:	   
 * 
 * 
 *******************************************************************************/
void FAULT_SetCounters(FAULT_OBJ_T *faultInput, uint16_t fltThresholdCnt, uint32_t fltHysCnt)
{
   faultInput->CounterSet = fltThresholdCnt;
   faultInput->CounterReset = fltHysCnt;
}

/*******************************************************************************
 * Function:       FAULT_SetThresholds
 * Inputs:         Pointer to faultStructure, 
 * 
 *  
 * Description:	   
 * 
 * 
 *******************************************************************************/
void FAULT_SetThresholds(FAULT_OBJ_T *faultInput, int16_t fltThreshold, int16_t fltHysLimit)
{
   faultInput->val1_Threshold = fltThreshold;
   faultInput->val1_Hysteresis = fltHysLimit;
}

/*******************************************************************************
 * Function:       FAULT_CheckMax
 * Inputs:         Pointer to faultStructure, input fault source, pointer to 
 *                 variable that captures fault code, and callback function pointer 
 * Output:         Returns true if fault active else false  
 * Description:	   Checks if input fault source is greater than set max threshold,
 *                 sets fltState which captures event and will remove fault once
 *                 input source goes below Hysteresis limit (if set). Function does 
 *                 not clear fltState. If callback pointer non null, then after
 *                 fault is set, routine will call the callback function allowing
 *                 quick shutdown response if needed.
 *******************************************************************************/

bool FAULT_CheckMax(FAULT_OBJ_T *faultInput, int16_t faultSource, FAULT_CALLBACK callback)
{
  if ((faultSource >= faultInput->val1_Threshold) && (faultInput->faultActive == false))
  {
    faultInput->fltCounter++;

    if (faultInput->fltCounter > faultInput->CounterSet)
    {
      if (callback != NULL)
      {
        callback(faultInput); // Call Back function used to quickly shutdown system if needed when fault detected
      }

      faultInput->faultActive = true;
      faultInput->fltCounter = 0;
    }
  } 
  else if ((faultSource < (faultInput->val1_Hysteresis)) && (faultInput->faultActive == true))
  {
    faultInput->fltCounter++;
    if (faultInput->fltCounter > faultInput->CounterReset)
    {
      faultInput->fltCounter = 0;
      faultInput->faultActive = false;
    }
  }
  else
  {
    faultInput->fltCounter = 0;
  }

  return (faultInput->faultActive);
}

/*******************************************************************************
 * Function:       FAULT_CheckMin
 * Inputs:         Pointer to faultStructure, input fault source, pointer to 
 *                 variable that captures fault code, and callback function pointer  
 * Output:         Returns true if fault active else false  
 * Description:    Checks if input fault source is less than set threshold,
 *                 sets fltState which captures event and will remove fault once
 *                 input source goes above Hysteresis limit (if set). Function does 
 *                 not clear fltState. If callback pointer non null, then after
 *                 fault is set, routine will call the callback function allowing
 *                 quick shutdown response if needed. 
 *******************************************************************************/

bool FAULT_CheckMin(FAULT_OBJ_T *faultInput, int16_t faultSource, FAULT_CALLBACK callback)
{
  if ((faultSource <= faultInput->val1_Threshold) && (faultInput->faultActive == false))
  {
    faultInput->fltCounter++;
    if (faultInput->fltCounter > faultInput->CounterSet)
    {
      if (callback != NULL)
      {
        callback(faultInput); // Call Back function used to quickly shutdown system if needed when fault detected
      }

      faultInput->faultActive = true;
      faultInput->fltCounter = 0;
    }
  }
  else if ((faultSource > (faultInput->val1_Hysteresis)) && (faultInput->faultActive == true))
  {
    faultInput->fltCounter++;
    if (faultInput->fltCounter > faultInput->CounterReset)
    {
      faultInput->fltCounter = 0;
      faultInput->faultActive = false;
    }
  }
  else
  {
    faultInput->fltCounter = 0;
  }

  return (faultInput->faultActive);
}


/*******************************************************************************
 * Function:       FAULT_CheckBit
 * Inputs:         Pointer to faultStructure, input fault source, pointer to 
 *                 variable that captures fault code, and callback function pointer 
 * Output:         Returns true if fault active else false  
 * Description:	   Checks if input fault bit is 1,
 *                 sets fltState which captures event and will remove fault once
 *                 input fault bit gets 0. Function does 
 *                 not clear fltState. If callback pointer non null, then after
 *                 fault is set, routine will call the callback function allowing
 *                 quick shutdown response if needed.
 *******************************************************************************/

bool FAULT_CheckBit(FAULT_OBJ_T *faultInput, bool faultBit, FAULT_CALLBACK callback)
{
  if ((faultBit) && (faultInput->faultActive == false))
  {
    faultInput->fltCounter++;

    if (faultInput->fltCounter > faultInput->CounterSet)
    {
      if (callback != NULL)
      {
        callback(faultInput); // Call Back function used to quickly shutdown system if needed when fault detected
      }

      faultInput->faultActive = true;
      faultInput->fltCounter = 0;
    }
  }
  else if ((!faultBit) && (faultInput->faultActive == true))
  {
    faultInput->fltCounter++;
    if (faultInput->fltCounter > faultInput->CounterReset)
    {
      faultInput->fltCounter = 0;
      faultInput->faultActive = false;
    }
  } else
  {
    faultInput->fltCounter = 0;
  }

  return (faultInput->faultActive);
}

/*******************************************************************************
 * Function:       FAULT_CheckRange
 * Inputs:         Pointer to faultStructure, input fault source, pointer to 
 *                 variable that captures fault code, and callback function pointer  
 * Output:         Returns true if fault active else false  
 * Description:    Checks if input fault source is less than set threshold,
 *                 sets fltState which captures event and will remove fault once
 *                 input source goes above Hysteresis limit (if set). Function does 
 *                 not clear fltState. If callback pointer non null, then after
 *                 fault is set, routine will call the callback function allowing
 *                 quick shutdown response if needed. 
 *******************************************************************************/

bool FAULT_CheckRange(FAULT_OBJ_T *faultInput, int16_t inputValue, FAULT_CALLBACK callback)
{
    if (faultInput->faultActive == false)
    {
        if ((inputValue <= faultInput->val1_Threshold) ||
            (inputValue >= faultInput->val2_Threshold))
        {
            if (++faultInput->fltCounter >= faultInput->CounterSet)
            {
                if (callback != NULL)
                {
                    callback(faultInput); // Call Back function used to quickly shutdown system if needed when fault detected
                }
                faultInput->faultActive = true;
                faultInput->fltCounter = 0;
            }
        }
        else
            faultInput->fltCounter = 0;
    }
    else
    {
        if ((inputValue > faultInput->val1_Hysteresis) &&
            (inputValue < faultInput->val2_Hysteresis))
        {
            if (++faultInput->fltCounter > faultInput->CounterReset)
            {
                faultInput->fltCounter = 0;
                faultInput->faultActive = false;
            }
        }
        else
            faultInput->fltCounter = 0;
    }
    return (faultInput->faultActive);
}
