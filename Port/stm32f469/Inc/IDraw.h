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
 * @file      IDraw.h
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Interface of Draw (HMI)
 ******************************************************************************
 */
#ifndef IDRAW_H
#define IDRAW_H

/*
 ------------------------------------------------------------------------------
    Include files
 ------------------------------------------------------------------------------
*/
#include <RoboticTypes.h>

/*
 ------------------------------------------------------------------------------
    Type definitions
 ------------------------------------------------------------------------------
*/
/* Pattern specific events */
typedef enum
{
    IDRAW_EVENT_DONE = ( 360 << 16 ), /* Operation done */
    IDRAW_EVENT_FAILED                /* Operation failed */
} tIDraw_Events;

/* Position */
typedef struct
{
    uint16 x;
    uint16 y;
} tIDraw_Position;


/* Image header */
typedef struct
{
    uint16              xSize;      /* x pixel size */
    uint16              ySize;      /* y pixel size */
    uint32              totalSize;  /* Total pixel data size in bytes */
} tIDraw_Header;

/* Pixel data type for RBG 565 format */
typedef struct
{
    uint16* pData;  /* Pointer to pixel data. Pixel data is HW dependent. */
    uint16  pitch;  /* Number of pixels between each row in the image */
} tIDraw_DataRGB565;


/* Image controlblock */
typedef struct
{
    tIDraw_Header header; /* Header */
    tIDraw_DataRGB565          dataRGB565;         /* RGB565 data format */
} tIDraw_ControlBlock;

/*
 ------------------------------------------------------------------------------
    Interface functions
 ------------------------------------------------------------------------------
*/

void IDraw_Init( void );
void IDraw_Start( void );
bool IDraw_ImageFromMemory( const void* const address, tIDraw_Position*  pPos);
bool IDraw_FillDisplay( uint32_t colorARGB888 );

bool IDraw_Draw( tIDraw_ControlBlock* pControlBlock, uint16 x, uint16 y, tEventCallback eventCallback );


#endif /* IDRAW_H */

