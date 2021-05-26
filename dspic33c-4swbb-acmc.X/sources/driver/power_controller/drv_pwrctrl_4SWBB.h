/**
  @Company
    Microchip Technology Inc.

  @File Name
    drv_pwrctrl_4SWBB.h

  @Summary
    This is the generated driver implementation file using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides all the function for the power controller to control and drive the power stage(s).
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

#ifndef _DRV_PWRCTRL_4SWBB_H_
#define	_DRV_PWRCTRL_4SWBB_H_

#include <xc.h> // include processor files - each processor file is guarded.  

//======================================================================================================================
// @brief power controller states
//======================================================================================================================

typedef enum
{
    PCS_INIT					= 1,    // Init peripherals that are involved
    PCS_WAIT_IF_FAULT_ACTIVE	= 2,    // goes on if no fault is active
    PCS_STANDBY					= 3,    // wait for reference to be greater X
    PCS_SOFT_START_OL			= 4,    // Soft-Start Ramp Up/Down Output Voltage
    PCS_SOFT_START				= 5,    // Soft-Start Ramp Up/Down Output Voltage
    PCS_UP_AND_RUNNING          = 6,    // Soft-Start is complete, power is up and running
    PCS_SHUTDOWN                = 7     // shutting down the controller/reference voltage
}PWR_CTRL_STATE_e;

extern PWR_CTRL_STATE_e pwr_ctrl_state;

//======================================================================================================================
// @brief structure of variables for power stage control and drive
//======================================================================================================================

typedef union
{
    struct
    {
        volatile unsigned /*Ref from Poti/UART */   : 1; // Bit  0: reserved for UART
        volatile unsigned /*Ref from GUI/UART */    : 1; // Bit  1: reserved for UART
        volatile unsigned /*Ref from intern/UART */ : 1; // Bit  2: reserved for UART
        volatile unsigned                           : 1; // Bit  3:
        volatile unsigned                           : 1; // Bit  4:
        volatile unsigned                           : 1; // Bit  5:
        volatile unsigned CCM_mode                  : 1; // Bit  6
        volatile unsigned avgcurrentmode            : 1; // Bit  7:
        volatile unsigned enabled                   : 1; // Bit  8: set after power controller init
        volatile unsigned run                       : 1; // Bit  9: 
        volatile unsigned auto_start                : 1; // Bit 10: 
        volatile unsigned pwm_running               : 1; // Bit 11: 
        volatile unsigned inopenloop                : 1; // Bit 12:
        volatile unsigned inclosedloop              : 1; // Bit 13:
        volatile unsigned inopenloop_saving         : 1; // Bit 14:
        volatile unsigned                           : 1; // Bit 15:
    };
    uint16_t CollectivePwrCtrlFlagbits;
}   __attribute__((packed))PWR_CTRL_FLAGBITS_t;

extern PWR_CTRL_FLAGBITS_t pwr_ctrl_flagbits;

//======================================================================================================================
// @brief function prototypes
//======================================================================================================================

void Drv_PwrCtrl_4SWBB_Init(void);
void Drv_PwrCtrl_4SWBB_Start(void);
void Drv_PwrCtrl_4SWBB_Stop(void);
void Drv_PwrCtrl_4SWBB_SetMode_OpenLoop(void);
void Drv_PwrCtrl_4SWBB_SetMode_ClosedLoop(void);
void Drv_PwrCtrl_4SWBB_SetReferenceRaw(uint16_t ref);
void Drv_PwrCtrl_4SWBB_SetReference_mV(uint16_t ref_mV);
void Drv_PwrCtrl_4SWBB_SetRampSpeed(uint16_t milliseconds);
void Drv_PwrCtrl_4SWBB_SetSlewRate(uint32_t millivolts_per_milliseconds);
void Drv_PwrCtrl_4SWBB_Task_100us(void);
void Drv_PwrCtrl_4SWBB_CtrlLoop(void);

//------------------------------------------------------------------------------

#endif	// _DRV_PWRCTRL_4SWBB_H_

