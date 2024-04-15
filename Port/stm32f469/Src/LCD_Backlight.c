//
// Copyright(C) 2023 Husqvarna AB
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//

/**
 ******************************************************************************
 * @file      LCD_Backlight.c
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Back light control for LCD display using PWM
 ******************************************************************************
 */

#include "LCD_Backlight.h"
#include <stm32_hal.h>

/*
 ---------------------------------
 Private functions
 ---------------------------------
 */

static void EnableClock( GPIO_TypeDef* port )
{
    switch ( (uint32_t)port )
    {
        case (uint32_t)GPIOB:
        {
            __HAL_RCC_GPIOB_CLK_ENABLE();
            break;
        }
        case (uint32_t)GPIOC:
        {
            __HAL_RCC_GPIOC_CLK_ENABLE();
            break;
        }
        case (uint32_t)GPIOD:
        {
            __HAL_RCC_GPIOD_CLK_ENABLE();
            break;
        }
        case (uint32_t)GPIOE:
        {
            __HAL_RCC_GPIOE_CLK_ENABLE();
            break;
        }
        case (uint32_t)GPIOF:
        {
            __HAL_RCC_GPIOF_CLK_ENABLE();
            break;
        }
        case (uint32_t)GPIOG:
        {
            __HAL_RCC_GPIOG_CLK_ENABLE();
            break;
        }
        case (uint32_t)GPIOH:
        {
            __HAL_RCC_GPIOH_CLK_ENABLE();
            break;
        }
        case (uint32_t)GPIOI:
        {
            __HAL_RCC_GPIOI_CLK_ENABLE();
            break;
        }
        case (uint32_t)GPIOJ:
        {
            __HAL_RCC_GPIOJ_CLK_ENABLE();
            break;
        }
        case (uint32_t)GPIOK:
        {
            __HAL_RCC_GPIOK_CLK_ENABLE();
            break;
        }
    }
}

/*
 ------------------------------
 Public functions
 ------------------------------
 */
void LCD_BacklightSetup( tLcdBacklightCfg* config )
{
    if ( !config )
    {
        return;
    }

    EnableClock( config->backlightPort );

    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    GPIO_InitStruct.Mode             = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull             = GPIO_NOPULL;
    GPIO_InitStruct.Speed            = GPIO_SPEED_LOW;
    GPIO_InitStruct.Pin              = config->backlightPin;
    HAL_GPIO_Init( config->backlightPort, &GPIO_InitStruct );
}

void LCD_BacklightEnable( tLcdBacklightCfg* config, bool enable )
{
    if ( enable )
    {
        HAL_GPIO_WritePin( config->backlightPort, config->backlightPin, GPIO_PIN_SET );
    }
    else
    {
        HAL_GPIO_WritePin( config->backlightPort, config->backlightPin, GPIO_PIN_RESET );
    }
}

void LCD_setBacklightLevel( tLcdBacklightCfg* config, uint16_t level )
{
    return;
}
