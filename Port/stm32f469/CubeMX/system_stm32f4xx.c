/**
  ******************************************************************************
  * @file    system_stm32f4xx.c
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-M4 Device Peripheral Access Layer System Source File.
  *
  *   This file provides two functions and one global variable to be called from 
  *   user application:
  *      - SystemInit(): This function is called at startup just after reset and 
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32f4xx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick 
  *                                  timer or configure other parameters.
  *                                     
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32f4xx_system
  * @{
  */  
  
/** @addtogroup STM32F4xx_System_Private_Includes
  * @{
  */


#include "stm32f4xx.h"

#if !defined  (HSE_VALUE) 
  #define HSE_VALUE    ((uint32_t)25000000) /*!< Default value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (HSI_VALUE)
  #define HSI_VALUE    ((uint32_t)16000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_Defines
  * @{
  */

/************************* Miscellaneous Configuration ************************/
/*!< Uncomment the following line if you need to use external SRAM or SDRAM as data memory  */
#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx) || defined(STM32F417xx)\
 || defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)\
 || defined(STM32F469xx) || defined(STM32F479xx) || defined(STM32F412Zx) || defined(STM32F412Vx)
/* #define DATA_IN_ExtSRAM */
#endif /* STM32F40xxx || STM32F41xxx || STM32F42xxx || STM32F43xxx || STM32F469xx || STM32F479xx ||\
          STM32F412Zx || STM32F412Vx */
 
#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)\
 || defined(STM32F446xx) || defined(STM32F469xx) || defined(STM32F479xx)
/* #define DATA_IN_ExtSDRAM */
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F446xx || STM32F469xx ||\
          STM32F479xx */

/* Note: Following vector table addresses must be defined in line with linker
         configuration. */
/*!< Uncomment the following line if you need to relocate the vector table
     anywhere in Flash or Sram, else the vector table is kept at the automatic
     remap of boot address selected */
/* #define USER_VECT_TAB_ADDRESS */

#if defined(USER_VECT_TAB_ADDRESS)
/*!< Uncomment the following line if you need to relocate your vector Table
     in Sram else user remap will be done in Flash. */
/* #define VECT_TAB_SRAM */
#if defined(VECT_TAB_SRAM)
#define VECT_TAB_BASE_ADDRESS   SRAM_BASE       /*!< Vector Table base address field.
                                                     This value must be a multiple of 0x200. */
#define VECT_TAB_OFFSET         0x00000000U     /*!< Vector Table base offset field.
                                                     This value must be a multiple of 0x200. */
#else
#define VECT_TAB_BASE_ADDRESS   FLASH_BASE      /*!< Vector Table base address field.
                                                     This value must be a multiple of 0x200. */
#define VECT_TAB_OFFSET         0x00000000U     /*!< Vector Table base offset field.
                                                     This value must be a multiple of 0x200. */
