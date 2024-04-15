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
 * @file      LCD_ST7789VICfgDef.h
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Interface for LCD Cfg.
 ******************************************************************************
 */
#ifndef LCD_ST7789VICFGDEF_H
#define LCD_ST7789VICFGDEF_H

#include <stdint.h>
#include "RoboticTypes.h"
#include "IDigitalOutputCfg.h"
#include "IDigitalOutput.h"
#include "ISpi.h"
#include <stm32_hal.h>

/* Backlight related configuration. */
typedef struct tLcdBacklightCfg_t
{
    GPIO_TypeDef* backlightPort; /* GPIO port for backlight pin */
    uint16_t      backlightPin;  /* GPIO pin backlight */

} tLcdBacklightCfg;

/* LTDC pin configuration for single GPIO pin. */
typedef struct tLcdRgbOnePinCfg_t
{
    GPIO_TypeDef* port; /* GPIO port */
    uint16_t      pin;  /* GPIO pin */
    uint32_t      af;   /* Alternate function to get LTDC peripheral on this port/pin configuration. */

} tRgbRgbOnePinCfg;

/* Clock configuration for LTDC clock source. */
typedef struct tLcdLtdcClockCfg_t
{
    uint32_t pllsaiN;
    uint32_t pllsaiP;
    uint32_t pllsaiR;
    uint32_t pllsaiDivider;
} tLcdLtdcClockCfg;

/* LCD control interface configuration. */
typedef struct tLcdST7789VICfg_t
{
    tISpiDevice              interface; /* SPI interface to LCD controller */
    tIDigitalOutput_PinIndex resetPin;  /* DigitalOutput Pin for LCD reset */
} tLcdST7789VICfg;

#endif /*LCD_ST7789VICFGDEF_H*/
