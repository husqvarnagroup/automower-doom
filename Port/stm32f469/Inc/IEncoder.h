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
 * @file      IEncoder.h
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Interface of an Encoder module
 ******************************************************************************
 */
#ifndef IENCODER_H
#define IENCODER_H

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

/*
-------------------------------------------------------------------------------
    Interface function prototypes
-------------------------------------------------------------------------------
*/
/**
 ******************************************************************************
 * @brief   Init module
 ******************************************************************************
 */
void IEncoder_Init( void );

/**
 ******************************************************************************
 * @brief   Start module
 ******************************************************************************
 */
void IEncoder_Start( void );

/**
 ******************************************************************************
 * @brief   GetCounter
 * @param   -
 * @returns uint16
 *          Current counter value in the encoder
 ******************************************************************************
 */
uint16 IEncoder_GetCounter( void );

#endif /* IENCODER_H */
