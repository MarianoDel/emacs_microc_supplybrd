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
#include "i2c_driver.h"

#include <stdio.h>
#include <string.h>


// Private Types Constants and Macros ------------------------------------------


// Externals -------------------------------------------------------------------
//--- Externals from timers
volatile unsigned short timer_standby = 0;
volatile unsigned short wait_ms_var = 0;
volatile unsigned char probe_detect_timer = 0;
volatile unsigned short probe_detect_filter = 0;

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
unsigned char Probe_Detect_Ch1 (void);
void Probe_Detect_Update (void);



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

    //-- Enable 5V to comms in channels
    Act_Probe_Ch1_On ();
    Act_Probe_Ch2_On ();

    // for (int i = 0; i < 10; i++)
    // {
    //     Wait_ms(1);
    //     SYNC_CH1_ON;
    //     Wait_ms(1);
    //     SYNC_CH1_OFF;
    // }
    
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
        i2c_driver_update ();
        
        // enable channel on probe detection
        Probe_Detect_Update ();

        // check ch1 enable
        if (Probe_Detect_Ch1 ())
            Ena_Ch1_On();
        else
            Ena_Ch1_Off();
        
    }
}

//--- End of Main ---//


// Other Module Functions ------------------------------------------------------
unsigned char Probe_Detect_Ch1 (void)
{
    if (probe_detect_filter > 60)
        return 1;

    return 0;
}


void Probe_Detect_Update (void)
{
    if (!probe_detect_timer)
    {
        probe_detect_timer = 1;

        if (PROBE_SENSE_CH1)
        {
            if (probe_detect_filter < 200)
                probe_detect_filter += 20;
        }
        else
        {
            if (probe_detect_filter)
                probe_detect_filter--;
        }
    }
}
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

    if (probe_detect_timer)
        probe_detect_timer--;

    i2c_driver_timeouts ();
    
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

