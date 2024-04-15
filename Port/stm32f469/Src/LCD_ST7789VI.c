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
 * @brief     LCD driver for ST7789VI display controller
 ******************************************************************************
 */
#include "LCD_ST7789VI.h"
#include "LCD_ST7789VICfgDef.h"
#include "LCD.h"
#include "Spi9bit.h"
#include "ISpi.h"
#include "IDigitalOutputCfg.h"

#include <stm32_hal.h>

/*
 ---------------------------------
 Local definitions
 ---------------------------------
 */
#define LCD_RESET_DELAY ( 11 * 3 ) /*11 ms based on display documentation and '3' based on tests*/

/* LCD commands */
#define ST7789H2_LCD_RESET         0x01
#define ST7789H2_LCD_ID            0x04
#define ST7789H2_SLEEP_IN          0x10
#define ST7789H2_SLEEP_OUT         0x11
#define ST7789H2_PARTIAL_DISPLAY   0x12
#define ST7789H2_DISPLAY_INVERSION 0x21
#define ST7789H2_DISPLAY_STRAIGHT  0x20
#define ST7789H2_DISPLAY_OFF       0x28
#define ST7789H2_DISPLAY_ON        0x29
#define ST7789H2_WRITE_RAM         0x2C
#define ST7789H2_READ_RAM          0x2E
#define ST7789H2_CASET             0x2A
#define ST7789H2_RASET             0x2B
#define ST7789H2_VSCRDEF           0x33 /* Vertical Scroll Definition */
#define ST7789H2_VSCSAD            0x37 /* Vertical Scroll Start Address of RAM */
#define ST7789H2_TEARING_EFFECT    0x35
#define ST7789H2_MAD_CTRL_W        0x36 /* Memory data access control */
#define ST7789H2_MAD_CTRL_R        0x0B
#define ST7789H2_IDLE_MODE_OFF     0x38
#define ST7789H2_IDLE_MODE_ON      0x39
#define ST7789H2_COLOR_MODE_W      0x3A
#define ST7789H2_COLOR_MODE_R      0x0C
#define ST7789H2_RAM_CTRL          0xB0
#define ST7789H2_RGB_CTRL          0xB1
#define ST7789H2_PORCH_CTRL        0xB2
#define ST7789H2_GATE_CTRL         0xB7
#define ST7789H2_VCOM_SET          0xBB
#define ST7789H2_DISPLAY_OFF       0x28
#define ST7789H2_LCM_CTRL          0xC0
#define ST7789H2_VDV_VRH_EN        0xC2
#define ST7789H2_VDV_SET           0xC4
#define ST7789H2_VCOMH_OFFSET_SET  0xC5
#define ST7789H2_FR_CTRL           0xC6
#define ST7789H2_POWER_CTRL        0xD0
#define ST7789H2_PV_GAMMA_CTRL     0xE0
#define ST7789H2_NV_GAMMA_CTRL     0xE1

/*
 ---------------------------------
 Local types
 ---------------------------------
 */
typedef struct
{
    LcdRotation rotationST7789VI;
    tISpiDevice spiST7789VI;
} tLcdVars;
/*
 ---------------------------------
 Local variables
 ---------------------------------
 */
static bool     initialized = false;
static tLcdVars lcdVars     = { 0 };

/*
 ---------------------------------
 Private functions
 ---------------------------------
 */
static void LCD_send( tISpiDevice device, Spi_CD cdBit, uint8_t byte, uint16_t delay );
static void LCD_read( tISpiDevice device, uint8_t command, uint16_t count, uint8_t* buffer );

/*
 ------------------------------
 Public functions
 ------------------------------
 */

