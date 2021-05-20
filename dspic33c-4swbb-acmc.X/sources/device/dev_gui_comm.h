/**
    (c) 2017 Microchip Technology Inc. and its subsidiaries. You may use this
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

/**
 * @file dev_gui_comm.h
 * @author M52409
 * @date 2020-ß7-30
 * @brief contains API functions to use the GUI via serial interface
 *
 * there are two standard communication data packages to the GUI:
 * 1.: data stream package: this package contains different single values
 *     that need to be transmitted cyclical like 10 times a second.
 *     for example, temperatires, voltages, statemachine states, bitfields,...
 * 2.: scope data package: this is a block of data that contains one page of scope data
 *     it can contain data from multiple data channels.
 *     example: to record real-time data for a step response of output voltage and current
 */

#ifndef DEV_GUICOMM_H
#define DEV_GUICOMM_H

#include "stdint.h"
#include "stdbool.h"

//abstraction of uart interface:
#include "../../mcc_generated_files/uart1.h"
#define GuiComm_ReadyToSend UART1_IsTxReady
#define GuiComm_Write       UART1_Write
#define GuiComm_IsRxReady   UART1_IsRxReady
#define GuiComm_Read        UART1_Read

//for buffer allocation of streaming data:
#define DATASTREAM_NUM_OF_VALUES 20
#define DATASTREAM_WAITTICKS  1000 //1000*100µs ticks ==> 100ms break between two gui data transmissions
//for buffer allocation of scope data:
#define SCOPE_NUM_OF_CHANNELS 2
#define SCOPE_NUM_OF_SAMPLES 1000


void Dev_GuiComm_Init(void);
void Dev_GuiComm_Task_100us(void);

bool Dev_GuiComm_Stream_ReadyToSend(void);
void Dev_GuiComm_Stream_SendWord(uint16_t data);
void Dev_GuiComm_Stream_FinishSending(void);

void Dev_GuiComm_Scope_Record_2CH(uint16_t dataCh1, uint16_t dataCh2);
void Dev_GuiComm_Scope_TriggerNow(void);

typedef void (*GuiComm_CommandHandlerFunc)(uint16_t, uint8_t*);
void Dev_GuiComm_Register_ProtocolHandler(GuiComm_CommandHandlerFunc, uint16_t cmdID);

#endif  //DEV_GUICOMM_H
