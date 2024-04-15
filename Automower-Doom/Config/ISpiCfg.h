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

#ifndef ISPICFG_H
#define ISPICFG_H

/*
 -------------------------
 Include files
 -------------------------
 */

#include "ISpiCfgDef.h"
#include "IDigitalOutputCfg.h"
#include <stm32_hal.h>

/*
 -------------------------
 Defines
 -------------------------
 */
/* Number of idle loops to hold each state for bitbanging SPI. Depends on required baudrate and bus speed. Range: 0-15 */
/* With 180MHz system clock 12 cycles is ~66,6ns. The value is sufficient for SCK HIGH or LOW according to LCD's data sheet. */
#define NUM_IDLE_LOOPS_TO_WAIT 12

/*
 -------------------------
 Type definitions
 -------------------------
 */

typedef enum
{
    ISPICFG_DEVICE_SPI_LCD = 0, /** LCD */

    /* ************************************************************************************ */
    ISPICFG_COUNT /**< Not used, always the last entry in the enumeration */
} tISpi_Device;

static const tSpi_DeviceCfg ISpiCfg_Devices[] =
    {
        { /*LCD*/
          .device            = ISPICFG_DEVICE_SPI_LCD,
          .chipSelect        = IDIGITALOUTPUTCFG_LCD_SPI_CS,
          .pBus              = SPI2,
          .pSckPort          = GPIOD,
          .sckPin            = GPIO_PIN_3,
          .sckAF             = GPIO_AF5_SPI2,
          .pMisoPort         = GPIOC,
          .misoPin           = GPIO_PIN_2,
          .misoAF            = GPIO_AF5_SPI2,
          .pMosiPort         = GPIOC,
          .mosiPin           = GPIO_PIN_3,
          .mosiAF            = GPIO_AF5_SPI2,
          .baudratePrescaler = SPI_BAUDRATEPRESCALER_16,
          .pHandle           = &hspi2 } };

#endif /* ISPICFG_H */
