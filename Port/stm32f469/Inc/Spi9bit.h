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
 * @file      Spi9bit.h
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Interface for Spi 9 bit communication.
 ******************************************************************************
 */
#ifndef SPI_H
#define SPI_H

/*
 ------------------------------------------------------------------------------
    Include files
 ------------------------------------------------------------------------------
*/
#include "ISpi.h"

/*
 ------------------------------------------------------------------------------
    Type definitions
 ------------------------------------------------------------------------------
*/

typedef enum
{
    CMD  = 0,
    DATA = 1
} Spi_CD;

/*
 ------------------------------------------------------------------------------
    Interface functions
 ------------------------------------------------------------------------------
*/

/**
*************************************************************************
* @brief   ISpi_SendDC
*          Sends DATA or COMMAND bit on SPI by bit banging (not MCU's peripheral).
* @param   device   Selects spi device
* @param   cd       Byte indicating command or data
*************************************************************************
*/
bool ISpi_SendDC( tISpiDevice device, Spi_CD cd );

/**
*************************************************************************
* @brief   ISpi_IdleCycle
*          Bitbangs one empty clock cycle to SPI.
*          Required for example when reading from ST7789VI.
* @param   device   Selects spi device
*************************************************************************
*/
void ISpi_IdleCycle( tISpiDevice device );

#endif /* ISPI_H */
