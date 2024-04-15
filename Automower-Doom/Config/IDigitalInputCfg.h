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
 * @file      IDigitalInputCfg.h
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Digital input module configuration header.
 ******************************************************************************
 */

#ifndef IDIGITALINPUTCFG_H
#define IDIGITALINPUTCFG_H

#include "DigitalInputCfgDefs.h"
#include <stm32_hal.h>

// Input ID's

typedef enum IDigitalInput_PinIndex
{
    IDIGITALINPUTCFG_MAINSWITCH,  /**< Main switch */
    IDIGITALINPUTCFG_STARTBUTTON, /**< Start switch */
} tIDigitalInput_PinIndexEnum;

// Inputs
static const tDigitalInput_Input IDigitalInputCfg_Input[] = {
    /*Index-------------------------------------------Port---Pin----------Inverted */
    [IDIGITALINPUTCFG_MAINSWITCH]  = { GPIOD, GPIO_PIN_13, false },
    [IDIGITALINPUTCFG_STARTBUTTON] = { GPIOG, GPIO_PIN_10, false },
};

#endif /* IDIGITALINPUTCFG_H */
