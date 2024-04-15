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
 * @file      IMagnetic3dSensor.h
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Magnetic 3D sensor implementation
 ******************************************************************************
 */
#ifndef IMAGNETIC_3D_SENSOR_H
#define IMAGNETIC_3D_SENSOR_H

/*
-------------------------------------------------------------------------------
    Include files
-------------------------------------------------------------------------------
*/
#include "RoboticTypes.h"

/*
-------------------------------------------------------------------------------
    Type definitions
-------------------------------------------------------------------------------
*/


typedef uint32 tIMagnetic3dSensor_PortIndex;
typedef sint16 tIMagnetic3dSensor_Flux; /**< Range: (-500)-500, Postfix: 0.1mT */
typedef struct
{
    /** Error */
    bool error;
    /** CommunicationFailure*/
    bool comFailure;
    /** CommunicationErrorCode */
    uint32 comErrorCode;
} tIMagnetic3dSensor_ErrorStatus;

/**
 * Magnetic flux densities.
 *
 * All values are in 0.1 mT (milli Tesla)
 */
typedef struct
{
    /** Flux density in the x direction */
    tIMagnetic3dSensor_Flux x;
    /** Flux density in the y direction */
    tIMagnetic3dSensor_Flux y;
    /** Flux density in the z direction */
    tIMagnetic3dSensor_Flux z;
} tIMagnetic3dSensor_FluxDensity;

typedef enum
{
    IMAGNETIC3DSENSOR_EVENT_FLUX_UPDATED = ( 327 << 16 ),
    IMAGNETIC3DSENSOR_EVENT_ERROR_STATUS_UPDATE
} tIMagnetic3dSensor_Events;

/*
-------------------------------------------------------------------------------
    Interface function prototypes
-------------------------------------------------------------------------------
*/

/**
*******************************************************************************
* @brief        Initialize module
* @param        -
* @returns      -
*******************************************************************************
*/
void IMagnetic3dSensor_Init( void );

/**
*******************************************************************************
* @brief        Start module
* @param        -
* @returns      -
*******************************************************************************
*/
void IMagnetic3dSensor_Start( void );

/**
*******************************************************************************
* @brief        Checks if module is ready
* @param[in]    portIndex
*               Sensor index specifying which sensor to check status for.
* @returns      true if ready, false otherwise
*******************************************************************************
*/
bool IMagnetic3dSensor_IsReady( tIMagnetic3dSensor_PortIndex portIndex );

/**
*******************************************************************************
* @brief        Open module
* @param[in]    portIndex
*               Sensor index specifying which sensor we want to open.
* @returns      true if successful, false otherwise
*******************************************************************************
*/
bool IMagnetic3dSensor_Open( tIMagnetic3dSensor_PortIndex portIndex );

/**
*******************************************************************************
* @brief        Get the current (most recent) magnetic flux densities
* @param[in]    portIndex
*               Sensor index specifying which sensor we want to obtain the
*               data for.
* @param[out]   pFlux
*               A tIMagnetic3dSensor_FluxDensity struct containing the current
*               Flux densities.
* @returns      true if successful, false otherwise.
*******************************************************************************
*/
bool IMagnetic3dSensor_GetCurrentFlux( tIMagnetic3dSensor_PortIndex    portIndex,
                                       tIMagnetic3dSensor_FluxDensity* pFlux );

/**
*******************************************************************************
* @brief        Calculate an approximate Bz value (magnetic flux denisity in mT
*               in the Z direction) from Bxy.
*               Bxy is the magnetic flux density in the XY plane (also mT).
* @param[in]    Bxy
*               The magnetic flux density in the XY plane (in mT).
* @param[in]    z
*               The height in mm for which we want to calculate our estimated
*               magnetic flux density.
* @returns      The approximate magnetic flux density in the Z direction.
*******************************************************************************
*/
sint16 IMagnetic3dSensor_CalcApproxBz( sint16 Bxy, uint8 z );

/**
*******************************************************************************
* @brief        Calculate an approximate Bxy value (magnetic flux denisity in mT
*               in the XY plane) from Bz.
*               Bz is the magnetic flux density in the Z direction (also mT).
* @param[in]    Bz
*               The magnetic flux density in the Z direction (in mT).
* @param[in]    z
*               The height in mm for which we want to calculate our estimated
*               magnetic flux density.
* @returns      The approximate magnetic flux density in the XY plane.
*******************************************************************************
*/
sint16 IMagnetic3dSensor_CalcApproxBxy( sint16 Bz, uint8 z );

/**
*******************************************************************************
* @brief        Calculate an approximate Z distance in mm.
* @param[in]    pFlux
*               The magnetic flux density.
* @param[out]   pZ
*               The approximated distance in Z axis [mm]
* @returns      True if calculation is ok.
*******************************************************************************
*/
bool IMagnetic3dSensor_CalcApproxZDistance( tIMagnetic3dSensor_FluxDensity* pFlux, sint16* pZ );

/**
*******************************************************************************
* @brief        Calculate an approximate X and Y distances in mm.
* @param[in]    pFlux
*               The magnetic flux density.
* @param[out]   pX
*               The approximated distance in x axis [mm]
* @param[out]   pY
*               The approximated distance in y axis [mm]
* @returns      True if calculation is ok.
*******************************************************************************
*/
bool IMagnetic3dSensor_CalcApproxXYDistance( tIMagnetic3dSensor_FluxDensity* pFlux, sint16* pX, sint16* pY );

/**
*******************************************************************************
* @brief        Register a callback function for updated magnetic flux values.
* @param[in]    portIndex
*               Sensor index specifying which sensor we want to set the
*               data for.
* @param[in]    pEventCallback
*               Function pointer to callback used when event is generated.
* @returns      True if subscription was succesful.
*******************************************************************************
*/
bool IMagnetic3dSensor_RegisterEventCallback( tIMagnetic3dSensor_PortIndex portIndex, tEventCallback pEventCallback );

/**
*******************************************************************************
* @brief        Get current error status.
* @param[in]    portIndex
*               Sensor index specifying which sensor we want to set the
*               data for.
* @param[in]    pErrorStatus
*               Pointer to error status struct.
* @returns      True if status read successfully, otherwise false
*******************************************************************************
*/
bool IMagnetic3dSensor_GetErrorStatus( tIMagnetic3dSensor_PortIndex portIndex, tIMagnetic3dSensor_ErrorStatus* pErrorStatus );

#endif /* IMAGNETIC_3D_SENSOR_H */
