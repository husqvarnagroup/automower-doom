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
 * @file      DigitalOutput.c
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Digital output implementation.
 ******************************************************************************
 */
/*
-------------------------------------------------------------------------------
    Include
-------------------------------------------------------------------------------
*/
#include "IDigitalOutput.h"
#include "IDigitalOutputCfg.h"
#include "stm32_hal.h"

/*
-------------------------------------------------------------------------------
    Local definitions
-------------------------------------------------------------------------------
*/
#define DIGITALOUTPUT_NUMBER_OF_OUTPUTS ( sizeof( IDigitalOutputCfg_Output ) / sizeof( tDigitalOutput_Output ) )

/*
-------------------------------------------------------------------------------
    Local types
-------------------------------------------------------------------------------
*/
typedef struct
{
    bool initialized;
} tDigitalOutput_Vars;

/*
-------------------------------------------------------------------------------
    Private data
-------------------------------------------------------------------------------
*/
static tDigitalOutput_Vars digitalOutputVars = { 0 };

/*
-------------------------------------------------------------------------------
    Private function prototypes
-------------------------------------------------------------------------------
*/

/*
-------------------------------------------------------------------------------
    Implementation of interface functions
-------------------------------------------------------------------------------
*/
void IDigitalOutput_Init( void )
{
    if ( digitalOutputVars.initialized )
    {
        return;
    }
    digitalOutputVars.initialized = true;

    MX_GPIO_Init_();

    for ( uint8 i = 0; i < DIGITALOUTPUT_NUMBER_OF_OUTPUTS; i++ )
    {
        tDigitalOutput_Output output = IDigitalOutputCfg_Output[ i ];
        if ( output.inverted != output.set )
        {
            HAL_GPIO_WritePin( output.port, output.pins, GPIO_PIN_SET );
        }
        else
        {
            HAL_GPIO_WritePin( output.port, output.pins, GPIO_PIN_RESET );
        }
    }
}

void IDigitalOutput_Start( void )
{
}

bool IDigitalOutput_Get( const tIDigitalOutput_PinIndex pinIndex )
{
    if ( DIGITALOUTPUT_NUMBER_OF_OUTPUTS <= pinIndex )
    {
        return false;
    }

    tDigitalOutput_Output output = IDigitalOutputCfg_Output[ pinIndex ];
    bool                  set    = GPIO_PIN_SET == HAL_GPIO_ReadPin( output.port, output.pins );
    if ( output.inverted )
    {
        set = !set;
    }
    return set;
}

void IDigitalOutput_Set( const tIDigitalOutput_PinIndex pinIndex, bool set )
{
    if ( DIGITALOUTPUT_NUMBER_OF_OUTPUTS <= pinIndex )
    {
        return;
    }

    tDigitalOutput_Output output = IDigitalOutputCfg_Output[ pinIndex ];
    bool                  val    = set;
    if ( output.inverted )
    {
        val = !val;
    }
    if ( val )
    {
        HAL_GPIO_WritePin( output.port, output.pins, GPIO_PIN_SET );
    }
    else
    {
        HAL_GPIO_WritePin( output.port, output.pins, GPIO_PIN_RESET );
    }
}
