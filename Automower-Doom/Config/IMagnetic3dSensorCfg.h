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

#ifndef IMAGNETIC_3D_SENSOR_CFG_H
#define IMAGNETIC_3D_SENSOR_CFG_H

/*
-------------------------
   Include files
-------------------------
*/

#include "Magnetic3dSensorCfgDefs.h"
#include "II2cCfg.h"

#define MAGNETIC_3D_SENSOR_NUMBER_OF_PORTS \
    ( sizeof( IMagnetic3dSensorTle493dCfg_Port ) / sizeof( IMagnetic3dSensorTle493dCfg_Port[ 0 ] ) )
/*
-------------------------
   Type definitions
-------------------------
*/

typedef enum IMagnetic3dSensorCfg_PortIndex
{
    IMAGNETIC3DSENSORCFG_TLE493D
} tIMagnetic3dSensor_PortIndexEnum;

/* Mapping of magnetic sensor ports to I2C port */
static const tMagnetic3dSensorTle493d_Config IMagnetic3dSensorTle493dCfg_Port[] =
    {
        {
            .i2cPortIndex               = II2CCFG_TLE493D,
            .i2cAddress                 = 0x6A,
            .hwResetEnabled             = false,
            .sdaHoldDuringResetDuration = 30, /* at least 30us*/
            .readDuringStart            = true,
            .i2cCloseAtReset            = true,
        } };

/* Scheduling period of execute operation */
static const tMagnetic3dSensorTle493d_Scheduler IMagnetic3dSensorTle493dCfg_Scheduler =
    {
        .timerTaskPeriod = 31,
};

#endif /* IMAGNETIC_3D_SENSOR_CFG_H */
