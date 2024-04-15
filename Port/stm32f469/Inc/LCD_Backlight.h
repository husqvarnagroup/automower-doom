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
 * @file      LCD_Backlight.h
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Interface for controlling LCD back light using a PWM timer.
 ******************************************************************************
 */

#ifndef LCD_BACKLIGHT_H
#define LCD_BACKLIGHT_H

#include <stdbool.h>
#include <stdint.h>
#include "LCD_ST7789VICfgDef.h"

/**
*************************************************************************
* @brief   LCD_BacklightSetup
*          Configure back light PWM according to passed configuration.
* @param   config   Pointer to PWM configuration.
*************************************************************************
*/
void LCD_BacklightSetup(tLcdBacklightCfg* config);

/**
*************************************************************************
* @brief   LCD_BacklightEnable
*          Turn the back light on or off (start or stop relevant timer).
* @param   config   Pointer to PWM configuration.
* @param   enable   TRUE if back light should be turned on.
*************************************************************************
*/
void LCD_BacklightEnable(tLcdBacklightCfg* config, bool enable);

/**
*************************************************************************
* @brief   LCD_setBacklightLevel
*          Change back light level
* @param   config   Pointer to PWM configuration.
* @param   level    Back light brightness in % (in range of 0-100)
*************************************************************************
*/
void LCD_setBacklightLevel(tLcdBacklightCfg* config, uint16_t level);

#endif /* LCD_BACKLIGHT_H */
