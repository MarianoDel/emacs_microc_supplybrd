//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### TIM.C #################################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "tim.h"
#include "stm32f10x.h"
#include "hard.h"


// Module Private Types Constants and Macros -----------------------------------
#define RCC_TIM1_CLK    (RCC->APB2ENR & 0x00000800)
#define RCC_TIM1_CLKEN    (RCC->APB2ENR |= 0x00000800)
#define RCC_TIM1_CLKDIS    (RCC->APB2ENR &= ~0x00000800)

#define RCC_TIM2_CLK    (RCC->APB1ENR & 0x00000001)
#define RCC_TIM2_CLKEN    (RCC->APB1ENR |= 0x00000001)
#define RCC_TIM2_CLKDIS    (RCC->APB1ENR &= ~0x00000001)

#define RCC_TIM3_CLK    (RCC->APB1ENR & 0x00000002)
#define RCC_TIM3_CLKEN    (RCC->APB1ENR |= 0x00000002)
#define RCC_TIM3_CLKDIS    (RCC->APB1ENR &= ~0x00000002)

#define RCC_TIM4_CLK    (RCC->APB1ENR & 0x00000004)
#define RCC_TIM4_CLKEN    (RCC->APB1ENR |= 0x00000004)
#define RCC_TIM4_CLKDIS    (RCC->APB1ENR &= ~0x00000004)

#ifdef STM32F10X_HD
#define RCC_TIM5_CLK    (RCC->APB1ENR & 0x00000008)
#define RCC_TIM5_CLKEN    (RCC->APB1ENR |= 0x00000008)
#define RCC_TIM5_CLKDIS    (RCC->APB1ENR &= ~0x00000008)

#define RCC_TIM6_CLK    (RCC->APB1ENR & 0x00000010)
#define RCC_TIM6_CLKEN    (RCC->APB1ENR |= 0x00000010)
#define RCC_TIM6_CLKDIS    (RCC->APB1ENR &= ~0x00000010)

#define RCC_TIM7_CLK    (RCC->APB1ENR & 0x00000020)
#define RCC_TIM7_CLKEN    (RCC->APB1ENR |= 0x00000020)
#define RCC_TIM7_CLKDIS    (RCC->APB1ENR &= ~0x00000020)

#define RCC_TIM8_CLK    (RCC->APB2ENR & 0x00002000)
#define RCC_TIM8_CLKEN    (RCC->APB2ENR |= 0x00002000)
#define RCC_TIM8_CLKDIS    (RCC->APB2ENR &= ~0x00002000)
#endif


// Externals -------------------------------------------------------------------
extern volatile unsigned short wait_ms_var;


// Globals ---------------------------------------------------------------------
// volatile unsigned char timer1_seq_ready = 0;
// volatile unsigned short timer1_seq_cnt = 0;

// volatile unsigned short sync_last_capt = 0;
// volatile unsigned short sync_capt = 0;
// volatile unsigned short sync_cnt = 0;
// volatile unsigned char sync_int = 0;
// volatile unsigned char sync_verify = 0;
volatile unsigned char timer_6_uif_flag = 0;
volatile unsigned char timer_7_uif_flag = 0;


// Module Functions ------------------------------------------------------------
void Wait_ms (unsigned short a)
{
    wait_ms_var = a;
    while (wait_ms_var);
}


