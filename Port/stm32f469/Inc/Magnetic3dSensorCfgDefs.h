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
 * @file      Magnetic3dSensorCfgDefs.h
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Interface for Uart Simulation Configuration Definitions.
 ******************************************************************************
 */
#ifndef __MAGNETIC_3D_SENSOR_TLE493D_CFGDEFS_H__
#define __MAGNETIC_3D_SENSOR_TLE493D_CFGDEFS_H__

#include "IMagnetic3dSensor.h"
#include "II2c.h"

typedef struct
{
    tII2c_PortIndex i2cPortIndex;
    uint8           i2cAddress;
    bool            hwResetEnabled;
    /* A value in [us] representing for how long to hold SDA line during soft reset. Has to be longer than 14us.*/
    uint16 sdaHoldDuringResetDuration;
    bool   readDuringStart;
    bool   i2cCloseAtReset;
} tMagnetic3dSensorTle493d_Config;

typedef struct
{
    uint32 timerTaskPeriod;
} tMagnetic3dSensorTle493d_Scheduler;

#endif /* __MAGNETIC_3D_SENSOR_TLE493D_CFGDEFS_H__ */
