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
 * @file dev_gui_comm.c
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

#include "stdint.h"
#include "stdbool.h"
#include <stddef.h>
#include "dev_gui_comm.h"

//-----------------------------------------------------------
// Configure Datastream function

//-----------------------------------------------------------


#define GUICOMM_RCV_DATABUFFER_SIZE     10
#define GUICOMM_PROTOCOL_ID_DATASTREAM_H  0x00
#define GUICOMM_PROTOCOL_ID_DATASTREAM_L  0x01
#define GUICOMM_PROTOCOL_ID_RCV_COMMANDS_H  0x00
#define GUICOMM_PROTOCOL_ID_RCV_COMMANDS_L  0x02
#define GUICOMM_PROTOCOL_ID_RCV_COMMANDS  0x0002

#define GUICOMM_PROTOCOL_ID_SCOPE_H   0x53
#define GUICOMM_PROTOCOL_ID_SCOPE_L   0x44

#define GUICOMM_PROTOCOL_ID_RCV_SCOPE_COMMAND_H 0x53
#define GUICOMM_PROTOCOL_ID_RCV_SCOPE_COMMAND_L 0x43
#define GUICOMM_PROTOCOL_ID_RCV_SCOPE_COMMAND   0x5343

#define GUICOMM_BEGIN_OF_FRAME	0x55
#define GUICOMM_END_OF_FRAME	0x0d
#define GUICOMM_CRC_POLYNOM		0xA001

//-----------------------------------------------------------
// Datastream defines and variables
#define DATASTREAM_MODE_WAITING         0
#define DATASTREAM_MODE_RECORDING       1
#define DATASTREAM_MODE_TRANSMIT_DATA   2

uint8_t  gui_stream_mode = DATASTREAM_MODE_WAITING;
uint8_t  gui_stream_sm_state = 0;
uint16_t gui_stream_waitcounter = 0;
uint16_t gui_stream_length;
uint16_t gui_stream_data[DATASTREAM_NUM_OF_VALUES];

typedef struct{
    uint16_t protocol_ID;
    GuiComm_CommandHandlerFunc func;
} rcvCommandHandler_t;

rcvCommandHandler_t rcvCommandHandler;

//-----------------------------------------------------------


//-----------------------------------------------------------
// Scope defines and variables
#define SCOPE_MODE_WAITING          0
#define SCOPE_MODE_RECORDING        1
#define SCOPE_MODE_TRANSMIT_DATA    2

uint8_t  gui_scope_mode = SCOPE_MODE_WAITING;
uint8_t  gui_scope_sm_state = 0;
uint16_t gui_scope_index;
uint16_t gui_scope_data[SCOPE_NUM_OF_CHANNELS * SCOPE_NUM_OF_SAMPLES];
//-----------------------------------------------------------
uint8_t gui_header[20];
uint16_t gui_transfer_index = 0;
bool transfer_highbyte = true;

uint8_t rcv_data_buffer[GUICOMM_RCV_DATABUFFER_SIZE];
uint16_t dataCRC;


//void Dev_Gui_Comm_SendDataByte(uint8_t data);
//void Dev_Gui_Comm_SendDataWord(uint16_t data);

void GuiComm_Scope_TransmitData(void);
void GuiComm_DataStream_TransmitData(void);

void GuiComm_Rcv_Task(void);

/**
 * \brief Function to register a receive function for a certain protocol id
 * @param rcvCmdFunc pointer to the receive function
 * @param cmdID protocol id of the data packet that will be handled by this function
 */
void Dev_GuiComm_Register_ProtocolHandler(GuiComm_CommandHandlerFunc rcvCmdFunc, uint16_t cmdID)
{
    rcvCommandHandler.protocol_ID = cmdID;
    rcvCommandHandler.func = rcvCmdFunc;
}


/**
 * \brief Init function to initialze this protocol layer. Needs to be called before using other functions
 */
void Dev_GuiComm_Init(void)
{
    rcvCommandHandler.protocol_ID = 0;
    rcvCommandHandler.func = NULL;    
}


/**
 * \brief Task function that needs to be called every 100µs to be able to handle 115 kbaud
 */
void Dev_GuiComm_Task_100us(void)
{
    GuiComm_Rcv_Task();
    if (gui_scope_mode == SCOPE_MODE_TRANSMIT_DATA)
    {
        GuiComm_Scope_TransmitData();
    }
    else if (gui_stream_mode == DATASTREAM_MODE_TRANSMIT_DATA)
    {
        GuiComm_DataStream_TransmitData();
    }
    
    if (gui_stream_mode == DATASTREAM_MODE_WAITING)
    {
        if (gui_stream_waitcounter < DATASTREAM_WAITTICKS)
        {
            gui_stream_waitcounter++;
        }
        else
        {
            gui_stream_waitcounter = 0;
            gui_stream_length = 0;
            gui_stream_mode = DATASTREAM_MODE_RECORDING;
        }
    }
}