//-------------------------------------------//
// @brief  TIM configure.
// @param  None
// @retval None
//------------------------------------------//
void TIM1_Init (void)
{
    if (!RCC_TIM1_CLK)
        RCC_TIM1_CLKEN;

    // Base timer config
    TIM1->CR1 = 0x00;        //clk int / 1;
    TIM1->CR2 = 0x00;
    TIM1->SMCR = 0x0000;

    // TIM1->CCMR1 = 0x0060;    //CH1 output PWM mode 2 (channel active TIM1->CNT < TIM1->CCR1)
    TIM1->CCMR1 = 0x6060; 
    TIM1->CCMR2 = 0x0060;
    // TIM1->CCER = 0x0000;
    TIM1->BDTR = 0x0000;
    TIM1->CCER |= TIM_CCER_CC3E | TIM_CCER_CC2E | TIM_CCER_CC1E;    // CH4 input inverted
    // TIM1->BDTR |= TIM_BDTR_MOE;

    TIM1->ARR = 1000;
    TIM1->PSC = 64 - 1;    // 13.88us tick
    
    TIM1->CNT = 0;
    TIM1->CCR2 = 200;
    TIM1->CCR3 = 300;    
    TIM1->BDTR |= TIM_BDTR_MOE;
    
    // Set ints and Enable timer
    // TIM1->DIER |= TIM_DIER_UIE | TIM_DIER_CC4IE;
    TIM1->CR1 |= TIM_CR1_CEN;

    // NVIC 
    // Timer1 Update event Int
    // NVIC_EnableIRQ(TIM1_UP_IRQn);
    // NVIC_SetPriority(TIM1_UP_IRQn, 4);

    // Timer1 CH4 input capture
    // NVIC_EnableIRQ(TIM1_CC_IRQn);
    // NVIC_SetPriority(TIM1_CC_IRQn, 8);
    
}


void TIM1_Init_Master_Output_Disable (void)
{
    if (!RCC_TIM1_CLK)
        RCC_TIM1_CLKEN;

    // Base timer config
    TIM1->CR1 = 0x00;        //clk int / 1;
    TIM1->CR2 = 0x00;

    // Master config
    TIM1->CR2 |= TIM_CR2_MMS_2 | TIM_CR2_MMS_0;    // TRG0 to OC2REF    
    // TIM1->CR2 |= TIM_CR2_MMS_2;    // TRG0 to OC1REF
    // TIM1->CR2 |= TIM_CR2_MMS_1;    // TRG0 to UEV    
    // Slave config
    TIM1->SMCR = 0x0000;

    // TIM1->CCMR1 = 0x0070;    //CH1 output PWM mode 2 (channel inactive TIM1->CNT < TIM1->CCR1)
    // TIM1->CCMR1 = 0x6060;    //CH1 output PWM mode 1 (channel active TIM1->CNT < TIM1->CCR1)
    // TIM1->CCMR1 = 0x7460;    //CH1 output PWM mode 1 (channel active TIM1->CNT < TIM1->CCR1)
    // TIM1->CCMR1 = 0x7060;    //CH1 output PWM mode 1 (channel active TIM1->CNT < TIM1->CCR1)    
    TIM1->CCMR1 = 0x1060;    //CH1 output PWM mode 1 (channel active TIM1->CNT < TIM1->CCR1)        
                             //CH2 active level on match
    TIM1->CCMR1 |= TIM_CCMR1_OC1CE;
    TIM1->CCMR2 = 0x0000;

    // TIM1->CCER |= TIM_CCER_CC2E | TIM_CCER_CC1E | TIM_CCER_CC1P;    // CH1 output
    TIM1->CCER |= TIM_CCER_CC2E | TIM_CCER_CC1E | TIM_CCER_CC1P;    // CH1 output inv Ch2
    // TIM1->CCER |= TIM_CCER_CC2E | TIM_CCER_CC1E;    // CH1 output Ch2       
    // TIM1->CCER |= TIM_CCER_CC1E;    // CH1 output    
    TIM1->BDTR |= TIM_BDTR_MOE;

    TIM1->PSC = 0;
    TIM1->ARR = 1000;    // 1000 pts -> 7.2KHz
    TIM1->CCR2 = 500;
    TIM1->CNT = 0;

    TIM1->EGR |= TIM_EGR_UG;
    // Set ints and Enable timer
    // TIM1->DIER |= TIM_DIER_UIE | TIM_DIER_CC4IE;
    TIM1->CR1 |= TIM_CR1_CEN;

    // NVIC 
    // Timer1 Update event Int
    // NVIC_EnableIRQ(TIM1_UP_IRQn);
    // NVIC_SetPriority(TIM1_UP_IRQn, 4);

    // Timer1 CH4 input capture
    // NVIC_EnableIRQ(TIM1_CC_IRQn);
    // NVIC_SetPriority(TIM1_CC_IRQn, 8);
}


