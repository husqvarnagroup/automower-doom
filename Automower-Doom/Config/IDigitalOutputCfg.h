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
 * Copyright (c) Husqvarna AB
 *
 * @brief   Configuration for DigitalOutput module
 *
 ******************************************************************************
 */
#ifndef IDIGITALOUTPUTCFG_H
#define IDIGITALOUTPUTCFG_H

/*
-------------------------
   Include files
-------------------------
*/

#include "DigitalOutputCfgDefs.h"
#include <stm32_hal.h>

/*
-------------------------
   Defines
-------------------------
*/

/*
-------------------------
   Type definitions
-------------------------
*/

typedef enum IDigitalOutput_PinIndex
{
    IDIGITALOUTPUTCFG_VBAT_EN,              /**< VBat enable */
    IDIGITALOUTPUTCFG_SAFE_PULSE,           /**< The safety reference pulse */
    IDIGITALOUTPUTCFG_DISP_EN,              /**< Pin to enable display */
    IDIGITALOUTPUTCFG_POWERSTATE_LATCH_R,   /**< Reset latch that disables safety blink on LEDs */
    IDIGITALOUTPUTCFG_LCD_RESET,            /**< Reset pin for LCD */
    IDIGITALOUTPUTCFG_AUX_CAN_POWER_ENABLE, /**< Power pin of aux CAN */
    IDIGITALOUTPUTCFG_LCD_SPI_CS,           /**< Chip select for LCD */
    IDIGITALOUTPUTCFG_VDD_HAL,              /**< Power pin of magnetic 3D sensor */
    IDIGITALOUTPUTCFG_CAN_STANDBY,          /**< CAN transceiver STB */
    IDIGITALOUTPUTCFG_RESET_BLE_WIFI,       /**< Pin to reset bluetooth and wifi */
} tIDigitalOutput_PinIndexEnum;

static const tDigitalOutput_Output IDigitalOutputCfg_Output[] =
    {
        /*Index-----------------------------------------Port----Pin-----------Inverted--Set-*/
        [IDIGITALOUTPUTCFG_VBAT_EN]              = { GPIOA, GPIO_PIN_10, false, true },
        [IDIGITALOUTPUTCFG_SAFE_PULSE]           = { GPIOB, GPIO_PIN_11, false, true },
        [IDIGITALOUTPUTCFG_DISP_EN]              = { GPIOB, GPIO_PIN_12, false, false },
        [IDIGITALOUTPUTCFG_POWERSTATE_LATCH_R]   = { GPIOD, GPIO_PIN_7, false, false },
        [IDIGITALOUTPUTCFG_LCD_RESET]            = { GPIOD, GPIO_PIN_11, false, true },
        [IDIGITALOUTPUTCFG_AUX_CAN_POWER_ENABLE] = { GPIOG, GPIO_PIN_6, false, true },
        [IDIGITALOUTPUTCFG_LCD_SPI_CS]           = { GPIOG, GPIO_PIN_9, true, false },
        [IDIGITALOUTPUTCFG_VDD_HAL]              = { GPIOH, GPIO_PIN_6, true, false },
        [IDIGITALOUTPUTCFG_CAN_STANDBY]          = { GPIOJ, GPIO_PIN_14, true, false },
        [IDIGITALOUTPUTCFG_RESET_BLE_WIFI]       = { GPIOJ, GPIO_PIN_15, false, false },
};

#endif /* IDIGITALOUTPUTCFG_H */
