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

#include "CanBroadcaster.h"
#include <string.h>
#include <stm32_hal.h>

extern CAN_HandleTypeDef hcan1;

/*
--------------------------------------------------------------------------------
    Private functions
--------------------------------------------------------------------------------
*/
static void InitHeader( CAN_TxHeaderTypeDef* pHeader, uint32_t canId, uint8_t dataSize );
/*
--------------------------------------------------------------------------------
    Implementation of public functions
--------------------------------------------------------------------------------
*/
void CanBroadcaster_Setup()
{

    MX_CAN1_Init();
    if ( HAL_CAN_Start( &hcan1 ) != HAL_OK )
    {
        /* Start Error */
        Error_Handler();
    }
}

bool CanBroadcaster_Transmit( uint32_t canId, const uint8_t* pData, uint8_t dataSize )
{
    CAN_TxHeaderTypeDef header;
    uint32_t            mailbox;
    InitHeader( &header, canId, dataSize );
    HAL_StatusTypeDef status = HAL_CAN_AddTxMessage( &hcan1, &header, (uint8_t*)pData, &mailbox );
    return ( HAL_OK == status );
}

/*
--------------------------------------------------------------------------------
    Implementation of private functions
--------------------------------------------------------------------------------
*/

static void InitHeader( CAN_TxHeaderTypeDef* pHeader, uint32_t canId, uint8_t dataSize )
{
    memset( pHeader, 0, sizeof( CAN_TxHeaderTypeDef ) );
    pHeader->ExtId = canId;
    pHeader->IDE   = CAN_ID_EXT;
    pHeader->RTR   = CAN_RTR_DATA;
    pHeader->DLC   = dataSize;
}