/**
 * \brief Function that needs to be called before starting the sending of a gui data block
 * @return true, if send data function can be called
 */
bool Dev_GuiComm_Stream_ReadyToSend(void)
{
    if (gui_stream_mode == DATASTREAM_MODE_RECORDING)
        return true;
    else
        return false;
}


/**
 * \brief Function to be called after sending the last gui data word.
 */
void Dev_GuiComm_Stream_FinishSending(void)
{
    if (gui_stream_mode == DATASTREAM_MODE_RECORDING)
        gui_stream_mode = DATASTREAM_MODE_TRANSMIT_DATA;
}


/**
 * \brief Internal function to start the scope data transmission
 */
void GuiComm_Scope_StartTransmit(void)
{
    gui_scope_sm_state = 0;
    gui_scope_mode = SCOPE_MODE_TRANSMIT_DATA;
}

/**
 * \brief Internal function that contains the state machine to transmit scope data to the gui
 */
void GuiComm_Scope_TransmitData(void)
{
    switch (gui_scope_sm_state)
    {
        case 0:     //prepare header
            gui_header[0] = GUICOMM_BEGIN_OF_FRAME;
            gui_header[1] = GUICOMM_PROTOCOL_ID_SCOPE_H;
            gui_header[2] = GUICOMM_PROTOCOL_ID_SCOPE_L;
            uint16_t length = 8 + (SCOPE_NUM_OF_CHANNELS * SCOPE_NUM_OF_SAMPLES * 2);
            gui_header[3] = length >> 8;
            gui_header[4] = length & 0xff;

            gui_header[5] = 3;      //time base 3..us
            gui_header[6] = 0;      //time base 100us H
            gui_header[7] = 100;    //time base 100us L
            gui_header[8] = 2;      //number of channels
            gui_header[9] = 0;      //channel info index - Vout
            gui_header[10] = 1;      //number info index - Iout
            gui_header[11] = (SCOPE_NUM_OF_SAMPLES) >> 8;   //number of sample per channel H
            gui_header[12] = (SCOPE_NUM_OF_SAMPLES) & 0xff; //number of sample per channel L
            gui_transfer_index = 0;
            gui_scope_sm_state = 1;
            break;
        
        case 1:   //transfer header
            while (GuiComm_ReadyToSend()) 
            {
                GuiComm_Write(gui_header[gui_transfer_index]);
                gui_transfer_index++;
                if (gui_transfer_index > 12)
                {
                    gui_transfer_index = 0;
                    transfer_highbyte = true;
                    gui_scope_sm_state = 2;
                    break;
                }
            }
            break;

        case 2:   //transfer data
            while (GuiComm_ReadyToSend())
            {
                //uint8_t* data = (uint8_t*)&scope_record_data;
                if (transfer_highbyte)
                {
                    GuiComm_Write(gui_scope_data[gui_transfer_index] >> 8);
                    transfer_highbyte = false;
                }
                else
                {
                    GuiComm_Write(gui_scope_data[gui_transfer_index] & 0xff);
                    transfer_highbyte = true;
                    gui_transfer_index++;
                }
                //GuiComm_Write(data[gui_transfer_index]);
                //gui_transfer_index++;
                if (gui_transfer_index >= SCOPE_NUM_OF_CHANNELS * SCOPE_NUM_OF_SAMPLES)
                {
                    gui_scope_sm_state = 3;
                    gui_transfer_index = 3;
                    break;
                }
            }
            break;
    
        case 3:   //transfer data
            gui_header[0] = 0;  //no crc
            gui_header[1] = 0;  //no crc
            gui_header[2] = GUICOMM_END_OF_FRAME;
            gui_transfer_index = 0;
            gui_scope_sm_state = 4;
            break;

        case 4:   //transfer header
            while (GuiComm_ReadyToSend())
            {
                GuiComm_Write(gui_header[gui_transfer_index]);
                gui_transfer_index++;
                if (gui_transfer_index >= 3)
                {
                    gui_scope_sm_state = 0;
                    gui_transfer_index = 0;
                    gui_scope_mode = SCOPE_MODE_WAITING;
                    break;
                }
            }
            break;
    }
}


