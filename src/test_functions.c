//----------------------------------------------------------
// #### MAGNET PROJECT - Custom Board ####
// ## Internal Test Functions Module
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### TEST_FUNCTIONS.C ###################################
//----------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "test_functions.h"
#include "hard.h"
#include "stm32f10x.h"
#include "gpio.h"
#include "adc.h"
#include "usart_channels.h"
#include "usart.h"
#include "dma.h"
#include "tim.h"

#include <stdio.h>
#include <string.h>


// Externals -------------------------------------------------------------------
extern volatile unsigned short adc_ch [];
extern volatile unsigned short wait_ms_var;
extern volatile unsigned short timer_standby;
// extern volatile unsigned char usart3_have_data;


// Globals ---------------------------------------------------------------------


// Module Private Functions ----------------------------------------------------
void TF_Act_Channels (void);
void TF_Enable_Channels (void);
void TF_Synchro_Channels (void);
void TF_Enable_5V_Comm (void);
void TF_Enable_Lcd (void);
void TF_Enable_Rpi (void);
void TF_Enable_Encoder (void);
void TF_Enable_Boost (void);

void TF_UsartChannel1_Tx (void);

// void TF_Adc_Usart1_Tx (void);
// void TF_Adc_Usart1_Voltages (void);


// Module Functions ------------------------------------------------------------
void TF_Hardware_Tests (void)
{
    // TF_Act_Channels ();
    // TF_Enable_Channels ();
    // TF_Synchro_Channels ();
    // TF_Enable_5V_Comm ();
    // TF_Enable_Lcd ();
    // TF_Enable_Rpi ();
    // TF_Enable_Encoder ();
    TF_Enable_Boost ();


// TF_UsartChannel1_Tx ();
    // TF_Usart1_Tx_String ();
    // TF_Adc_Usart1_Tx ();
    // TF_Adc_Usart1_Voltages ();

}


void TF_Act_Channels (void)
{
    while (1)
    {
        Wait_ms(1000);

        if (ACT_PROBE_CH1)
        {
            ACT_PROBE_CH1_OFF;
            ACT_PROBE_CH2_OFF;
            ACT_PROBE_CH3_OFF;
            ACT_PROBE_CH4_OFF;            
        }
        else
        {
            ACT_PROBE_CH1_ON;
            ACT_PROBE_CH2_ON;
            ACT_PROBE_CH3_ON;
            ACT_PROBE_CH4_ON;            
        }
    }
}


void TF_Enable_Channels (void)
{
    while (1)
    {
        Wait_ms(1000);

        if (ENA_CH1)
        {
            ENA_CH1_OFF;
            ENA_CH2_OFF;
            ENA_CH3_OFF;
            ENA_CH4_OFF;            
        }
        else
        {
            ENA_CH1_ON;
            ENA_CH2_ON;
            ENA_CH3_ON;
            ENA_CH4_ON;            
        }
    }
}


void TF_Synchro_Channels (void)
{
    while (1)
    {
        Wait_ms(1000);

        if (SYNC_CH1)
        {
            SYNC_CH1_OFF;
            SYNC_CH2_OFF;
            SYNC_CH3_OFF;
            SYNC_CH4_OFF;            
        }
        else
        {
            SYNC_CH1_ON;
            SYNC_CH2_ON;
            SYNC_CH3_ON;
            SYNC_CH4_ON;            
        }
    }
}


void TF_Enable_5V_Comm (void)
{
    while (1)
    {
        Wait_ms(1000);

        if (ENA_5V_COMM)
        {
            ENA_5V_COMM_OFF;
        }
        else
        {
            ENA_5V_COMM_ON;
        }
    }
}


void TF_Enable_Lcd (void)
{
    while (1)
    {
        Wait_ms(1000);

        if (ENA_LCD)
        {
            ENA_LCD_OFF;
        }
        else
        {
            ENA_LCD_ON;
        }
    }
}


void TF_Enable_Rpi (void)
{
    while (1)
    {
        Wait_ms(1000);

        if (ENA_RPI)
        {
            ENA_RPI_OFF;
        }
        else
        {
            ENA_RPI_ON;
        }
    }
}


void TF_Enable_Encoder (void)
{
    while (1)
    {
        Wait_ms(1000);

        if (ENA_ENCODER)
        {
            ENA_ENCODER_OFF;
        }
        else
        {
            ENA_ENCODER_ON;
        }
    }
}


void TF_Enable_Boost (void)
{
    while (1)
    {
        Wait_ms(1000);

        if (ENA_BOOST)
        {
            ENA_BOOST_OFF;
        }
        else
        {
            ENA_BOOST_ON;
        }
    }
}


void TF_UsartChannel1_Tx (void)
{
    UsartChannel1Config();
    
    while (1)
    {
        UsartChannel1Send("Mariano\n");
        Wait_ms(1000);
    }
}


// void TF_Adc_Usart1_Tx (void)
// {
//     char buff [100] = { 0 };

//     //-- Test ADC Multiple conversion Scanning (starts each sequence) and DMA 
//     //-- DMA configuration.
//     DMAConfig();
//     DMA_ENABLE;
    
//     //Uso ADC con DMA
//     AdcConfig();
//     // ADC_START;

