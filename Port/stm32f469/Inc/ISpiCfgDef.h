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
 * @file      ISpiCfg.h
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Interface for SPI Cfg.
 ******************************************************************************
 */
#ifndef ISPICFGDEF_H
#define ISPICFGDEF_H

#include <stdint.h>
#include "RoboticTypes.h"
#include "spi.h"
#include "ISpi.h"
#include "IDigitalOutputCfg.h"
#include <stm32_hal.h>

typedef struct tSpi_Device_t
{
    tISpiDevice              device;     /* device identifier */
    tIDigitalOutput_PinIndex chipSelect; /* chip select in digital output */
    SPI_TypeDef*             pBus;       /* bus on chip */
    GPIO_TypeDef*            pSckPort;   /* pin configuration for SCK */
    uint16_t                 sckPin;
    uint8_t                  sckAF;
    GPIO_TypeDef*            pMisoPort; /* pin configuration for MISO */
    uint16_t                 misoPin;
    uint8_t                  misoAF;
    GPIO_TypeDef*            pMosiPort; /* pin configuration for MOSI */
    uint16_t                 mosiPin;
    uint8_t                  mosiAF;
    uint16_t                 baudratePrescaler; /* Depends on BUS speed for specific SPI peripheral and required target baudrate. */
    SPI_HandleTypeDef*       pHandle;           /* Pointer to SPI handle */
} tSpi_DeviceCfg;

#endif /*ISPICFGDEF_H*/
