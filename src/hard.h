//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### HARD.H #################################
//---------------------------------------------

#ifndef HARD_H_
#define HARD_H_


//----------- Defines For Configuration -------------

//----- Board Configuration -------------------//
//--- Hardware ------------------//
#define HARDWARE_VERSION_1_0    // first prototype


//--- Software ------------------//
#define FIRMWARE_VERSION_1_0    // init version


//-------- Type of Program (depending on software version) ----------------


//--- Serial Number / Device Id Bytes length ----------
#define USE_DEVICE_ID_4BYTES
// #define USE_DEVICE_ID_12BYTES



//-------- Type of Program and Features ----------------
// #define USE_NO_TREATMENT_DETECT    //cuando esta en tratamiento revisa si las potencias tambien
// #define USE_BUZZER_ON_START


//-------- Kind of Reports Sended ----------------

//-------- Others Configurations depending on the formers ------------
// #define K_200V    0.0957    //con diodo z prot de 3.1V da error
// #define K_200V    0.0806    //con diodo z 5.1V
// #define K_15V    0.00804    //con z 3.1V
// #define K_15V    0.00619    //con z 5.1V


//-------- Oscillator and Crystal selection (Freq in startup_clocks.h) ---
#define HSI_INTERNAL_RC
// #define HSE_CRYSTAL_OSC

#ifdef HSE_CRYSTAL_OSC
// #define CRYSTAL_8MHZ
#define CRYSTAL_12MHZ
#endif

#ifdef HSE_CRYSTAL_OSC
// #define SYSCLK_FREQ_72MHz
#define SYSCLK_FREQ_8MHz
#endif

#ifdef HSI_INTERNAL_RC
#define SYSCLK_FREQ_64MHz
// #define SYSCLK_FREQ_8MHz
#endif

//-------- End Of Defines For Configuration ------




//--- Hardware & Software Messages ------------------//
#ifdef HARDWARE_VERSION_1_0
#define HARD "Hardware Version: 1.0"
#endif
#ifdef FIRMWARE_VERSION_1_0
#define SOFT "Firmware Version: 1.0"
#endif
//--- End of Hardware & Software Messages ------------------//



// Exported Types --------------------------------------------------------------
#ifdef HARDWARE_VERSION_1_0

// PA defines ----
// PA0
#define ENA_ENCODER    ((GPIOA->ODR & 0x0001) == 0)
#define ENA_ENCODER_OFF    (GPIOA->BSRR = 0x00000001)
#define ENA_ENCODER_ON    (GPIOA->BSRR = 0x00010000)

// PA1 NC
// PA2 PA3 Alternative Usart2 Tx Rx

// PA4 Analog Channel 4 (SENSE_12V_EXT)
// PA5 Analog Channel 5 (SENSE_BAT_A)
// PA6 Analog Channel 6 (SENSE_BAT_B)

// PA7 NC

// PA8
#define ENA_BOOST    ((GPIOA->ODR & 0x0100) != 0)
#define ENA_BOOST_ON    (GPIOA->BSRR = 0x00000100)
#define ENA_BOOST_OFF    (GPIOA->BSRR = 0x01000000)

// PA9 PA10 Alternative Usart1 Tx Rx

// PA11
#define PROBE_SENSE_CH4    ((GPIOA->IDR & 0x0800) == 0)

// PA12
#define ACT_PROBE_CH4    ((GPIOA->ODR & 0x1000) != 0)
#define ACT_PROBE_CH4_ON    (GPIOA->BSRR = 0x00001000)
#define ACT_PROBE_CH4_OFF    (GPIOA->BSRR = 0x10000000)

// PA13 PA14 PA15 NC jtag

// PB defines ----
// PB0 
#define ACT_PROBE_CH2    ((GPIOB->ODR & 0x0001) != 0)
#define ACT_PROBE_CH2_ON    (GPIOB->BSRR = 0x00000001)
#define ACT_PROBE_CH2_OFF    (GPIOB->BSRR = 0x00010000)

// PB1
#define SYNC_CH2    ((GPIOB->ODR & 0x0002) != 0)
#define SYNC_CH2_ON    (GPIOB->BSRR = 0x00000002)
#define SYNC_CH2_OFF    (GPIOB->BSRR = 0x00020000)

//PB2
#define ENA_CH2    ((GPIOB->ODR & 0x0004) == 0)
#define ENA_CH2_OFF    (GPIOB->BSRR = 0x00000004)
#define ENA_CH2_ON    (GPIOB->BSRR = 0x00040000)

// PB3 PB4 NC jtag
// PB5 NC