/**
 * \brief Internal function that contains the state machine to transmit the gui data
 */
void GuiComm_DataStream_TransmitData(void)
{
 static uint16_t MsCounter_=0;  //@ftx
     switch (gui_stream_sm_state)
    {
        case 0:     //prepare header
            gui_header[0] = GUICOMM_BEGIN_OF_FRAME;
            gui_header[1] = GUICOMM_PROTOCOL_ID_DATASTREAM_H;
            gui_header[2] = GUICOMM_PROTOCOL_ID_DATASTREAM_L;
            uint16_t length =  gui_stream_length*2;
            gui_header[3] = length >> 8;
            gui_header[4] = length & 0xff;

            gui_transfer_index = 0;
            gui_stream_sm_state = 1;
            break;
        
        case 1:   //transfer header
            while (GuiComm_ReadyToSend()&& (MsCounter_++ > 1)) //@ftx
            {
                GuiComm_Write(gui_header[gui_transfer_index]);
                gui_transfer_index++;
                if (gui_transfer_index > 4)
                {
                    gui_transfer_index = 0;
                    transfer_highbyte = true;
                    gui_stream_sm_state = 2;
                    break;
                }
                MsCounter_ = 0; //@ftx
            }
            break;

        case 2:   //transfer data
            while (GuiComm_ReadyToSend() && (MsCounter_++ > 1)) //@ftx
            {
                if (transfer_highbyte)
                {
                    GuiComm_Write(gui_stream_data[gui_transfer_index] >> 8);
                    transfer_highbyte = false;
                }
                else
                {
                    GuiComm_Write(gui_stream_data[gui_transfer_index] & 0xff);
                    transfer_highbyte = true;
                    gui_transfer_index++;
                }
                if (gui_transfer_index >= gui_stream_length)
                {
                    gui_stream_sm_state = 3;
                    gui_transfer_index = 0;
                    break;
                }
                
                MsCounter_ = 0; //@ftx
            }
            break;
    
        case 3:   //transfer end header
            gui_header[0] = 0;  //no crc
            gui_header[1] = 0;  //no crc
            gui_header[2] = GUICOMM_END_OF_FRAME;
            gui_transfer_index = 0;
            gui_stream_sm_state = 4;
            break;

        case 4:   //transfer header
            while (GuiComm_ReadyToSend() && (MsCounter_++ > 1)) //@ftx
            {
                GuiComm_Write(gui_header[gui_transfer_index]);
                gui_transfer_index++;
                if (gui_transfer_index >= 3)
                {
                    gui_stream_sm_state = 0;
                    gui_transfer_index = 0;
                    gui_stream_mode = DATASTREAM_MODE_WAITING;
                    break;
                }
                MsCounter_ = 0; //@ftx
            }
            break;
    }
}

/**
 * \brief Function to send data words to the GUI. The function Dev_GuiComm_Stream_ReadyToSend needs to be called first
 * @param data word that need to sent to the gui
 */
void Dev_GuiComm_Stream_SendWord(uint16_t data)
{
    if (gui_stream_length < (DATASTREAM_NUM_OF_VALUES))
    {
//        datastream_data[gui_stream_length++] = data >> 8;
//        datastream_data[gui_stream_length++] = data & 0xff8;
        gui_stream_data[gui_stream_length++] = data;
    }    
}


/**
 * \brief Function to record one scope sample for two channels. This function is getting activated by
 * the function Dev_GuiComm_Scope_TriggerNow and gets deactivated automatically when the buffer is full
 * @param dataCh1 sample of channel 1
 * @param dataCh2 sample of channel 2
 */
void Dev_GuiComm_Scope_Record_2CH(uint16_t dataCh1, uint16_t dataCh2)
{
    if (gui_scope_mode == SCOPE_MODE_RECORDING)
    {
        gui_scope_data[gui_scope_index++] = dataCh1;
        gui_scope_data[gui_scope_index++] = dataCh2;
        if (gui_scope_index >= SCOPE_NUM_OF_CHANNELS * SCOPE_NUM_OF_SAMPLES)
        {
            gui_scope_index = 0;
            gui_scope_mode = SCOPE_MODE_TRANSMIT_DATA;            
        }
    }
}

/**
 * \brief Function to trigger the recording of one scope page
 */
void Dev_GuiComm_Scope_TriggerNow(void)
{
    if (gui_scope_mode == SCOPE_MODE_WAITING)
    {
        gui_scope_index = 0;
        gui_scope_mode = SCOPE_MODE_RECORDING;
    }
}

