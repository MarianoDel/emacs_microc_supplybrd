//---------------------------------------------------------------
// #### PROJECT MICRO-CURRENTS SUPPLY-BRD F103 - Custom Board ###
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### MAIN.C ##################################################
//---------------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "stm32f10x.h"
#include "hard.h"

#include "adc.h"
#include "dma.h"
#include "dac.h"
#include "tim.h"
#include "gpio.h"
#include "usart_channels.h"
#include "usart.h"
#include "i2c.h"

#include "comms.h"
#include "test_functions.h"


#include <stdio.h>
#include <string.h>


// Private Types Constants and Macros ------------------------------------------


// Externals -------------------------------------------------------------------
//--- Externals from timers
volatile unsigned short timer_standby = 0;
volatile unsigned short wait_ms_var = 0;

//--- Externals from adc
volatile unsigned short adc_ch [ADC_CHANNEL_QUANTITY];


// Globals ---------------------------------------------------------------------
// volatile unsigned short timer_sync_xxx_ms = 0;
// volatile unsigned short timer_out4 = 0;

// parameters_typedef * pmem = (parameters_typedef *) (unsigned int *) FLASH_PAGE_FOR_BKP;	//en flash
// parameters_typedef mem_conf;


// Module Private Functions ----------------------------------------------------
void TimingDelay_Decrement(void);
void SysTickError (void);



// Module Functions ------------------------------------------------------------
int main (void)
{
    // Gpio Configuration.
    GpioInit();
    
    // Systick Timer Activation
    if (SysTick_Config(64000))
        SysTickError();

    // Hardware Tests
    // TF_Hardware_Tests ();

    // --- main program inits. ---
    // //-- DMA configuration.
    // DMAConfig();
    // DMA_ENABLE;
    
    // //-- ADC with DMA
    // AdcConfig();
    // // ADC_START;

    // //-- DAC init for signal generation
    // DAC_Config ();
    // DAC_Output1(1400);
    
    // //-- Comms with rasp
    // Usart1Config ();

    // //-- Comms with probes
    // // Usart3Config ();

    // //-- TIM1 for signals module sequence ready
    // TIM6_Init();
    // TIM7_Init();
    //-- Reboot Encoders Board
    ENA_ENCODER_OFF;
    Wait_ms(300);
    ENA_ENCODER_ON;
    //-- Comms with encoders board
    I2C1_Init();
    I2C1_OwnAddress (0x44);
    I2C1_Ack (1);
    
    //-- Starts with all channels disabled
    Ena_Ch1_Off();
    Ena_Ch2_Off();
    Ena_Ch3_Off();
    Ena_Ch4_Off();

    //-- Comms with rasp & channels
    UsartRpiConfig ();
    UsartChannel1Config ();
    UsartChannel2Config ();

    char buff [120];
    char buff_tx [128];            
    
    //-- Main Loop --------------------------
    while (1)
    {
        // update the channels & rpi comms
        Comms_Update ();

        // channels bridge
        if (UsartChannel1HaveData())
        {
            UsartChannel1HaveDataReset();
            UsartChannel1ReadBuffer(buff, 128);
            sprintf(buff_tx,"ch1 %s\n", buff);
            UsartRpiSend(buff_tx);
        }

        if (UsartChannel2HaveData())
        {
            UsartChannel2HaveDataReset();
            UsartChannel2ReadBuffer(buff, 128);
            sprintf(buff_tx,"ch2 %s\n", buff);
            UsartRpiSend(buff_tx);
        }

        // rx from I2C
        if (I2C1->SR1 & I2C_SR1_ADDR)
        {
            char * pb = buff;
            unsigned char len = 0;
            unsigned char end_rx = 1;
            unsigned short dummy = 0;

            dummy = I2C1->SR2;
            dummy++;
            
            timer_standby = 5;
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

                if (!timer_standby)
                    end_rx = 0;
                
            } while (end_rx);

            // free lines on slave
            I2C1->CR1 |= I2C_CR1_STOP;
            buff[len] = '\n';
            buff[len+1] = '\0';
            UsartRpiSend(buff);
            I2C1->CR1 &= ~I2C_CR1_STOP;
        }
    }
}

//--- End of Main ---//


// Other Module Functions ------------------------------------------------------
// extern void TF_Prot_Int_Handler (unsigned char ch);
// void EXTI2_IRQHandler (void)
// {
//     if(EXTI->PR & EXTI_PR_PR2)    //Line2
//     {
//         Signals_Overcurrent_Handler (CH3);
//         // TF_Prot_Int_Handler (3);    // PROT_CH3 for tests
//         EXTI->PR |= EXTI_PR_PR2;
//     }
// }


// void EXTI4_IRQHandler (void)
// {
//     if(EXTI->PR & EXTI_PR_PR4)    //Line4
//     {
//         Signals_Overcurrent_Handler (CH4);        
//         // TF_Prot_Int_Handler (4);    // PROT_CH4 for tests
//         EXTI->PR |= EXTI_PR_PR4;
//     }
// }


// void EXTI15_10_IRQHandler (void)
// {
//     if(EXTI->PR & EXTI_PR_PR13)    //Line13
//     {
//         Signals_Overcurrent_Handler (CH2);
//         // TF_Prot_Int_Handler (2);    // PROT_CH2 for tests
//         EXTI->PR |= EXTI_PR_PR13;
//     }
//     else if (EXTI->PR & EXTI_PR_PR15)    //Line15
//     {
//         Signals_Overcurrent_Handler (CH1);        
//         // TF_Prot_Int_Handler (1);    // PROT_CH1 for tests
//         EXTI->PR |= EXTI_PR_PR15;
//     }
// }


void TimingDelay_Decrement(void)
{
    if (wait_ms_var)
        wait_ms_var--;

    if (timer_standby)
        timer_standby--;

// AntennaTimeouts ();

    // Treatment_Timeouts ();
    
    // HARD_Timeouts();
    
}


void SysTickError (void)
{
    //Capture systick error...
    while (1)
    {
        // if (LED)
        //     LED_OFF;
        // else
        //     LED_ON;

        for (unsigned char i = 0; i < 255; i++)
        {
            asm ("nop \n\t"
                 "nop \n\t"
                 "nop \n\t" );
        }
    }
}

//--- end of file ---//