// PB6 Alternative I2C1_SCL
// PB7 Alternative I2C1_SDA

// PB8
#define SYNC_CH4    ((GPIOB->ODR & 0x0100) != 0)
#define SYNC_CH4_ON    (GPIOB->BSRR = 0x00000100)
#define SYNC_CH4_OFF    (GPIOB->BSRR = 0x01000000)

// PB9
#define ENA_CH4    ((GPIOB->ODR & 0x0200) == 0)
#define ENA_CH4_OFF    (GPIOB->BSRR = 0x00000200)
#define ENA_CH4_ON    (GPIOB->BSRR = 0x02000000)

// PB10 PB11 Alternative Usart3 Tx Rx

// PB12
#define PROBE_SENSE_CH3    ((GPIOB->IDR & 0x1000) == 0)

// PB13
#define ACT_PROBE_CH3    ((GPIOB->ODR & 0x2000) != 0)
#define ACT_PROBE_CH3_ON    (GPIOB->BSRR = 0x00002000)
#define ACT_PROBE_CH3_OFF    (GPIOB->BSRR = 0x20000000)

// PB14
#define SYNC_CH3    ((GPIOB->ODR & 0x4000) != 0)
#define SYNC_CH3_ON    (GPIOB->BSRR = 0x00004000)
#define SYNC_CH3_OFF    (GPIOB->BSRR = 0x40000000)

// PB15
#define ENA_CH3    ((GPIOB->ODR & 0x8000) == 0)
#define ENA_CH3_OFF    (GPIOB->BSRR = 0x00008000)
#define ENA_CH3_ON    (GPIOB->BSRR = 0x80000000)

// PC defines ----
// PC0
#define PROBE_SENSE_CH1    ((GPIOC->IDR & 0x0001) == 0)

// PC1
#define ACT_PROBE_CH1    ((GPIOC->ODR & 0x0002) != 0)
#define ACT_PROBE_CH1_ON    (GPIOC->BSRR = 0x00000002)
#define ACT_PROBE_CH1_OFF    (GPIOC->BSRR = 0x00020000)

// PC2
#define SYNC_CH1    ((GPIOC->ODR & 0x0004) != 0)
#define SYNC_CH1_ON    (GPIOC->BSRR = 0x00000004)
#define SYNC_CH1_OFF    (GPIOC->BSRR = 0x00040000)

// PC3
#define ENA_CH1    ((GPIOC->ODR & 0x0008) == 0)
#define ENA_CH1_OFF    (GPIOC->BSRR = 0x00000008)
#define ENA_CH1_ON    (GPIOC->BSRR = 0x00080000)

// PC4 NC

// PC5
#define PROBE_SENSE_CH2    ((GPIOC->IDR & 0x0020) == 0)

// PC6 NC

// PC7 
#define ENA_5V_COMM    ((GPIOC->ODR & 0x0080) == 0)
#define ENA_5V_COMM_OFF    (GPIOC->BSRR = 0x00000080)
#define ENA_5V_COMM_ON    (GPIOC->BSRR = 0x00800000)

// PC8
#define ENA_LCD    ((GPIOC->ODR & 0x0100) != 0)
#define ENA_LCD_ON    (GPIOC->BSRR = 0x00000100)
#define ENA_LCD_OFF    (GPIOC->BSRR = 0x01000000)

// PC9
#define ENA_RPI    ((GPIOC->ODR & 0x0200) == 0)
#define ENA_RPI_OFF    (GPIOC->BSRR = 0x00000200)
#define ENA_RPI_ON    (GPIOC->BSRR = 0x02000000)

// PC10 PC11 Alternative Uart4 Tx Rx

// PC12 Alternative Uart5 Tx

// PC13 PC14 PC15 NC

// PD defines ----
// PD0 PD1 NC

// PD2 Alternative Uart5 Rx

#endif //HARDWARE_VERSION_1_0



//--- Exported Module Functions ----
void Hard_GetVoltages (char * buff);
void Hard_GetHardSoft (char * buff);
void Hard_GetVoltages_Complete (void);

void Ena_Ch1_On (void);
void Ena_Ch2_On (void);
void Ena_Ch3_On (void);
void Ena_Ch4_On (void);
void Ena_Ch1_Off (void);
void Ena_Ch2_Off (void);
void Ena_Ch3_Off (void);
void Ena_Ch4_Off (void);

void Act_Probe_Ch1_Off (void);
void Act_Probe_Ch2_Off (void);
void Act_Probe_Ch1_On (void);
void Act_Probe_Ch2_On (void);

#endif