void TIM1_Output_Enable (void)
{
    unsigned int temp = 0;

    //PA8 Alternative (TIM1_CH1), default to alternative
    temp = GPIOA->CRH;
    temp &= 0xFFFFFFF0;
    temp |= 0x0000000F;
    GPIOA->CRH = temp;
    
}


void TIM1_Output_Disable (void)
{
    unsigned int temp = 0;

    //PA8 Alternative (TIM1_CH1), revert to default
    temp = GPIOA->CRH;
    temp &= 0xFFFFFFF0;
    temp |= 0x00000000;
    GPIOA->CRH = temp;
    
}


void TIM1_Update_CH1 (unsigned short a)
{
    TIM1->CCR1 = a;
}


void TIM1_UP_IRQHandler (void)
{
    // reset int flag
    if (TIM1->SR & TIM_SR_UIF)
        TIM1->SR &= ~TIM_SR_UIF;

    
}


void TIM1_CC_IRQHandler (void)
{
    // reset int flag
    if (TIM1->SR & TIM_SR_CC4IF)
        TIM1->SR &= ~TIM_SR_CC4IF;

    
}


void TIM1_DeInit (void)
{
    TIM1->CR1 &= ~(TIM_CR1_CEN);
    TIM1->CCMR1 = TIM_CCMR1_OC2M_2;
    TIM1->EGR |= TIM_EGR_UG;
    
    if (RCC_TIM1_CLK)
        RCC_TIM1_CLKDIS;
}


// unsigned char TIM1_SyncGet (void)
// {
//     return sync_int;
// }


// void TIM1_SyncReset (void)
// {
//     sync_int = 0;
// }


// unsigned char TIM1_SyncVerify (unsigned char * freq_int, unsigned char * freq_dec)
// {
//     if ((!sync_verify) ||
//         (timer1_seq_cnt == 65000))
//         return 0;

//     unsigned int calc_int = 0;
//     unsigned int calc_dec = 0;
//     unsigned int calc_div = 0;

//     calc_div = 1000 * sync_cnt + sync_capt - sync_last_capt;

//     if (calc_div == 0)
//         return 0;

//     calc_dec = 7200 * 1000 * 100;
//     calc_dec = calc_dec / calc_div;
    
//     calc_int = 7200 * 1000;
//     calc_int = calc_int / calc_div;

//     calc_dec = calc_dec - calc_int * 100;

//     *freq_int = (unsigned char) calc_int;
//     *freq_dec = (unsigned char) calc_dec;

//     return 1;
// }

// void TIM_1_Init (void)    //for one pulse mode
// {
//     if (!RCC_TIM1_CLK)
//         RCC_TIM1_CLKEN;

//     //Configuracion del timer.
//     TIM1->CR1 |= TIM_CR1_OPM;        //clk int / 1; upcounting; one pulse
//     // TIM1->CR1 = 0x00;        //clk int / 1;
//     // TIM1->CR1 |= TIM_CR1_ARPE;        //clk int / 1, Preload;
//     // TIM1->CR2 |= TIM_CR2_MMS_1;        //UEV -> TRG0
//     TIM1->CR2 = 0x00;
//     //TIM1->SMCR |= TIM_SMCR_MSM | TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1 | TIM_SMCR_TS_1;    //link timer3
//     TIM1->SMCR = 0x0000;

//     TIM1->CCMR1 = 0x0060;            //CH1 output PWM mode 1 (channel active TIM1->CNT < TIM1->CCR1)
// #ifdef USE_CHANNELS_WITH_PRELOAD
//     TIM1->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;
// #endif
//     TIM1->CCMR2 = 0x0000;
//     // TIM1->CCER |= TIM_CCER_CC1NE | TIM_CCER_CC1NP | TIM_CCER_CC2NE | TIM_CCER_CC2NP;
//     TIM1->CCER |= TIM_CCER_CC1E;
        
