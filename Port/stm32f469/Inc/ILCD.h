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
 * @file      ILCD.h
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Interface for LCD.
 ******************************************************************************
 */
#ifndef ILCD_H
#define ILCD_H

#include "RoboticTypes.h"
/*
-------------------------
   Type definitions
-------------------------
*/
#define LCD_SMALL   0x00
#define LCD_NORMAL  0x01
#define ILCD_COMMUNICATION_CHECK_PERIOD 10000 //ms
typedef enum {LCD_ROTATION_NORMAL, LCD_ROTATION_REVERSE} LcdRotation;

/*
-------------------------
   Interface functions
-------------------------
*/


/**
 ******************************************************************************
 * @brief   Init module
 ******************************************************************************
 */
void ILCD_Init(void);

/**
 ******************************************************************************
 * @brief   Start module
 ******************************************************************************
 */
void ILCD_Start(void);

/**
 ******************************************************************************
 * @brief   Control backlight
 * @param   backlight
 *          true if lit
 ******************************************************************************
 */
void ILCD_Backlight(bool backlight);

/**
 ******************************************************************************
 * @brief   Sets the screen rotation
 ******************************************************************************
 */
void ILCD_SetRotation(LcdRotation rotation);

/**
 ******************************************************************************
 * @brief   Get LCD Width
 * @returns width
 ******************************************************************************
 */
uint16 ILCD_getWidth();

/**
 ******************************************************************************
 * @brief   Get LCD Height
 * @returns height
 ******************************************************************************
 */
uint16 ILCD_getHeight();

/**
 ******************************************************************************
 * @brief   reset lcd driver chip
 * @returns
 ******************************************************************************
 */
void ILCD_Reset(void);


#endif
