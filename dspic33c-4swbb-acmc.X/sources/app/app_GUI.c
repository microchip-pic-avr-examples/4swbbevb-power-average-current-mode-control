/**
  @Company
    Microchip Technology Inc.

  @File Name
    app_GUI.c

  @Summary
    This source file provides the application specific HUI functions

  @Description
    This source file provides the application specific HUI functions
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
#include "mcc_generated_files/pin_manager.h"
#include "driver/drv_led.h"
#include "driver/drv_adc.h"
#include "driver/power_controller/drv_pwrctrl_4SWBB_settings.h"
#include "device/dev_gui_comm.h"
#include "app_HMI.h"
#include "driver/power_controller/Controller_pwrctrl_4SWBB.h"

#define APP_GUI_PROTOCOL_ID 0x40EF

void App_GUI_ProtocolHandler(uint16_t length, uint8_t* data);

//======================================================================================================================
// @brief initializes the GUI application
//======================================================================================================================
void App_GUI_Init(void)
{
    Dev_GuiComm_Register_ProtocolHandler(App_GUI_ProtocolHandler, APP_GUI_PROTOCOL_ID);
}


void App_GUI_Task_1ms(void)
{
    if (Dev_GuiComm_Stream_ReadyToSend())
    {
        //Message pos 0
        Dev_GuiComm_Stream_SendWord(pwr_ctrl_adc_data.drv_adc_val_FB_Vdd);

        //Message pos 1
        Dev_GuiComm_Stream_SendWord(pwr_ctrl_adc_data.drv_adc_val_FB_Vout);

        //Message pos 2
        Dev_GuiComm_Stream_SendWord(pwr_ctrl_adc_data.drv_adc_val_FB_Iout);

        //Message pos 3
        Dev_GuiComm_Stream_SendWord(pwr_ctrl_adc_data.drv_adc_val_FB_Iin);

        //Message pos 4
        Dev_GuiComm_Stream_SendWord(pwr_ctrl_adc_data.drv_adc_val_AN15);

        //Message pos 5
        Dev_GuiComm_Stream_SendWord(pwr_ctrl_adc_data.drv_adc_val_FB_Vaux);

        //Message pos 6
        Dev_GuiComm_Stream_SendWord(1 << (pwr_ctrl_state-1));

        //Message pos 7
        Dev_GuiComm_Stream_SendWord(FourSWBBFaults.CollectiveFaults);

        //Message pos 8
        if(App_HMI_useRefFromPoti) pwr_ctrl_flagbits.CollectivePwrCtrlFlagbits |= 0x0001;
        else pwr_ctrl_flagbits.CollectivePwrCtrlFlagbits &= 0xFFFE;        
        if(App_HMI_useRefFromGUI) pwr_ctrl_flagbits.CollectivePwrCtrlFlagbits |= 0x0002;
        else pwr_ctrl_flagbits.CollectivePwrCtrlFlagbits &= 0xFFFD;        
        if(App_HMI_useFixedRef) pwr_ctrl_flagbits.CollectivePwrCtrlFlagbits |= 0x0004;
        else pwr_ctrl_flagbits.CollectivePwrCtrlFlagbits &= 0xFFFB;        

        Dev_GuiComm_Stream_SendWord(pwr_ctrl_flagbits.CollectivePwrCtrlFlagbits);

        //Message pos 9
        Dev_GuiComm_Stream_SendWord(Controller_4SWBB._DCBuckset);

        //Message pos 10
        Dev_GuiComm_Stream_SendWord(Controller_4SWBB._DCBoostset);

        //Message pos 11
        Dev_GuiComm_Stream_SendWord(Controller_4SWBB._CurrentControllerResult);

        Dev_GuiComm_Stream_FinishSending();
    }
}


//======================================================================================================================
// @brief handle the data that is coming from the GUI
//======================================================================================================================

#define GUI_CMD_SET_VOLTAGE         01
#define GUI_CMD_SET_REF_FROM_POTI   02


void App_GUI_ProtocolHandler(uint16_t length, uint8_t* data)
{
    uint8_t cmd = data[0];
    
    if (cmd == GUI_CMD_SET_REF_FROM_POTI)
    {
        if (!App_HMI_useFixedRef)
            App_HMI_useRefFromPoti = true;
        App_HMI_useRefFromGUI = false;
    }
    else if (cmd == GUI_CMD_SET_VOLTAGE)
    {
        if (length >= 3)
        {
            uint16_t Gui_VoutRef = ((data[1]) << 8) + data[2];
            Drv_PwrCtrl_4SWBB_SetReferenceRaw(Gui_VoutRef); // raw adc vaule
            App_HMI_useRefFromPoti = false;
            App_HMI_useRefFromGUI = true;
            App_HMI_useFixedRef = false;
        }
    }
}
