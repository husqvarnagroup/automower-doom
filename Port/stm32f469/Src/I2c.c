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
 * @file      I2c.c
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Implementation of II2c for STM32G4xx
 ******************************************************************************
 */

/*
 -------------------------------------------------------------------------------
 Include files
 -------------------------------------------------------------------------------
 */
#include "II2cCfg.h"
/*
 -------------------------------------------------------------------------------
 Types
 -------------------------------------------------------------------------------
 */

typedef enum
{
    I2C_TRANSFER_IDLE,
    I2C_TRANSFER_BUSY,
    I2C_TRANSFER_COMPLETE,
    I2C_TRANSFER_ERROR,
} tI2c_TransferStatus;

typedef struct
{
    void*  cb;
    uint32 id;
} tI2c_cbData;

typedef struct
{
    tI2c_cbData cbData[ I2C_MAX_NUMBER_OF_SUBSCRIBERS ];
    bool        isOpen;
} tI2c_PortData;

typedef struct
{
    tI2c_PortData portData[ I2C_NUMBER_OF_PORTS ];
    bool          isStarted;
} tI2c_Vars;

/*
 -------------------------------------------------------------------------------
 Prototypes
 -------------------------------------------------------------------------------
 */

static uint32             GetIndex( const I2C_HandleTypeDef* hi2c );
static I2C_HandleTypeDef* GetHandle( tII2c_PortIndex index );
static tI2c_PortData*     GetPortData( tII2c_PortIndex portIndex );
static void               NotifyEventSubscribers( tI2c_cbData* subscribers, tII2c_Events eventId );
static bool               AddEventSubscriber( tII2c_PortIndex portIndex,
                                              tEventCallback  eventCallback,
                                              uint32          identifier );

/*
 -------------------------------------------------------------------------------
 Defines
 -------------------------------------------------------------------------------
 */

/*
 -------------------------------------------------------------------------------
 Global variables
 -------------------------------------------------------------------------------
 */
