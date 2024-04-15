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

#ifndef IWATCHDOG_H
#define IWATCHDOG_H

/*
 ------------------------------------------------------------------------------
    Interface functions
 ------------------------------------------------------------------------------
 */

/**
 ******************************************************************************
 * @brief   Initialize Watchdog module
 ******************************************************************************
 */
extern void IWatchdog_Init( void );

/**
 ******************************************************************************
 * @brief   Starts Watchdog module
 ******************************************************************************
 */
extern void IWatchdog_Start( void );

/**
 ******************************************************************************
 * @brief   Activates the Watchdog (can never be turned off)
 ******************************************************************************
 */
extern void IWatchdog_Activate( void );

/**
 ******************************************************************************
 * @brief   Refresh the Watchdog, i.e. signals that the application is alive
 ******************************************************************************
 */
extern void IWatchdog_Refresh( void );

#endif /* IWATCHDOG_H */
