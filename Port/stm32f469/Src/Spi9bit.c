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
 * @file      Spi9bit.c
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Implementation of 9-bit SPI.
 ******************************************************************************
 */

/*
 ------------------------------------------------------------------------------
 Include files
 ------------------------------------------------------------------------------
 */
#include "Spi9bit.h"
#include "IDigitalOutput.h"
#include "ISpi.h"
#include "ISpiCfg.h"
#include <stm32_hal.h>

/*
 ------------------------------------------------------------------------------
 Local definitions
 ------------------------------------------------------------------------------
 */

#define SPI_TXRX_TIMEOUT 10

#define SPI_NUMBER_OF_DEVICES ( sizeof( ISpiCfg_Devices ) / sizeof( tSpi_DeviceCfg ) )

/* Insert a number (0 to 15) of NOP commands into code. */
static inline void __no_operation( void )
{
    asm( "nop" );
}

#define IDLE_CYCLES( x )                                                                                  \
    ( ( (x)&1 ) ? __no_operation() : (void)0 ),                                                           \
    ( ( (x)&2 ) ? __no_operation(), __no_operation() : (void)0 ),                                     \
    ( ( (x)&4 ) ? __no_operation(), __no_operation(), __no_operation(), __no_operation() : (void)0 ), \
    ( ( (x)&8 ) ? __no_operation(), __no_operation(), __no_operation(), __no_operation(), __no_operation(), __no_operation(), __no_operation(), __no_operation() : (void)0 )

/* States */
typedef enum
{
    SPI_STATE_IDLE,
    SPI_STATE_BUSY
} tSpiState;

/** SPI module local vars */
typedef struct
{
    tSpiState             state;
    const tSpi_DeviceCfg* pActiveDevice;
} tSpi_Vars;

/*
 ------------------------------------------------------------------------------
 Private data
 ------------------------------------------------------------------------------
 */
static tSpi_Vars spiVars;
static bool      spiInitialized = false;

/*
 ------------------------------------------------------------------------------
 Private function prototypes
 ------------------------------------------------------------------------------
 */
static void                  ConfigurePinsAsSPI( tISpiDevice device );
static void                  ConfigurePinsAsDigitalIO( tISpiDevice device );
static void                  ConfigureSPI( size_t deviceIndex );
static const tSpi_DeviceCfg* GetDeviceData( tISpiDevice device );
static size_t                GetDeviceIndex( tISpiDevice device );
static void                  EnableGPIOClock( GPIO_TypeDef* port );

