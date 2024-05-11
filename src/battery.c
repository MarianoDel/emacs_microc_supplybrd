//------------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### BATTERY.C ################################
//------------------------------------------------

// Includes --------------------------------------------------------------------
#include "battery.h"
// #include "hard.h"
#include "adc.h"


#include <stdio.h>


// Module Private Types Constants and Macros -----------------------------------
typedef enum {
    START_12V_EXT_CONVERTION,
    WAIT_12V_EXT_CONVERTION,
    START_BAT_A_CONVERTION,
    WAIT_BAT_A_CONVERTION,
    START_BAT_B_CONVERTION,
    WAIT_BAT_B_CONVERTION
    
} battery_states_e;

#define Sense_12V_Ext    ADC_Channel_4
#define Sense_Bat_A    ADC_Channel_5
#define Sense_Bat_B    ADC_Channel_6


// Externals -------------------------------------------------------------------



// Globals ---------------------------------------------------------------------
volatile unsigned short battery_timeout = 0;
unsigned short mains_input_voltage = 0;
unsigned short batt_a_voltage = 0;
unsigned short batt_b_voltage = 0;


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void Battery_Timeout (void)
{
    if (battery_timeout)
        battery_timeout--;
}


battery_status_e batt_status = STATUS_UNKNOWN;
battery_status_e Battery_Check (void)
{
    return batt_status;
}


unsigned short Battery_Check_BatA (void)
{
    return batt_a_voltage;
}


unsigned short Battery_Check_BatB (void)
{
    return batt_b_voltage;
}


battery_states_e battery_state = START_12V_EXT_CONVERTION;
void Battery_Status (void)
{
    if (battery_timeout)
        return;

    battery_timeout = 50;    // take meas every 50ms
    
    switch (battery_state)
    {
    case START_12V_EXT_CONVERTION:
        AdcConvertChannel (Sense_12V_Ext);
        battery_state++;
        break;

    case WAIT_12V_EXT_CONVERTION:
        if (AdcConvertSingleChannelFinishFlag ())
        {
            mains_input_voltage = AdcConvertChannelResult ();

            if (mains_input_voltage > MAINS_VOLTAGE_MIN)
                batt_status = RUNNING_ON_MAINS;
            else
                batt_status = RUNNING_ON_BATTERY;                

            battery_state++;
        }
        break;

    case START_BAT_A_CONVERTION:
        AdcConvertChannel (Sense_Bat_A);
        battery_state++;
        break;

    case WAIT_BAT_A_CONVERTION:
        if (AdcConvertSingleChannelFinishFlag ())
        {
            batt_a_voltage = AdcConvertChannelResult ();
            battery_state++;
        }
        break;

    case START_BAT_B_CONVERTION:
        AdcConvertChannel (Sense_Bat_B);
        battery_state++;
        break;

    case WAIT_BAT_B_CONVERTION:
        if (AdcConvertSingleChannelFinishFlag ())
        {
            batt_b_voltage = AdcConvertChannelResult ();
            battery_state = START_12V_EXT_CONVERTION;
        }
        break;

    default:
        battery_state = START_12V_EXT_CONVERTION;
        break;
    }
}


//--- end of file ---//
