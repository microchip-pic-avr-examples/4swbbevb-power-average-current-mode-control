/**
  @Company
    Microchip Technology Inc.

  @File Name
    dev_button.c

  @Summary
    This is the generated driver implementation file using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for the button driver. This driver is not suited for multiple buttons but only for one button.
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
#include "dev_button.h"
#include "mcc_generated_files/pin_manager.h"

#define BUTTON_PRESSED   0 // low level active button
#define BUTTON_RELEASED  1
#define SCAN_PERIOD      (10)   // 10ms
#define DEBOUNCE_TIME    (20 / SCAN_PERIOD)   // 20ms
#define LONG_PRESS_TIME  (1500 / SCAN_PERIOD) //1500ms !!max 2.5seconds !!

typedef struct
{
    uint8_t debouncer;  // debouncing timer
    uint8_t timer;      // long press timer
    uint8_t event;      // event to be passed to the calling function
    bool    state;      // previous state
} button_data_t;

static volatile button_data_t buttonData;

//======================================================================================================================
// @brief initializes the button device, needs to be called once at boot up before that device can be used
//======================================================================================================================
void Dev_Button_Init(void)
{
    buttonData.debouncer = 0;
    buttonData.timer     = 0;
    buttonData.event     = 0;
    buttonData.state     = BUTTON_RELEASED;
}

//======================================================================================================================
// @brief this function returns the event of the button like specified in the Dev_BUTTON_EVENT_* events above
// @returns event of the button
//======================================================================================================================
uint8_t Dev_Button_GetEvent(void)
{
    uint8_t ret = buttonData.event;
    buttonData.event = DEV_BUTTON_EVENT_NONE;
    return ret;
}

//======================================================================================================================
// @brief this function evaluates the button status/events and needs to be called approximately every 10 ms
//======================================================================================================================
void Dev_Button_Task_10ms(void)
{
    bool input_state = Button_SW2_GetValue();    
    bool last_state = buttonData.state;

    if(input_state == last_state)
    {
        buttonData.debouncer = 0; // (re)start debouncing counter
        if((input_state == BUTTON_PRESSED) && (buttonData.timer < LONG_PRESS_TIME))
        {
            buttonData.timer++;
            if(buttonData.timer == LONG_PRESS_TIME)
                buttonData.event = DEV_BUTTON_EVENT_PRESSED_LONG;
        }
    }
    else
    {
        buttonData.debouncer++; // increment debouncing counter
        if(buttonData.debouncer == DEBOUNCE_TIME)
        {
            buttonData.state = input_state; //debounce time passed, then accept the new state
            buttonData.debouncer = 0;
            if((input_state == BUTTON_RELEASED) && (buttonData.timer < LONG_PRESS_TIME))
            {
                buttonData.event = DEV_BUTTON_EVENT_PRESSED_SHORT;
            }
            buttonData.timer = 0;
        }
    }
}


