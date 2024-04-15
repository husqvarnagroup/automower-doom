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
 * @file      Draw.c
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Draw module implementation.
 ******************************************************************************
 */
/*
 ------------------------------------------------------------------------------
 Include files
 ------------------------------------------------------------------------------
 */
#include "Draw.h"
#include "LCD.h"

#include <stdlib.h>
#include <string.h>
#include <stm32_hal.h>

/*
 ------------------------------------------------------------------------------
 Local defines
 ------------------------------------------------------------------------------
 */
#define GLOBAL_ROTATION 270

/*
-------------------------------------------------------------------------------
    Local types
-------------------------------------------------------------------------------
*/
typedef struct
{
    tEventCallback eventCallback;
} tDrawVars;

/*
 ------------------------------------------------------------------------------
 Private data
 ------------------------------------------------------------------------------
 */
static tDrawVars drawVars;

/*
 ------------------------------------------------------------------------------
 Private function prototypes
 ------------------------------------------------------------------------------
 */
static void DMA2D_Init_M2M( uint32_t pixelFormat,
                            uint32_t outputAddress,
                            uint32_t inputAddress,
                            uint32_t width, uint32_t height,
                            uint32_t outputSkip,
                            uint32_t inputSkip );

// Interrupt callbacks
void TransferErrorCallback( DMA2D_HandleTypeDef* hdma2d );
void TransferCompleteCallback( DMA2D_HandleTypeDef* hdma2d );

/*
 ------------------------------------------------------------------------------
 Implementation of interface functions
 ------------------------------------------------------------------------------
 */
