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
 * @file      Magnetic3dSensor.h
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Firmware Upgrade module implementation header
 *            This module implements the Firmware Upgrade interface
 ******************************************************************************
 */
#ifndef MAGNETIC_3D_SENSOR_H
#define MAGNETIC_3D_SENSOR_H

/*
-------------------------------------------------------------------------------
    Include files
-------------------------------------------------------------------------------
*/
#include "IMagnetic3dSensor.h"

/*
-------------------------------------------------------------------------------
    Local types
-------------------------------------------------------------------------------
*/
typedef enum MagneticSensorState
{
    MS_STATE_I2C_OPEN,
    MS_STATE_SW_RESET,
    MS_STATE_CONFIGURE,
    MS_STATE_ACTIVE,
    MS_STATE_ERROR,
    MS_STATE_HW_RESET,
    MS_STATE_FATAL_ERROR,
    MS_STATE_OFF,
    MS_STATE_BACKOFF,
    MS_STATE_NUMBER_OF_STATES,
} tMagneticSensorStateEnum;
typedef uint8 tMagneticSensorState;

/*
-------------------------------------------------------------------------------
    Public functions
-------------------------------------------------------------------------------
*/

void                 Magnetic3dSensor_Reset( tIMagnetic3dSensor_PortIndex portIndex );
void                 Magnetic3dSensor_EnableLogs( void );
uint32               Magnetic3dSensor_GetErrorCount( void );
tMagneticSensorState Magnetic3dSensor_GetState( tIMagnetic3dSensor_PortIndex portIndex );
void                 Magnetic3dSensor_EnableProfiling( void );
sint32               Magnetic3dSensor_GetProfilingTime( void );
void                 Magnetic3dSensor_SetFluxEventEnabled( tIMagnetic3dSensor_PortIndex portIndex, bool isEventsEnabled );
bool                 Magnetic3dSensor_GetFluxEventEnabled( tIMagnetic3dSensor_PortIndex portIndex );
bool                 Magnetic3dSensor_WakeUpInit( tIMagnetic3dSensor_PortIndex portIndex, sint16 xl, sint16 xh, sint16 yl, sint16 yh, sint16 zl, sint16 zh );
void                 Magnetic3dSensor_GetWakeupThresholds( tIMagnetic3dSensor_PortIndex portIndex, sint16* xl, sint16* xh, sint16* yl, sint16* yh, sint16* zl, sint16* zh );
#endif /* MAGNETIC_3D_SENSOR_H */
