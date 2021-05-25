/**
  @Company
    Microchip Technology Inc.

  @File Name
    app_hmi.c

  @Summary
    This is the generated driver implementation file using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for the inputs from the buttons and controls the 4SWBB and LEDs.
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


#include <xc.h>
#include <stdbool.h>
#include "device/dev_button.h"
#include "driver/power_controller/drv_pwrctrl_4SWBB.h"
#include "driver/power_controller/drv_pwrctrl_4SWBB_fault.h"
#include "../mcc_generated_files/pin_manager.h"
#include "driver/drv_led.h"
#include"driver/drv_adc.h"
#include "../sources/driver/power_controller/drv_pwrctrl_4SWBB_settings.h"

//local functions
void CheckButtons(void);
void SetLEDs_100ms(void);
bool CheckForBoardSetup (void);

extern uint16_t os_resetCause;  

bool App_HMI_useRefFromPoti = false;
bool App_HMI_useRefFromGUI = false;
bool App_HMI_useFixedRef = true;

//======================================================================================================================
// @brief initializes the HMI application
//======================================================================================================================

void App_HMI_Init(void)
{
    Drv_LED_Blink(LED_PIM_RED);
    Drv_PwrCtrl_4SWBB_SetRampSpeed(100);
}

//======================================================================================================================
// @brief this function contains the button SW2 check and following actions
//======================================================================================================================

void CheckButtons(void)
{
    uint8_t buttonresponse = Dev_Button_GetEvent();

    if (buttonresponse == DEV_BUTTON_EVENT_PRESSED_SHORT)
    {
        if (pwr_ctrl_flagbits.run)
        {
            Drv_PwrCtrl_4SWBB_Stop();
            Drv_LED_Off(LED_BOARD_GREEN);
        }
        else
        {
            Drv_PwrCtrl_4SWBB_Start();
            Drv_LED_On(LED_BOARD_GREEN);
        }
    } 
    else if (buttonresponse == DEV_BUTTON_EVENT_PRESSED_LONG)
    {
        if (pwr_ctrl_flagbits.inopenloop)
        {  
            Drv_PwrCtrl_4SWBB_SetMode_ClosedLoop();
        }  
        else
        {  
            Drv_PwrCtrl_4SWBB_SetMode_OpenLoop();
        }
    }
}

//======================================================================================================================
// @brief this function contains LED properties called every 100ma from scheduler
//======================================================================================================================

void SetLEDs_100ms()
{
    //state < running --> toggle grenn LED on power board
    if (!pwr_ctrl_flagbits.run)
    {
        Drv_LED_Blink(LED_BOARD_GREEN);
    }

    //fault fheck
    PWR_CTRL_FAULTS faults = Drv_PwrCtrl_4SWBB_GetFaults(); //@ftx

    //any fault --> fast toggle red LED on power board
    if (os_resetCause & 0x01 || faults.CollectiveFaults != 0)
    {
        Drv_LED_On(LED_BOARD_RED);
    } 
    else if (pwr_ctrl_flagbits.inopenloop)
    {
        Drv_LED_Blink(LED_BOARD_RED);
    }
    else
    {
        //LED_Red_SetLow();
        Drv_LED_Off(LED_BOARD_RED);
    }
}

//======================================================================================================================
// @brief this function contains the HMI application that handles the LED outputs
//======================================================================================================================

void App_HMI_Task_100ms(void)
{
    if (CheckForBoardSetup() == false)
    {
        CheckButtons();

        if (App_HMI_useRefFromPoti)
            Drv_PwrCtrl_4SWBB_SetReferenceRaw(pwr_ctrl_adc_data.drv_adc_val_AN15); //Poti connected
        if (App_HMI_useRefFromGUI)
            Nop();
        else if (App_HMI_useFixedRef)
            Drv_PwrCtrl_4SWBB_SetReferenceRaw(VREF_FIXED12V); //12V
        SetLEDs_100ms();
    }
}


//==============================================================================
// @brief this function contains the board check if an potentiometer is connected
//        as reference input; otherwise a fixed reference value is set 
//==============================================================================

bool CheckForBoardSetup (void)
{
    static uint16_t CFB_state = 1, ADCvaluePD, ADCvaluePU;
    
    if (CFB_state > 6)
    {
        return false;   //board setup check finished
    }

    switch (CFB_state)
    {
        case 1:
            CNPDCbits.CNPDC3 = 0;
            CNPUCbits.CNPUC3 = 0;
            CFB_state++;
            break;
    
        case 2:
            CNPDCbits.CNPDC3 = 1;
            CNPUCbits.CNPUC3 = 0;
            CFB_state++;
            break;

        case 3: //wait
            CFB_state++;
            break;
            
        case 4:
            ADCvaluePD = pwr_ctrl_adc_data.drv_adc_val_AN15;
            CNPDCbits.CNPDC3 = 0;
            CNPUCbits.CNPUC3 = 1;
            CFB_state++;
            break;
    
        case 5: //wait
            CFB_state++;
            break;
            
        case 6:
            ADCvaluePU = pwr_ctrl_adc_data.drv_adc_val_AN15;
            if((ADCvaluePD < 200) && (ADCvaluePU > 3900))
            {  
                App_HMI_useRefFromPoti = false;
                App_HMI_useFixedRef = true;
            }
            else
            {
                App_HMI_useRefFromPoti = true;
                App_HMI_useFixedRef = false;        
            }  
            CNPDCbits.CNPDC3 = 0;
            CNPUCbits.CNPUC3 = 0;
            CFB_state++;
            break;
    }

    if (CFB_state > 6)
        return false;   //board setup check finished
    else
        return true;    //board setup check still running
}

//======================================================================================================================
//======================================================================================================================
