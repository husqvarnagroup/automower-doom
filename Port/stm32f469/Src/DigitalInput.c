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
 * @file      DigitalInput.c
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     implementation
 ******************************************************************************
 */
/*
-------------------------------------------------------------------------------
    Include
-------------------------------------------------------------------------------
*/
#include "IDigitalInput.h"
#include "IDigitalInputCfg.h"
#include "IDigitalOutput.h"
#include "IDigitalOutputCfg.h"
#include "stm32_hal.h"

#include "main.h"

/*
-------------------------------------------------------------------------------
    Local definitions
-------------------------------------------------------------------------------
*/
#define DIGITALINPUT_NUMBER_OF_INPUTS ( sizeof( IDigitalInputCfg_Input ) / sizeof( tDigitalInput_Input ) )

/*
-------------------------------------------------------------------------------
    Local types
-------------------------------------------------------------------------------
*/
typedef struct
{
    bool started;

    bool safeISR_Toggle;
    bool safeStarted;
    bool jogWheelState;
    bool startButtonState;

} tDigitalInput_Data;

/*
-------------------------------------------------------------------------------
    Private data
-------------------------------------------------------------------------------
*/
tDigitalInput_Data DigitalInputVars = { 0 };

/*
-------------------------------------------------------------------------------
    Private function prototypes
-------------------------------------------------------------------------------
*/
__weak void DigitalInputExt_Init();
__weak void DigitalInputExt_Start();

/*
-------------------------------------------------------------------------------
    Implementation of interface functions
-------------------------------------------------------------------------------
*/

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void IDigitalInput_Init( void )
{
    static bool Initialized = false;
    if ( !Initialized )
    {
        Initialized = true;

        MX_GPIO_Init_();
        DigitalInputExt_Init();
    }
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void IDigitalInput_Start( void )
{
    DigitalInputVars.started = true;
    DigitalInputExt_Start();
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
bool IDigitalInput_Get( const tIDigitalInput_PinIndex pinIndex )
{
    if ( DIGITALINPUT_NUMBER_OF_INPUTS <= pinIndex )
    {
        return false;
    }

    tDigitalInput_Input input = IDigitalInputCfg_Input[ pinIndex ];
    bool                ret   = GPIO_PIN_SET == HAL_GPIO_ReadPin( input.port, input.pin );
    if ( input.inverted )
    {
        ret = !ret;
    }
    return ret;
}

/*
-------------------------------------------------------------------------------
    Implementation of public functions
-------------------------------------------------------------------------------
*/
/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
uint32 DigitalInput_Execute( void )
{
    static uint32_t lastSafeToggle = 0;
    if ( DigitalInputVars.started )
    {
        uint32_t currentTime = my_get_systime();
        if ( currentTime - lastSafeToggle >= 2 )
        {
            lastSafeToggle = currentTime;
            // Toggle
            if ( TRUE == DigitalInputVars.safeISR_Toggle )
            {
                IDigitalOutput_Set( IDIGITALOUTPUTCFG_SAFE_PULSE, FALSE );
                DigitalInputVars.safeISR_Toggle   = FALSE;
                DigitalInputVars.startButtonState = !IDigitalInput_Get( IDIGITALINPUTCFG_STARTBUTTON );
            }
            else
            {
                IDigitalOutput_Set( IDIGITALOUTPUTCFG_SAFE_PULSE, TRUE );
                DigitalInputVars.safeISR_Toggle = TRUE;
                DigitalInputVars.jogWheelState  = !IDigitalInput_Get( IDIGITALINPUTCFG_MAINSWITCH );
            }
        }
    }
    return 186;
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
bool DigitalInput_GetJogWheelDown( void )
{
    return DigitalInputVars.jogWheelState;
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
bool DigitalInput_GetStartButtonDown( void )
{
    return DigitalInputVars.startButtonState;
}

/*
-------------------------------------------------------------------------------
    Implementation of private functions
-------------------------------------------------------------------------------
*/

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
__weak void DigitalInputExt_Init()
{
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
__weak void DigitalInputExt_Start()
{
}