//     Usart1Config ();
    
//     while (1)
//     {
//         for (int i = 0; i < ADC_CHANNEL_QUANTITY; i++)
//             adc_ch[i] = 0;
        
//         Wait_ms(1000);
//         Usart1Send("starting conversion with channels in:\n");
//         sprintf(buff, "%d %d %d %d %d %d\n",
//                 SENSE_POWER,
//                 SENSE_MEAS,
//                 V_SENSE_28V,
//                 V_SENSE_25V,
//                 V_SENSE_11V,
//                 V_SENSE_8V);
        
//         Usart1Send (buff);

//         LED_ON;
//         ADC_START;
//         Wait_ms(100);
//         LED_OFF;
        
//         Wait_ms(900);
//         Usart1Send("conversion ended:\n");
//         sprintf(buff, "%d %d %d %d %d %d\n",
//                 SENSE_POWER,
//                 SENSE_MEAS,
//                 V_SENSE_28V,
//                 V_SENSE_25V,
//                 V_SENSE_11V,
//                 V_SENSE_8V);
        
//         Usart1Send (buff);
        
//     }
//     //--- End Test ADC Multiple conversion Scanning Continuous Mode and DMA ----------------//        
// }


// void TF_Adc_Usart1_Voltages (void)
// {
//     char buff [100] = { 0 };

//     //-- Test ADC Multiple conversion Scanning (starts each sequence) and DMA 
//     //-- DMA configuration.
//     DMAConfig();
//     DMA_ENABLE;
    
//     //Uso ADC con DMA
//     AdcConfig();
//     // ADC_START;

//     Usart1Config ();

//     int calc_int, calc_dec;
//     while (1)
//     {
//         for (int i = 0; i < ADC_CHANNEL_QUANTITY; i++)
//             adc_ch[i] = 0;
        
//         Wait_ms(1000);
//         Usart1Send("starting conversion with channels in:\n");
//         sprintf(buff, "%d %d %d %d %d %d\n",
//                 SENSE_POWER,
//                 SENSE_MEAS,
//                 V_SENSE_28V,
//                 V_SENSE_25V,
//                 V_SENSE_11V,
//                 V_SENSE_8V);
        
//         Usart1Send (buff);

//         LED_ON;
//         ADC_START;
//         Wait_ms(100);
//         LED_OFF;
        
//         Wait_ms(900);
//         Usart1Send("conversion ended:\n");
//         sprintf(buff, "%d %d %d %d %d %d\n",
//                 SENSE_POWER,
//                 SENSE_MEAS,
//                 V_SENSE_28V,
//                 V_SENSE_25V,
//                 V_SENSE_11V,
//                 V_SENSE_8V);

//         // SENSE_POWER resistor multiplier 11
//         calc_int = SENSE_POWER * 330 * 11;
//         calc_int >>= 12;
//         calc_dec = calc_int;
//         calc_int = calc_int / 100;
//         calc_dec = calc_dec - calc_int * 100;
//         sprintf(buff, "Power: %d.%02d, ", calc_int, calc_dec);
//         Usart1Send (buff);

//         // SENSE_MEAS resistor multiplier 2
//         calc_int = SENSE_MEAS * 330 * 2;
//         calc_int >>= 12;
//         calc_dec = calc_int;
//         calc_int = calc_int / 100;
//         calc_dec = calc_dec - calc_int * 100;
//         sprintf(buff, "Meas: %d.%02d, ", calc_int, calc_dec);
//         Usart1Send (buff);

//         // V_SENSE_28V resistor multiplier 11
//         calc_int = V_SENSE_28V * 330 * 11;
//         calc_int >>= 12;
//         calc_dec = calc_int;
//         calc_int = calc_int / 100;
//         calc_dec = calc_dec - calc_int * 100;
//         sprintf(buff, "V28V: %d.%02d, ", calc_int, calc_dec);
//         Usart1Send (buff);

//         // V_SENSE_25V resistor multiplier 11
//         calc_int = V_SENSE_25V * 330 * 11;
//         calc_int >>= 12;
//         calc_dec = calc_int;
//         calc_int = calc_int / 100;
//         calc_dec = calc_dec - calc_int * 100;
//         sprintf(buff, "V25V: %d.%02d, ", calc_int, calc_dec);
//         Usart1Send (buff);

//         // V_SENSE_11V resistor multiplier 11
//         calc_int = V_SENSE_11V * 330 * 11;
//         calc_int >>= 12;
//         calc_dec = calc_int;
//         calc_int = calc_int / 100;
//         calc_dec = calc_dec - calc_int * 100;
//         sprintf(buff, "V11V: %d.%02d, ", calc_int, calc_dec);
//         Usart1Send (buff);

//         // V_SENSE_8V resistor multiplier 11
//         calc_int = V_SENSE_8V * 330 * 11;
//         calc_int >>= 12;
//         calc_dec = calc_int;
//         calc_int = calc_int / 100;
//         calc_dec = calc_dec - calc_int * 100;
//         sprintf(buff, "V8V: %d.%02d\n", calc_int, calc_dec);
//         Usart1Send (buff);
                
//     }
//     //--- End Test ADC Multiple conversion Scanning Continuous Mode and DMA ----------------//        
// }


//--- end of file ---//
