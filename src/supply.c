//------------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### SUPPLY.C #################################
//------------------------------------------------

// Includes --------------------------------------------------------------------
#include "supply.h"
#include "hard.h"
#include "adc.h"
#include "dsp.h"
#include "neopixel.h"
#include "neopixel_driver.h"
#include "boost.h"
#include "usart.h"

#include <stdio.h>


// Module Private Types Constants and Macros -----------------------------------
typedef enum {
    INIT_FILTERS,
    INIT_STANDBY,
    STANDBY_WITH_MAINS,
    STANDBY_WITH_BATT,
    POWERON_WITH_MAINS,
    POWERON_WITH_BATT
    
} supply_states_e;


// Externals -------------------------------------------------------------------
extern volatile unsigned short adc_ch [];


// Globals ---------------------------------------------------------------------
volatile unsigned short supply_timeout = 0;
volatile unsigned short supply_filter_timer = 0;
volatile unsigned short supply_neopixel_timer = 0;


ma16_u16_data_obj_t boost_f;
ma16_u16_data_obj_t mains_f;
ma16_u16_data_obj_t batt1_f;
ma16_u16_data_obj_t batt2_f;
ma16_u16_data_obj_t batt3_f;
ma16_u16_data_obj_t batt4_f;

unsigned short boost_filtered = 0;
unsigned short mains_filtered = 0;
unsigned short batt1_filtered = 0;
unsigned short batt2_filtered = 0;
unsigned short batt3_filtered = 0;
unsigned short batt4_filtered = 0;


// Module Private Functions ----------------------------------------------------
void Supply_Filters_Update (void);

unsigned short Supply_Get_Charge_Batt (void);
unsigned short Supply_Get_Mains (void);
unsigned short Supply_Get_Batt (unsigned char batt_num);

void Supply_Neopixel_Charging_With_Mains (void);
void Supply_Neopixel_With_Batt (void);
void Supply_Set_Neopixel_Voltage (pixel_t * ppix, unsigned short batt_volts);
unsigned short Supply_Get_Charge_Batt (void);
void Supply_Neopixel_Shutdown (void);

void Supply_Send_Voltage_SM (void);


// Module Functions ------------------------------------------------------------
void Supply_Timeouts (void)
{
    if (supply_timeout)
        supply_timeout--;

    if (supply_filter_timer)
        supply_filter_timer--;

    if (supply_neopixel_timer)
        supply_neopixel_timer--;
}


supply_states_e supply_state = INIT_FILTERS;
void Supply_Status_Reset (void)
{
    supply_state = INIT_FILTERS;
}


