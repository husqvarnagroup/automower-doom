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

/*
--------------------------------------------------------------------------------
    Include files
--------------------------------------------------------------------------------
*/
#include "ISystemPower.h"
#include "ISystemTick.h"
#include "CanBroadcaster.h"
/*
--------------------------------------------------------------------------------
    Private functions
--------------------------------------------------------------------------------
*/
static void EventHandler( tEvent event );
/*
--------------------------------------------------------------------------------
    Implementation of interface functions
--------------------------------------------------------------------------------
*/
void ISystemPower_Init( void )
{
    ISystemTick_Init();
}

void ISystemPower_Start( void )
{
    ISystemTick_Start();
    CanBroadcaster_Setup();
    tEvent               ev  = { .id = 0, .data = 0 };
    tISystemTick_TimerId tmr = ISystemTick_CreateTimer( ev, EventHandler );
    ISystemTick_StartTimer( tmr, 1000, true );
}
/*
--------------------------------------------------------------------------------
    Implementation of private functions
--------------------------------------------------------------------------------
*/
static void EventHandler( tEvent event )
{
    CanBroadcaster_Transmit( 0x4048421, NULL, 0 );
}
