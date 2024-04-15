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
 * @file      Joystick.c
 *
 * @copyright Husqvarna AB
 *
 * @date      2023-10-01
 *
 * @brief     Implementation file for Joystick
 ******************************************************************************
 */
/*
 ------------------------------------------------------------------------------
    Include files
 ------------------------------------------------------------------------------
 */

#include "doomtype.h"
#include "Joystick.h"
#include "DigitalInput.h"
#include "IDigitalInput.h"
#include "IDigitalInputCfg.h"
#include "IDigitalOutputCfg.h"
#include "ISystemTick.h"
#include "IMagnetic3dSensor.h"
#include "IMagnetic3dSensorCfg.h"

#include "IEncoderCfg.h"
#include "IEncoder.h"

#include <stm32_hal.h>
#include <stdlib.h>

/*
 ------------------------------------------------------------------------------
    Defines
 ------------------------------------------------------------------------------
 */

#define JOYSTICK_JOGWHEEL_PERIOD ( menuactive ? 1 : 150 )
/*
 ------------------------------------------------------------------------------
    Type definitions
 ------------------------------------------------------------------------------
 */

typedef enum
{
    JOYSTICK_INTEVENT_TIMER,
    JOYSTICK_INTEVENT_LEFTTIMER,
    JOYSTICK_INTEVENT_RIGHTTIMER,
    JOYSTICK_OPEN_PORTS,

    /* ********************************************************************* */
    JOYSTICK_INTEVENT_COUNT
} tJoystick_InternalEvents;

typedef struct
{
    bool                           started;
    bool                           up;
    bool                           down;
    bool                           left;
    bool                           right;
    tISystemTick_TimerId           timer;
    tISystemTick_TimerId           leftTimer;
    tISystemTick_TimerId           rightTimer;
    tIMagnetic3dSensor_FluxDensity flux;
    bool                           jogWheelDown;
    int                            encoderCounter;

} tJoystick_Vars;

/*
 ------------------------------------------------------------------------------
    Private data
 ------------------------------------------------------------------------------
 */

static tJoystick_Vars joystickVars;

/*
 ------------------------------------------------------------------------------
    External data
 ------------------------------------------------------------------------------
 */
extern boolean menuactive; // Menu overlayed?

/*
 ------------------------------------------------------------------------------
    Private function prototypes
 ------------------------------------------------------------------------------
 */
static void Joystick_EventCallback( tEvent e );

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
void IJoystick_Init( void )
{
    static bool init = false;
    if ( !init )
    {
        init                 = true;
        joystickVars.started = false;
        ISystemTick_Init();
    }
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void IJoystick_Start( void )
{
    if ( !joystickVars.started )
    {
        joystickVars.started = true;

        ISystemTick_Start();

        joystickVars.up    = false;
        joystickVars.down  = false;
        joystickVars.left  = false;
        joystickVars.right = false;
        tEvent e;
        e.id = JOYSTICK_INTEVENT_RIGHTTIMER;

        joystickVars.rightTimer = ISystemTick_CreateTimer( e, Joystick_EventCallback );

        e.id = JOYSTICK_INTEVENT_LEFTTIMER;

        joystickVars.leftTimer = ISystemTick_CreateTimer( e, Joystick_EventCallback );
    }
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
bool IJoystick_IsReady( void )
{
    if ( joystickVars.started )
    {
        return true;
    }
    return false;
}

/**
 ******************************************************************************
 * @brief   Get if up is pressed.
 * @returns dummy
 ******************************************************************************
 */
bool IJoystick_GetUp( void )
{
    return DigitalInput_GetStartButtonDown();
}

/**
 ******************************************************************************
 * @brief   Get if up is pressed.
 * @returns dummy
 ******************************************************************************
 */
bool IJoystick_GetFire( void )
{
    return DigitalInput_GetJogWheelDown();
}
/**
 ******************************************************************************
 * @brief   Get if down is pressed.
 * @returns dummy
 ******************************************************************************
 */
bool IJoystick_GetDown( void )
{
    joystickVars.down     = false;
    static bool firstTime = true;

    if ( firstTime )
    {
        // take a reference value at startup, this will be our "0-level" for the button
        // we know that this means that we can be fooled if the user starts with the button pressed
        // but we are NOT sensitive if the magnet has switched polarity, which is a bigger issue
        IMagnetic3dSensor_GetCurrentFlux( IMAGNETIC3DSENSORCFG_TLE493D, &joystickVars.flux );
        firstTime = false;
    }

    tIMagnetic3dSensor_FluxDensity flux;
    IMagnetic3dSensor_GetCurrentFlux( IMAGNETIC3DSENSORCFG_TLE493D, &flux );

    // all flux values are fluttering in the normal case
    // the z flux seams to have largest "pressed" - "not pressed" variation so we use this value
    // looking at the absolute diff between the reference value and the current makes us insensitive towards switched
    // magnet polarity
    // we use the value 10, which looks like a good threshold delta, testing on the bench with some boards/buttons
    if ( abs( joystickVars.flux.z - flux.z ) >= 10 )
    {
        joystickVars.down = true;
    }
    return joystickVars.down;
}

/**
 ******************************************************************************
 * @brief   Get if left is pressed.
 * @returns dummy
 ******************************************************************************
 */
bool IJoystick_GetLeft( void )
{
    uint16 newEncoder = IEncoder_GetCounter();
    // Wraparound-handling of encoder counter
    bool turnedLeft = ( (sint16)( (sint16)newEncoder - (sint16)joystickVars.encoderCounter ) < 0 ) ? true : false;

    // left is set to false via timer callback
    if ( turnedLeft )
    {
        joystickVars.left           = true;
        joystickVars.encoderCounter = newEncoder;
        ISystemTick_StartTimer( joystickVars.leftTimer, JOYSTICK_JOGWHEEL_PERIOD, false );
    }

    return joystickVars.left;
}

/**
 ******************************************************************************
 * @brief   Get if right is pressed.
 * @returns dummy
 ******************************************************************************
 */
bool IJoystick_GetRight( void )
{
    uint16 newEncoder = IEncoder_GetCounter();
    // Wraparound-handling of encoder counter
    bool turnedRight = ( (sint16)( (sint16)newEncoder - (sint16)joystickVars.encoderCounter ) > 0 ) ? true : false;

    // right is set to false via timer callback
    if ( turnedRight )
    {
        joystickVars.right          = true;
        joystickVars.encoderCounter = newEncoder;
        ISystemTick_StartTimer( joystickVars.rightTimer, JOYSTICK_JOGWHEEL_PERIOD, false );
    }

    return joystickVars.right;
}

/**
 ******************************************************************************
 * @brief   Take over port to Joystick
 * @returns dummy
 ******************************************************************************
 */
void IJoystick_Connect( void )
{
    // nop
}

void IJoystick_Execute( void )
{
    // nop
}

/*
 ------------------------------------------------------------------------------
    Private functions
 ------------------------------------------------------------------------------
 */

static void Joystick_EventCallback( tEvent e )
{
    switch ( e.id )
    {

        case JOYSTICK_INTEVENT_LEFTTIMER:
        {
            joystickVars.left = false;
            break;
        }

        case JOYSTICK_INTEVENT_RIGHTTIMER:
        {
            joystickVars.right = false;
            break;
        }
    }
}
