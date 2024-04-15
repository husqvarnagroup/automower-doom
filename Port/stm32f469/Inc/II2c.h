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
 * @file      II2c.h
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Interface of an I2c module
 ******************************************************************************
 */
#ifndef II2C_H
#define II2C_H

/*
-------------------------------------------------------------------------------
    Include files
-------------------------------------------------------------------------------
*/
#include "RoboticTypes.h"


/*
-------------------------------------------------------------------------------
    Type definitions
-------------------------------------------------------------------------------
*/
typedef uint8 tII2c_PortIndex; /* I2C port index type */

typedef enum
{
    II2C_EVENT_RX_DONE = ( 325 << 16 ),
    II2C_EVENT_TX_DONE,
    II2C_EVENT_ERROR,
} tII2c_Events;

typedef enum
{
    II2C_BUS_STATUS_IDLE,
    II2C_BUS_STATUS_NOT_INITIALIZED,
    II2C_BUS_STATUS_RX_ONGOING,
    II2C_BUS_STATUS_TX_ONGOING,
    II2C_BUS_STATUS_ERROR,
    II2C_BUS_STATUS_RETRANSMIT
} tII2c_BusStatusEnum;
typedef uint8 tII2c_BusStatus;

/*
-------------------------------------------------------------------------------
    Interface function prototypes
-------------------------------------------------------------------------------
*/
/**
 ******************************************************************************
 * @brief   Init module
 ******************************************************************************
 */
void II2c_Init( void );

/**
 ******************************************************************************
 * @brief   Start module
 ******************************************************************************
 */
void II2c_Start( void );

/**
 ******************************************************************************
 * @brief   Open port
 * @param   portIndex
 *          I2C Id for port to open
 * @param   eventCallback
 *          Callback function pointer.
 * @param   identifier
 *          An identifier value free of choice that will be included as
 *          in parameter to callback to know who is calling
 * @returns TRUE if port opened successfully
 *          FALSE if error
 ******************************************************************************
 */
bool II2c_Open( tII2c_PortIndex portIndex,
                tEventCallback eventCallback,
                uint32 identifier );

/**
 ******************************************************************************
 * @brief   Close port
 * @param   portIndex
 *          I2C Id for port to close
 ******************************************************************************
 */
void II2c_Close( tII2c_PortIndex portIndex );

/**
 ******************************************************************************
 * @brief   II2C_RxRequest
 *          Start an I2C read transfer from a slave.
 *          The I2C controller will take control over the supplied data buffer
 *          (pData) and write exactly bufLen bytes into it.
 *          When the RX transfer is complete, an II2C_EVENT_RX_DONE event will be
 *          generated (if the user provided an eventCallback to II2C_Open).
 *          II2C_GetStatus can be used to poll the status of the RX transfer.
 * @param   portIndex
 *          I2C port Id for the request.
 * @param   slaveAddr
 *          I2C slave address of device to read data from.
 * @param   bufLen
 *          number of bytes to read out.
 * @param   pData
 *          Pointer to data buffer where the read data will be copied into.
 *          The caller of this function should not access this pointer
 *          until the II2C_EVENT_RX_DONE has been fired or II2c_GetStatus
 *          has indicated that the bus is idle.
 *          If the platform has DMA support, pData must be DMA'able.
 * @returns true on success, false otherwise.
 ******************************************************************************
 */
bool II2C_RxRequest( tII2c_PortIndex portIndex,
                     uint16 slaveAddr,
                     uint32 bufLen,
                     uint8* pData );

/**
 ******************************************************************************
 * @brief   II2C_TxRequest
 *          Start an I2C write transfer to a slave.
 *          The I2C controller will take control over the supplied data buffer
 *          (pData) during transfer.
 *          When the TX transfer is complete, an II2C_EVENT_TX_DONE event will be
 *          generated (if the user provided an eventCallback to II2C_Open).
 *          II2C_GetStatus can be used to poll the status of the TX transfer.
 * @param   portIndex
 *          I2C port Id for the request.
 * @param   slaveAddr
 *          I2C slave address of device to write data to.
 * @param   bufLen
 *          number of bytes to write.
 * @param   pData
 *          Pointer to transmit data buffer.
 *          The caller of this function should not access this pointer
 *          until the II2C_EVENT_TX_DONE has been fired or II2c_GetStatus
 *          has indicated that the bus is idle.
 *          If the platform has DMA support, pData must be DMA'able.
 * @returns true on success, false otherwise.
 ******************************************************************************
 */
bool II2C_TxRequest( tII2c_PortIndex portIndex,
                     uint16 slaveAddr,
                     uint32 bufLen,
                     uint8* pData );

/**
 ******************************************************************************
 * @brief   II2C_ReadMem
 *          Read register / memory of I2C device.
 * @param   portIndex
 *          I2C port Id for the request.
 * @param   slaveAddr
 *          I2C slave address of device to write data to.
 * @param   memAddr
 *          Address of memory/register to read on I2C device.
 * @param   bufLen
 *          number of bytes to read.
 * @param   pData
 *          Pointer to data buffer where the read data will be copied into.
 *          The caller of this function should not access this pointer
 *          until the II2C_EVENT_RX_DONE has been fired or II2c_GetStatus
 *          has indicated that the bus is idle.
 *          If the platform has DMA support, pData must be DMA'able.
 * @returns true on success, false otherwise.
 ******************************************************************************
 */
bool II2C_ReadMem( tII2c_PortIndex portIndex,
                   uint16 slaveAddr,
                   uint16 memAddr,
                   uint32 bufLen,
                   uint8* pData );

/**
 ******************************************************************************
 * @brief   II2C_WriteMem
 *          Write to register / memory of I2C device.
 * @param   portIndex
 *          I2C port Id for the request.
 * @param   slaveAddr
 *          I2C slave address of device to write data to.
 * @param   memAddr
 *          Address of memory/register to write on I2C device.
 * @param   bufLen
 *          number of bytes to read.
 * @param   pData
 *          Pointer to transmit data buffer.
 *          The caller of this function should not access this pointer
 *          until the II2C_EVENT_TX_DONE has been fired or II2c_GetStatus
 *          has indicated that the bus is idle.
 *          If the platform has DMA support, pData must be DMA'able.
 * @returns true on success, false otherwise.
 ******************************************************************************
 */
bool II2C_WriteMem( tII2c_PortIndex portIndex,
                    uint16 slaveAddr,
                    uint16 memAddr,
                    uint32 bufLen,
                    uint8* pData );

/**
 ******************************************************************************
 * @brief   II2c_GetStatus
 *          Get the current status of on an I2C bus.
 *          This function can be used to check if the bus is available for new
 *          requests.
 * @param   portIndex
 *          I2C Id of the port we want to obtain the status for.
 * @returns See tII2c_BusStatus
 ******************************************************************************
 */
tII2c_BusStatus II2c_GetStatus( tII2c_PortIndex portIndex);

/**
 * @brief II2C_PutByte Send byte via I2C.
 *
 * @param portIndex I2C peripheral index.
 *
 * @param data I2C byte to send.
 *
 * @returns Always true.
 */
bool II2C_PutByte( tII2c_PortIndex portIndex, uint8 data );

#endif /* II2C_H */
