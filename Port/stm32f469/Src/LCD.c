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
 * @file      LCD.c
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Driver for LCD display
 ******************************************************************************
 */

#include "LCD_Backlight.h"
#include "LCD.h"
#include "LCD_ST7789VI.h"
#include "ILCDCfg.h"
#include <stdlib.h>
#include <string.h>

#include <stm32_hal.h>

#ifndef FRAMEBUFFER_BPP
#define FRAMEBUFFER_BPP 2 // Bytes per pixel
#endif

typedef struct
{
    uint8* address;
} tLcdVars;

static bool FBSwapped;

void ILCD_Init( void )
{
    static bool initialized = FALSE;
    if ( !initialized )
    {
        FBSwapped = true; // Has to be true to allow drawing on startup

        // dependent modules
        IDigitalOutput_Init();
        MX_SPI2_Init();

        // Initialize LTDC
        MX_LTDC_Init();

        initialized = TRUE;
    }

    return;
}

void ILCD_Start( void )
{
    static bool started = FALSE;
    if ( !started )
    {
        // other modules
        IDigitalOutput_Start();

        // backlinght
        LCD_BacklightSetup( &lcdBacklightConfig );

        // controller
        ILCD_Reset();                             // this takes more than 30us
        LCD_InitSequence( &lcdControllerConfig ); // this contain delays topping up to 475ms

        started = TRUE;
    }
}

void ILCD_Reset( void )
{
    LCD_Reset( &lcdControllerConfig );
}

void ILCD_Backlight( bool backlight )
{
    LCD_BacklightEnable( &lcdBacklightConfig, backlight );
}

void ILCD_SetRotation( LcdRotation rotation )
{
    LCD_SetRotation( &lcdControllerConfig, &rotation );
}

uint32 LCD_GetDisplayId( void )
{
    return LCD_ReadDisplayId( &lcdControllerConfig );
}

uint16 ILCD_getWidth()
{
    return LCD_ST7789VI_PIXELS;
}

uint16 ILCD_getHeight()
{
    return LCD_ST7789VI_LINES;
}
