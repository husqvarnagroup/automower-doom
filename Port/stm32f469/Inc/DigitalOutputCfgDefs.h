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
 * @file      DigitalOutputCfgDefs.h
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Digital output configuration definitions header.
 ******************************************************************************
 */
#ifndef DIGITALOUTPUTCFGDEFS_H
#define DIGITALOUTPUTCFGDEFS_H

/*
-------------------------------------------------------------------------------
    Include
-------------------------------------------------------------------------------
*/
#include "RoboticTypes.h"
#include "stm32_hal.h"

/*
-------------------------------------------------------------------------------
    Types
-------------------------------------------------------------------------------
*/
typedef struct
{
    GPIO_TypeDef*  port;
    uint16         pins;
    bool           inverted;
    bool           set;
} tDigitalOutput_Output;


#endif
