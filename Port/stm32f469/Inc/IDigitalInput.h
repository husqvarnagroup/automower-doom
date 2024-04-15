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



#ifndef IDIGITALINPUT_H
#define IDIGITALINPUT_H

/*
 ------------------------------------------------------------------------------
 Include files
 ------------------------------------------------------------------------------
 */
#include "RoboticTypes.h"



/*
 ------------------------------------------------------------------------------
 Type definitions
 ------------------------------------------------------------------------------
 */
typedef uint32 tIDigitalInput_PinIndex;

typedef enum
{
	IDIGITALINPUT_EVENT_EXTI0 = (186 << 16),  /**< Event when external interrupt on line 0 is triggered */
	IDIGITALINPUT_EVENT_EXTI1,
	IDIGITALINPUT_EVENT_EXTI2,
	IDIGITALINPUT_EVENT_EXTI3,
	IDIGITALINPUT_EVENT_EXTI4,
	IDIGITALINPUT_EVENT_EXTI5,
	IDIGITALINPUT_EVENT_EXTI6,
	IDIGITALINPUT_EVENT_EXTI7,
	IDIGITALINPUT_EVENT_EXTI8,
	IDIGITALINPUT_EVENT_EXTI9,
	IDIGITALINPUT_EVENT_EXTI10,
	IDIGITALINPUT_EVENT_EXTI11,
	IDIGITALINPUT_EVENT_EXTI12,
	IDIGITALINPUT_EVENT_EXTI13,
	IDIGITALINPUT_EVENT_EXTI14,
	IDIGITALINPUT_EVENT_EXTI15
	
} tIDigitalInput_Events;

typedef enum
{
    IDIGITALINPUT_IT_RISING_FALLING = 0,  /**< External Interrupt Mode with Rising/Falling edge trigger detection  */
    IDIGITALINPUT_IT_RISING,              /**< External Interrupt Mode with Rising edge trigger detection */
    IDIGITALINPUT_IT_FALLING,             /**< External Interrupt Mode with Falling edge trigger detection */
} tIDigitalInput_ExtiMode;

/*
 ------------------------------------------------------------------------------
 Interface functions
 ------------------------------------------------------------------------------
 */

/**
 ******************************************************************************
 * @brief   Initialize digital input module
 * @param   -
 * @returns -
 ******************************************************************************
 */
void IDigitalInput_Init( void );

/**
 ******************************************************************************
 * @brief   Initialize digital input module
 * @param   -
 * @returns -
 ******************************************************************************
 */
void IDigitalInput_Start( void );

/**
 ******************************************************************************
 * @brief   Gets the digital pin state
 * @param   pinIndex
 *          Digital input pin index
 * @returns TRUE if set
 ******************************************************************************
 */
bool IDigitalInput_Get( const tIDigitalInput_PinIndex pinIndex );


#endif /* IDIGITALINPUT_H */
