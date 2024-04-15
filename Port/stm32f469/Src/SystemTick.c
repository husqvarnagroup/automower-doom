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
 * @file      SystemTick.c
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Implementation of systemTickVars.
 ******************************************************************************
 */
#include "SystemTick.h"
#include "IDigitalInput.h"
#include "DigitalInput.h"
#include <stm32_hal.h>

/*
 ---------------------------------
 Local definitions
 ---------------------------------
 */
#define MAX_TIMEOUT_DIFF ( 5 )
#ifndef MAX_NR_OF_TIMERS
#define MAX_NR_OF_TIMERS 40
#endif

static inline bool CheckIfTimeStampHasExpiredAgainstReference( uint32 timeStamp, uint32 referenceTime )
{
    return ( (sint32)( (sint32)referenceTime - (sint32)timeStamp ) > 0 ) ? true : false;
}

typedef struct
{
    tEvent         event;         /**< event sent to the callback function */
    tEventCallback pEventHandler; /**< pointer to the registered callback function triggered when timer expires */
    uint32         interval;      /**< the user set interval for this timer */
    uint32         timeout;       /**< next timeout value */
    bool           used;          /**< true if timer is in use, false if free to use */
    bool           running;       /**< true if timer is currently running */
    bool           repeat;        /**< true if timer is automatically repeating */
} tSystemTick_Timer;

typedef struct
{
    tSystemTick_Timer timerList[ MAX_NR_OF_TIMERS ];
    uint32            systemTick;
    bool              started;
} tSystemTickVars;

static void RunTimers( uint32 tick );

/*
 ---------------------------------
 Private data
 ---------------------------------
 */
static tSystemTickVars systemTickVars;

/*
 ---------------------------------
 Private function prototypes
 ---------------------------------
 */

static bool systemTickInitDone = false;

void ISystemTick_Init( void )
{
    if ( !systemTickInitDone )
    {
        systemTickVars.systemTick = 0;

        systemTickVars.started = FALSE;

        for ( uint32 i = 0; i < MAX_NR_OF_TIMERS; i++ )
        {
            systemTickVars.timerList[ i ].used          = false;
            systemTickVars.timerList[ i ].event.id      = 0;
            systemTickVars.timerList[ i ].event.data    = 0;
            systemTickVars.timerList[ i ].pEventHandler = NULL;
            systemTickVars.timerList[ i ].running       = 0;
            systemTickVars.timerList[ i ].repeat        = 0;
            systemTickVars.timerList[ i ].timeout       = 0;
            systemTickVars.timerList[ i ].interval      = 0;
        }

        systemTickInitDone = true;
    }
}

void ISystemTick_Start( void )
{
    if ( !systemTickVars.started )
    {
        systemTickVars.systemTick = 0;
        SystemCoreClockUpdate();

        /* Start SysTick counter with configured number of ticks between two interrupts */
        if ( SysTick_Config( SystemCoreClock / 1000 ) )
        {
            Error_Handler();
        }

        systemTickVars.started = TRUE;
    }
}

void ISystemTick_RegisterEventCallback( tEventCallback eventCb )
{
}

uint32 ISystemTick_GetTick( void )
{
    return systemTickVars.systemTick;
}

uint32 ISystemTick_GetCnt( void )
{
    return SysTick->VAL;
}

uint32 ISystemTick_GetReload( void )
{
    return SysTick->LOAD;
}

void ISystemTick_BusyWait( uint32 ticks )
{
    uint32 start = systemTickVars.systemTick;

    while ( systemTickVars.systemTick < ( start + ticks ) && ( start + ticks > start ) )
        ;
}

/******************************************************************************
 Function
 ******************************************************************************/
tISystemTick_TimerId ISystemTick_CreateTimer( tEvent event, tEventCallback pEventCb )
{

    tSystemTick_Timer* pTimer = NULL;

    for ( uint32 i = 0; i < MAX_NR_OF_TIMERS; i++ )
    {
        if ( !systemTickVars.timerList[ i ].used )
        {
            pTimer = &systemTickVars.timerList[ i ];
            break;
        }
    }

    if ( pTimer == NULL )
    {
        return 0;
    }
    else
    {
        pTimer->used          = true;
        pTimer->pEventHandler = pEventCb;
        pTimer->event         = event;
    }

    /* Use the self pointer as id */
    return (tISystemTick_TimerId)pTimer;
}

/******************************************************************************
 Function
 ******************************************************************************/

void ISystemTick_StartTimer( tISystemTick_TimerId timerId, uint32 timeout, bool repeat )
{
    tSystemTick_Timer* pSwtimer = (tSystemTick_Timer*)timerId;

    if ( pSwtimer == NULL )
    {
        return;
    }

    pSwtimer->running  = true;
    pSwtimer->repeat   = repeat;
    pSwtimer->timeout  = timeout + systemTickVars.systemTick;
    pSwtimer->interval = timeout;

    return;
}

/******************************************************************************
 Function
 ******************************************************************************/

void ISystemTick_StopTimer( tISystemTick_TimerId timerId )
{
    tSystemTick_Timer* pSwtimer = (tSystemTick_Timer*)timerId;

    if ( pSwtimer == NULL )
    {
        return;
    }

    pSwtimer->running  = false;
    pSwtimer->repeat   = false;
    pSwtimer->timeout  = 0;
    pSwtimer->interval = 0;

    return;
}

/******************************************************************************
 Function
 ******************************************************************************/
void RunTimers( uint32 tick )
{
    if ( systemTickVars.started )
    {
        tSystemTick_Timer* pSwtimer;

        for ( uint32 i = 0; i < MAX_NR_OF_TIMERS; i++ )
        {
            pSwtimer = &( systemTickVars.timerList[ i ] );

            if ( pSwtimer->running )
            {
                if ( CheckIfTimeStampHasExpiredAgainstReference( pSwtimer->timeout - 1, tick ) )
                {
                    if ( pSwtimer->repeat )
                    {
                        if ( ( tick - pSwtimer->timeout < MAX_TIMEOUT_DIFF ) && ( ( pSwtimer->timeout + pSwtimer->interval ) > tick ) )
                        {
                            pSwtimer->timeout = pSwtimer->timeout + pSwtimer->interval;
                        }
                        else
                        {
                            pSwtimer->timeout = tick + pSwtimer->interval;
                        }
                    }
                    else
                    {
                        pSwtimer->running = false;
                    }

                    if ( pSwtimer->pEventHandler != NULL )
                    {
                        pSwtimer->pEventHandler( pSwtimer->event );
                    }
                }
            }
        }
    }
}

/* *********************************
 * Private functions
 * ***********************************/

void HAL_IncTick( void )
{
    /* increment tick counter */
    systemTickVars.systemTick++;
    /* And Run stuff... */
    DigitalInput_Execute();
    RunTimers( systemTickVars.systemTick );
}

/* *********************************
 * HAL wrapper functions
 * ***********************************/
uint32_t HAL_GetTick( void )
{
    return (uint32_t)ISystemTick_GetTick();
}
