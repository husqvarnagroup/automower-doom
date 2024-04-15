/**
 ******************************************************************************
 * Copyright (c) 2023 Husqvarna AB.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/*
 ------------------------------------------------------------------------------
    Include files
 ------------------------------------------------------------------------------
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "RoboticTypes.h"
#include "IDraw.h"
#include "IWatchdog.h"
#include "LCD.h"
#include "mylcd.h"

/*
 ------------------------------------------------------------------------------
    Defines
 ------------------------------------------------------------------------------
 */
#define ROWS ( 240 ) // physical LCD screen height (screen is rotated 270 degrees in relation to user)
#define COLS ( 320 ) // physical LCD screen width (screen is rotated 270 degrees in relation to user)

/*
 ------------------------------------------------------------------------------
    Private data
 ------------------------------------------------------------------------------
 */
/*
 * we have a simple buffer that we let the DOOM code write into, before we transfer the data to the LCD driver
 */
static uint8_t* lcd_frame_buffer = NULL;

/*
 ------------------------------------------------------------------------------
    Interface functions
 ------------------------------------------------------------------------------
 */
/**
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void lcd_init( void )
{
    lcd_frame_buffer = malloc( 320 * 200 * 2 );
    memset( lcd_frame_buffer, '\0', 320 * 200 * 2 );
}

/**
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void lcd_start( void )
{
    // deliberately a NOP!
}

/**
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void lcd_refresh( void )
{
    static bool once = false;

    if ( !once )
    {
        once = true;

        ILCD_SetRotation( LCD_ROTATION_REVERSE );

        // make the entire screen black
        IDraw_FillDisplay( 0xFF000000 );
    }
    // simply draw the entire screen buffer on the LCD display
    // as the DOOM screen has less rows than the actual physical screen, we center it on the screen (y-offset)
    const tIDraw_Position pos = { .x = 0, .y = 20 };
    IDraw_ImageFromMemory( (const void* const)lcd_frame_buffer, (tIDraw_Position*)&pos );

    // each time we are asked to refresh the screen, the DOOM game seems to be working
    IWatchdog_Refresh();
}

/**
 ******************************************************************************
 * Function
 ******************************************************************************
 */
uint8_t* lcd_get_frame_buffer( void )
{
    return (uint8_t*)lcd_frame_buffer;
}
