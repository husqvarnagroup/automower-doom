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
 * @file      IInterrupt.h
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Interrrupt implementation
 ******************************************************************************
 */
#ifndef IINTERRUPT_H
#define IINTERRUPT_H

/*
-------------------------------------------------------------------------------
	Include files
-------------------------------------------------------------------------------
*/
#include <RoboticTypes.h>

/*
-------------------------------------------------------------------------------
	Interface functions prototype
-------------------------------------------------------------------------------
*/

/**
*******************************************************************************
* @brief   Initialize interrupt module
* @param   -
* @returns -
*******************************************************************************
*/
void IInterrupt_Init( void );

/**
*******************************************************************************
* @brief   Start interrupt module
* @param   -
* @returns -
*******************************************************************************
*/
void IInterrupt_Start( void );

/**
*******************************************************************************
* @brief   Disable interrupt
* @param   -
* @returns -
*******************************************************************************
*/
void IInterrupt_GlobalDisable( void );

/**
*******************************************************************************
* @brief   Enable interrupt
* @param   -
* @returns -
*******************************************************************************
*/
void IInterrupt_GlobalEnable( void );

/**
*******************************************************************************
* @brief   Gets interrupt status
* @param   -
* @returns 1 (one) if enabled, otherwise 0.
*******************************************************************************
*/
uint8 IInterrupt_GetStatus( void );

/**
*******************************************************************************
* @brief   Gets if we are executing an interrupt
* @param   -
* @returns 1 (one) if in isr, otherwise 0.
*******************************************************************************
*/
bool IInterrupt_InIsr( void );

#endif /* IINTERRUPT_H */
