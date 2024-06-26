//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### I2C_DRIVER.C ##########################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "i2c_driver.h"
#include "i2c.h"
#include "tim.h"
#include "stm32f10x.h"
#include "usart.h"
#include "usart_channels.h"

#include "hard.h"

#include <string.h>
// Private Types Constants and Macros ------------------------------------------


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
// unsigned char encoder_data [8] = { 0 };
unsigned char encoder_data [10] = { 0 };
volatile unsigned char i2c_driver_tt = 0;



// Module Private Functions ----------------------------------------------------
void i2c_send_eight (unsigned char * tt);
unsigned char i2c_send_ten (unsigned char * tt);
void i2c_validate_encod_data (char * buff);


// Module Funtions -------------------------------------------------------------
void i2c_driver_timeouts (void)
{
    if (i2c_driver_tt)
        i2c_driver_tt--;
}


unsigned short i2c_driver_error_cnt = 0;
void i2c_driver_update (void)
{
    if (I2C1->SR1 & I2C_SR1_ADDR)
    {
        char buff [120];
        char * pb = buff;
        unsigned char len = 0;
        unsigned char end_rx = 1;
        unsigned short dummy = 0;

        // clean addr
        dummy = I2C1->SR2;
        dummy++;

        // check for Tx
        i2c_driver_tt = 100;
        if (I2C1->SR1 & I2C_SR1_TXE)
        {
            unsigned char error = 0;
            // i2c_send_eight ((unsigned char *) &i2c_driver_tt);
            error = i2c_send_ten ((unsigned char *) &i2c_driver_tt);
            
            // free lines on slave
            I2C1->CR1 |= I2C_CR1_STOP;
            I2C1->CR1 &= ~I2C_CR1_STOP;

            // if (error)
            // {
            //     if (i2c_driver_error_cnt < 100)
            //         i2c_driver_error_cnt++;
            //     else
            //     {
            //         i2c_driver_error_cnt = 0;
            //         I2C1_Shutdown();
            //         Wait_ms(10);
            //         I2C1_Init();
            //     }
            // }
            return;
        }

        // check rx data
        i2c_driver_tt = 100;
        do {
            if (I2C1->SR1 & I2C_SR1_RXNE)
            {
                *pb = I2C1->DR;
                pb++;
                len++;
            }

            if (I2C1->SR1 & I2C_SR1_STOPF)
            {
                // I2C1->CR1 &= ~I2C_CR1_STOP;                    
                end_rx = 0;
            }

            if (!i2c_driver_tt)
                end_rx = 0;
                
        } while (end_rx);

        // free lines on slave
        I2C1->CR1 |= I2C_CR1_STOP;
        buff[len] = '\n';
        buff[len+1] = '\0';
        // i2c_validate_encod_data (buff);
        UsartRpiSend(buff);
        I2C1->CR1 &= ~I2C_CR1_STOP;
    }
}


// unsigned char dummy_vec [8] = { 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF };
void i2c_send_eight (unsigned char * tt)
{
    unsigned char loop_data = 0;
    int loop = 1;
    
    while (loop)
    {
        // reset ack fail
        I2C1->SR1 &= ~I2C_SR1_AF;
        I2C1->DR = encoder_data[loop_data];
        // I2C1->DR = 0x55;
        // I2C1->DR = dummy_vec[loop_data];
        
        if (loop_data < 8)
            loop_data++;
        else
            loop_data = 0;

        // wait for TxE or tt
        while ((!(I2C1->SR1 & I2C_SR1_TXE)) &&
               (*tt));

        if ((I2C1->SR1 & I2C_SR1_AF) ||
            (*tt == 0))
            loop = 0;
    }
}


unsigned char i2c_send_ten (unsigned char * tt)
{
    unsigned char loop_data = 0;
    int loop = 1;
    
    while (loop)
    {
        // reset ack fail
        I2C1->SR1 &= ~I2C_SR1_AF;
        I2C1->DR = encoder_data[loop_data];
        // I2C1->DR = 0x55;
        // I2C1->DR = dummy_vec[loop_data];
        
        if (loop_data < 10)
            loop_data++;
        else
            loop_data = 0;

        // wait for TxE or tt
        while ((!(I2C1->SR1 & I2C_SR1_TXE)) &&
               (*tt));

        if ((I2C1->SR1 & I2C_SR1_AF) ||
            (*tt == 0))
            loop = 0;
    }

    if (*tt == 0)
        return 1;

    return 0;
}


void i2c_driver_set_encod (unsigned char encoder_num, unsigned char encoder_pos)
{
    encoder_data[encoder_num] = encoder_pos;
}


void i2c_validate_encod_data (char * buff)
{
    // enc 2 pos 1\n
    // enc 2 pos 10\n
    unsigned char len = strlen(buff);
    if ((strncmp ((buff + 0), "enc ", sizeof("enc ") - 1) == 0) &&
        (strncmp ((buff + 6), "pos ", sizeof("pos ") - 1) == 0))
    {
        char * penc = buff + sizeof("enc ") - 1;
        char * ppos = buff + sizeof("enc 0 pos ") - 1;       
        // check valid encoder number
        if ((*penc >= '0') && (*penc <= '7') &&
            (*ppos >= '0') && (*ppos <= '9'))
        {
            if (len == 12)    // one position digit
            {
                i2c_driver_set_encod (*penc - '0', *ppos - '0');
            }
            else if ((*(ppos + 1) >= '0') && (*(ppos + 1) <= '9'))
            {
                unsigned char posi;
                posi = *ppos - '0';
                posi = posi * 10;
                posi = posi + *(ppos + 1) - '0';

                i2c_driver_set_encod (*penc - '0', posi);
            }
        }
    }
}
//--- end of file ---//
