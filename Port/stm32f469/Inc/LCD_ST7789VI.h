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
 * @file      LCD_ST7789VI.h
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Very HW specific interface for LCD driver for ST7789VI display controller
 ******************************************************************************
 */
#ifndef LCD_ST7789VI_H
#define LCD_ST7789VI_H

#include <stdint.h>
#include "LCD_ST7789VICfgDef.h"
#include "LCD.h"

/**
*************************************************************************
* @brief   LCD_InitSequence
*          Execute initialization command sequence for ST7789VI.
* @param   config Pointer to LCD controller interface configuration.
*************************************************************************
*/
void LCD_InitSequence(tLcdST7789VICfg* config);

/**
*************************************************************************
* @brief   LCD_CheckConfiguration
*          Check if the send configuration is valid.
* @param   config Pointer to LCD controller interface configuration.
*************************************************************************
*/
void LCD_CheckConfiguration(tLcdST7789VICfg* config);

/**
*************************************************************************
* @brief   LCD_Reset
*          HW LCD reset by toggling reset pin.
* @param   config Pointer to LCD controller interface configuration.
*************************************************************************
*/
void LCD_Reset(tLcdST7789VICfg* config);

/**
*************************************************************************
* @brief   LCD_ReadDisplayId
* @param   config A pointer to LCD config structure required to get
*          the connection to display controller
* @returns LCD id combined into 32bit value
*************************************************************************
*/
uint32_t LCD_ReadDisplayId(tLcdST7789VICfg* config);

/**
*************************************************************************
* @brief   LCD_SetRotation
* @param   config A pointer to LCD config structure required to get
*          the connection to display controller
* @param   rotation LcdRotation type
*************************************************************************
*/
void LCD_SetRotation(tLcdST7789VICfg* config, const LcdRotation* rotation);

/**
*************************************************************************
* @brief   LCD_Sleep
* @param   config A pointer to LCD config structure required to get
*          the connection to display controller
*************************************************************************
*/
void LCD_Sleep(tLcdST7789VICfg* config);

/**
*************************************************************************
* @brief   LCD_WakeUp
* @param   config A pointer to LCD config structure required to get
*          the connection to display controller
*************************************************************************
*/
void LCD_WakeUp(tLcdST7789VICfg* config);

#endif /* LCD_ST7789VI_H */
