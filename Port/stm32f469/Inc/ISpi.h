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
 * @file      ISpi.h
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Interface for Spi communication.
 ******************************************************************************
 */
#ifndef ISPI_H
#define ISPI_H

/*
 ------------------------------------------------------------------------------
    Include files
 ------------------------------------------------------------------------------
*/
#include "RoboticTypes.h"

/*
 ------------------------------------------------------------------------------
    Type definitions
 ------------------------------------------------------------------------------
*/

typedef uint32 tISpiDevice;

/*
 ------------------------------------------------------------------------------
    Interface functions
 ------------------------------------------------------------------------------
*/

/**
 ******************************************************************************
 * @brief   ISpi_Init
 *          Initialize SPI module
 * @param   -
 * @returns -
 ******************************************************************************
 */
void ISpi_Init( void );

/**
*************************************************************************
* @brief   ISpi_IsIdle
*          Check if bus for requested device is idle
* @param   device   Selects spi device
*************************************************************************
*/
bool ISpi_IsIdle( tISpiDevice device );

/**
 *****************************************************************************
 * @brief   ISpi_Start
 *          Start SPI module
 * @param   -
 * @returns -
 ******************************************************************************
 */
void ISpi_Start( void );

/**
*************************************************************************
* @brief   ISPI_StartTransfer
*          Secures exclusive access to selected SPI bus and sets CS
*          according to corrsponding value in the array ISpiCfg_Devices in
*          ISpiCfg.h
* @param   device   Selects spi device
*************************************************************************
*/
bool ISpi_StartTransfer( tISpiDevice device );

/**
*************************************************************************
* @brief   ISPI_StopTransfer
*          Releases exclusive access to selected SPI bus and resets CS
* @param   device   Selects spi device
*************************************************************************
*/
bool ISpi_StopTransfer( tISpiDevice device );

/**
*************************************************************************
* @brief   ISPI_TxRx
*          Sends and receives one byte
* @param   device   Selects spi device
* @param   txByte   Byte to send
* @param   pRxByte  Received byte
*************************************************************************
*/
bool ISpi_TxRx( tISpiDevice device, uint8 txByte, uint8* pRxByte );

/**
 *************************************************************************
 * @brief   Spi_Write
 *          Write multiple bytes to specified register.
 *
 * @param   device   Selects spi device
 * @param   reg      Register to send bytes to
 * @param   data     Bytes to send
 * @param   len      Number of bytes to send
 *
 * @return  true on success
 *************************************************************************
 */
bool ISpi_Write( tISpiDevice device, uint8 reg, uint8* data, uint32 len );

/**
 *************************************************************************
 * @brief   Spi_Read
 *          Read multiple bytes from specified register.
 *
 * @param   device   Selects spi device
 * @param   reg      Register to read bytes from
 * @param   data     Buffer to read bytes in
 * @param   len      Number of bytes to read
 *
 * @return  true on success
 *************************************************************************
 */
bool ISpi_Read( tISpiDevice device, uint8 reg, uint8* data, uint32 len );

#endif /* ISPI_H */
