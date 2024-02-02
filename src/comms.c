//---------------------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### COMMS.C ###########################################
//---------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "comms.h"
#include "answers_defs.h"
#include "hard.h"
// #include "adc.h"

#include "usart_channels.h"
#include "usart.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


// Module Private Types Constants and Macros -----------------------------------
char s_ans_ok [] = {"ok\n"};
char s_ans_nok [] = {"nok\n"};
#define SIZEOF_LOCAL_BUFF    128
#define COMMS_TT_RELOAD    3000


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
char local_buff [SIZEOF_LOCAL_BUFF];
volatile unsigned short comms_timeout = 0;
    

// Module Private Functions ----------------------------------------------------
static void Comms_Messages (char * msg_str);


// Module Functions ------------------------------------------------------------
void Comms_Timeouts (void)
{
    if (comms_timeout)
        comms_timeout--;
}


unsigned char Comms_Rpi_Answering (void)
{
    if (comms_timeout)
        return 1;
    
    return 0;
}


void Comms_Update (void)
{
    if (UsartRpiHaveData())
    {
        UsartRpiHaveDataReset();
        UsartRpiReadBuffer(local_buff, SIZEOF_LOCAL_BUFF);
        Comms_Messages(local_buff);

        comms_timeout = COMMS_TT_RELOAD;
    }
}


static void Comms_Messages (char * msg_str)
{
    // resp_e resp;
    char buff [128];    
    
    // check if its own, broadcast or channel
    // for channel 1
    if (strncmp (msg_str, "ch1", sizeof("ch1") - 1) == 0)
    {
        // check enable or bridged
        if (strncmp ((msg_str + 4), "enable", sizeof("enable") - 1) == 0)
        {
            Ena_Ch1_On();
            UsartRpiSend(s_ans_ok);
        }
        else if (strncmp ((msg_str + 4), "disable", sizeof("disable") - 1) == 0)
        {
            Ena_Ch1_Off();
            UsartRpiSend(s_ans_ok);
        }
        else    // bridge the message
        {
            sprintf(buff, "%s\n", (msg_str + 4));
            UsartChannel1Send (buff);
        }
    }
    else if (strncmp (msg_str, "ch2", sizeof("ch2") - 1) == 0)
    {
        // check enable or bridged
        if (strncmp ((msg_str + 4), "enable", sizeof("enable") - 1) == 0)
        {
            Ena_Ch2_On();
            UsartRpiSend(s_ans_ok);
        }
        else if (strncmp ((msg_str + 4), "disable", sizeof("disable") - 1) == 0)
        {
            Ena_Ch2_Off();
            UsartRpiSend(s_ans_ok);
        }
        else    // bridge the message
        {
            sprintf(buff, "%s\n", (msg_str + 4));
            UsartChannel2Send (buff);
        }
    }
    else if (strncmp (msg_str, "chf", sizeof("chf") - 1) == 0)
    {
        // check enable or bridged
        if (strncmp ((msg_str + 4), "enable", sizeof("enable") - 1) == 0)
        {
            Ena_Ch1_On();
            Ena_Ch2_On();
            Ena_Ch3_On();
            Ena_Ch4_On();            
            UsartRpiSend(s_ans_ok);
        }
        else if (strncmp ((msg_str + 4), "disable", sizeof("disable") - 1) == 0)
        {
            Ena_Ch1_Off();
            Ena_Ch2_Off();
            Ena_Ch3_Off();
            Ena_Ch4_Off();            
            UsartRpiSend(s_ans_ok);
        }
        else    // bridge the message
        {
            sprintf(buff, "%s\n", (msg_str + 4));
            UsartChannel1Send (buff);
            UsartChannel2Send (buff);
            // UsartChannel3Send (buff);
            // UsartChannel4Send (buff);            
        }
    }
    else if (strncmp (msg_str, "sup", sizeof("sup") - 1) == 0)
    {
        // not implemented yet!
        UsartRpiSend(s_ans_ok);
    }
    else
        UsartRpiSend(s_ans_nok);

}


//---- End of File ----//