//     TIM1->BDTR |= TIM_BDTR_MOE;
//     TIM1->ARR = DUTY_100_PERCENT;    //cada tick 20.83ns con PSC = 0

//     TIM1->CNT = 0;
// // #if defined USE_FREQ_18KHZ
// //     TIM1->PSC = 3;
// // #elif defined USE_FREQ_9KHZ
// //     TIM1->PSC = 7;
// // #elif defined USE_FREQ_4_5KHZ
// //     TIM1->PSC = 15;
// // #else
// // #error "Select working frequency on hard.h"
// // #endif

//     // Enable timer ver UDIS
//     //TIM1->DIER |= TIM_DIER_UIE;
//     // TIM1->CR1 |= TIM_CR1_CEN;

//     TIM1->CCR1 = 0;
// }


void TIM2_Init (void)
{
    if (!RCC_TIM2_CLK)
        RCC_TIM2_CLKEN;

    // Base timer config
    TIM2->CR1 |= TIM_CR1_OPM;        //clk int / 1;
    TIM2->CR2 |= TIM_CR2_MMS_1;    // TRG0 to UEV    
    TIM2->SMCR |= TIM_SMCR_MSM | TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1;    // master/slave & trigger TRG0

    TIM2->CCMR1 = 0x0000; 
    TIM2->CCMR2 = 0x0000;
    // TIM2->CCMR1 = 0x0060;    //CH1 output PWM mode 2 (channel active TIM2->CNT < TIM2->CCR1)
    // TIM2->CCER = 0x0000;
    // TIM2->BDTR = 0x0000;
    // TIM2->CCER |= TIM_CCER_CC4P | TIM_CCER_CC4E;    // CH4 input inverted
    // TIM2->BDTR |= TIM_BDTR_MOE;

    TIM2->PSC = 64 - 1;
    TIM2->ARR = 500 - 1;
    TIM2->CNT = 0;

    // Set ints and Enable timer
    // TIM2->DIER |= TIM_DIER_UIE | TIM_DIER_CC4IE;
    TIM2->CR1 |= TIM_CR1_CEN;
}


void TIM3_Init (void)
{
    if (!RCC_TIM3_CLK)
        RCC_TIM3_CLKEN;

    //Configuracion del timer.
    // TIM3->CR1 |= TIM_CR1_OPM;        //clk int / 1; upcounting; one pulse
    TIM3->CR1 = 0x0000;        //clk int / 1; upcounting;
    //TIM3->CR2 |= TIM_CR2_MMS_1;        //UEV -> TRG0
    TIM3->CR2 = 0x0000;
    //TIM3->SMCR |= TIM_SMCR_SMS_2 |TIM_SMCR_SMS_1 | TIM_SMCR_TS_1 | TIM_SMCR_TS_0;    //reset mode
    //TIM3->SMCR |= TIM_SMCR_SMS_2;    //reset mode link timer1    OJO no anda
    // TIM3->SMCR |= TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1;    //trigger mode link timer1
    TIM3->SMCR = 0x0000;    //
    TIM3->CCMR1 = 0x6000;    //CH2 output PWM mode 1
    TIM3->CCMR1 |= TIM_CCMR1_OC2PE;
    // TIM3->CCMR1 = 0x0060;    //CH1 output PWM mode 1
    // TIM3->CCMR1 = 0x0070;    //CH1 output PWM mode 2 (channel inactive TIM3->CNT < TIM3->CCR1)
    TIM3->CCMR2 = 0x0000;    //
    //  TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC1P;    //CH1 enable on pin active high
    TIM3->CCER |= TIM_CCER_CC2E;    //CH2 enable on pin active high
    //TIM3->CCER |= TIM_CCER_CC2E | TIM_CCER_CC2P;    //CH2 enable on pin active high
    TIM3->ARR = 80 - 1;    // 64MHz to 800KHz
    TIM3->CNT = 0;
    TIM3->PSC = 0;
    //TIM3->EGR = TIM_EGR_UG;    //generate event

    // DMA on UP
    TIM3->DIER |= TIM_DIER_UDE;

    // Enable timer ver UDIS
    //TIM3->DIER |= TIM_DIER_UIE;
    TIM3->CR1 |= TIM_CR1_CEN;

    TIM3->CCR2 = 0;
}