#endif /* VECT_TAB_SRAM */
#endif /* USER_VECT_TAB_ADDRESS */
/******************************************************************************/

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_Variables
  * @{
  */
  /* This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetHCLKFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency 
         Note: If you use this function to configure the system clock; then there
               is no need to call the 2 first functions listed above, since SystemCoreClock
               variable is updated automatically.
  */
uint32_t SystemCoreClock = 16000000;
const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[8]  = {0, 0, 0, 0, 1, 2, 3, 4};
/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_FunctionPrototypes
  * @{
  */

#if defined (DATA_IN_ExtSRAM) || defined (DATA_IN_ExtSDRAM)
  static void SystemInit_ExtMemCtl(void); 
#endif /* DATA_IN_ExtSRAM || DATA_IN_ExtSDRAM */

/**
  * @}
  */

/** @addtogroup STM32F4xx_System_Private_Functions
  * @{
  */

/**
  * @brief  Setup the microcontroller system
  *         Initialize the FPU setting, vector table location and External memory 
  *         configuration.
  * @param  None
  * @retval None
  */
void SystemInit(void)
{
  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
  #endif

#if defined (DATA_IN_ExtSRAM) || defined (DATA_IN_ExtSDRAM)
  SystemInit_ExtMemCtl(); 
#endif /* DATA_IN_ExtSRAM || DATA_IN_ExtSDRAM */

  /* Configure the Vector Table location -------------------------------------*/
#if defined(USER_VECT_TAB_ADDRESS)
  SCB->VTOR = VECT_TAB_BASE_ADDRESS | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#endif /* USER_VECT_TAB_ADDRESS */
}

/**
   * @brief  Update SystemCoreClock variable according to Clock Register Values.
  *         The SystemCoreClock variable contains the core clock (HCLK), it can
  *         be used by the user application to setup the SysTick timer or configure
  *         other parameters.
  *           
  * @note   Each time the core clock (HCLK) changes, this function must be called
  *         to update SystemCoreClock variable value. Otherwise, any configuration
  *         based on this variable will be incorrect.         
  *     
  * @note   - The system frequency computed by this function is not the real 
  *           frequency in the chip. It is calculated based on the predefined 
  *           constant and the selected clock source:
  *             
  *           - If SYSCLK source is HSI, SystemCoreClock will contain the HSI_VALUE(*)
  *                                              
  *           - If SYSCLK source is HSE, SystemCoreClock will contain the HSE_VALUE(**)
  *                          
  *           - If SYSCLK source is PLL, SystemCoreClock will contain the HSE_VALUE(**) 
  *             or HSI_VALUE(*) multiplied/divided by the PLL factors.
  *         
  *         (*) HSI_VALUE is a constant defined in stm32f4xx_hal_conf.h file (default value
  *             16 MHz) but the real value may vary depending on the variations
  *             in voltage and temperature.   
  *    
  *         (**) HSE_VALUE is a constant defined in stm32f4xx_hal_conf.h file (its value
  *              depends on the application requirements), user has to ensure that HSE_VALUE
  *              is same as the real frequency of the crystal used. Otherwise, this function
  *              may have wrong result.
  *                
  *         - The result of this function could be not correct when using fractional
  *           value for HSE crystal.
  *     
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate(void)
{
  uint32_t tmp = 0, pllvco = 0, pllp = 2, pllsource = 0, pllm = 2;
  
  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & RCC_CFGR_SWS;

  switch (tmp)
  {
    case 0x00:  /* HSI used as system clock source */
      SystemCoreClock = HSI_VALUE;
      break;
    case 0x04:  /* HSE used as system clock source */
      SystemCoreClock = HSE_VALUE;
      break;
    case 0x08:  /* PLL used as system clock source */

      /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N
         SYSCLK = PLL_VCO / PLL_P
         */    
      pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22;
      pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;
      
      if (pllsource != 0)
      {
        /* HSE used as PLL clock source */
        pllvco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
      }
      else
      {
        /* HSI used as PLL clock source */
        pllvco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
      }

      pllp = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >>16) + 1 ) *2;
      SystemCoreClock = pllvco/pllp;
      break;
    default:
      SystemCoreClock = HSI_VALUE;
      break;
  }
  /* Compute HCLK frequency --------------------------------------------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
  /* HCLK frequency */
  SystemCoreClock >>= tmp;
}

#if defined (DATA_IN_ExtSRAM) && defined (DATA_IN_ExtSDRAM)
#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)\
 || defined(STM32F469xx) || defined(STM32F479xx)
/**
  * @brief  Setup the external memory controller.
  *         Called in startup_stm32f4xx.s before jump to main.
  *         This function configures the external memories (SRAM/SDRAM)
  *         This SRAM/SDRAM will be used as program data memory (including heap and stack).
  * @param  None
  * @retval None
  */
