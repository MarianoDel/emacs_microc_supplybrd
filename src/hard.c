//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### HARD.C #################################
//---------------------------------------------
#include "hard.h"
#include "stm32f10x.h"
#include "supply.h"

#include <stdio.h>


// Module Private Types Constants and Macros -----------------------------------


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
volatile unsigned short hard_timer = 0;



// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
#ifdef HARDWARE_VERSION_2_0

void Hard_Timeouts (void)
{
    if (hard_timer)
	hard_timer--;
}


#ifdef USE_POWER_LED_INDICATOR
typedef enum {
    CHECK_STATUS_INIT,
    CHECK_STATUS,
    WAIT_NEXT,
    WAIT_NEXT_LOW_BATT
    
} pwr_led_status_e;
unsigned short hard_sw_power_on_cnt = 0;
unsigned short led_status_cnt = 0;
pwr_led_status_e pwr_status;
void Sw_Power_On_Update (void)
{
    if (hard_timer)
	return;

    switch (pwr_status)
    {
    case CHECK_STATUS_INIT:
	LED_STATUS_ON;
	hard_timer = 1;
	led_status_cnt = 0;
	pwr_status++;
	break;

    case CHECK_STATUS:
	if (SW_POWER_ON)
	{
	    if (hard_sw_power_on_cnt < 8)
		hard_sw_power_on_cnt++;
	}
	else if (hard_sw_power_on_cnt)
	    hard_sw_power_on_cnt--;


	if (led_status_cnt < 10)
	    led_status_cnt++;
	else
	{
	    LED_STATUS_OFF;

	    // show led status checking supply status
	    if (Supply_Get_Status() == 1)    // batt not good INIT_STANDBY state eq 1
		pwr_status = WAIT_NEXT_LOW_BATT;
	    else
		pwr_status++;
	}
	break;

    case WAIT_NEXT:
	if (led_status_cnt > 1000)
	{
	    pwr_status = CHECK_STATUS_INIT;
	    break;
	}

	if (Sw_Power_On())
	    LED_STATUS_ON;
	else
	    LED_STATUS_OFF;    
	    
	hard_timer = 1;
	led_status_cnt++;	
	break;

    case WAIT_NEXT_LOW_BATT:
	if (led_status_cnt > 2000)
	{
	    pwr_status = CHECK_STATUS_INIT;
	    break;
	}

	if (Sw_Power_On())
	{
	    if (((led_status_cnt > 0) && (led_status_cnt < 90)) ||
		((led_status_cnt > 240) && (led_status_cnt < 340)) ||
		((led_status_cnt > 490) && (led_status_cnt < 590)))
		LED_STATUS_ON;
	    else
		LED_STATUS_OFF;
	}
	else
	    LED_STATUS_OFF;    
	    
	hard_timer = 1;
	led_status_cnt++;	
	break;
	
    default:
	pwr_status = 0;
	break;
    }
}


// void Sw_Power_Led_Status (unsigned char new_status)
// {
//     led_status_from_supply = new_status;
// }


unsigned char Sw_Power_On (void)
{
    unsigned char a = 0;

    if (hard_sw_power_on_cnt >= 5)
	a = 1;
    
    return a; 
}
#else
unsigned short hard_sw_power_on_cnt = 0;
void Sw_Power_On_Update (void)
{
    if (hard_timer)
	return;

    hard_timer = 10;

    if (SW_POWER_ON)
    {
	if (hard_sw_power_on_cnt < 30)
	    hard_sw_power_on_cnt++;
    }
    else if (hard_sw_power_on_cnt)
	hard_sw_power_on_cnt--;
    
}


unsigned char Sw_Power_On (void)
{
    unsigned char a = 0;

    if (hard_sw_power_on_cnt > 15)
	a = 1;
    
    return a; 
}
#endif    // USE_POWER_LED_INDICATOR


unsigned char Led_Is_On (void)
{
    return LED;
}


void Led_On (void)
{
    LED_ON;
}


void Led_Off (void)
{
    LED_OFF;
}



unsigned char OnOff_Is_On (void)
{
    return ON_OFF;
}


void OnOff_On (void)
{
    ON_OFF_ON;
}


void OnOff_Off (void)
{
    ON_OFF_OFF;
}

#endif

#ifdef HARDWARE_VERSION_1_0
#include "adc.h"
#include "tim.h"
#include "usart.h"

extern volatile unsigned short adc_ch [];

void Ena_Ch1_On (void)
{
    ENA_CH1_ON;
}


void Ena_Ch2_On (void)
{
    ENA_CH2_ON;
}


void Ena_Ch3_On (void)
{
    ENA_CH3_ON;
}
    
void Ena_Ch4_On (void)
{
    ENA_CH4_ON;
}


