/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
uint32_t my_get_systime(void);
void sleep_ms( uint32_t ms );
char* strupr( char* s );
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FRAMEBUFFER_BPP 2
#define IMAGE_WIDTH 240
#define IMAGE_HEIGHT 320
#define WIFI_BT_FW_UPGRADE_Pin GPIO_PIN_12
#define WIFI_BT_FW_UPGRADE_GPIO_Port GPIOC
#define CAN_Tx_Pin GPIO_PIN_12
#define CAN_Tx_GPIO_Port GPIOA
#define START_BUTTON_Pin GPIO_PIN_10
#define START_BUTTON_GPIO_Port GPIOG
#define CAN_STB_n_Pin GPIO_PIN_14
#define CAN_STB_n_GPIO_Port GPIOJ
#define SPI2_SCK_Pin GPIO_PIN_3
#define SPI2_SCK_GPIO_Port GPIOD
#define CAN_Rx_Pin GPIO_PIN_11
#define CAN_Rx_GPIO_Port GPIOA
#define SPI2_LCD_CS_Pin GPIO_PIN_9
#define SPI2_LCD_CS_GPIO_Port GPIOG
#define nRESET_BLE_WIFI_Pin GPIO_PIN_15
#define nRESET_BLE_WIFI_GPIO_Port GPIOJ
#define VBAT_EN_Pin GPIO_PIN_10
#define VBAT_EN_GPIO_Port GPIOA
#define SPI2_EEPROM_CS_Pin GPIO_PIN_9
#define SPI2_EEPROM_CS_GPIO_Port GPIOC
#define I2C2_SDA_3D_Pin GPIO_PIN_5
#define I2C2_SDA_3D_GPIO_Port GPIOH
#define CAN_EN_Pin GPIO_PIN_6
#define CAN_EN_GPIO_Port GPIOG
#define SPI2_MOSI_Pin GPIO_PIN_3
#define SPI2_MOSI_GPIO_Port GPIOC
#define DISP_EN_Pin GPIO_PIN_12
#define DISP_EN_GPIO_Port GPIOB
#define SPI2_MISO_Pin GPIO_PIN_2
#define SPI2_MISO_GPIO_Port GPIOC
#define MAIN_SW_Pin GPIO_PIN_13
#define MAIN_SW_GPIO_Port GPIOD
#define RST_LCD_N_Pin GPIO_PIN_11
#define RST_LCD_N_GPIO_Port GPIOD
#define I2C2_SCL_3D_Pin GPIO_PIN_10
#define I2C2_SCL_3D_GPIO_Port GPIOB
#define VDD_HAL_Pin GPIO_PIN_6
#define VDD_HAL_GPIO_Port GPIOH
#define SPI2_IMU_CS_Pin GPIO_PIN_1
#define SPI2_IMU_CS_GPIO_Port GPIOJ
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