void SystemInit_ExtMemCtl(void)
{
  __IO uint32_t tmp = 0x00;

  register uint32_t tmpreg = 0, timeout = 0xFFFF;
  register __IO uint32_t index;

  /* Enable GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH and GPIOI interface clock */
  RCC->AHB1ENR |= 0x000001F8;

  /* Delay after an RCC peripheral clock enabling */
  tmp = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);
  
  /* Connect PDx pins to FMC Alternate function */
  GPIOD->AFR[0]  = 0x00CCC0CC;
  GPIOD->AFR[1]  = 0xCCCCCCCC;
  /* Configure PDx pins in Alternate function mode */  
  GPIOD->MODER   = 0xAAAA0A8A;
  /* Configure PDx pins speed to 100 MHz */  
  GPIOD->OSPEEDR = 0xFFFF0FCF;
  /* Configure PDx pins Output type to push-pull */  
  GPIOD->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PDx pins */ 
  GPIOD->PUPDR   = 0x00000000;

  /* Connect PEx pins to FMC Alternate function */
  GPIOE->AFR[0]  = 0xC00CC0CC;
  GPIOE->AFR[1]  = 0xCCCCCCCC;
  /* Configure PEx pins in Alternate function mode */ 
  GPIOE->MODER   = 0xAAAA828A;
  /* Configure PEx pins speed to 100 MHz */ 
  GPIOE->OSPEEDR = 0xFFFFC3CF;
  /* Configure PEx pins Output type to push-pull */  
  GPIOE->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PEx pins */ 
  GPIOE->PUPDR   = 0x00000000;
  
  /* Connect PFx pins to FMC Alternate function */
  GPIOF->AFR[0]  = 0xCCCCCCCC;
  GPIOF->AFR[1]  = 0xCCCCCCCC;
  /* Configure PFx pins in Alternate function mode */   
  GPIOF->MODER   = 0xAA800AAA;
  /* Configure PFx pins speed to 50 MHz */ 
  GPIOF->OSPEEDR = 0xAA800AAA;
  /* Configure PFx pins Output type to push-pull */  
  GPIOF->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PFx pins */ 
  GPIOF->PUPDR   = 0x00000000;

  /* Connect PGx pins to FMC Alternate function */
  GPIOG->AFR[0]  = 0xCCCCCCCC;
  GPIOG->AFR[1]  = 0xCCCCCCCC;
  /* Configure PGx pins in Alternate function mode */ 
  GPIOG->MODER   = 0xAAAAAAAA;
  /* Configure PGx pins speed to 50 MHz */ 
  GPIOG->OSPEEDR = 0xAAAAAAAA;
  /* Configure PGx pins Output type to push-pull */  
  GPIOG->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PGx pins */ 
  GPIOG->PUPDR   = 0x00000000;
  
  /* Connect PHx pins to FMC Alternate function */
  GPIOH->AFR[0]  = 0x00C0CC00;
  GPIOH->AFR[1]  = 0xCCCCCCCC;
  /* Configure PHx pins in Alternate function mode */ 
  GPIOH->MODER   = 0xAAAA08A0;
  /* Configure PHx pins speed to 50 MHz */ 
  GPIOH->OSPEEDR = 0xAAAA08A0;
  /* Configure PHx pins Output type to push-pull */  
  GPIOH->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PHx pins */ 
  GPIOH->PUPDR   = 0x00000000;
  
  /* Connect PIx pins to FMC Alternate function */
  GPIOI->AFR[0]  = 0xCCCCCCCC;
  GPIOI->AFR[1]  = 0x00000CC0;
  /* Configure PIx pins in Alternate function mode */ 
  GPIOI->MODER   = 0x0028AAAA;
  /* Configure PIx pins speed to 50 MHz */ 
  GPIOI->OSPEEDR = 0x0028AAAA;
  /* Configure PIx pins Output type to push-pull */  
  GPIOI->OTYPER  = 0x00000000;
  /* No pull-up, pull-down for PIx pins */ 
  GPIOI->PUPDR   = 0x00000000;
  
