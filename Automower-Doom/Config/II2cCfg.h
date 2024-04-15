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
 * @brief   Configuration for I2c module
 *
 ******************************************************************************
 */
#ifndef II2CCFG_H
#define II2CCFG_H

/*
-------------------------
   Include files
-------------------------
*/
#include "II2c.h"
#include "I2cCfgDefs.h"
#include <stm32_hal.h>

/*
 -------------------------
 Constants definitions
 -------------------------
 */

#define I2C_MAX_NUMBER_OF_SUBSCRIBERS ( 1 )
#define I2C_NUMBER_OF_PORTS           ( sizeof( II2cCfg_Port ) / sizeof( tI2c_Config ) )

typedef enum
{
    /** Magnetic3DSensor*/
    II2CCFG_TLE493D = 0,
} II2cCfg_PortIndex;

static const tI2c_Config II2cCfg_Port[] =
    {
        { MX_I2C2_Init, &hi2c2 }, /* port index = 0 -> I2C2 */
};

#endif /* II2CCFG_H */