void Supply_Status (void)
{
    switch (supply_state)
    {
    case INIT_FILTERS:
	MA16_U16Circular_Reset (&boost_f);
	MA16_U16Circular_Reset (&mains_f);
	MA16_U16Circular_Reset (&batt1_f);
	MA16_U16Circular_Reset (&batt2_f);
	MA16_U16Circular_Reset (&batt3_f);
	MA16_U16Circular_Reset (&batt4_f);	

	// fill filters
	for (int i = 0; i < 16; i++)
	{
	    MA16_U16Circular (&boost_f, Sense_BOOST);
	    MA16_U16Circular (&mains_f, Sense_MAINS);	    
	    MA16_U16Circular (&batt1_f, Sense_BATT1);
	    MA16_U16Circular (&batt2_f, Sense_BATT2);
	    MA16_U16Circular (&batt3_f, Sense_BATT3);
	    MA16_U16Circular (&batt4_f, Sense_BATT4);
	}
	Usart3Send("\r\n");
	Usart3Send("to standby\r\n");	
	supply_state++;
	break;

    case INIT_STANDBY:

	if (supply_timeout)
	    break;
	    
	if (Supply_Get_Mains() > MAINS_10V)
	{
	    char buff [100];
	    sprintf(buff, "to mains standby v:%d\r\n", Supply_Get_Mains());
	    Usart3Send(buff);

	    supply_state = STANDBY_WITH_MAINS;
	    break;
	}

	if (Supply_Get_Charge_Batt () > BATT_7V)
	{
	    char buff [100];
	    sprintf(buff, "to batt standby v:%d\r\n", Supply_Get_Charge_Batt());
	    Usart3Send(buff);

	    supply_state = STANDBY_WITH_BATT;
	    break;
	}

	supply_timeout = 1000;
	break;
	
    case STANDBY_WITH_MAINS:
	if (Sw_Power_On())
	{
	    Usart3Send("to poweron with mains\r\n");
	    // TIM boost init
	    Boost_Start();
	    supply_state = POWERON_WITH_MAINS;
	    OnOff_On();    // for tests
	    break;
	}

	if (Supply_Get_Mains() < MAINS_10V)
	{
	    char buff [100];
	    sprintf(buff, "low mains go batt mains:%d batt:%d\r\n",
		    Supply_Get_Mains(),
		    Supply_Get_Charge_Batt());
	    Usart3Send(buff);

	    supply_state = STANDBY_WITH_BATT;
	    break;
	}
	    
	Supply_Neopixel_Charging_With_Mains();
	break;

    case STANDBY_WITH_BATT:
	if (Supply_Get_Charge_Batt () < BATT_7V)
	{
	    Usart3Send("to init standby low batt\r\n");	    
	    supply_state = INIT_STANDBY;
	    break;
	}

	if (Sw_Power_On())
	{
	    Usart3Send("to poweron with batt\r\n");
	    // TIM boost init
	    Boost_Start();
	    supply_state = POWERON_WITH_BATT;
	    break;
	}

	if (Supply_Get_Mains() > MAINS_10V)
	{
	    char buff [100];
	    sprintf(buff, "to mains standby v:%d\r\n", Supply_Get_Mains());
	    Usart3Send(buff);

	    supply_state = STANDBY_WITH_MAINS;
	    break;
	}
	
	Supply_Neopixel_With_Batt();
	break;

    case POWERON_WITH_MAINS:
	Boost_Update();
	// if ((Supply_Get_Boost() > BOOST_13V) &&
	//     (Supply_Get_Boost() < BOOST_15V))
	// {
	//     OnOff_On();
	// }
	// else
	// {
	//     OnOff_Off();
	// }

	if (!Sw_Power_On())
	{
	    Usart3Send("poweroff, to standby with mains\r\n");
	    // shutdowm
	    Boost_Stop();
	    OnOff_Off();
	    supply_state = STANDBY_WITH_MAINS;
	    break;
	}
	    
	if (Supply_Get_Mains() < MAINS_10V)
	{
	    Usart3Send("low mains, to poweron with batt\r\n");	    
	    // change to batt
	    supply_state = POWERON_WITH_BATT;
	}
	    
	Supply_Neopixel_Charging_With_Mains();
	break;

    case POWERON_WITH_BATT:
	Boost_Update();
	// if ((Supply_Get_Boost() > BOOST_13V) &&
	//     (Supply_Get_Boost() < BOOST_15V))
	if ( Supply_Get_Boost() < BOOST_15V)
	{
	    OnOff_On();
	}
	else
	{
	    OnOff_Off();
	}

	if (!Sw_Power_On())
	{
	    Usart3Send("poweroff, to standby with batt\r\n");
	    // shutdowm
	    Boost_Stop();
	    OnOff_Off();
	    supply_state = STANDBY_WITH_BATT;
	    break;
	}
	    
	if (Supply_Get_Mains() > MAINS_10V)
	{
	    Usart3Send("mains is good, to poweron with mains\r\n");
	    // change to mains
	    supply_state = POWERON_WITH_MAINS;
	}

	if (Supply_Get_Charge_Batt () < BATT_6_4V)
	{
	    Usart3Send("batt too low, shutdown, to init standby\r\n");	    
	    supply_state = INIT_STANDBY;
	    Supply_Neopixel_Shutdown();	    
	    break;
	}

	Supply_Neopixel_With_Batt();	    
	break;

    default:
	supply_state = INIT_FILTERS;
	break;
    }

    Supply_Filters_Update ();
    Supply_Send_Voltage_SM ();
}


// #define Sense_BOOST    adc_ch[0]
// #define Sense_MAINS    adc_ch[1]
// #define Sense_BATT1    adc_ch[2]
// #define Sense_BATT2    adc_ch[3]
// #define Sense_BATT3    adc_ch[4]
// #define Sense_BATT4    adc_ch[5]

