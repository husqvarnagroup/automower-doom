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

// ****************************************************************************
// We can't really use the WWDG as-is, we need to extend the timeout time.
// We have scenarios where the WWDG timeout is really too short...
// Extending the WWDG timeout is done by letting the WWDG IRQ handle a count-
// down counter. As long as the extended counter > 0 we kick/refresh the WWDG
// but we stop this when the counter reaches 0.
// ****************************************************************************
/*
 ------------------------------------------------------------------------------
    Include files
 ------------------------------------------------------------------------------
 */
#include <stm32_hal.h>
#include "RoboticTypes.h"
#include "IWatchdog.h"

/*
 ------------------------------------------------------------------------------
    Defines
 ------------------------------------------------------------------------------
 */
#ifndef IWATCHDOGCFG_BASE_CLK_FREQUENCY_MHZ
#define IWATCHDOGCFG_BASE_CLK_FREQUENCY_MHZ ( 45 ) // setup in the STM32CubeMX and ending up in the generated HAL driver code
#endif
#ifndef IWATCHDOGCFG_BASE_CLK_PRESCALER
#define IWATCHDOGCFG_BASE_CLK_PRESCALER ( 8 ) // setup in the STM32CubeMX and ending up in the generated HAL driver code
#endif
#ifndef IWATCHDOGCFG_COUNTER_VALUE
#define IWATCHDOGCFG_COUNTER_VALUE ( 127 ) // setup in the STM32CubeMX and ending up in the generated HAL driver code
#endif

#ifndef IWATCHDOGCFG_EXTENDED_TIMEOUT_MS
#define IWATCHDOGCFG_EXTENDED_TIMEOUT_MS ( 8000 ) // if nothing else is configured - lets have 8 seconds as extended timeout
#endif

/**
 * The extended reload counter value is based on the wanted extended timeout value and the WWDG setup.
 * WWDG clock = <base-clock> / ( 4096 * <pre-scaler> ( 1/8 ) )
 * WWDG timeout (ms) = 1000 * ( <counter-value> ( 127..64 ) - 0x40 ( == 63 ) ) / WWDG clock (Hz)
 */
const uint32 extendedWwdgReloadCounter = ( ( IWATCHDOGCFG_EXTENDED_TIMEOUT_MS * IWATCHDOGCFG_BASE_CLK_FREQUENCY_MHZ * 1000 ) / ( ( IWATCHDOGCFG_COUNTER_VALUE - 0x40 ) * 4096 * IWATCHDOGCFG_BASE_CLK_PRESCALER ) );

/*
 ------------------------------------------------------------------------------
    Types
 ------------------------------------------------------------------------------
 */
/**
 * NOTE!
 * As the WWDG is our safe-guard towards various types of coding bugs and glitches in HW we really don't want the
 * extended timeout handling to mess up stuff (i.e we get stuck kicking the WWDG but the extended counter value is
 * locked/set to a too high value and so on...) we at least store it double in RAM and make some simple plausibility
 * checks on the value.
 */
typedef struct
{
    union
    {
        uint32 atom;
        struct
        {
            sint16 value;
            sint16 valueInverted;
        };
    };
} tSafeUint16;

/**
 * The Watchdog control block.
 */
typedef struct
{
    tSafeUint16 irqCounter;      // counter and its inverted value
    tSafeUint16 worstIrqCounter; // debug value; to keep track on the worst-case scenario
} tWatchdogVars;

static tWatchdogVars watchdogVars = { 0 };

/*
 ------------------------------------------------------------------------------
    Implementation of interface functions
 ------------------------------------------------------------------------------
 */
/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void IWatchdog_Init( void )
{
    // deliberately a NOP!
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void IWatchdog_Start( void )
{
    // deliberately a NOP!
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void IWatchdog_Activate( void )
{
    const tSafeUint16 safeReloadCounter = { .value = extendedWwdgReloadCounter, .valueInverted = (sint16)~extendedWwdgReloadCounter };

    watchdogVars.irqCounter      = safeReloadCounter;
    watchdogVars.worstIrqCounter = safeReloadCounter;

    MX_WWDG_Init();
}
/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void IWatchdog_Refresh( void )
{
    const tSafeUint16 safeReloadCounter = { .value = extendedWwdgReloadCounter, .valueInverted = (sint16)~extendedWwdgReloadCounter };

    watchdogVars.irqCounter = safeReloadCounter;

    HAL_WWDG_Refresh( &hwwdg );
}

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void HAL_WWDG_EarlyWakeupCallback( WWDG_HandleTypeDef* hwwdg )
{
    tSafeUint16 safeReloadCounter = { .atom = watchdogVars.irqCounter.atom };

    // sanity check; value and the inverted value should be OK
    if ( ( ( safeReloadCounter.value ^ safeReloadCounter.valueInverted ) & 0xFFFF ) != 0xFFFF )
    {
        return;
    }
    // sanity check; the value should never be higher than the reload value
    if ( safeReloadCounter.value > extendedWwdgReloadCounter )
    {
        return;
    }

    // refresh the WWDG, if there is still time left on the extended timeout
    if ( 0 < safeReloadCounter.value )
    {
        --safeReloadCounter.value;
        ++safeReloadCounter.valueInverted;

        watchdogVars.irqCounter = safeReloadCounter;

        HAL_WWDG_Refresh( hwwdg );

        // for debugging; the worst-case of counter value seen, so far
        if ( safeReloadCounter.value < watchdogVars.worstIrqCounter.value )
        {
            watchdogVars.worstIrqCounter = safeReloadCounter;
        }
    }
}