/*-- FMC Configuration -------------------------------------------------------*/
  /* Enable the FMC interface clock */
  RCC->AHB3ENR |= 0x00000001;
  /* Delay after an RCC peripheral clock enabling */
  tmp = READ_BIT(RCC->AHB3ENR, RCC_AHB3ENR_FMCEN);

  FMC_Bank5_6->SDCR[0] = 0x000019E4;
  FMC_Bank5_6->SDTR[0] = 0x01115351;      
  
  /* SDRAM initialization sequence */
  /* Clock enable command */
  FMC_Bank5_6->SDCMR = 0x00000011; 
  tmpreg = FMC_Bank5_6->SDSR & 0x00000020; 
  while((tmpreg != 0) && (timeout-- > 0))
  {
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020; 
  }

  /* Delay */
  for (index = 0; index<1000; index++);
  
  /* PALL command */
  FMC_Bank5_6->SDCMR = 0x00000012;           
  tmpreg = FMC_Bank5_6->SDSR & 0x00000020;
  timeout = 0xFFFF;
  while((tmpreg != 0) && (timeout-- > 0))
  {
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020; 
  }
  
  /* Auto refresh command */
  FMC_Bank5_6->SDCMR = 0x00000073;
  tmpreg = FMC_Bank5_6->SDSR & 0x00000020;
  timeout = 0xFFFF;
  while((tmpreg != 0) && (timeout-- > 0))
  {
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020; 
  }
 
  /* MRD register program */
  FMC_Bank5_6->SDCMR = 0x00046014;
  tmpreg = FMC_Bank5_6->SDSR & 0x00000020;
  timeout = 0xFFFF;
  while((tmpreg != 0) && (timeout-- > 0))
  {
    tmpreg = FMC_Bank5_6->SDSR & 0x00000020; 
  } 
  
  /* Set refresh count */
  tmpreg = FMC_Bank5_6->SDRTR;
  FMC_Bank5_6->SDRTR = (tmpreg | (0x0000027C<<1));
  
  /* Disable write protection */
  tmpreg = FMC_Bank5_6->SDCR[0]; 
  FMC_Bank5_6->SDCR[0] = (tmpreg & 0xFFFFFDFF);

#if defined(STM32F427xx) || defined(STM32F437xx) || defined(STM32F429xx) || defined(STM32F439xx)
  /* Configure and enable Bank1_SRAM2 */
  FMC_Bank1->BTCR[2]  = 0x00001011;
  FMC_Bank1->BTCR[3]  = 0x00000201;
  FMC_Bank1E->BWTR[2] = 0x0fffffff;
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx */ 
#if defined(STM32F469xx) || defined(STM32F479xx)
  /* Configure and enable Bank1_SRAM2 */
  FMC_Bank1->BTCR[2]  = 0x00001091;
  FMC_Bank1->BTCR[3]  = 0x00110212;
  FMC_Bank1E->BWTR[2] = 0x0fffffff;
#endif /* STM32F469xx || STM32F479xx */

  (void)(tmp); 
}
#endif /* STM32F427xx || STM32F437xx || STM32F429xx || STM32F439xx || STM32F469xx || STM32F479xx */
#elif defined (DATA_IN_ExtSRAM) || defined (DATA_IN_ExtSDRAM)
/**
  * @brief  Setup the external memory controller.
  *         Called in startup_stm32f4xx.s before jump to main.
  *         This function configures the external memories (SRAM/SDRAM)
  *         This SRAM/SDRAM will be used as program data memory (including heap and stack).
  * @param  None
  * @retval None
  */
