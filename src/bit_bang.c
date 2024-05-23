//----------------------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### BIT_BANG.C #########################################
//----------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "bit_bang.h"
#include "hard.h"
// #include "stm32f10x.h"
// #include "gpio.h"
// #include "adc.h"
// #include "usart_channels.h"
// #include "usart.h"
// #include "dma.h"
// #include "tim.h"

#include <stdio.h>
#include <string.h>


// Module Private Types Constants and Macros -----------------------------------
typedef enum {
    WAIT_START,
    BYTES_LEFT,
    SEND_START,
    SEND_BITS,
    SEND_STOP
    
} tx_states_e;


typedef enum {
    RX_CHECK_START,
    RX_WAIT_TRANSITION,
    RX_GET_BITS,
    RX_WAIT_LAST_TRANSITION,
    RX_CHECK_STOP
    
} rx_states_e;

// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
volatile unsigned char tx_buff[128] = { 0 };
volatile unsigned char tx_bytes_left = 0;
volatile unsigned char tx_state = 0;
volatile unsigned char tx_byte_to_send = 0;
volatile unsigned char tx_byte_cnt = 0;
volatile unsigned char tx_bit_cnt = 0;
volatile unsigned char tx_mask = 0;


volatile unsigned char rx_buff[128] = { 0 };
volatile unsigned char rx_state = 0;
volatile unsigned char rx_bits_cnt = 0;
volatile unsigned char rx_bytes_cnt = 0;
volatile unsigned char rx_byte = 0;
volatile unsigned char rx_ready = 0;

volatile unsigned char rx_int_handler = 0;
// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void Bit_Bang_Init (void)
{
    // init tx interface
    Tim_Bit_Bang_Tx_Init();
    tx_state = WAIT_START;
    Tx_Pin_On();
    Tim_Bit_Bang_Tx_Reset();

    // init rx interface
    Tim_Bit_Bang_Rx_Init();
    rx_bytes_cnt = 0;
    Rx_Int_Enable();    
}


void Bit_Bang_Shutdown (void)
{
    // shut timers and ints
    Tim_Bit_Bang_Tx_Reset();
    Tim_Bit_Bang_Rx_Reset();
    Rx_Int_Disable();    
}


void Bit_Bang_Tx_Send (char * buff)    
{
    unsigned char len = strlen(buff);
    if (len < 128)
    {
        tx_bytes_left = len;
        memcpy((unsigned char *) tx_buff, buff, tx_bytes_left);
        tx_state = BYTES_LEFT;
        tx_byte_cnt = 0;
        Tim_Bit_Bang_Tx_Set();
    }
}


void Bit_Bang_Tx_Tim_Handler (void)
{
    //
    switch (tx_state)
    {
    case WAIT_START:
        break;
        
    case BYTES_LEFT:
        if (tx_bytes_left)
        {
            tx_bytes_left--;
            tx_byte_to_send = tx_buff[tx_byte_cnt];
            tx_byte_cnt++;
            tx_state++;
            // printf(" bytes_left: %d\n", tx_bytes_left);
        }
        else
        {
            tx_byte_cnt = 0;
            Tim_Bit_Bang_Tx_Reset();
            tx_state = WAIT_START;
        }
        break;

    case SEND_START:
        Tx_Pin_Off();
        // tx_mask = 0x80;
        tx_mask = 0x01;        
        tx_bit_cnt = 0;
        tx_state++;
        break;

    case SEND_BITS:
        if (tx_bit_cnt < 7)
            tx_bit_cnt++;
        else
            tx_state++;

        // send bit0 first
        if (tx_byte_to_send & tx_mask)
            Tx_Pin_On();
            // Tx_Pin_Off();
        else
            Tx_Pin_Off();
            // Tx_Pin_On();
        
        // tx_mask >>= 1;
        tx_mask <<= 1;
        break;

    case SEND_STOP:
        Tx_Pin_On();
        tx_state = BYTES_LEFT;
        break;

    default:
        tx_state = WAIT_START;
        break;
    }    
}


void Bit_Bang_Rx_Int_Handler (void)
{
    // rx_int_handler = 1;
    // have to be the start
    Rx_Int_Disable();
    Tim_Bit_Bang_Rx_Set();
    rx_state = RX_CHECK_START;
}


void Bit_Bang_Rx_Tim_Handler (void)
{
    //
    switch (rx_state)
    {
    case RX_CHECK_START:
        if (Rx_Pin())
        {
            // not a start
            Tim_Bit_Bang_Rx_Reset();
            Rx_Int_Enable();
        }
        else
        {
            rx_state = RX_WAIT_TRANSITION;
            rx_bits_cnt = 0;
        }
        break;

    case RX_WAIT_TRANSITION:
        rx_state++;        
        break;
        
    case RX_GET_BITS:
        if (Rx_Pin())
        {
            rx_byte |= (1 << rx_bits_cnt);
        }
        
        if (rx_bits_cnt < 7)
        {
            rx_bits_cnt++;
            rx_state--;
        }
        else
        {
            rx_state++;
        }
        break;

    case RX_WAIT_LAST_TRANSITION:
        rx_state++;
        break;

    case RX_CHECK_STOP:
        if (Rx_Pin())
        {
            // valid byte
            if (rx_byte == '\r')
            {
                // discard silently
            }
            else if (rx_byte == '\n')
            {
                rx_ready = 1;
                rx_buff[rx_bytes_cnt] = 0;
                rx_byte = 0;
                // ready for new byte ???
                if (rx_bytes_cnt < 127 - 1)
                    rx_bytes_cnt++;
                else
                    rx_bytes_cnt = 0;                
            }
            else
            {
                // echo for debug
                // char echo_buff[10] = { 0 };
                // echo_buff[0] = rx_byte;
                
                // any other byte
                rx_buff[rx_bytes_cnt] = rx_byte;
                rx_byte = 0;
                if (rx_bytes_cnt < 127 - 1)
                    rx_bytes_cnt++;
                else
                    rx_bytes_cnt = 0;

                // send the echo
                // Bit_Bang_Tx_Send(echo_buff);
            }
        }

        // invalid byte or valid byte ready wait for next one
        Tim_Bit_Bang_Rx_Reset();
        Rx_Int_Enable();
        break;

    default:
        rx_state = RX_CHECK_START;
        break;
    }    
}


unsigned char Bit_Bang_Rx_Have_Data (void)
{
    return rx_ready;
}


unsigned char Bit_Bang_Rx_ReadBuffer (char * data_read)
{
    unsigned char len;
    len = strlen((const char *)rx_buff);
    memcpy(data_read, (unsigned char *) rx_buff, len);
    rx_ready = 0;
    rx_bytes_cnt = 0;
    return len;
}

//--- end of file ---//