void TIM3_IRQHandler (void)	//1 ms
{
    //Code Handler
    
    //bajar flag
    if (TIM3->SR & 0x01)	//bajo el flag
        TIM3->SR = 0x00;
}


void TIM4_Init(void)
{
    //---- Clk ----//
    if (!RCC_TIM4_CLK)
        RCC_TIM4_CLKEN;

    //--- Config ----//
    TIM4->CR1 = 0x00;
    TIM4->CR2 = 0x00;
    
    TIM4->CCMR1 = 0x6000;    //CH2 output PWM mode 2 (channel active CNT < CCR1)
    TIM4->CCMR2 = 0x0060;    //CH3 output PWM mode 2 (channel active CNT < CCR1)
    
    TIM4->CCER |= TIM_CCER_CC3E | TIM_CCER_CC2E;
    
    TIM4->ARR = DUTY_100_PERCENT - 1;    //1000 pwm points freq-> 72MHz / 10 = 7.2KHz
    TIM4->PSC = 9;

    // Enable timer ver UDIS
    TIM4->CR1 |= TIM_CR1_CEN;
}


void TIM4_Update_CH2 (unsigned short a)
{
    TIM4->CCR2 = a;
}


void TIM4_Update_CH3 (unsigned short a)
{
    TIM4->CCR3 = a;
}


#ifdef STM32F10X_HD
//-- Timer 5 Functions --
void TIM5_Init (void)
{
//    Counter Register (TIMx_CNT)
//    Prescaler Register (TIMx_PSC)
//    Auto-Reload Register (TIMx_ARR)
//    The counter clock frequency CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1)

    //---- Clk ----//
    if (!RCC_TIM5_CLK)
        RCC_TIM5_CLKEN;

    //--- Config ----//
    TIM5->CR1 = 0x0000;        //clk int / 1; upcounting;
    TIM5->CR2 = 0x0000;

    TIM5->CCMR1 = 0x6060;    //CH1 CH2 output PWM mode 2 (channel active TIM5->CNT < TIM5->CCR1)
    TIM5->CCER |= TIM_CCER_CC2E | TIM_CCER_CC1E;    // ch1 ch2 enabled

    // try to be near 7KHz
    TIM5->ARR = DUTY_100_PERCENT - 1;    //1000 pwm points freq-> 72MHz / 10 = 7.2KHz
    TIM5->PSC = 9;
    
    // Enable the timer
    TIM5->CR1 |= TIM_CR1_CEN;
}


void TIM5_Update_CH1 (unsigned short a)
{
    TIM5->CCR1 = a;
}


void TIM5_Update_CH2 (unsigned short a)
{
    TIM5->CCR2 = a;
}


///////////////////////
// Timer 6 Functions //
///////////////////////
void TIM6_Init(void)
{
//    Counter Register (TIMx_CNT)
//    Prescaler Register (TIMx_PSC)
//    Auto-Reload Register (TIMx_ARR)
//    The counter clock frequency CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1)
//    Activate with ints

    //---- Clk ----//
    if (!RCC_TIM6_CLK)
        RCC_TIM6_CLKEN;

    //--- Config ----//
    TIM6->ARR = 833;
    TIM6->CNT = 0;
    TIM6->PSC = 63;
    TIM6->EGR = TIM_EGR_UG; //update registers

    // Enable timer with ints
    TIM6->DIER |= TIM_DIER_UIE;
    // TIM6->CR1 |= TIM_CR1_CEN;

    // NVIC enable for timer 6
    NVIC_EnableIRQ(TIM6_IRQn);
    NVIC_SetPriority(TIM6_IRQn, 9);
}


