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
 * @file      I2cCfgDefs.h
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Interface for I2c Cfg Defs.
 ******************************************************************************
 */
#ifndef __I2CCFGDEFS_H__
#define __I2CCFGDEFS_H__


#include "stm32_hal.h"
#include "II2c.h"

typedef void (*tI2cInitFunction)(void);

typedef struct
{
    tI2cInitFunction mxI2cInit;  /* I2C port init function */
    I2C_HandleTypeDef *handle;   /* Stm32 HAL I2C port handle */
} tI2c_Config;

#endif /* __I2CCFGDEFS_H__ */