#define SCOPE_COMMAND_TRIGGER_NOW   0x01
/**
 * \brief Internal function to receive scope commands from the gui.
 * So far only the trigger-now function is supported
 * @param len
 * @param data
 */
inline void GuiComm_Scope_Protocol_Rcv(uint16_t len, uint8_t* data)
{
    if (len > 0)
    {
        uint8_t cmd = data[0];
        if (cmd == SCOPE_COMMAND_TRIGGER_NOW)
        {
            Dev_GuiComm_Scope_TriggerNow();
        }
    }
}

#define RCV_WAIT_FOR_STARTBYTE		0
#define RCV_READ_ID_HIGHBYTE		1
#define RCV_READ_ID_LOWBYTE			2
#define RCV_READ_LENGTH_HIGHBYTE	3
#define RCV_READ_LENGTH_LOWBYTE		4
#define RCV_READ_DATA				5
#define RCV_READ_CRC_HIGHBYTE		6
#define RCV_READ_CRC_LOWBYTE		7
#define RRC_READ_EOF				8


#define RCV_TIMEOUT 10000

/**
 * \brief Internal function that contains the statemachine to receive data from the gui
 */
void GuiComm_Rcv_Task(void)
{
  static uint8_t rcv_state = 0;
  static uint16_t rcv_protocol_id = 0;
  static uint16_t rcv_data_length = 0;
  static uint16_t rcv_data_index = 0;
  static uint16_t rcv_CRC_calculated = 0;
  static uint16_t rcv_CRC = 0;
  static uint16_t rcv_timeout = 0;
  uint8_t data;

  while (1)
  {
    if (GuiComm_IsRxReady() == false)
    {
      if (++rcv_timeout >= 10000)
      {
        rcv_timeout = 0;
        rcv_state = RCV_WAIT_FOR_STARTBYTE;
      }
      break;
    }
    // ok, we have some data coming in, lets read and process
    rcv_timeout = 0;
    data = GuiComm_Read();
    switch (rcv_state)
    {
      case RCV_WAIT_FOR_STARTBYTE:
        rcv_CRC_calculated = 0;
        if (data == GUICOMM_BEGIN_OF_FRAME)
          rcv_state = RCV_READ_ID_HIGHBYTE;
        break;

      case RCV_READ_ID_HIGHBYTE:
        rcv_protocol_id = data << 8;
        rcv_state = RCV_READ_ID_LOWBYTE;
        break;

      case RCV_READ_ID_LOWBYTE:
        rcv_protocol_id |= data;
        rcv_state = RCV_READ_LENGTH_HIGHBYTE;
        break;

      case RCV_READ_LENGTH_HIGHBYTE:
        rcv_data_length = data << 8;
        rcv_state = RCV_READ_LENGTH_LOWBYTE;
        break;

      case RCV_READ_LENGTH_LOWBYTE:
        rcv_data_length |= data;
        rcv_data_index = 0;
        rcv_state = RCV_READ_DATA;
        break;

      case RCV_READ_DATA:
        if (rcv_data_index < GUICOMM_RCV_DATABUFFER_SIZE)
        {
          rcv_data_buffer[rcv_data_index] = data;
          // todo: update calculated crc if needed (not implemented yet)
        }
        rcv_data_index++;
        if (rcv_data_index >= rcv_data_length) //are we finished receiving data???
        {
          rcv_state = RCV_READ_CRC_HIGHBYTE;
        }
        break;

      case RCV_READ_CRC_HIGHBYTE:
        rcv_CRC = data << 8;
        rcv_state = RCV_READ_CRC_LOWBYTE;
        break;

      case RCV_READ_CRC_LOWBYTE:
        rcv_CRC |= data;
        rcv_data_index = 0;
        rcv_state = RRC_READ_EOF;
        break;

      case RRC_READ_EOF:
        if (data == 0x0d)
        {
          // do we have to check the CRC first?
          if ((rcv_protocol_id == rcvCommandHandler.protocol_ID) &&
            (rcvCommandHandler.func != NULL))
            rcvCommandHandler.func(rcv_data_length, rcv_data_buffer);
          else if (rcv_protocol_id == GUICOMM_PROTOCOL_ID_RCV_SCOPE_COMMAND)
            GuiComm_Scope_Protocol_Rcv(rcv_data_length, rcv_data_buffer);
        }
        rcv_protocol_id = 0;
        rcv_data_length = 0;
        rcv_data_index = 0;
        rcv_CRC_calculated = 0;
        rcv_CRC = 0;
        rcv_state = RCV_WAIT_FOR_STARTBYTE;
        break;
    }
  }
}