unsigned char supply_filters = 0;
void Supply_Filters_Update (void)
{
    if (supply_filter_timer)
	return;

    supply_filter_timer = 2;

    switch (supply_filters)
    {
    case 0:
	boost_filtered = MA16_U16Circular (&boost_f, Sense_BOOST);
	supply_filters++;
	break;

    case 1:
	mains_filtered = MA16_U16Circular (&mains_f, Sense_MAINS);
	supply_filters++;
	break;

    case 2:
	batt1_filtered = MA16_U16Circular (&batt1_f, Sense_BATT1);
	supply_filters++;
	break;

    case 3:
	batt2_filtered = MA16_U16Circular (&batt2_f, Sense_BATT2);
	supply_filters++;
	break;

    case 4:
	batt3_filtered = MA16_U16Circular (&batt3_f, Sense_BATT3);
	supply_filters++;
	break;

    case 5:
	batt4_filtered = MA16_U16Circular (&batt4_f, Sense_BATT4);
	supply_filters = 0;
	break;
	
    default:
	supply_filters = 0;
	break;
    }
}


unsigned short Supply_Get_Mains (void)
{
    return mains_filtered;
}


unsigned short Supply_Get_Boost (void)
{
    return boost_filtered;
}


unsigned short Supply_Get_Batt (unsigned char batt_num)
{
    unsigned short a = 0;
    
    if (batt_num == 3)
	a = batt4_filtered;
    else if (batt_num == 2)
	a = batt3_filtered;
    else if (batt_num == 1)
	a = batt2_filtered;
    else
	a = batt1_filtered;

    return a;
}


void Supply_Neopixel_Charging_With_Mains (void)
{
    unsigned short charge = 0;

    if (supply_neopixel_timer)
	return;

    supply_neopixel_timer = 5;
    
    // check batt voltages
    charge = Supply_Get_Charge_Batt ();

    
}


unsigned char neo_cnt = 0;
unsigned char neo_light = 0;
void Supply_Neopixel_With_Batt (void)
{
    unsigned short charge = 0;

    if (supply_neopixel_timer)
	return;

    supply_neopixel_timer = 5;

    if (neo_cnt)
    {
	neo_cnt--;
	return;
    }

    // check batt voltages
    charge = Supply_Get_Charge_Batt ();
    
    if (!neo_light)
    {
	pixel_t my_pixel;

	Supply_Set_Neopixel_Voltage(&my_pixel, charge);
	Neo_Set_Pixel(0, &my_pixel);
	Neo_Driver_Send_Pixel_Data();

	neo_light = 1;
	neo_cnt = 4;	
    }
    else
    {
	pixel_t my_pixel;

	my_pixel.R = 0;
	my_pixel.G = 0;
	my_pixel.B = 0;

	Neo_Set_Pixel(0, &my_pixel);
	Neo_Driver_Send_Pixel_Data();

	neo_light = 0;
	neo_cnt = 200;	
    }
}


void Supply_Neopixel_Shutdown (void)
{
    pixel_t my_pixel;

    my_pixel.R = 0;
    my_pixel.G = 0;
    my_pixel.B = 0;

    Neo_Set_Pixel(0, &my_pixel);
    Neo_Driver_Send_Pixel_Data();
}


void Supply_Set_Neopixel_Voltage (pixel_t * ppix, unsigned short batt_volts)
{
    ppix->R = 0;
    ppix->G = 0;
    ppix->B = 0;

    if (batt_volts > BATT_7_2V)
	ppix->G = 255;
    else if (batt_volts > BATT_6_8V)
    {
	ppix->G = 127;
	ppix->R = 255 - 127;	
    }
    else if (batt_volts > BATT_6_4V)
    {
	ppix->G = 64;
	ppix->R = 255 - 64;	
    }
    else
	ppix->R = 255;
    
}


unsigned short Supply_Get_Charge_Batt (void)
{
    if (batt1_filtered > batt2_filtered)
    {
	if (batt1_filtered > batt3_filtered)
	{
	    if (batt1_filtered > batt4_filtered)
	    {
		return batt1_filtered;
	    }
	}
    }

    if (batt2_filtered > batt3_filtered)
    {
	if (batt2_filtered > batt4_filtered)
	{
	    return batt2_filtered;
	}
    }

    if (batt3_filtered > batt4_filtered)
    {
	return batt3_filtered;
    }
    else
	return batt4_filtered;
    
    return batt1_filtered;
}


