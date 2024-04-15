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
 * @file      IDigitalOutput.h
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Interface for Digital Output.
 ******************************************************************************
 */
#ifndef IDIGITALOUTPUT_H
#define IDIGITALOUTPUT_H

/*
 ------------------------------------------------------------------------------
    Include files
 ------------------------------------------------------------------------------
*/
#include "RoboticTypes.h"


/*
 ------------------------------------------------------------------------------
    Type definitions
 ------------------------------------------------------------------------------
*/
typedef uint32 tIDigitalOutput_PinIndex;


/*
 ------------------------------------------------------------------------------
    Interface functions
 ------------------------------------------------------------------------------
*/

/**
 ******************************************************************************
 * @brief   IDigitalOutput_Init
 *          Initialize Digital output module
 * @param   -
 * @returns -
 ******************************************************************************
 */
void IDigitalOutput_Init( void );

/**
 ******************************************************************************
 * @brief   IDigitalOutput_Start
 *          Start Digital output module
 * @param   -
 * @returns -
 ******************************************************************************
 */
void IDigitalOutput_Start( void );

/**
 ******************************************************************************
 * @brief   IDigitalOutput_Get
 *          Return value of a digital output
 * @param   pinIndex
 *          Index if the pin asked for
 * @returns bool. Value of asked digital output
 ******************************************************************************
 */
bool IDigitalOutput_Get( const tIDigitalOutput_PinIndex pinIndex );


/**
 ******************************************************************************
 * @brief   IDigitalOutput_Set
 *          Set value of a digital output
 * @param   pinIndex
 *          Index if the pin asked for
 * @param   set
 *          Set pin to either TRUE or FALSE
 * @returns -
 ******************************************************************************
 */
void IDigitalOutput_Set( tIDigitalOutput_PinIndex pinIndex, bool set );

#endif
