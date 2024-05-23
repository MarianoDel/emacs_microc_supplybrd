//----------------------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### BIT_BANG.H #########################################
//----------------------------------------------------------
#ifndef _BIT_BANG_H_
#define _BIT_BANG_H_

// Config Defines --------------------------------------------------------------


// Includes --------------------------------------------------------------------
#include "tim.h"
#include "gpio.h"

// Module Exported Types Constants and Macros ----------------------------------
#define Tim_Bit_Bang_Tx_Init()    TIM6_Init()
#define Tim_Bit_Bang_Tx_Reset()    TIM6_Stop()
#define Tim_Bit_Bang_Tx_Set()    TIM6_Start()

#define Rx_Int_Enable()    EXTIOn()
#define Rx_Int_Disable()    EXTIOff()

#define Tim_Bit_Bang_Rx_Init()    TIM7_Init()
#define Tim_Bit_Bang_Rx_Reset()    TIM7_Stop()
#define Tim_Bit_Bang_Rx_Set()    TIM7_Start()





// Module Exported Functions ---------------------------------------------------
void Bit_Bang_Init (void);
void Bit_Bang_Shutdown (void);

void Bit_Bang_Tx_Send (char * buff);
void Bit_Bang_Tx_Tim_Handler (void);

unsigned char Bit_Bang_Rx_Have_Data (void);
unsigned char Bit_Bang_Rx_ReadBuffer (char * data_read);
void Bit_Bang_Rx_Tim_Handler (void);
void Bit_Bang_Rx_Int_Handler (void);

#endif    /* _BIT_BANG_H_ */

//--- end of file ---//