static tI2c_Vars                    i2c;
static bool                         i2cInitialized = false;
static volatile tI2c_TransferStatus transferStatus[ I2C_NUMBER_OF_PORTS ];
/*
 -------------------------------------------------------------------------------
 Public functions
 -------------------------------------------------------------------------------
 */

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void II2c_Init( void )
{
    if ( i2cInitialized )
    {
        return;
    }

    i2c.isStarted = false;
    for ( uint32 i = 0; i < I2C_NUMBER_OF_PORTS; i++ )
    {
        transferStatus[ i ]      = I2C_TRANSFER_IDLE;
        i2c.portData[ i ].isOpen = false;
        for ( uint32 j = 0; j < I2C_MAX_NUMBER_OF_SUBSCRIBERS; j++ )
        {
            i2c.portData[ i ].cbData[ j ].id = 0;
            i2c.portData[ i ].cbData[ j ].cb = NULL;
        }
    }

    i2cInitialized = true;
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void II2c_Start( void )
{
    if ( i2c.isStarted )
    {
        return;
    }
    i2c.isStarted = true;
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
bool II2c_Open( tII2c_PortIndex portIndex,
                tEventCallback  eventCallback,
                uint32          identifier )
{
    if ( I2C_NUMBER_OF_PORTS <= portIndex )
    {
        return false;
    }

    tI2c_PortData* pPort = GetPortData( portIndex );

    if ( pPort->isOpen )
        return true;

    II2cCfg_Port[ portIndex ].mxI2cInit();

    if ( !AddEventSubscriber( portIndex, eventCallback, identifier ) )
    {
        return false;
    }

    pPort->isOpen = true;

    return true;
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void II2c_Close( tII2c_PortIndex portIndex )
{
    if ( I2C_NUMBER_OF_PORTS <= portIndex )
    {
        return;
    }

    tI2c_PortData* pPort = GetPortData( portIndex );
    pPort->isOpen        = false;

    I2C_HandleTypeDef* handle = GetHandle( portIndex );
    HAL_I2C_DeInit( handle );
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
bool II2C_RxRequest( tII2c_PortIndex portIndex,
                     uint16          slaveAddr,
                     uint32          bufLen,
                     uint8*          pData )
{
    if ( I2C_NUMBER_OF_PORTS <= portIndex || transferStatus[ portIndex ] == I2C_TRANSFER_BUSY )
    {
        return false;
    }

    transferStatus[ portIndex ] = I2C_TRANSFER_BUSY;

    if ( HAL_OK == HAL_I2C_Master_Receive_IT( GetHandle( portIndex ), slaveAddr, pData, bufLen ) )
    {
        while ( I2C_TRANSFER_BUSY == transferStatus[ portIndex ] )
        {
            // blocking
        }
        if ( I2C_TRANSFER_COMPLETE == transferStatus[ portIndex ] )
        {
            NotifyEventSubscribers( i2c.portData[ portIndex ].cbData, II2C_EVENT_RX_DONE );
            transferStatus[ portIndex ] = I2C_TRANSFER_IDLE;
            return true;
        }
        else
        {
            NotifyEventSubscribers( i2c.portData[ portIndex ].cbData, II2C_EVENT_ERROR );
            return false;
        }
    }
    else
    {
        NotifyEventSubscribers( i2c.portData[ portIndex ].cbData, II2C_EVENT_ERROR );
        transferStatus[ portIndex ] = I2C_TRANSFER_ERROR;
        return false;
    }
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
bool II2C_TxRequest( tII2c_PortIndex portIndex,
                     uint16          slaveAddr,
                     uint32          bufLen,
                     uint8*          pData )
{

    if ( I2C_NUMBER_OF_PORTS <= portIndex || transferStatus[ portIndex ] == I2C_TRANSFER_BUSY )
    {
        return false;
    }

    transferStatus[ portIndex ] = I2C_TRANSFER_BUSY;

    if ( HAL_OK == HAL_I2C_Master_Transmit_IT( GetHandle( portIndex ), slaveAddr, pData, bufLen ) )
    {
        while ( I2C_TRANSFER_BUSY == transferStatus[ portIndex ] )
        {
            // blocking
        }
        if ( I2C_TRANSFER_COMPLETE == transferStatus[ portIndex ] )
        {
            NotifyEventSubscribers( i2c.portData[ portIndex ].cbData, II2C_EVENT_TX_DONE );
            transferStatus[ portIndex ] = I2C_TRANSFER_IDLE;
            return true;
        }
        else
        {
            NotifyEventSubscribers( i2c.portData[ portIndex ].cbData, II2C_EVENT_ERROR );
            return false;
        }
    }
    else
    {
        NotifyEventSubscribers( i2c.portData[ portIndex ].cbData, II2C_EVENT_ERROR );
        transferStatus[ portIndex ] = I2C_TRANSFER_ERROR;
        return false;
    }
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
bool II2C_WriteMem( tII2c_PortIndex portIndex,
                    uint16          slaveAddr,
                    uint16          memAddr,
                    uint32          bufLen,
                    uint8*          pData )
{
    if ( I2C_NUMBER_OF_PORTS <= portIndex || transferStatus[ portIndex ] == I2C_TRANSFER_BUSY )
    {
        return false;
    }

    transferStatus[ portIndex ] = I2C_TRANSFER_BUSY;
    if ( HAL_OK == HAL_I2C_Mem_Write_IT( GetHandle( portIndex ), slaveAddr, memAddr, bufLen, pData, bufLen ) )
    {
        // Handle transfer completion etc. in non-blocking way with callbacks
        return true;
    }
    else
    {
        NotifyEventSubscribers( i2c.portData[ portIndex ].cbData, II2C_EVENT_ERROR );
        transferStatus[ portIndex ] = I2C_TRANSFER_ERROR;
        return false;
    }
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
bool II2C_ReadMem( tII2c_PortIndex portIndex,
                   uint16          slaveAddr,
                   uint16          memAddr,
                   uint32          bufLen,
                   uint8*          pData )
{
    if ( I2C_NUMBER_OF_PORTS <= portIndex || transferStatus[ portIndex ] == I2C_TRANSFER_BUSY )
    {
        return false;
    }

    transferStatus[ portIndex ] = I2C_TRANSFER_BUSY;
    if ( HAL_OK == HAL_I2C_Mem_Read_IT( GetHandle( portIndex ), slaveAddr, memAddr, bufLen, pData, bufLen ) )
    {
        // Handle transfer completion etc. in non-blocking way with callbacks
        return true;
    }
    else
    {
        NotifyEventSubscribers( i2c.portData[ portIndex ].cbData, II2C_EVENT_ERROR );
        transferStatus[ portIndex ] = I2C_TRANSFER_ERROR;
        return false;
    }
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
tII2c_BusStatus II2c_GetStatus( tII2c_PortIndex portIndex )
{
    HAL_I2C_StateTypeDef halStatus;

    halStatus = HAL_I2C_GetState( GetHandle( portIndex ) );
    switch ( halStatus )
    {
        case HAL_I2C_STATE_RESET:
            return II2C_BUS_STATUS_NOT_INITIALIZED;
        case HAL_I2C_STATE_READY:
            return II2C_BUS_STATUS_IDLE;
        case HAL_I2C_STATE_BUSY_TX:
            return II2C_BUS_STATUS_TX_ONGOING;
        case HAL_I2C_STATE_BUSY_RX:
            return II2C_BUS_STATUS_RX_ONGOING;
        default:
            return II2C_BUS_STATUS_ERROR;
    }
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
// this is needed to reset magnetic sensor (TLE493D)
bool II2C_PutByte( tII2c_PortIndex portIndex, uint8 data )
{
    I2C_HandleTypeDef* handle = GetHandle( portIndex );

    if ( data & 0x01 ) // LSB bit is sent as R/W bit
        HAL_I2C_Master_Receive( handle, data, NULL, 0, 20 );
    else
        HAL_I2C_Master_Transmit( handle, data, NULL, 0, 20 );

    return true;
}

/*
 -------------------------------------------------------------------------------
 Private functions
 -------------------------------------------------------------------------------
 */

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
static bool AddEventSubscriber( tII2c_PortIndex portIndex,
                                tEventCallback  eventCallback,
                                uint32          identifier )
{
    bool result = false;
    // Find free slot to save callback
    tEventCallback cb    = NULL;
    tI2c_PortData* pPort = GetPortData( portIndex );
    for ( int i = 0; i < I2C_MAX_NUMBER_OF_SUBSCRIBERS; i++ )
    {
        cb = (tEventCallback)pPort->cbData[ i ].cb;
        if ( cb == NULL || cb == eventCallback )
        {
            pPort->cbData[ i ].cb = (void*)eventCallback;
            pPort->cbData[ i ].id = identifier;
            result                = true;
            break;
        }
    }
    return result;
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
static void NotifyEventSubscribers( tI2c_cbData* subscribers,
                                    tII2c_Events eventId )
{
    uint32         i;
    tEventCallback eventCallback;
    tEvent         event;

    for ( i = 0; i < I2C_MAX_NUMBER_OF_SUBSCRIBERS; i++ )
    {
        eventCallback = (tEventCallback)subscribers[ i ].cb;
        if ( NULL != eventCallback )
        {
            event.id   = eventId;
            event.data = subscribers[ i ].id;
            eventCallback( event );
        }
    }
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
static I2C_HandleTypeDef* GetHandle( tII2c_PortIndex index )
{
    return II2cCfg_Port[ index ].handle;
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
static uint32 GetIndex( const I2C_HandleTypeDef* hi2c )
{
    for ( uint8 portIndex = 0; portIndex < I2C_NUMBER_OF_PORTS; portIndex++ )
    {
        if ( GetHandle( portIndex ) == hi2c )
            return portIndex;
    }
    Error_Handler();
    return 0;
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
static tI2c_PortData* GetPortData( tII2c_PortIndex portIndex )
{
    return ( &( i2c.portData[ portIndex ] ) );
}

/*
 -------------------------------------------------------------------------------
 Completion "callbacks" (these functions override weak defintions in the
 STM32 HAL library)
 -------------------------------------------------------------------------------
 */

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void HAL_I2C_MasterTxCpltCallback( I2C_HandleTypeDef* hi2c )
{
    transferStatus[ GetIndex( hi2c ) ] = I2C_TRANSFER_COMPLETE;
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void HAL_I2C_MasterRxCpltCallback( I2C_HandleTypeDef* hi2c )
{
    transferStatus[ GetIndex( hi2c ) ] = I2C_TRANSFER_COMPLETE;
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void HAL_I2C_MemTxCpltCallback( I2C_HandleTypeDef* hi2c )
{
    transferStatus[ GetIndex( hi2c ) ] = I2C_TRANSFER_COMPLETE;
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void HAL_I2C_MemRxCpltCallback( I2C_HandleTypeDef* hi2c )
{
    transferStatus[ GetIndex( hi2c ) ] = I2C_TRANSFER_COMPLETE;
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void HAL_I2C_ErrorCallback( I2C_HandleTypeDef* hi2c )
{
    transferStatus[ GetIndex( hi2c ) ] = I2C_TRANSFER_ERROR;
}