void Ena_Ch1_Off (void)
{
    ENA_CH1_OFF;
}


void Ena_Ch2_Off (void)
{
    ENA_CH2_OFF;
}


void Ena_Ch3_Off (void)
{
    ENA_CH3_OFF;
}


void Ena_Ch4_Off (void)
{
    ENA_CH4_OFF;
}


void Act_Probe_Ch1_Off (void)
{
    ACT_PROBE_CH1_OFF;
}


void Act_Probe_Ch2_Off (void)
{
    ACT_PROBE_CH2_OFF;
}


void Act_Probe_Ch3_Off (void)
{
    ACT_PROBE_CH3_OFF;
}


void Act_Probe_Ch4_Off (void)
{
    ACT_PROBE_CH4_OFF;
}


void Act_Probe_Ch1_On (void)
{
    ACT_PROBE_CH1_ON;
}


void Act_Probe_Ch2_On (void)
{
    ACT_PROBE_CH2_ON;
}


void Act_Probe_Ch3_On (void)
{
    ACT_PROBE_CH3_ON;
}


void Act_Probe_Ch4_On (void)
{
    ACT_PROBE_CH4_ON;
}


void Hard_GetHardSoft (char * buff)
{
    sprintf(buff, "%s %s\n", HARD, SOFT);
}


void Hard_GetVoltages (char * buff)
{
    ADC_START;
    Wait_ms(1);

    sprintf(buff, "%d %d %d %d %d %d\n",
            SENSE_POWER,
            SENSE_MEAS,
            V_SENSE_28V,
            V_SENSE_25V,
            V_SENSE_11V,
            V_SENSE_8V);

}


void Hard_GetVoltages_Complete (void)
{
    char buff [50];
    int calc_int, calc_dec;
    
    ADC_START;
    Wait_ms(1);

    sprintf(buff, "%d %d %d %d %d %d\n",
            SENSE_POWER,
            SENSE_MEAS,
            V_SENSE_28V,
            V_SENSE_25V,
            V_SENSE_11V,
            V_SENSE_8V);

    Usart1Send (buff);

    // SENSE_POWER resistor multiplier 11
    calc_int = SENSE_POWER * 330 * 11;
    calc_int >>= 12;
    calc_dec = calc_int;
    calc_int = calc_int / 100;
    calc_dec = calc_dec - calc_int * 100;
    sprintf(buff, "Power: %d.%02dV, ", calc_int, calc_dec);
    Usart1Send (buff);

    // SENSE_MEAS resistor multiplier 2
    calc_int = SENSE_MEAS * 330 * 2;
    calc_int >>= 12;
    calc_dec = calc_int;
    calc_int = calc_int / 100;
    calc_dec = calc_dec - calc_int * 100;
    sprintf(buff, "Meas: %d.%02dV, ", calc_int, calc_dec);
    Usart1Send (buff);

    // V_SENSE_28V resistor multiplier 11
    calc_int = V_SENSE_28V * 330 * 11;
    calc_int >>= 12;
    calc_dec = calc_int;
    calc_int = calc_int / 100;
    calc_dec = calc_dec - calc_int * 100;
    sprintf(buff, "V28: %d.%02dV, ", calc_int, calc_dec);
    Usart1Send (buff);

    // V_SENSE_25V resistor multiplier 11
    calc_int = V_SENSE_25V * 330 * 11;
    calc_int >>= 12;
    calc_dec = calc_int;
    calc_int = calc_int / 100;
    calc_dec = calc_dec - calc_int * 100;
    sprintf(buff, "V25: %d.%02dV, ", calc_int, calc_dec);
    Usart1Send (buff);

    // V_SENSE_11V resistor multiplier 11
    calc_int = V_SENSE_11V * 330 * 11;
    calc_int >>= 12;
    calc_dec = calc_int;
    calc_int = calc_int / 100;
    calc_dec = calc_dec - calc_int * 100;
    sprintf(buff, "V11: %d.%02dV, ", calc_int, calc_dec);
    Usart1Send (buff);

    // V_SENSE_8V resistor multiplier 11
    calc_int = V_SENSE_8V * 330 * 11;
    calc_int >>= 12;
    calc_dec = calc_int;
    calc_int = calc_int / 100;
    calc_dec = calc_dec - calc_int * 100;
    sprintf(buff, "V8: %d.%02dV\n", calc_int, calc_dec);
    Usart1Send (buff);
    
}

void Tx_Pin_On (void)
{
    PB7_ON;
}


void Tx_Pin_Off (void)
{
    PB7_OFF;
}

unsigned char Rx_Pin (void)
{
    return PB6;
}
#endif    // HARDWARE_VERSION_1_0
//--- end of file ---//
