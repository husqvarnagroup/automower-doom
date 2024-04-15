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
 * @file      ISystemTick.h
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Interface of SystemTick
 ******************************************************************************
 */
#ifndef ISYSTEMTICK_H
#define ISYSTEMTICK_H

#include "RoboticTypes.h"

typedef enum
{
    ISYSTEMTICK_EVENT_TICK = 209 << 16
} tSystemTickEvents;

typedef void* tISystemTick_TimerId;

/*
 ------------------------------------------------------------------------------
    Interface functions
 ------------------------------------------------------------------------------
*/

/**
 * @brief Init module
 */
void ISystemTick_Init( void );

/**
 * @brief Start module
 */
void ISystemTick_Start( void );

/**
 * @brief Get current tick
 */
uint32 ISystemTick_GetTick( void );

/**
 * @brief Register event listener callback (optional)
 */
void ISystemTick_RegisterEventCallback( tEventCallback eventCb );

/**
 * @brief Get system tick timer counter value
 */
uint32 ISystemTick_GetCnt( void );

/**
 * @brief Get system tick timer reload value
 */
uint32 ISystemTick_GetReload( void );

/**
 * @brief restart module
 */
void ISystemTick_Restart( void );

/**
 ******************************************************************************
 * @brief   Creates timer
 * @param   event
 *          Event that shall be sent when timer expires.
 * @param   pEventCb
 *          Handler to be called when timer expires.
 * @returns Timer id. 0 means that no timer was created.
 ******************************************************************************
 */
tISystemTick_TimerId ISystemTick_CreateTimer( tEvent event, tEventCallback pEventCb );

/**
 ******************************************************************************
 * @brief   Starts timer
 * @param   timerId
 *          Timer to start
 * @param   timeout
 *          Timeout (ms) until the timer shall expire.
 * @param   repeat
 *          True if timer shall be repeated, false if it is a one-time timer.
 ******************************************************************************
 */
void ISystemTick_StartTimer( tISystemTick_TimerId timerId, uint32 timeout, bool repeat );

/**
 ******************************************************************************
 * @brief   Stops timer
 * @param   timerId
 *          Timer to stop
 ******************************************************************************
 */
void ISystemTick_StopTimer( tISystemTick_TimerId timerId );

#endif /* ISYSTEMTICK_H */