void SystemInit_ExtMemCtl(void)
{
    register uint32_t tmpreg = 0, timeout = 0xFFFF;
        register uint32_t index;

        /* Enable GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH and GPIOI interface
            clock */
        RCC->AHB1ENR |= 0x000001FC; /*RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOx, ENABLE);*/

        /*GPIO_AF_FMC 0xC FMC Alternate Function mapping */


        /*PC0   ------> FMC_SDNWE*/
        /* Connect PCx pins to FMC Alternate function */
        GPIOC->AFR[0]  |= 0x0000000C;
        GPIOC->AFR[1]  |= 0x00000000;
        /* Configure PCx pins in Alternate function mode */
        GPIOC->MODER   = 0x00000002;
        /* Configure PCx pins speed to 100 MHz */
        GPIOC->OSPEEDR = 0x00000003;
        /* Configure PCx pins Output type to push-pull */
        GPIOC->OTYPER  = 0x00000000;
        /* No pull-up, pull-down for PCx pins */
        GPIOC->PUPDR   = 0x00000000;

        /*
        PD0   ------> FMC_D2
        PD1   ------> FMC_D3
        PD8   ------> FMC_D13
        PD9   ------> FMC_D14
        PD10   ------> FMC_D15
        PD14   ------> FMC_D0
        PD15   ------> FMC_D1
        */
        /* Connect PDx pins to FMC Alternate function */
        GPIOD->AFR[0]  |= 0x000000CC;
        GPIOD->AFR[1]  |= 0xCC000CCC;
        /* Configure PDx pins in Alternate function mode */
        GPIOD->MODER   = 0xA02A000A;
        /* Configure PDx pins speed to 100 MHz */
        GPIOD->OSPEEDR = 0xF03F000F;
        /* Configure PDx pins Output type to push-pull */
        GPIOD->OTYPER  = 0x00000000;
        /* No pull-up, pull-down for PDx pins */
        GPIOD->PUPDR   = 0x00000000;


        /*
        PE0   ------> FMC_NBL0
        PE1   ------> FMC_NBL1
        PE7   ------> FMC_D4
        PE8   ------> FMC_D5
        PE9   ------> FMC_D6
        PE10   ------> FMC_D7
        PE11   ------> FMC_D8
        PE12   ------> FMC_D9
        PE13   ------> FMC_D10
        PE14   ------> FMC_D11
        PE15   ------> FMC_D12
         */

        /* Connect PEx pins to FMC Alternate function */
        GPIOE->AFR[0]  |= 0xC00000CC;
        GPIOE->AFR[1]  |= 0xCCCCCCCC;
        /* Configure PEx pins in Alternate function mode */
        GPIOE->MODER   = 0xAAAA800A;
        /* Configure PEx pins speed to 100 MHz */
        GPIOE->OSPEEDR = 0xFFFFC00F;
        /* Configure PEx pins Output type to push-pull */
        GPIOE->OTYPER  = 0x00000000;
        /* No pull-up, pull-down for PEx pins */
        GPIOE->PUPDR   = 0x00000000;


        /*
        PF0   ------> FMC_A0
        PF1   ------> FMC_A1
        PF2   ------> FMC_A2
        PF3   ------> FMC_A3
        PF4   ------> FMC_A4
        PF5   ------> FMC_A5
        PF11   ------> FMC_SDNRAS
        PF12   ------> FMC_A6
        PF13   ------> FMC_A7
        PF14   ------> FMC_A8
        PF15   ------> FMC_A9
        */
        /* Connect PFx pins to FMC Alternate function */
        GPIOF->AFR[0]  |= 0x00CCCCCC;
        GPIOF->AFR[1]  |= 0xCCCCC000;
        /* Configure PFx pins in Alternate function mode */
        GPIOF->MODER   = 0xAA800AAA;
        /* Configure PFx pins speed to 100 MHz */
        GPIOF->OSPEEDR = 0xFFC00FFF;
        /* Configure PFx pins Output type to push-pull */
        GPIOF->OTYPER  = 0x00000000;
        /* No pull-up, pull-down for PFx pins */
        GPIOF->PUPDR   = 0x00000000;


        /*
         PG0   ------> FMC_A10
         PG1   ------> FMC_A11
         PG4   ------> FMC_BA0
         PG5   ------> FMC_BA1
         PG8   ------> FMC_SDCLK
         PG15   ------> FMC_SDNCAS
         */
        /* Connect PGx pins to FMC Alternate function */
        GPIOG->AFR[0]  |= 0x00CC00CC;
        GPIOG->AFR[1]  |= 0xC000000C;
        /* Configure PGx pins in Alternate function mode */
        GPIOG->MODER   = 0x80020A0A;
        /* Configure PGx pins speed to 50 MHz */
        GPIOG->OSPEEDR = 0xC0030F0F;
        /* Configure PGx pins Output type to push-pull */
        GPIOG->OTYPER  = 0x00000000;
        /* No pull-up, pull-down for PGx pins */
        GPIOG->PUPDR   = 0x00000000;


        /*
        PH2   ------> FMC_SDCKE0
        PH3   ------> FMC_SDNE0
        PH8   ------> FMC_D16
        PH9   ------> FMC_D17
        PH10   ------> FMC_D18
        PH11   ------> FMC_D19
        PH12   ------> FMC_D20
        PH13   ------> FMC_D21
        PH14   ------> FMC_D22
        PH15   ------> FMC_D23
        */
        /* Connect PHx pins to FMC Alternate function */
        GPIOH->AFR[0]  |= 0x0000CC00;
        GPIOH->AFR[1]  |= 0xCCCCCCCC;
        /* Configure PHx pins in Alternate function mode */
        GPIOH->MODER   = 0xAAAA00A0;
        /* Configure PHx pins speed to 100 MHz */
        GPIOH->OSPEEDR = 0xFFFF00F0;
        /* Configure PHx pins Output type to push-pull */
        GPIOH->OTYPER  = 0x00000000;
        /* No pull-up, pull-down for PHx pins */
        GPIOH->PUPDR   = 0x00000000;


        /*
        PI0   ------> FMC_D24
        PI1   ------> FMC_D25
        PI2   ------> FMC_D26
        PI3   ------> FMC_D27
        PI4   ------> FMC_NBL2
        PI5   ------> FMC_NBL3
        PI6   ------> FMC_D28
        PI7   ------> FMC_D29
        PI9   ------> FMC_D30
        PI10   ------> FMC_D31
        */
        /* Connect PIx pins to FMC Alternate function */
        GPIOI->AFR[0]  |= 0xCCCCCCCC;
        GPIOI->AFR[1]  |= 0x00000CC0;
        /* Configure PIx pins in Alternate function mode */
        GPIOI->MODER   = 0x0028AAAA;
        /* Configure PIx pins speed to 100 MHz */
        GPIOI->OSPEEDR = 0x003CFFFF;
        /* Configure PIx pins Output type to push-pull */
        GPIOI->OTYPER  = 0x00000000;
        /* No pull-up, pull-down for PIx pins */
        GPIOI->PUPDR   = 0x00000000;

      /*-- FMC Configuration ------------------------------------------------------*/
        /* Enable the FMC interface clock */
        RCC->AHB3ENR |= 0x00000001; /*RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FMC, ENABLE);*/

        /* Configure and enable SDRAM bank1 */
        FMC_Bank5_6->SDCR[0] = 0x000019E4;
        FMC_Bank5_6->SDTR[0] = 0x01116361;

        /* SDRAM initialization sequence */
        /* Clock enable command */
        FMC_Bank5_6->SDCMR = FMC_SDCMR_CTB1 | FMC_SDCMR_MODE_0;
        tmpreg = FMC_Bank5_6->SDSR & FMC_SDSR_BUSY;
        while((tmpreg != 0) & (timeout-- > 0))
        {
          tmpreg = FMC_Bank5_6->SDSR & FMC_SDSR_BUSY;
        }

        /* Delay */
        for (index = 0; index<1000; index++);

        /* PALL command */
        FMC_Bank5_6->SDCMR = FMC_SDCMR_CTB1 | FMC_SDCMR_MODE_1;
        timeout = 0xFFFF;
        while((tmpreg != 0) & (timeout-- > 0))
        {
        tmpreg = FMC_Bank5_6->SDSR & FMC_SDSR_BUSY;
        }

        /* Auto refresh command */
        FMC_Bank5_6->SDCMR = FMC_SDCMR_CTB1 | FMC_SDCMR_MODE_2 | FMC_SDCMR_NRFS_3;
        timeout = 0xFFFF;
        while((tmpreg != 0) & (timeout-- > 0))
        {
        tmpreg = FMC_Bank5_6->SDSR & FMC_SDSR_BUSY;
        }

        /* MRD register program */
        FMC_Bank5_6->SDCMR = 0x00046014;/* Command constants taken from STM32F469I discovery BSP */
        timeout = 0xFFFF;
        while((tmpreg != 0) & (timeout-- > 0))
        {
        tmpreg = FMC_Bank5_6->SDSR & FMC_SDSR_BUSY;
        }

        /* Set refresh count */
        tmpreg = FMC_Bank5_6->SDRTR;
        FMC_Bank5_6->SDRTR = (tmpreg | (0x1429));

        /* Disable write protection */
        tmpreg = FMC_Bank5_6->SDCR[0];
        FMC_Bank5_6->SDCR[0] = (tmpreg & 0xFFFFFDFF);


      /*
          (SDCMR) (MDR << 9)
          MODEREG_BURST_LENGTH_1 = 0x0
          MODEREG_BURST_TYPE_SEQUENTIAL = 0x0
          MODEREG_CAS_LATENCY_3 = 0x30
          MODEREG_OPERATING_MODE_STANDARD = 0x0
          MODEREG_WRITEBURST_MODE_SINGLE = 0x200

          CTB1
          LOAD MODE REGISTER
      */



      /*
        Bank1_SDRAM is configured as follow:

        FMC_SDRAMTimingInitStructure.FMC_LoadToActiveDelay = 2;       //FMC_SDTR1_TMRD    ( FMC_SDTR1_TMRD_0                          = (FMC_LoadToActiveDelay - 1) )
        FMC_SDRAMTimingInitStructure.FMC_ExitSelfRefreshDelay = 7;    //FMC_SDTR1_TXSR    ( FMC_SDTR1_TXSR_1 | FMC_SDTR1_TXSR_2       = (FMC_ExitSelfRefreshDelay - 1) << 4 )
        FMC_SDRAMTimingInitStructure.FMC_SelfRefreshTime = 4;         //FMC_SDTR1_TRAS    ( FMC_SDTR1_TRAS_0 | FMC_SDTR1_TRAS_1       = (FMC_SelfRefreshTime - 1) << 8 )
        FMC_SDRAMTimingInitStructure.FMC_RowCycleDelay = 7;           //FMC_SDTR1_TRC     ( FMC_SDTR1_TRC_1 | FMC_SDTR1_TRC_2         = (FMC_RowCycleDelay - 1) << 12 )
        FMC_SDRAMTimingInitStructure.FMC_WriteRecoveryTime = 2;       //FMC_SDTR1_TWR     ( FMC_SDTR1_TWR_0                           = (FMC_WriteRecoveryTime - 1) << 16 )
        FMC_SDRAMTimingInitStructure.FMC_RPDelay = 2;                 //FMC_SDTR1_TRP     ( FMC_SDTR1_TRP_0                           = (FMC_RPDelay - 1) << 20 )
        FMC_SDRAMTimingInitStructure.FMC_RCDDelay = 2;                //FMC_SDTR1_TRCD    ( FMC_SDTR1_TRCD_0                          = (FMC_RCDDelay - 1) << 24 )

        FMC_SDRAMInitStructure.FMC_Bank = SDRAM_BANK;                                 //--                BANK1
        FMC_SDRAMInitStructure.FMC_ColumnBitsNumber = FMC_ColumnBits_Number_8b;       //FMC_SDCR1_NC      (0)
        FMC_SDRAMInitStructure.FMC_RowBitsNumber = FMC_RowBits_Number_12b;            //FMC_SDCR1_NR      (FMC_SDCR1_NR_0)
        FMC_SDRAMInitStructure.FMC_SDMemoryDataWidth = FMC_SDMemory_Width_32b;        //FMC_SDCR1_MWID    (FMC_SDCR1_MWID_1)
        FMC_SDRAMInitStructure.FMC_InternalBankNumber = FMC_InternalBank_Number_4;    //FMC_SDCR1_NB      (FMC_SDCR1_NB)
        FMC_SDRAMInitStructure.FMC_CASLatency = FMC_CAS_Latency_3;                    //FMC_SDCR1_CAS     (FMC_SDCR1_CAS_0 | FMC_SDCR1_CAS_1)
        FMC_SDRAMInitStructure.FMC_WriteProtection = FMC_Write_Protection_Disable;    //FMC_SDCR1_WP      (0)
        FMC_SDRAMInitStructure.FMC_SDClockPeriod = FMC_SDClock_Period_2;              //FMC_SDCR1_SDCLK   (FMC_SDCR1_SDCLK_1)
        FMC_SDRAMInitStructure.FMC_ReadBurst = FMC_Read_Burst_enable;                 //FMC_SDCR1_RBURST  (FMC_SDCR1_RBURST)
        FMC_SDRAMInitStructure.FMC_ReadPipeDelay = FMC_ReadPipe_Delay_0;              //FMC_SDCR1_RPIPE   (0)
        FMC_SDRAMInitStructure.FMC_SDRAMTimingStruct = &FMC_SDRAMTimingInitStructure;
      */
}
#endif /* DATA_IN_ExtSRAM && DATA_IN_ExtSDRAM */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