/*
 ------------------------------------------------------------------------------
 Public functions
 ------------------------------------------------------------------------------
 */

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void ISpi_Init( void )
{
    if ( spiInitialized )
    {
        return;
    }
    IDigitalOutput_Init();
    MX_SPI2_Init_();
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void ISpi_Start( void )
{

    static bool started = FALSE;
    uint8       i;

    if ( !started )
    {
        IDigitalOutput_Start();

        spiVars.state = SPI_STATE_IDLE;

        for ( i = 0; i < SPI_NUMBER_OF_DEVICES; i++ )
        {
            ConfigureSPI( i );
        }

        started = TRUE;
    }
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
bool ISpi_StartTransfer( tISpiDevice device )
{
    const tSpi_DeviceCfg* pDevice;

    if ( spiVars.state != SPI_STATE_IDLE )
    {
        return FALSE;
    }

    pDevice = GetDeviceData( device );

    if ( pDevice == NULL )
    {
        return FALSE;
    }

    /* Enable Chip Select */
    IDigitalOutput_Set( pDevice->chipSelect, TRUE );

    spiVars.state = SPI_STATE_BUSY;

    spiVars.pActiveDevice = pDevice;

    return TRUE;
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
bool ISpi_StopTransfer( tISpiDevice device )
{
    size_t                deviceIndex = GetDeviceIndex( device );
    const tSpi_DeviceCfg* pDevice     = &( ISpiCfg_Devices[ deviceIndex ] );

    if ( pDevice == NULL || pDevice != spiVars.pActiveDevice )
    {
        return FALSE;
    }

    /* Disable Chip Select */
    IDigitalOutput_Set( pDevice->chipSelect, FALSE );

    spiVars.state         = SPI_STATE_IDLE;
    spiVars.pActiveDevice = NULL;

    return TRUE;
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
bool ISpi_TxRx( tISpiDevice device, uint8 txByte, uint8* pRxByte )
{
    size_t                deviceIndex = GetDeviceIndex( device );
    const tSpi_DeviceCfg* pDevice     = &( ISpiCfg_Devices[ deviceIndex ] );
    SPI_HandleTypeDef*    pHandle     = pDevice->pHandle;

    pDevice = GetDeviceData( device );

    if ( pDevice == NULL || device != spiVars.pActiveDevice->device || spiVars.state == SPI_STATE_IDLE )
    {
        return FALSE;
    }

    pDevice = spiVars.pActiveDevice;

    if ( __HAL_SPI_GET_FLAG( pHandle, SPI_FLAG_RXNE ) == SET )
    {
        /* Flush rx register */
        uint8 dummy = 0;
        HAL_SPI_Receive( pHandle, &dummy, 1, SPI_TXRX_TIMEOUT );
    }

    /* Loop while DR register is not empty */
    while ( __HAL_SPI_GET_FLAG( pHandle, SPI_FLAG_TXE ) == RESET )
        ;

    /* Send byte through SPIx peripheral and read response */
    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive( pHandle, &txByte, pRxByte, 1, SPI_TXRX_TIMEOUT );
    if ( HAL_OK != status )
    {
        return FALSE;
    }
    return TRUE;
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
bool ISpi_SendDC( tISpiDevice device, Spi_CD cd )
{
    size_t                deviceIndex = GetDeviceIndex( device );
    const tSpi_DeviceCfg* pDevice     = &( ISpiCfg_Devices[ deviceIndex ] );
    SPI_HandleTypeDef*    pHandle     = pDevice->pHandle;

    __HAL_SPI_DISABLE( pHandle );

    ConfigurePinsAsDigitalIO( device );
    /* prepare the data/command bit */
    HAL_GPIO_WritePin( pDevice->pSckPort, pDevice->sckPin, GPIO_PIN_RESET ); // set clock low
    if ( cd == CMD )
    {
        HAL_GPIO_WritePin( pDevice->pMosiPort, pDevice->mosiPin, GPIO_PIN_RESET ); // write 0
    }
    else
    {
        HAL_GPIO_WritePin( pDevice->pMosiPort, pDevice->mosiPin, GPIO_PIN_SET ); // write 1
    }
    IDLE_CYCLES( NUM_IDLE_LOOPS_TO_WAIT );
    HAL_GPIO_WritePin( pDevice->pSckPort, pDevice->sckPin, GPIO_PIN_SET );   // set clock high (rising edge)
    IDLE_CYCLES( NUM_IDLE_LOOPS_TO_WAIT );                                   // SCK H pulse
    HAL_GPIO_WritePin( pDevice->pSckPort, pDevice->sckPin, GPIO_PIN_RESET ); // set clock low again
    IDLE_CYCLES( NUM_IDLE_LOOPS_TO_WAIT );                                   // SCK L pulse

    ConfigurePinsAsSPI( device ); // SPI mode
    __HAL_SPI_ENABLE( pHandle );
    return TRUE;
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void ISpi_IdleCycle( tISpiDevice device )
{
    size_t                deviceIndex = GetDeviceIndex( device );
    const tSpi_DeviceCfg* pDevice     = &( ISpiCfg_Devices[ deviceIndex ] );
    SPI_HandleTypeDef*    pHandle     = pDevice->pHandle;

    __HAL_SPI_DISABLE( pHandle );
    ConfigurePinsAsDigitalIO( device );

    HAL_GPIO_WritePin( pDevice->pSckPort, pDevice->sckPin, GPIO_PIN_SET );   // set clock high (rising edge)
    IDLE_CYCLES( NUM_IDLE_LOOPS_TO_WAIT );                                   // SCK H pulse
    HAL_GPIO_WritePin( pDevice->pSckPort, pDevice->sckPin, GPIO_PIN_RESET ); // set clock low again
    IDLE_CYCLES( NUM_IDLE_LOOPS_TO_WAIT );                                   // SCK L pulse

    ConfigurePinsAsSPI( device ); // SPI mode
    __HAL_SPI_ENABLE( pHandle );
}

/*
 ------------------------------------------------------------------------------
    Private functions
 ------------------------------------------------------------------------------
 */

/*
 ******************************************************************************
 * Function
 * Configure SPI pins as regular digital output (SCK, MOSI) or input (MISO).
 ******************************************************************************
 */
static void ConfigurePinsAsDigitalIO( tISpiDevice device )
{
    GPIO_InitTypeDef      GPIO_InitStruct = { 0 };
    size_t                deviceIndex     = GetDeviceIndex( device );
    const tSpi_DeviceCfg* pDevice         = &( ISpiCfg_Devices[ deviceIndex ] );

    EnableGPIOClock( pDevice->pSckPort );
    EnableGPIOClock( pDevice->pMisoPort );
    EnableGPIOClock( pDevice->pMosiPort );

    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;

    /* Setup pins as digital IO for bit banging SPI */
    GPIO_InitStruct.Pin = pDevice->sckPin;
    HAL_GPIO_Init( pDevice->pSckPort, &GPIO_InitStruct );

    GPIO_InitStruct.Pin = pDevice->mosiPin;
    HAL_GPIO_Init( pDevice->pMosiPort, &GPIO_InitStruct );

    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pin  = pDevice->misoPin;
    HAL_GPIO_Init( pDevice->pMisoPort, &GPIO_InitStruct );
}

/*
 ******************************************************************************
 * Function
 * Configure SPI pins SPI peripheral.
 ******************************************************************************
 */
static void ConfigurePinsAsSPI( tISpiDevice device )
{
    GPIO_InitTypeDef      GPIO_InitStruct = { 0 };
    size_t                deviceIndex     = GetDeviceIndex( device );
    const tSpi_DeviceCfg* pDevice         = &( ISpiCfg_Devices[ deviceIndex ] );

    EnableGPIOClock( pDevice->pSckPort );
    EnableGPIOClock( pDevice->pMisoPort );
    EnableGPIOClock( pDevice->pMosiPort );

    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;

    /* Setup pins as SPI */
    GPIO_InitStruct.Alternate = pDevice->sckAF;
    GPIO_InitStruct.Pin       = pDevice->sckPin;
    HAL_GPIO_Init( pDevice->pSckPort, &GPIO_InitStruct );

    GPIO_InitStruct.Alternate = pDevice->misoAF;
    GPIO_InitStruct.Pin       = pDevice->misoPin;
    HAL_GPIO_Init( pDevice->pMisoPort, &GPIO_InitStruct );

    GPIO_InitStruct.Alternate = pDevice->mosiAF;
    GPIO_InitStruct.Pin       = pDevice->mosiPin;
    HAL_GPIO_Init( pDevice->pMosiPort, &GPIO_InitStruct );
}

/*
 ******************************************************************************
 * Function
 * Configure SPI peripheral for specific SPI device.
 ******************************************************************************
 */
static void ConfigureSPI( size_t deviceIndex )
{
    const tSpi_DeviceCfg* pDevice = &( ISpiCfg_Devices[ deviceIndex ] );
    SPI_HandleTypeDef*    pHandle = pDevice->pHandle;

    ConfigurePinsAsSPI( pDevice->device );
    switch ( (uint32_t)pDevice->pBus )
    {
        case (uint32_t)SPI1:
            __HAL_RCC_SPI1_CLK_ENABLE();
            break;
        case (uint32_t)SPI2:
            __HAL_RCC_SPI2_CLK_ENABLE();
            break;
        case (uint32_t)SPI3:
            __HAL_RCC_SPI3_CLK_ENABLE();
            break;
        case (uint32_t)SPI4:
            __HAL_RCC_SPI4_CLK_ENABLE();
            break;
        case (uint32_t)SPI5:
            __HAL_RCC_SPI5_CLK_ENABLE();
            break;
        case (uint32_t)SPI6:
            __HAL_RCC_SPI6_CLK_ENABLE();
            break;
    }

    /*--- SPI configuration ---*/
    /* Common settings */
    pHandle->Instance               = pDevice->pBus;
    pHandle->Init.Mode              = SPI_MODE_MASTER;
    pHandle->Init.DataSize          = SPI_DATASIZE_8BIT;
    pHandle->Init.CLKPolarity       = SPI_POLARITY_LOW;
    pHandle->Init.CLKPhase          = SPI_PHASE_1EDGE;
    pHandle->Init.NSS               = SPI_NSS_SOFT;
    pHandle->Init.FirstBit          = SPI_FIRSTBIT_MSB;
    pHandle->Init.CRCPolynomial     = 7;
    pHandle->Init.BaudRatePrescaler = pDevice->baudratePrescaler;

    if ( HAL_SPI_DeInit( pHandle ) != HAL_OK )
    {
        while ( 1 )
            ;
    }

    if ( HAL_SPI_Init( pHandle ) != HAL_OK )
    {
        while ( 1 )
            ;
    }

    __HAL_SPI_ENABLE( pHandle );
}

/*
 ******************************************************************************
 * Function
 * Get whole SPI device info structure depending on device ID.
 ******************************************************************************
 */
static size_t GetDeviceIndex( tISpiDevice device )
{
    uint8 i;

    for ( i = 0; i < SPI_NUMBER_OF_DEVICES; i++ )
    {
        if ( ISpiCfg_Devices[ i ].device == device )
        {
            break;
        }
    }

    return i;
}

/*
 ******************************************************************************
 * Function
 * Get whole SPI device info structure depending on device ID.
 ******************************************************************************
 */
static const tSpi_DeviceCfg* GetDeviceData( tISpiDevice device )
{
    uint8 i;

    for ( i = 0; i < SPI_NUMBER_OF_DEVICES; i++ )
    {
        if ( ISpiCfg_Devices[ i ].device == device )
        {
            break;
        }
    }

    if ( i == SPI_NUMBER_OF_DEVICES )
    {
        return NULL;
    }

    return &( ISpiCfg_Devices[ i ] );
}

/*
 ******************************************************************************
 * Function
 * Get AHB1 GPIO peripheral value from GPIO port
 ******************************************************************************
 */
static void EnableGPIOClock( GPIO_TypeDef* port )
{
    switch ( (uint32_t)port )
    {
        case (uint32_t)GPIOA:
            __HAL_RCC_GPIOB_CLK_ENABLE();
            break;
        case (uint32_t)GPIOB:
            __HAL_RCC_GPIOB_CLK_ENABLE();
            break;
        case (uint32_t)GPIOC:
            __HAL_RCC_GPIOC_CLK_ENABLE();
            break;
        case (uint32_t)GPIOD:
            __HAL_RCC_GPIOD_CLK_ENABLE();
            break;
        case (uint32_t)GPIOE:
            __HAL_RCC_GPIOE_CLK_ENABLE();
            break;
        case (uint32_t)GPIOF:
            __HAL_RCC_GPIOF_CLK_ENABLE();
            break;
        case (uint32_t)GPIOG:
            __HAL_RCC_GPIOG_CLK_ENABLE();
            break;
        case (uint32_t)GPIOH:
            __HAL_RCC_GPIOH_CLK_ENABLE();
            break;
        case (uint32_t)GPIOI:
            __HAL_RCC_GPIOI_CLK_ENABLE();
            break;
        case (uint32_t)GPIOJ:
            __HAL_RCC_GPIOJ_CLK_ENABLE();
            break;
        case (uint32_t)GPIOK:
            __HAL_RCC_GPIOK_CLK_ENABLE();
            break;
    }
}