void LCD_InitSequence( tLcdST7789VICfg* config )
{
    tISpiDevice lcd;
    if ( !config )
    {
        return;
    }
    lcd = config->interface;
    HAL_Delay( 120 );
    LCD_send( lcd, CMD, ST7789H2_SLEEP_IN, 10 );
    LCD_send( lcd, CMD, ST7789H2_LCD_RESET, 200 );
    LCD_send( lcd, CMD, ST7789H2_SLEEP_OUT, 120 );

    LCD_send( lcd, CMD, ST7789H2_MAD_CTRL_W, 0 );
    LCD_send( lcd, DATA, 0x14, 0 ); // Rotate display, RGB order

    LCD_send( lcd, CMD, ST7789H2_COLOR_MODE_W, 0 );
    LCD_send( lcd, DATA, 0x55, 0 ); // 16 bit color on RGB and control interface

    LCD_send( lcd, CMD, ST7789H2_RGB_CTRL, 0 );
    LCD_send( lcd, DATA, 0xC0, 0 ); // use shift register|RGB DE interface|polarity
    LCD_send( lcd, DATA, 0x04, 0 ); // vertical back porch
    LCD_send( lcd, DATA, 0x0A, 0 ); // horizontal back porch

    LCD_send( lcd, CMD, ST7789H2_RAM_CTRL, 0 );
    LCD_send( lcd, DATA, 0x11, 0 ); // RAM access from RGB, display RGB
    LCD_send( lcd, DATA, 0xE0, 0 ); // 18 bit bus, MSB for missing colour bits (R&B LSB)

    LCD_send( lcd, CMD, ST7789H2_DISPLAY_STRAIGHT, 0 );
    LCD_send( lcd, CMD, ST7789H2_CASET, 0 );
    LCD_send( lcd, DATA, 0x00, 0 );
    LCD_send( lcd, DATA, 0x00, 0 );
    LCD_send( lcd, DATA, 0x00, 0 );
    LCD_send( lcd, DATA, 0xEF, 0 ); // 240

    LCD_send( lcd, CMD, ST7789H2_RASET, 0 );
    LCD_send( lcd, DATA, 0x00, 0 );
    LCD_send( lcd, DATA, 0x00, 0 );
    LCD_send( lcd, DATA, 0x01, 0 );
    LCD_send( lcd, DATA, 0x3F, 0 ); // 320

    LCD_send( lcd, CMD, ST7789H2_DISPLAY_ON, 60 );
    /* @note: Configuring display controller can take even 60 ms acording to display manufacturer,
     * wait before turning on backlight */

    initialized = true;
}

void LCD_CheckConfiguration( tLcdST7789VICfg* config )
{
    tISpiDevice   lcd;
    uint8_t       returnedVal  = 0xFF;
    static uint32 errorCounter = 0;
    bool          isError      = false;
    if ( !config )
    {
        return;
    }
    lcd = config->interface;

    LCD_read( lcd, ST7789H2_MAD_CTRL_R, 1, &returnedVal );
    if ( returnedVal != 0x00 && returnedVal != 0x14 )
    {
        isError = true;
    }

    LCD_read( lcd, ST7789H2_COLOR_MODE_R, 1, &returnedVal );
    // Display returns correctly only low nibble of byte
    if ( ( 0x0F & returnedVal ) != ( 0x0F & 0x55 ) )
    {
        isError = true;
    }

    if ( isError )
    {
        errorCounter++;
    }

    return;
}

void LCD_Reset( tLcdST7789VICfg* config )
{
    if ( !config )
    {
        return;
    }
    /* Delays here should be more than 10us */
    IDigitalOutput_Set( config->resetPin, TRUE );
    HAL_Delay( LCD_RESET_DELAY );
    IDigitalOutput_Set( config->resetPin, FALSE );
    HAL_Delay( LCD_RESET_DELAY );
    IDigitalOutput_Set( config->resetPin, TRUE );
    HAL_Delay( LCD_RESET_DELAY );
}