char buff_volts[100];
unsigned char supply_send_voltage = 0;
void Supply_Send_Voltage_SM (void)
{
    unsigned char vint = 0;
    unsigned char vdec = 0;
    
    switch (supply_send_voltage)
    {
    case 0:
	break;

    case 1:
	// mode 0 unknow
	if (Supply_Get_Mode() == SUPPLY_MODE_BATT)
	{
	    Usart3Send("\r\n");
	    Usart3Send("supply battery ");
	}
	else if (Supply_Get_Mode() == SUPPLY_MODE_MAINS)
	{
	    Usart3Send("\r\n");	    
	    Usart3Send("supply mains ");
	}
	else
	{
	    supply_send_voltage = 0;
	    break;
	}
	    
	supply_send_voltage++;
	break;

    case 2:
	// send 12v input
	Supply_Convert_To_Volts (Supply_Get_Mains(),
				 &vint,
				 &vdec);

	sprintf(buff_volts, "%02d.%d ", vint, vdec);
	Usart3Send(buff_volts);
	supply_send_voltage++;
	break;

    case 3:
	// send boost
	Supply_Convert_To_Volts (Supply_Get_Boost(),
				 &vint,
				 &vdec);

	sprintf(buff_volts, "%02d.%d ", vint, vdec);
	Usart3Send(buff_volts);
	
	supply_send_voltage++;
	break;

    case 4:
	// send bat1
	Supply_Convert_To_Volts (Supply_Get_Batt(0),
				 &vint,
				 &vdec);

	sprintf(buff_volts, "%02d.%d ", vint, vdec);
	Usart3Send(buff_volts);
	
	supply_send_voltage++;
	break;

    case 5:
	// send bat2
	Supply_Convert_To_Volts (Supply_Get_Batt(1),
				 &vint,
				 &vdec);

	sprintf(buff_volts, "%02d.%d ", vint, vdec);
	Usart3Send(buff_volts);
	
	supply_send_voltage++;
	break;

    case 6:
	// send bat3
	Supply_Convert_To_Volts (Supply_Get_Batt(2),
				 &vint,
				 &vdec);

	sprintf(buff_volts, "%02d.%d ", vint, vdec);
	Usart3Send(buff_volts);
	
	supply_send_voltage++;
	break;

    case 7:
	// send bat4
	Supply_Convert_To_Volts (Supply_Get_Batt(3),
				 &vint,
				 &vdec);

	sprintf(buff_volts, "%02d.%d\r\n", vint, vdec);
	Usart3Send(buff_volts);
	
	supply_send_voltage = 0;
	break;
	
    default:
	supply_send_voltage = 0;
	break;
    }
}


void Supply_Send_Voltage_Start (void)
{
    if (!supply_send_voltage)
	supply_send_voltage = 1;
}


void Supply_Convert_To_Volts (unsigned short adc_value,
			      unsigned char * v_int,
			      unsigned char * v_dec)
{
    // Rmult is 0.211
    // Vx = adc * 3.3 
    float fcalc = 1.0;

    fcalc = adc_value * 15.64;
    // fcalc = adc_value * 15.08;    // adjust 8.1 -> 7.81
    fcalc = fcalc / 4095.;

    *v_int = (unsigned char) fcalc;
    fcalc = fcalc - *v_int;
    fcalc = fcalc * 10;
    *v_dec = (unsigned char) fcalc;    
}


// 0 unknow
// 1 batt
// 2 mains
unsigned char Supply_Get_Mode (void)
{
    unsigned char a = SUPPLY_MODE_UNKNOW;
    
    if ((supply_state == STANDBY_WITH_BATT) ||
	(supply_state == POWERON_WITH_BATT))
	a = SUPPLY_MODE_BATT;
    else if ((supply_state == STANDBY_WITH_MAINS) ||
	     (supply_state == POWERON_WITH_MAINS))
	a = SUPPLY_MODE_MAINS;

    return a;
}

//--- end of file ---//