void TIM6_Stop (void)
{
    TIM6->CR1 &= ~(TIM_CR1_CEN);
    TIM6->CNT = 0;
}


void TIM6_Start (void)
{
    TIM6->CR1 |= TIM_CR1_CEN;
}



void TIM6_Change (unsigned short new_psc, unsigned short new_arr)
{
    TIM6->CR1 &= ~(TIM_CR1_CEN);
    TIM6->CNT = 0;
    TIM6->PSC = new_psc;
    TIM6->ARR = new_arr;
    TIM6->CR1 |= TIM_CR1_CEN;
}


void TIM6_IRQHandler (void)
{
    if (TIM6->SR & TIM_SR_UIF)
        TIM6->SR = ~(TIM_SR_UIF);

    // Bit_Bang_Tx_Tim_Handler ();
    // timer_6_uif_flag = 1;
}


unsigned char TIM6_UIF_Get_Flag (void)
{
    return timer_6_uif_flag;
}

    
void TIM6_UIF_Reset (void)
{
    timer_6_uif_flag = 0;
}


void TIM6_UIF_Set (void)
{
    timer_6_uif_flag = 1;
}


///////////////////////
// Timer 7 Functions //
///////////////////////
void TIM7_Init(void)
{
//    Counter Register (TIMx_CNT)
//    Prescaler Register (TIMx_PSC)
//    Auto-Reload Register (TIMx_ARR)
//    The counter clock frequency CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1)
//
//    Quiero una interrupcion por ms CK_INT es 72MHz

    //---- Clk ----//
    if (!RCC_TIM7_CLK)
        RCC_TIM7_CLKEN;

    //--- Config ----//
    TIM7->ARR = 416;
    TIM7->CNT = 0;
    TIM7->PSC = 63;
    TIM7->EGR = TIM_EGR_UG; //update registers

    // Enable timer ver UDIS
    TIM7->DIER |= TIM_DIER_UIE;
    // TIM7->CR1 |= TIM_CR1_CEN;

    // NVIC enable for timer 7
    NVIC_EnableIRQ(TIM7_IRQn);
    NVIC_SetPriority(TIM7_IRQn, 10);
}


void TIM7_Stop (void)
{
    TIM7->CR1 &= ~(TIM_CR1_CEN);
    TIM7->CNT = 0;
}


void TIM7_Start (void)
{
    TIM7->CR1 |= TIM_CR1_CEN;
}


void TIM7_IRQHandler (void)
{
    if (TIM7->SR & TIM_SR_UIF)
        TIM7->SR = ~(TIM_SR_UIF);

    // Bit_Bang_Rx_Tim_Handler ();
}


void TIM7_Change (unsigned short new_psc, unsigned short new_arr)
{
    TIM7->CR1 &= ~(TIM_CR1_CEN);
    TIM7->CNT = 0;
    TIM7->PSC = new_psc;
    TIM7->ARR = new_arr;
    TIM7->CR1 |= TIM_CR1_CEN;
}


unsigned char TIM7_UIF_Get_Flag (void)
{
    return timer_7_uif_flag;
}

    
void TIM7_UIF_Reset (void)
{
    timer_7_uif_flag = 0;
}


void TIM7_UIF_Set (void)
{
    timer_7_uif_flag = 1;
}


