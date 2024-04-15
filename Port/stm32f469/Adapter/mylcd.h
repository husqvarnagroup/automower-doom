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

#ifndef __MY_LCD_H__
#define __MY_LCD_H__

/*
 ------------------------------------------------------------------------------
    Defines
 ------------------------------------------------------------------------------
 */
#define LCD_MAX_X ( 200 ) // DOOM LCD screen max width (screen is rotated 270 degrees in relation to user)
#define LCD_MAX_Y ( 320 ) // DOOM LCD screen max height (screen is rotated 270 degrees in relation to user)

#define LCD_RGB565( r, g, b )   ( ( ( ( r & 0xF8 ) >> 3 ) << 11 ) | ( ( ( g & 0xFC ) >> 2 ) << 5 ) | ( ( b & 0xF8 ) >> 3 ) )
#define LCD_RGB565_R( color )   ( ( 0xF800 & color ) >> 11 )
#define LCD_RGB565_G( color )   ( ( 0x07E0 & color ) >> 5 )
#define LCD_RGB565_B( color )   ( 0x001F & color )

/*
 ------------------------------------------------------------------------------
    Interface functions
 ------------------------------------------------------------------------------
 */
/**
 ******************************************************************************
 * @brief   Initializes LCD adapter module
 ******************************************************************************
 */
extern void lcd_init( void );

/**
 ******************************************************************************
 * @brief   Starts LCD adapter module
 ******************************************************************************
 */
extern void lcd_start( void );

/**
 ******************************************************************************
 * @brief   Called whenever the screen buffer has been updated
 ******************************************************************************
 */
extern void lcd_refresh( void );

/**
 ******************************************************************************
 * @brief   Gets hold of the adapter's allocated LCD screen buffer
 *          Simply write to this and call lcd_refresh() to draw a new screen
 ******************************************************************************
 */
extern uint8_t* lcd_get_frame_buffer( void );

#endif // __MY_LCD_H__