/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void IDraw_Init( void )
{
    static bool initialised = false;
    if ( !initialised )
    {
        initialised = true;

        ILCD_Init();

        // Initialize DMA2D
        memset( &( hdma2d ), 0, sizeof( hdma2d ) );
        hdma2d.Instance          = DMA2D;
        hdma2d.XferCpltCallback  = &TransferCompleteCallback;
        hdma2d.XferErrorCallback = &TransferErrorCallback;
        MX_DMA2D_Init();
    }
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void IDraw_Start( void )
{
    static bool started = false;
    if ( !started )
    {

        ILCD_Start();

        // Blank display before enabling backlight
        IDraw_FillDisplay( 0xFF000000 );
        ILCD_Backlight( true );
        started = true;
    }
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
bool IDraw_ImageFromMemory( const void* const address, tIDraw_Position* pPos )
{
    tIDraw_ControlBlock cb;
    cb.header.xSize     = 320;
    cb.header.ySize     = 200;
    cb.header.totalSize = 320 * 200 * 2;

    cb.dataRGB565.pData = (uint16*)address;
    cb.dataRGB565.pitch = 320;

    if ( !IDraw_Draw( &cb, pPos->x, pPos->y, NULL ) )
    {
        return false;
    }
    return true;
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
bool IDraw_FillDisplay( uint32_t colorARGB888 )
{
    // Use DMA2D to fill display with black using R2M
    HAL_DMA2D_DeInit( &hdma2d );
    hdma2d.Init.Mode         = DMA2D_R2M;
    hdma2d.Init.ColorMode    = DMA2D_OUTPUT_RGB565;
    hdma2d.Init.OutputOffset = 0;
    HAL_DMA2D_Init( &hdma2d );

    // Start DMA2D transfer
    HAL_DMA2D_Start( &hdma2d, colorARGB888, ltdc_frame_buffer, IMAGE_WIDTH, IMAGE_HEIGHT );
    // Reload display
    HAL_LTDC_Reload( &hltdc, LTDC_RELOAD_IMMEDIATE );

    HAL_Delay( 1 ); // Delay for just a millisecond to allow display to clear. Should prob. be done with callbacks from interrupt.
    return true;
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
bool IDraw_Draw( tIDraw_ControlBlock* pControlBlock,
                 uint16               x,
                 uint16               y,
                 tEventCallback       eventCallback )
{
    // Checking if image is not zero
    if ( !pControlBlock->header.totalSize )
    {
        tEvent sendEvent;
        sendEvent.id = IDRAW_EVENT_DONE;
        // IScheduler_PushEvent( sendEvent, eventCallback );
        eventCallback( sendEvent );
        return true;
    }

    // Set up scoped variables
    int32_t physicalImageWidth, x1 = 0, y1 = 0, delta = 0;
    int32_t imageWidth    = (int32_t)pControlBlock->header.xSize; /*< Width of source image  */
    int32_t imageHeight   = (int32_t)pControlBlock->header.ySize; /*< Height of source image */
    int32_t displayWidth  = (int32_t)IMAGE_WIDTH;                 /*< Width of display       */
    int32_t displayHeight = (int32_t)IMAGE_HEIGHT;                /*< Height of display      */

    // Transpose image according to rotation
    switch ( GLOBAL_ROTATION )
    {
        case 0:
            physicalImageWidth = imageWidth;
            // Calculating new position taking into account software rotation.
            // This is hardcoded rotation by 0 degrees, assuming that imageHeight
            //  is already updated to "new, rotated" value.
            x1 = (int32_t)x;
            y1 = (int32_t)y;
            break;
        case 180:
            physicalImageWidth = imageWidth;
            // Calculating new position taking into account software rotation.
            // This is hardcoded rotation by 180 degrees, assuming that imageHeight
            //  is already updated to "new, rotated" value.
            x1 = ( displayWidth - (int32_t)y - imageWidth );
            y1 = ( displayHeight - (int32_t)x - imageHeight );
            break;
        case 90:
        {
            physicalImageWidth = imageHeight;
            imageHeight        = imageWidth;
            imageWidth         = physicalImageWidth;
            // Calculating new position taking into account software rotation.
            // This is hardcoded rotation by 90 degrees, assuming that imageHeight
            //  is already updated to "new, rotated" value.
            x1 = ( displayWidth - (int32_t)y - imageWidth );
            y1 = (int32_t)x;
            break;
        }
        case 270:
        {
            physicalImageWidth = imageHeight;
            imageHeight        = imageWidth;
            imageWidth         = physicalImageWidth;
            // Calculating new position taking into account software rotation.
            // This is hardcoded rotation by 270 degrees, assuming that imageHeight
            //  is already updated to "new, rotated" value.
            x1 = (int32_t)y;
            y1 = ( displayHeight - (int32_t)x - imageHeight );
            break;
        }
    }

    // Checking if at least part of an image is inside the display
    if ( ( x1 >= displayWidth ) || ( ( y1 + imageHeight ) < 0 ) )
    {
        return false;
    }

    // If the bottomRight corner is outside the display, image is truncated
    // x
    if ( ( x1 + physicalImageWidth ) >= displayWidth )
    {
        imageWidth = displayWidth - x1;
    }

    // y
    if ( y1 < 0 )
    {
        delta = -y1;
        imageHeight += y1;
        y1 = 0;
    }

    drawVars.eventCallback = eventCallback;

    // Get initial output frame buffer for DMA2D
    uint32 fb = (uint32)ltdc_frame_buffer;

    // Calculate output address start given rotation
    uint32 outputAddress = fb + x1 * FRAMEBUFFER_BPP + y1 * IMAGE_WIDTH * FRAMEBUFFER_BPP;

    // Buffer sanity check, assuming only RGB_565_BPP output and that frame buffer is allocated to correct size.
    if ( outputAddress < fb || outputAddress + imageWidth * imageHeight * FRAMEBUFFER_BPP > fb + displayWidth * displayHeight * FRAMEBUFFER_BPP )
    {
        return false;
    }

    // Set up input address and frame buffer
    uint32 inputAddress = (uint32)pControlBlock->dataRGB565.pData + (uint32)delta * FRAMEBUFFER_BPP * physicalImageWidth;

    // Send to DMA2D for transfer/handling
    DMA2D_Init_M2M( DMA2D_RGB565,
                    outputAddress,
                    inputAddress,
                    imageWidth, imageHeight,
                    // Skips are counted as a pixel, not as memory amount
                    IMAGE_WIDTH - imageWidth,
                    physicalImageWidth - imageWidth );

    return true;
}

/*
 -------------------------------------------------------------------------------
 Implementation of private functions
 -------------------------------------------------------------------------------
 */

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
static void DMA2D_Init_M2M( uint32_t pixelFormat,
                            uint32_t outputAddress,
                            uint32_t inputAddress,
                            uint32_t width, uint32_t height,
                            uint32_t outputSkip,
                            uint32_t inputSkip )
{
    /* Initialize DMA2D for memory to memory mode (copy pixels from source to destination memory). */
    /* DMA2D configuration */
    HAL_DMA2D_DeInit( &hdma2d );
    hdma2d.Init.Mode         = DMA2D_M2M;
    hdma2d.Init.ColorMode    = pixelFormat;
    hdma2d.Init.OutputOffset = outputSkip;
    if ( HAL_DMA2D_Init( &hdma2d ) != HAL_OK )
    {
        Error_Handler();
    }

    /* Layer config*/
    hdma2d.LayerCfg[ 1 ].InputOffset = inputSkip;
    if ( HAL_DMA2D_ConfigLayer( &hdma2d, 1 ) != HAL_OK )
    {
        Error_Handler();
    }

    /* Start DMA2D transfer*/
    HAL_DMA2D_Start_IT( &hdma2d, inputAddress, outputAddress, width, height );
}

/*
 ------------------------------------------------------------------------------
 Interrupt callbacks
 ------------------------------------------------------------------------------
 */
/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void TransferErrorCallback( DMA2D_HandleTypeDef* hdma2d )
{
    // DMA2D transfer error callback
    if ( NULL != drawVars.eventCallback )
    {
        tEvent sendEvent;
        sendEvent.id = IDRAW_EVENT_FAILED;
        drawVars.eventCallback( sendEvent );
    }
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void TransferCompleteCallback( DMA2D_HandleTypeDef* hdma2d )
{
    // DMA2D transfer completed callback
    if ( NULL != drawVars.eventCallback )
    {
        tEvent sendEvent;
        sendEvent.id = IDRAW_EVENT_DONE;
        drawVars.eventCallback( sendEvent );
    }
}
