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
 * @file      IJoystick.h
 *
 * @copyright Husqvarna AB
 *
 * @date      2020-02-03
 *
 * @brief     Joystick interface
 ******************************************************************************
 */
#ifndef IJOYSTICK_H
#define IJOYSTICK_H

/*
 ------------------------------------------------------------------------------
    Include files
 ------------------------------------------------------------------------------
 */

#include "RoboticTypes.h"

/*
 ------------------------------------------------------------------------------
    Defines
 ------------------------------------------------------------------------------
 */

/*
 ------------------------------------------------------------------------------
    Type definitions
 ------------------------------------------------------------------------------
 */


/*
 ------------------------------------------------------------------------------
    Interface functions
 ------------------------------------------------------------------------------
 */

/**
 ******************************************************************************
 * @brief   Initialize Joystick module
 * @param   -
 * @returns -
 ******************************************************************************
 */
void IJoystick_Init( void );

/**
 ******************************************************************************
 * @brief   Starts Joystick module
 * @param   -
 * @returns -
 ******************************************************************************
 */
void IJoystick_Start( void );

/**
 ******************************************************************************
 * @brief   Checks if module is ready.
 * @returns True if module is ready, false otherwise.
 ******************************************************************************
 */
bool IJoystick_IsReady( void );

/**
 ******************************************************************************
 * @brief   Get if fire is pressed.
 * @returns dummy
 ******************************************************************************
 */
bool IJoystick_GetFire( void );

/**
 ******************************************************************************
 * @brief   Get if up is pressed.
 * @returns dummy
 ******************************************************************************
 */
bool IJoystick_GetUp( void );

/**
 ******************************************************************************
 * @brief   Get if down is pressed.
 * @returns dummy
 ******************************************************************************
 */
bool IJoystick_GetDown( void );

/**
 ******************************************************************************
 * @brief   Get if left is pressed.
 * @returns dummy
 ******************************************************************************
 */
bool IJoystick_GetLeft( void );

/**
 ******************************************************************************
 * @brief   Get if right is pressed.
 * @returns dummy
 ******************************************************************************
 */
bool IJoystick_GetRight( void );

/**
 ******************************************************************************
 * @brief   Connect to joystick.
 * @returns dummy
 ******************************************************************************
 */
void IJoystick_Connect( void );

#endif /* IJOYSTICK_H */