///////////////////////
// Timer 8 Functions //
///////////////////////
void TIM8_Init (void)
{
//    Counter Register (TIMx_CNT)
//    Prescaler Register (TIMx_PSC)
//    Auto-Reload Register (TIMx_ARR)
//    The counter clock frequency CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1)
//
    //---- Clk ----//
    if (!RCC_TIM8_CLK)
        RCC_TIM8_CLKEN;

    //--- Config ----//
    TIM8->CR1 = 0x0000;        //clk int / 1; upcounting;
    TIM8->CR2 = 0x0000;

    TIM8->CCMR1 = 0x6000;    //CH2 output PWM mode 2 (channel active CNT < CCR1)
    TIM8->CCMR2 = 0x0000;    //

    // ch2 enabled
    TIM8->CCER = 0;    // start on a know state
    TIM8->BDTR = 0;
    TIM8->CCER |= TIM_CCER_CC2E | TIM_CCER_CC2NE;
    TIM8->BDTR |= TIM_BDTR_MOE;
    
    // always on 0.3Hz
    TIM8->CCR2 = 16666;
    TIM8->ARR = 33333 - 1;
    TIM8->PSC = 6399;
    
    // Enable the timer
    TIM8->CR1 |= TIM_CR1_CEN;
}


void TIM8_Init_Slave_Output_Disable (void)
{
//    Counter Register (TIMx_CNT)
//    Prescaler Register (TIMx_PSC)
//    Auto-Reload Register (TIMx_ARR)
//    The counter clock frequency CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1)
//
    //---- Clk ----//
    if (!RCC_TIM8_CLK)
        RCC_TIM8_CLKEN;

    //--- Config ----//
    TIM8->CR1 = 0x0000;        //clk int / 1; upcounting;
    TIM8->CR2 = 0x0000;

    // slave mode, trigger mode in ITR0
    TIM8->SMCR |= TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1;    // trigger start
    
    TIM8->CCMR1 = 0x0060;    //CH1 output PWM mode 2 (channel active CNT < CCR1)
    TIM8->CCMR1 |= TIM_CCMR1_OC1CE;
    TIM8->CCMR2 = 0x0000;    //

    // ch1 enabled
    TIM8->CCER = 0;    // start on a know state
    TIM8->BDTR = 0;
    TIM8->CCER |= TIM_CCER_CC1E | TIM_CCER_CC1P;
    // TIM8->CCER |= TIM_CCER_CC1E;    
    TIM8->BDTR |= TIM_BDTR_MOE;
    
    TIM8->PSC = 0;
    TIM8->ARR = 1000;
    TIM8->CCR1 = 0;
    TIM8->CNT = 0;

    TIM8->EGR |= TIM_EGR_UG;
    
    // Wait trigger for timer enable
    // TIM8->CR1 |= TIM_CR1_CEN;
}


void TIM8_Stop (void)
{
    TIM8->CR1 &= ~(TIM_CR1_CEN);
    TIM8->CR1 = 0x0000;        //clk int / 1; upcounting;
    TIM8->CR2 = 0x0000;
    TIM8->CCER = 0;
    TIM8->BDTR = 0;    
}


void TIM8_Output_Enable (void)
{
    unsigned int temp = 0;

    // PC6 Alterantive (TIM8_CH1), default to alternative
    temp = GPIOC->CRL;
    temp &= 0xF0FFFFFF;
    temp |= 0x0F000000;
    GPIOC->CRL = temp;
    
}


void TIM8_Output_Disable (void)
{
    unsigned int temp = 0;

    // PC6 Alterantive (TIM8_CH1), revert to default
    temp = GPIOC->CRL;
    temp &= 0xF0FFFFFF;
    temp |= 0x00000000;
    GPIOC->CRL = temp;

}


void TIM8_DeInit (void)
{
    TIM8->CR1 &= ~(TIM_CR1_CEN);
    if (RCC_TIM8_CLK)
        RCC_TIM8_CLKDIS;
}


void TIM8_Update_CH1 (unsigned short a)
{
    TIM8->CCR1 = a;
}


void TIM8_Update_CH2 (unsigned short a)
{
    TIM8->CCR2 = a;
}


void TIM8_Update_CH3 (unsigned short a)
{
    TIM8->CCR3 = a;
}


void TIM8_Update_CH4 (unsigned short a)
{
    TIM8->CCR4 = a;
}
#endif    //STM32F10X_HD

//--- end of file ---//