uint32_t LCD_ReadDisplayId( tLcdST7789VICfg* config )
{
    tISpiDevice lcd;
    uint8_t     response[ 3 ] = { 0 };
    if ( !config )
    {
        return 0;
    }
    lcd = config->interface;
    LCD_read( lcd, ST7789H2_LCD_ID, 3, response );
    return ( response[ 2 ] | ( response[ 1 ] << 8 ) | ( response[ 0 ] << 16 ) );
}

void LCD_SetRotation( tLcdST7789VICfg* config, const LcdRotation* rotation )
{
    lcdVars.rotationST7789VI = *rotation;
    lcdVars.spiST7789VI      = config->interface;
}

/* LTDC interrupt callbacks */
void HAL_LTDC_ErrorCallback( LTDC_HandleTypeDef* hltdc )
{
}

void HAL_LTDC_LineEventCallback( LTDC_HandleTypeDef* hltdc )
{
    if ( false == initialized )
    {
        return;
    }
    switch ( lcdVars.rotationST7789VI )
    {
        case LCD_ROTATION_NORMAL:
            LCD_send( lcdVars.spiST7789VI, CMD, ST7789H2_MAD_CTRL_W, 0 );
            LCD_send( lcdVars.spiST7789VI, DATA, 0x00, 0 ); // 270 rotate respect to default display orientation
            break;

        case LCD_ROTATION_REVERSE:
            LCD_send( lcdVars.spiST7789VI, CMD, ST7789H2_MAD_CTRL_W, 0 );
            LCD_send( lcdVars.spiST7789VI, DATA, 0x14, 0 ); // 90 rotate respect to default display orientation
            break;

        default:
            LCD_send( lcdVars.spiST7789VI, CMD, ST7789H2_MAD_CTRL_W, 0 );
            LCD_send( lcdVars.spiST7789VI, DATA, 0x00, 0 ); // 270 rotate respect to default display orientation
            break;
    }
}

void HAL_LTDC_ReloadEventCallback( LTDC_HandleTypeDef* hltdc )
{
    // NOP
}

/* Send a sleep command */
void LCD_Sleep( tLcdST7789VICfg* config )
{
    tISpiDevice lcd;
    if ( !config )
    {
        return;
    }
    lcd = config->interface;

    LCD_send( lcd, CMD, ST7789H2_SLEEP_IN, 5 );

    return;
}

/* Send a wake up command */
void LCD_WakeUp( tLcdST7789VICfg* config )
{
    tISpiDevice lcd;
    if ( !config )
    {
        return;
    }
    lcd = config->interface;

    LCD_send( lcd, CMD, ST7789H2_SLEEP_OUT, 5 );

    return;
}

/*
 ------------------------------
 Private functions
 ------------------------------
 */

/**
 * Send a command or data to ST7789VI controller using 3 wire, 9 bit SPI
 * and wait given amount of milliseconds afterwards.
 */
static void LCD_send( tISpiDevice device, Spi_CD cdBit, uint8_t byte, uint16_t delay )
{
    uint8_t dummy;
    if ( !ISpi_StartTransfer( device ) )
    {
        return;
    }
    ISpi_SendDC( device, cdBit );
    ISpi_TxRx( device, byte, &dummy );
    ISpi_StopTransfer( device );
    HAL_Delay( delay );
}

/**
 * Read from ST7789VI using 3 wire, 9 bit SPI.
 * There should be 1 empty clock cycle after sending the command,
 * before reading incoming data if there is more than 1 byte to read.
 */

static void LCD_read( tISpiDevice device, uint8_t command, uint16_t count, uint8_t* buffer )
{
    uint8_t dummy;
    if ( !ISpi_StartTransfer( device ) )
    {
        return;
    }
    ISpi_SendDC( device, CMD );
    ISpi_TxRx( device, command, &dummy );
    if ( count > 1 )
    {
        ISpi_IdleCycle( device );
    }
    for ( uint16_t i = 0; i < count; i++ )
    {
        ISpi_TxRx( device, dummy, &buffer[ i ] );
    }
    ISpi_StopTransfer( device );
}
