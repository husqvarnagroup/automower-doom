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
 * @file      Restart.c
 * @brief     Restarts Hw
 * *****************************************************************************
 */
/*
 ------------------------------------------------------------------------------
    Include files
 ------------------------------------------------------------------------------
 */

#include "IDigitalInput.h"
#include "DigitalInput.h"
#include "IDigitalInputCfg.h"
#include "IRestart.h"
#include "ISystemTick.h"
#include "stm32_hal.h"

#define RESTART_EXECUTE_MS   1000
#define RESTART_LONGPRESS_MS 10000

typedef enum
{
    RESTART_INTEVENT_EXECUTE,
    RESTART_INTEVENT_LONGPRESS,

    /* ********************************************************************* */
    RESTART_INTEVENT_COUNT
} tRestart_InternalEvents;

typedef struct
{
    bool                 started;
    bool                 lastRead;
    tISystemTick_TimerId executeTimer;
    tISystemTick_TimerId longPressTimer;
} tRestart_Vars;

static tRestart_Vars restartVars = { 0 };

static void Restart( void );
static void EventHandler( tEvent event );

/*
 ------------------------------------------------------------------------------
    Interface functions
 ------------------------------------------------------------------------------
 */
/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void IRestart_Init( void )
{
    ISystemTick_Init();
    IDigitalInput_Init();
}
/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void IRestart_Start( void )
{
    if ( restartVars.started )
    {
        return;
    }

    tEvent event;

    ISystemTick_Start();
    IDigitalInput_Start();
    event.data                 = 0;
    event.id                   = RESTART_INTEVENT_EXECUTE;
    restartVars.executeTimer   = ISystemTick_CreateTimer( event, EventHandler );
    event.id                   = RESTART_INTEVENT_LONGPRESS;
    restartVars.longPressTimer = ISystemTick_CreateTimer( event, EventHandler );

    ISystemTick_StartTimer( restartVars.executeTimer, RESTART_EXECUTE_MS, true );
    restartVars.started = true;
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void IRestart_Restart( void )
{
    Restart();
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
static void EventHandler( tEvent event )
{
    switch ( event.id )
    {
        case RESTART_INTEVENT_EXECUTE:
        {
            static bool first      = true;
            bool        mainSwitch = DigitalInput_GetJogWheelDown();

            if ( first )
            {
                first = false;
            }
            else if ( mainSwitch != restartVars.lastRead )
            {
                if ( mainSwitch )
                {
                    ISystemTick_StartTimer( restartVars.longPressTimer, RESTART_LONGPRESS_MS, false );
                }
                else
                {
                    ISystemTick_StopTimer( restartVars.longPressTimer );
                }
            }
            restartVars.lastRead = mainSwitch;
            break;
        }
        case RESTART_INTEVENT_LONGPRESS:
        {
            Restart();
            break;
        }
    }
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
static void Restart( void )
{
    uint16* pTmp = (uint16*)0x20000000;
    *pTmp        = 0x0000;
    NVIC_SystemReset();
}
