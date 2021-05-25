/**
  @Company
    Microchip Technology Inc.

  @File Name
    drv_pwrctrl_4SWBB_misc.c

  @Summary
    This is the generated driver implementation file using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides the additional functions for the power controller aside the core power controller.
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


#include <p33CK256MP506.h>
#include <stdint.h>

#include "driver/power_controller/drv_pwrctrl_4SWBB.h"
#include "driver/power_controller/drv_pwrctrl_4SWBB_internal.h"
#include "driver/drv_adc.h"
#include "mcc_generated_files/pwm.h"
#include "mcc_generated_files/pin_manager.h"
#include "driver/power_controller/drv_pwrctrl_4SWBB_settings.h"

PWR_CTRL_REFDATA_t pwr_ctrl_ref_data;

static volatile uint16_t rampspeed_softStart_milliseconds = 90;
volatile uint16_t rampspeed_value_16 = 1;
static volatile uint16_t rampspeed_up_and_running_16 = RUNNING_RAMP_SPEED;
uint16_t softStart_counter = 0;

//==============================================================================
// @brief this function contains the run flag handling ON
//==============================================================================

void Drv_PwrCtrl_4SWBB_Start(void)
{
  while (pwr_ctrl_flagbits.run == 0) //it needs to be a while function to avoid interrupt access conflicts
  {
    pwr_ctrl_flagbits.run = 1;
  }
}

//==============================================================================
// @brief this function contains the run flag handling OFF
//==============================================================================

void Drv_PwrCtrl_4SWBB_Stop(void)
{
  while (pwr_ctrl_flagbits.run == 1) //it needs to be a while function to avoid interrupt access conflicts
  {
    pwr_ctrl_flagbits.run = 0;
  }
}
//==============================================================================
// @brief this function contains the open loop flag handling
//==============================================================================

void Drv_PwrCtrl_4SWBB_SetMode_OpenLoop(void)
{
  //it needs to be a while function to avoid interrupt access conflicts
  while (pwr_ctrl_flagbits.inopenloop == 0 || pwr_ctrl_flagbits.inclosedloop == 1)
  {
    pwr_ctrl_flagbits.inopenloop = 1;
    pwr_ctrl_flagbits.inclosedloop = 0;
  }
}

//==============================================================================
// @brief this function contains the closed loop flag handling
//==============================================================================

void Drv_PwrCtrl_4SWBB_SetMode_ClosedLoop(void)
{
  //it needs to be a while function to avoid interrupt access conflicts
  while (pwr_ctrl_flagbits.inopenloop == 1 || pwr_ctrl_flagbits.inclosedloop == 0)
  {
    pwr_ctrl_flagbits.inopenloop = 0;
    pwr_ctrl_flagbits.inclosedloop = 1;
  }
}

//==============================================================================
// @brief this function contains the update calculation for the ramp speed
//        for the softstart
//==============================================================================

void Drv_PwrCtrl_4SWBB_CalculateRampValue(void)
{
  uint16_t adcdiff;
  uint16_t adcstart;
  if (pwr_ctrl_flagbits.inclosedloop)
    adcstart = pwr_ctrl_ref_data.val_VoutRef_internal;
  else
    adcstart = 0;
  if (pwr_ctrl_ref_data.drv_val_VoutRef > adcstart)
  {
    adcdiff = pwr_ctrl_ref_data.drv_val_VoutRef - adcstart;
  }
  else
  {
    adcdiff = 1;
  }
  //    static volatile uint8_t softStart_speed_milliseconds = 90;
  //calculate value to add every 100us with 4 bits higher resolution
  rampspeed_value_16 = ((uint32_t) adcdiff << 4) / ((uint32_t) rampspeed_softStart_milliseconds * 10);
  if (rampspeed_value_16 == 0)
    rampspeed_value_16 = 1;
  softStart_counter = 0;
}

//==============================================================================
// @brief this function contains 
//==============================================================================

void Drv_PwrCtrl_4SWBB_SetReferenceRaw(uint16_t ref)
{
  pwr_ctrl_ref_data.drv_val_VoutRef = ref;
}

//==============================================================================
// @brief this function contains 
//==============================================================================

void Drv_PwrCtrl_4SWBB_SetReference_mV(uint16_t ref_mV)
{
  //Todo
}

//==============================================================================
// @brief this function sets the startup time in milliseconds 
//==============================================================================

void Drv_PwrCtrl_4SWBB_SetRampSpeed(uint16_t milliseconds)
{
  // first subtract the initial power on delay of 10 milliseconds
  if (milliseconds <= 10)
    rampspeed_softStart_milliseconds = 10;
  else
    rampspeed_softStart_milliseconds = milliseconds - 10;
}

//==============================================================================
// @brief this function sets the startup time in milliseconds 
//==============================================================================

void Drv_PwrCtrl_4SWBB_SetSlewRate(uint32_t millivolts_per_milliseconds)
{
  if (millivolts_per_milliseconds == 0)
  {
    millivolts_per_milliseconds = 1;
  }
  //calculate value to add every 100us with 4 bits higher resolution
  rampspeed_up_and_running_16 = (millivolts_per_milliseconds * (uint32_t) VOUT_10V << 4) / ((uint32_t) 1000 * 10 * 10);
  if (rampspeed_up_and_running_16 == 0)
    rampspeed_up_and_running_16 = 1;
}

//==============================================================================
// @brief this function contains 
//==============================================================================

void SoftStart_RampUpDownVoltage(void)
{
  //UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU 
  softStart_counter += rampspeed_value_16;
  uint16_t softstart_delta = softStart_counter >> 4;

  if (softstart_delta > 0)
  {
    softStart_counter -= softstart_delta << 4;
    if (pwr_ctrl_ref_data.val_VoutRef_internal < pwr_ctrl_ref_data.drv_val_VoutRef)
    {
      if (pwr_ctrl_ref_data.val_VoutRef_internal + softstart_delta < pwr_ctrl_ref_data.drv_val_VoutRef)
        pwr_ctrl_ref_data.val_VoutRef_internal += softstart_delta;
      else
      {
        pwr_ctrl_ref_data.val_VoutRef_internal = pwr_ctrl_ref_data.drv_val_VoutRef;
        rampspeed_value_16 = rampspeed_up_and_running_16;
        pwr_ctrl_state = PCS_UP_AND_RUNNING;
      }
    }
    else
    {
      if (pwr_ctrl_ref_data.val_VoutRef_internal + softstart_delta > pwr_ctrl_ref_data.drv_val_VoutRef)
        pwr_ctrl_ref_data.val_VoutRef_internal -= softstart_delta;
      else
      {
        pwr_ctrl_ref_data.val_VoutRef_internal = pwr_ctrl_ref_data.drv_val_VoutRef;
        rampspeed_value_16 = rampspeed_up_and_running_16;
        pwr_ctrl_state = PCS_UP_AND_RUNNING;
      }
    }
  }
}

//==============================================================================
// @brief this function contains 
//==============================================================================

void PreBias(void)
{
  if (pwr_ctrl_adc_data.drv_adc_val_FB_Vout < VOUT_PREBIAS)
  {
    pwr_ctrl_ref_data.val_VoutRef_internal += 4;
  }
  if (pwr_ctrl_flagbits.inclosedloop)
  {
    PWM_OverrideHighEnable(BoostPWM); //disable PWM output on Boost-H 
  }

}

//==============================================================================
//==============================================================================
