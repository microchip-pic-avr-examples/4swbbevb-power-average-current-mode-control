/**
  @Company
    Microchip Technology Inc.

  @File Name
    drv_led.c

  @Summary
    adc driver

  @Description
    This source file provides the functions to drive and control the HMI LEDs.
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
#include "drv_led.h"
#include "mcc_generated_files/pin_manager.h"

#define LED_STATE_OFF 0
#define LED_STATE_ON 1
#define LED_STATE_BLINK 2
#define LED_STATE_BLINK_SLOW 3
#define LED_STATE_BLINK_FAST 4

uint8_t ledstates[NUM_OF_LEDS];
uint8_t ledtimer[NUM_OF_LEDS];

/**
 * \brief Init function that contains the initialization of the LED driver. To be called before using any other function
 */
void Drv_LED_Init(void)
{
    //Peripherals are initialized by MCC
}


/**
 * \brief Function to switch on the LED with the given id
 * @param led_id id of the LED to be switched on
 */
void Drv_LED_On(uint8_t led_id)
{
    if (led_id >= NUM_OF_LEDS)
        return;
    if (ledstates[led_id] == LED_STATE_ON)
        return;
    ledstates[led_id] = LED_STATE_ON;
    switch(led_id)
    {
        case LED_PIM_RED:       PIM_LEDred_SetHigh(); break;
        case LED_BOARD_RED:     LED_Red_SetHigh(); break;
        case LED_BOARD_GREEN:   LED_Green_SetHigh(); break;
        default: Nop(); break;
    }
}

/**
 * \brief Function to switch off the LED with the given id
 * @param led_id id of the LED to be switched off
 */
void Drv_LED_Off(uint8_t led_id)
{
    if (led_id >= NUM_OF_LEDS)
        return;
    if (ledstates[led_id] == LED_STATE_OFF)
        return;
    ledstates[led_id] = LED_STATE_OFF;
    switch(led_id)
    {
        case LED_PIM_RED:       PIM_LEDred_SetLow(); break;
        case LED_BOARD_RED:     LED_Red_SetLow(); break;
        case LED_BOARD_GREEN:   LED_Green_SetLow(); break;
        default: Nop(); break;
    }
}

/**
 * \brief Function to toggle the LED with the given id
 * @param led_id id of the LED to be toggled
 */
void Drv_LED_Toggle(uint8_t led_id)
{
    switch(led_id)
    {
        case LED_PIM_RED:       PIM_LEDred_Toggle(); break;
        case LED_BOARD_RED:     LED_Red_Toggle(); break;
        case LED_BOARD_GREEN:   LED_Green_Toggle(); break;
        default: Nop(); break;
    }
}

/**
 * \brief Function to start blinking the LED with the given id with normal speed
 * @param led_id id of the LED to start blinking
 */
void Drv_LED_Blink(uint8_t led_id)
{
    if (led_id >= NUM_OF_LEDS)
        return;
    ledstates[led_id] = LED_STATE_BLINK;
}

/**
 * \brief Function to start blinking the LED with the given id with slow speed
 * @param led_id id of the LED to start blinking
 */
void Drv_LED_Blink_Slow(uint8_t led_id)
{
    if (led_id >= NUM_OF_LEDS)
        return;
    ledstates[led_id] = LED_STATE_BLINK_SLOW;
}

/**
 * \brief Function to start blinking the LED with the given id with fast speed
 * @param led_id id of the LED to start blinking
 */
void Drv_LED_Blink_Fast(uint8_t led_id)
{
    if (led_id >= NUM_OF_LEDS)
        return;
    ledstates[led_id] = LED_STATE_BLINK_FAST;
}

/**
 * \brief Task function that needs to be called every 100ms
 */
void Drv_LED_Task_100ms()
{
    uint8_t led_index=0;
    uint8_t timermax=0; 

    for (led_index = 0; led_index < NUM_OF_LEDS; led_index++)
    {
        if (ledstates[led_index] > LED_STATE_ON)
        {
            if (ledstates[led_index] == LED_STATE_BLINK_SLOW)
                timermax = 10;
            else if (ledstates[led_index] == LED_STATE_BLINK_FAST)
                timermax = 2;
            else    // normal speed
                timermax = 5;
            if (++ledtimer[led_index] >= timermax)
            {
                ledtimer[led_index] = 0;
                switch(led_index)
                {
                    case LED_PIM_RED:       PIM_LEDred_Toggle(); break;
                    case LED_BOARD_RED:     LED_Red_Toggle(); break;
                    case LED_BOARD_GREEN:   LED_Green_Toggle(); break;
                    default: Nop(); break;
                }
            }
        }
    }
}

