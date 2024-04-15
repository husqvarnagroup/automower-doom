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
 * @file      Interrupt.c
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Implementation of Interrupt.
 ******************************************************************************
 */
#include "Interrupt.h"
#include "RoboticTypes.h"
#include "stm32_hal.h"

typedef struct
{
    sint32 enter_critical_cnt;
} tInterrupt_Vars;

static tInterrupt_Vars interruptVars = { 0 };

void IInterrupt_Init( void )
{
    __disable_irq();
    interruptVars.enter_critical_cnt = 1;
}

void IInterrupt_GlobalDisable( void )
{
    __disable_irq();
    interruptVars.enter_critical_cnt++;
}

void IInterrupt_GlobalEnable( void )
{
    interruptVars.enter_critical_cnt--;
    if ( interruptVars.enter_critical_cnt == 0 )
    {
        __enable_irq();
    }
    else if ( interruptVars.enter_critical_cnt < 0 )
    {
        Error_Handler();
    }
}

uint8 IInterrupt_GetStatus( void )
{
    return 0 == interruptVars.enter_critical_cnt ? 1 : 0;
}

bool IInterrupt_InIsr( void )
{
    return ( SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk ) != 0;
}

void IInterrupt_Start( void )
{
    // nop
}
