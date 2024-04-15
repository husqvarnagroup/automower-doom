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
 * @file      LCD.h
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Interface for LCD
 ******************************************************************************
 */
#ifndef LCD_H
#define LCD_H

#include "ILCD.h"

/* Screen resolutions and rotations defines */
#define LCD_ST7789VI_PIXELS 240
#define LCD_ST7789VI_LINES  320

/**
 * @brief   Get display ID of the connected LCD.
 * @returns display ID as one number (may be combined from, for example, manufacturer ID and driver version).
 */
uint32 LCD_GetDisplayId( void );

/**
 * @brief   Get current address of frame buffer. That is the address from where pixels are taken to screen.
 * @returns Address of frame buffer that is displayed on the screen.
 */
uint8* LCD_GetFrameBuffer();

/**
 * @brief   Get current address of displayed buffer.
 * @returns Address of display buffer that is displayed on the screen.
 */
uint8* LCD_GetDisplayBuffer();

#endif /* LCD_H */
