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
 * @file      Encoder.c
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Implementation of Encoder.
 ******************************************************************************
 */

/*
 -------------------------------------------------------------------------------
 Include files
 -------------------------------------------------------------------------------
 */
#include "Encoder.h"
#include "IEncoderCfg.h"
#include "RoboticUtils.h"

#include "stm32_hal.h"

/*
 -------------------------------------------------------------------------------
 Local definitions
 -------------------------------------------------------------------------------
 */
/*
 -------------------------------------------------------------------------------
 Local types
 -------------------------------------------------------------------------------
 */
typedef struct
{
    bool initialized;
    bool started;
} tEncoder_Vars;

typedef enum
{
    ENCODER_INTEVENT_TIMER_TICK,
    ENCODER_INTEVENT_COUNT
} tEncoder_InternalEvents;

/*
 -------------------------------------------------------------------------------
 Private data
 -------------------------------------------------------------------------------
 */
static tEncoder_Vars encoderVars;

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
void IEncoder_Init( void )
{
    if ( !encoderVars.initialized )
    {
        encoderVars.initialized = true;
        encoderVars.started     = false;
    }

    if ( NULL == IEncoderCfg.TimInit || NULL == IEncoderCfg.TimHandle )
    {
        Error_Handler();
    }

    /* Run HAL initialization for TIM, pins, etc  */
    IEncoderCfg.TimInit();
}

void IEncoder_Start( void )
{
    if ( !encoderVars.started )
    {
        /* Turn on the timer/counters right away */
        HAL_TIM_Encoder_Start( IEncoderCfg.TimHandle, IEncoderCfg.TimChannels );
        encoderVars.started = true;
    }
}

uint16 IEncoder_GetCounter( void )
{
    if ( !encoderVars.started )
    {
        Error_Handler();
    }
    return __HAL_TIM_GET_COUNTER( IEncoderCfg.TimHandle );
}
