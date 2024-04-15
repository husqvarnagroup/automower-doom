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
 *******************************************************************************
 * @file     Magnetic3dSensor.c
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief    Magnetic3dSensor implementation file for TLE493D sensor
 *******************************************************************************
 */

/*
-------------------------------------------------------------------------------
    Include files
-------------------------------------------------------------------------------
 */
#include "IMagnetic3dSensorCfg.h"
#include "IMagnetic3dSensor.h"
#include "Magnetic3dSensor.h"
#include "II2c.h"
#include <math.h>
#include <IDigitalOutput.h>
#include <IDigitalOutputCfg.h>
#include "RoboticUtils.h"
#include "ISystemTick.h"

/*
-------------------------------------------------------------------------------
    Definitions
-------------------------------------------------------------------------------
 */

#define TLE493D_RX_SIZE                  ( 23 ) /* We pull all 23 registers. */
#define TLE493D_TX_SIZE                  ( 14 )
#define TLE493D_INVALID_FRAME_CNT        ( 0xFFFF )
#define TLE493D_MAX_RETRANSMIT           ( 2 )
#define TLE493D_WAIT_RETRANSMIT_MICROSEC ( 200 )
#define TLE493D_WAIT_FIRST_READ_MICROSEC ( 4000 )
#define TLE493_MAX_RESET_ATTEMPTS        ( 3 )

#define TLE493D_MAX_NO_EVENT_CB ( 6 ) // Col + Lift + Angle + DeckSensor + ChargerConnected + TIF

#define TLE493D_MAX_FRAME_LIMIT ( 10 )

#define TLE493D_MAX_RESET_LOOPS ( 10 )

#ifndef TLE493D_LOG_ENABLED
#define TLE493D_LOG_ENABLED ( true )
#endif

/*
-------------------------------------------------------------------------------
    Local types
-------------------------------------------------------------------------------
 */

typedef enum
{
    MS_EVENT_TIMER,
    MS_EVENT_NEXT,
    MS_EVENT_RX_DONE,
    MS_EVENT_TX_DONE,
    MS_EVENT_COM_FAIL,
    MS_EVENT_HW_RESET,
    MS_EVENT_I2C_OPEN,
    MS_EVENT_SW_RESET,
    MS_EVENT_ERROR,
    MS_EVENT_FATAL_ERROR,
    MS_EVENT_NUMBER_OF_EVENTS,
} tMagneticSensor_Event;

typedef struct
{
    /** Error*/
    bool error;
    /** CommunicationFailure*/
    bool comFailure;
    /** CommunicationErrorCode */
    uint16 comErrorCode;
} tIMagnetic3dSensor_SafeErrorStatus;

typedef struct
{
    /** Flux density in the x direction */
    sint16 x;
    /** Flux density in the y direction */
    sint16 y;
    /** Flux density in the z direction */
    sint16 z;
} tMagnetic3dSensor_SafeFluxDensity;

typedef struct
{
    uint16                             safeState;
    tMagnetic3dSensor_SafeFluxDensity  safeCurFlux;
    uint16                             frameCount;
    uint16                             hwResetCount;
    uint16                             swResetCount;
    uint16                             resetFailedCount;
    uint16                             safeStuckFrameCount;
    uint16                             zeroFluxCount;
    bool                               validDataReceived;
    void*                              safeEventCb[ TLE493D_MAX_NO_EVENT_CB ];
    tIMagnetic3dSensor_SafeErrorStatus safeErrorStatus;
    bool                               safeFluxEventEnabled;
    uint16                             portIndex;
    void*                              timerTaskId; /**< Timer ID for periodic task callback */
} tMagnetic3dSensor_InstanceData;

typedef struct
{
    uint8 txData[ TLE493D_TX_SIZE ];
    uint8 rxData[ TLE493D_RX_SIZE ];
} tMagnetic3dSensor_I2cData;

typedef struct
{
    uint16                         errorCount;
    uint16                         safeStarted;
    bool                           logEnabled; // debug variable, not needed to be safe
    tMagnetic3dSensor_InstanceData safeInstanceData[ MAGNETIC_3D_SENSOR_NUMBER_OF_PORTS ];
} tMagnetic3dSensor_Vars;

typedef struct
{
    bool   wakeupInitialize;
    sint16 thresholdXl;
    sint16 thresholdXh;
    sint16 thresholdYl;
    sint16 thresholdYh;
    sint16 thresholdZl;
    sint16 thresholdZh;
} tMagnetic3dSensor_WakeupConfig;

/**
 * @def   MAGNETIC_3D_SENSOR_BACKOFF_PERIOD [ms]
 * @brief Time to wait before a new try to check sensor connected.
 */
#ifdef MAGNETIC_3D_SENSOR_BACKOFF_PERIOD
#define MAGNETIC_3D_SENSOR_BACKOFF_TIME MAGNETIC_3D_SENSOR_BACKOFF_PERIOD
#else
#define MAGNETIC_3D_SENSOR_BACKOFF_TIME 2000
#endif

/*
-------------------------------------------------------------------------------
    Function prototypes
-------------------------------------------------------------------------------
 */
static void   ChangeState( tMagnetic3dSensor_InstanceData* pInstanceData, tMagneticSensorState newState );
static void   EventHandler( tEvent event );
static void   HandleStateConfigure( tIMagnetic3dSensor_PortIndex portIndex );
static bool   HandleStateSwReset( tIMagnetic3dSensor_PortIndex portIndex );
static void   HandleStateActive( tMagnetic3dSensor_InstanceData* pInstanceData );
static sint16 ConvertUint12ToSint16( uint16 u12 );
static void   I2cEventCallback( tEvent event );

static void                                   BusyWait( uint32 microsec );
static bool                                   RxRequest( tIMagnetic3dSensor_PortIndex index );
static bool                                   RxRequest_Startup( tIMagnetic3dSensor_PortIndex index );
static bool                                   TxRequest( tIMagnetic3dSensor_PortIndex index, uint16 len );
static void                                   ErrorStatusUpdate( tMagnetic3dSensor_InstanceData* pInstanceData, bool newStatus );
static tMagnetic3dSensorTle493d_Config const* GetConfig( tIMagnetic3dSensor_PortIndex index );
static uint8*                                 GetRxData( tIMagnetic3dSensor_PortIndex index );
static uint8*                                 GetTxData( tIMagnetic3dSensor_PortIndex index );

static void StartWatch( void );
static void StopWatch( void );


/*
-------------------------------------------------------------------------------
    Private data
-------------------------------------------------------------------------------
 */

static tMagnetic3dSensor_Vars         vars;
static tMagnetic3dSensor_I2cData      i2cData[ MAGNETIC_3D_SENSOR_NUMBER_OF_PORTS ];
static bool                           magneticInitialized = false;
static tMagnetic3dSensor_WakeupConfig wakeupConfig[ MAGNETIC_3D_SENSOR_NUMBER_OF_PORTS ];
/*
-------------------------------------------------------------------------------
    Implementation of interface functions
-------------------------------------------------------------------------------
 */

void IMagnetic3dSensor_Init( void )
{
    if ( magneticInitialized )
    {
        return;
    }
    magneticInitialized = true;
    IDigitalOutput_Init();
    II2c_Init();

    ISystemTick_Init();
    vars.safeStarted = 0;
    vars.errorCount  = 0;
    vars.logEnabled  = TLE493D_LOG_ENABLED;

    for ( uint32 i = 0; i < MAGNETIC_3D_SENSOR_NUMBER_OF_PORTS; i++ )
    {
        for ( uint32 j = 0; j < TLE493D_MAX_NO_EVENT_CB; j++ )
        {
            vars.safeInstanceData[ i ].safeEventCb[ j ] = NULL;
        }
        vars.safeInstanceData[ i ].safeErrorStatus.error        = false;
        vars.safeInstanceData[ i ].safeErrorStatus.comFailure   = false;
        vars.safeInstanceData[ i ].safeErrorStatus.comErrorCode = 0;
        vars.safeInstanceData[ i ].portIndex                    = i;
        vars.safeInstanceData[ i ].resetFailedCount             = 0;
        vars.safeInstanceData[ i ].safeState                    = MS_STATE_I2C_OPEN;
        vars.safeInstanceData[ i ].hwResetCount                 = 0;
        vars.safeInstanceData[ i ].swResetCount                 = 0;
        vars.safeInstanceData[ i ].zeroFluxCount                = 0;
        vars.safeInstanceData[ i ].validDataReceived            = false;
        vars.safeInstanceData[ i ].safeStuckFrameCount          = 0;
        vars.safeInstanceData[ i ].frameCount                   = TLE493D_INVALID_FRAME_CNT;
        vars.safeInstanceData[ i ].safeCurFlux.x                = 0;
        vars.safeInstanceData[ i ].safeCurFlux.y                = 0;
        vars.safeInstanceData[ i ].safeCurFlux.z                = 0;
        vars.safeInstanceData[ i ].timerTaskId                  = NULL;
        vars.safeInstanceData[ i ].safeFluxEventEnabled         = true;
    }
}

void IMagnetic3dSensor_Start()
{
    if ( 0 != vars.safeStarted )
    {
        /* Already started */
        return;
    }
    IDigitalOutput_Start();
    II2c_Start();

    ISystemTick_Start();

    /* Power on magnetic 3D sensor */
    IDigitalOutput_Set( IDIGITALOUTPUTCFG_VDD_HAL, true );

    /* Setup timer for execute callback*/
    for ( uint32 i = 0; i < MAGNETIC_3D_SENSOR_NUMBER_OF_PORTS; i++ )
    {
        tEvent event;
        event.id                               = MS_EVENT_TIMER;
        event.data                             = i;
        tISystemTick_TimerId timerId           = ISystemTick_CreateTimer( event, EventHandler );
        vars.safeInstanceData[ i ].timerTaskId = timerId;
        ISystemTick_StartTimer( (tISystemTick_TimerId)vars.safeInstanceData[ i ].timerTaskId, IMagnetic3dSensorTle493dCfg_Scheduler.timerTaskPeriod, true );

        II2c_Open( IMagnetic3dSensorTle493dCfg_Port[ i ].i2cPortIndex, I2cEventCallback, i );
        if ( IMagnetic3dSensorTle493dCfg_Port[ i ].readDuringStart )
        {
            const uint16 maxRetries = 5;
            for ( uint16 j = 0; j < maxRetries; j++ )
            {
                for ( uint8 n = 0; n < TLE493_MAX_RESET_ATTEMPTS; n++ )
                {
                    if ( HandleStateSwReset( i ) )
                        break;
                }
                BusyWait( TLE493D_WAIT_RETRANSMIT_MICROSEC ); // Needed since synchronous read during startup
                HandleStateConfigure( i );
                BusyWait( TLE493D_WAIT_RETRANSMIT_MICROSEC ); // Needed since synchronous read during startup

                // Data is normally read in EventHandler, since i2c rx/tx functions are asynchronous.
                // However, must read once before leaving start function.
                // Hence, request data from i2c using the "old"/blocking i2c rx function.
                if ( RxRequest_Startup( i ) )
                {
                    tMagnetic3dSensor_InstanceData* pInstanceData = &vars.safeInstanceData[ i ];
                    HandleStateActive( pInstanceData );

                    if ( 0 == vars.safeInstanceData[ i ].safeCurFlux.x &&
                         0 == vars.safeInstanceData[ i ].safeCurFlux.y &&
                         0 == vars.safeInstanceData[ i ].safeCurFlux.z )
                    {
                    }
                    else
                    {
                        // stop loop
                        j = maxRetries;
                    }
                }
            }

            vars.safeInstanceData[ i ].safeState = MS_STATE_ACTIVE;
        }
        else
        {
            event.id = MS_EVENT_NEXT;
            EventHandler( event );
        }
    }

    vars.safeStarted = 1;
}

bool IMagnetic3dSensor_IsReady( tIMagnetic3dSensor_PortIndex portIndex )
{
    if ( portIndex >= MAGNETIC_3D_SENSOR_NUMBER_OF_PORTS )
    {
        /* Invalid port */
        return false;
    }

    const tMagneticSensorState state = vars.safeInstanceData[ portIndex ].safeState;
    return ( state == MS_STATE_ACTIVE );
}

bool IMagnetic3dSensor_GetCurrentFlux( tIMagnetic3dSensor_PortIndex    portIndex,
                                       tIMagnetic3dSensor_FluxDensity* pFlux )
{
    if ( portIndex >= MAGNETIC_3D_SENSOR_NUMBER_OF_PORTS )
    {
        /* Invalid port */
        return false;
    }

    if ( MS_STATE_ACTIVE != vars.safeInstanceData[ portIndex ].safeState ||
         !vars.safeInstanceData[ portIndex ].validDataReceived )
    {
        /* Port is suffering from an error condition */
        pFlux->x = 0;
        pFlux->y = 0;
        pFlux->z = 0;
        return false;
    }

    pFlux->x = vars.safeInstanceData[ portIndex ].safeCurFlux.x;
    pFlux->y = vars.safeInstanceData[ portIndex ].safeCurFlux.y;
    pFlux->z = vars.safeInstanceData[ portIndex ].safeCurFlux.z;

    return true;
}

bool IMagnetic3dSensor_RegisterEventCallback( tIMagnetic3dSensor_PortIndex portIndex, tEventCallback pEventCallback )
{
    if ( portIndex > MAGNETIC_3D_SENSOR_NUMBER_OF_PORTS - 1 )
    {
        /* Invalid port */
        return false;
    }
    if ( NULL == pEventCallback )
    {
        return false;
    }

    // Find free slot
    for ( uint8 i = 0; i < TLE493D_MAX_NO_EVENT_CB; ++i )
    {
        tEventCallback cb = (tEventCallback)vars.safeInstanceData[ portIndex ].safeEventCb[ i ];
        if ( cb == NULL ||
             cb == pEventCallback )
        {
            vars.safeInstanceData[ portIndex ].safeEventCb[ i ] = (void*)pEventCallback;
            return true;
        }
    }

    return false;
}

bool IMagnetic3dSensor_GetErrorStatus( tIMagnetic3dSensor_PortIndex portIndex, tIMagnetic3dSensor_ErrorStatus* pErrorStatus )
{
    if ( portIndex > MAGNETIC_3D_SENSOR_NUMBER_OF_PORTS - 1 )
    {
        /* Invalid port */
        return false;
    }

    pErrorStatus->error        = vars.safeInstanceData[ portIndex ].safeErrorStatus.error;
    pErrorStatus->comErrorCode = vars.safeInstanceData[ portIndex ].safeErrorStatus.comErrorCode;
    pErrorStatus->comFailure   = vars.safeInstanceData[ portIndex ].safeErrorStatus.comFailure;
    return true;
}
/*
-------------------------------------------------------------------------------
    Implementation of public functions
-------------------------------------------------------------------------------
 */

void Magnetic3dSensor_Reset( tIMagnetic3dSensor_PortIndex portIndex )
{
    if ( portIndex > MAGNETIC_3D_SENSOR_NUMBER_OF_PORTS - 1 )
    {
        /* Invalid port */
        return;
    }

    tEvent event;
    event.id   = MS_EVENT_ERROR;
    event.data = portIndex;
    EventHandler( event );
}

void Magnetic3dSensor_EnableLogs( void )
{
    vars.logEnabled = true;
}

tMagneticSensorState Magnetic3dSensor_GetState( tIMagnetic3dSensor_PortIndex portIndex )
{
    if ( portIndex > MAGNETIC_3D_SENSOR_NUMBER_OF_PORTS - 1 )
    {
        /* Invalid port */
        return MS_STATE_NUMBER_OF_STATES;
    }
    return vars.safeInstanceData[ portIndex ].safeState;
}
uint32 Magnetic3dSensor_GetErrorCount( void )
{
    return vars.errorCount;
}

void Magnetic3dSensor_EnableProfiling( void )
{
}

sint32 Magnetic3dSensor_GetProfilingTime( void )
{
    return 0;
}

void Magnetic3dSensor_SetFluxEventEnabled( tIMagnetic3dSensor_PortIndex portIndex, bool isEventsEnabled )
{
    if ( portIndex < MAGNETIC_3D_SENSOR_NUMBER_OF_PORTS )
    {
        vars.safeInstanceData[ portIndex ].safeFluxEventEnabled = isEventsEnabled;
    }
}

bool Magnetic3dSensor_GetFluxEventEnabled( tIMagnetic3dSensor_PortIndex portIndex )
{
    if ( portIndex < MAGNETIC_3D_SENSOR_NUMBER_OF_PORTS )
    {
        return vars.safeInstanceData[ portIndex ].safeFluxEventEnabled;
    }
    return true;
}

bool Magnetic3dSensor_WakeUpInit( tIMagnetic3dSensor_PortIndex portIndex, sint16 xl, sint16 xh, sint16 yl, sint16 yh, sint16 zl, sint16 zh )
{
    tMagnetic3dSensor_WakeupConfig* wakeupCfg = &wakeupConfig[ portIndex ];
    bool                            result    = true;

    if ( xh > 2047 || xl < -2048 || xh < xl || ( xh - xl ) > 2047 )
        result = false;

    if ( yh > 2047 || yl < -2048 || yh < yl || ( yh - yl ) > 2047 )
        result = false;

    if ( zh > 2047 || zl < -2048 || zh < zl || ( zh - zl ) > 2047 )
        result = false;

    if ( result )
    {
        wakeupCfg->thresholdXl = xl;
        wakeupCfg->thresholdXh = xh;
        wakeupCfg->thresholdYl = yl;
        wakeupCfg->thresholdYh = yh;
        wakeupCfg->thresholdZl = zl;
        wakeupCfg->thresholdZh = zh;

        wakeupCfg->wakeupInitialize = true;
    }

    return result;
}

void Magnetic3dSensor_GetWakeupThresholds( tIMagnetic3dSensor_PortIndex portIndex, sint16* xl, sint16* xh, sint16* yl, sint16* yh, sint16* zl, sint16* zh )
{
    tMagnetic3dSensor_WakeupConfig* wakeupCfg = &wakeupConfig[ portIndex ];

    *xl = wakeupCfg->thresholdXl;
    *xh = wakeupCfg->thresholdXh;
    *yl = wakeupCfg->thresholdYl;
    *yh = wakeupCfg->thresholdYh;
    *zl = wakeupCfg->thresholdZl;
    *zh = wakeupCfg->thresholdZh;

    return;
}

/*
-------------------------------------------------------------------------------
    Implementation of private functions
-------------------------------------------------------------------------------
 */
void EventHandler( tEvent event )
{
    const tIMagnetic3dSensor_PortIndex portIndex = event.data;

    tMagnetic3dSensor_InstanceData*        pInstanceData = &vars.safeInstanceData[ portIndex ];
    const tMagneticSensorState             state         = pInstanceData->safeState;
    tMagnetic3dSensorTle493d_Config const* pCfg          = GetConfig( portIndex );

    if ( event.id == MS_EVENT_FATAL_ERROR )
    {
        ChangeState( pInstanceData, MS_STATE_FATAL_ERROR );
        return;
    }

    if ( event.id == MS_EVENT_ERROR )
    {
        if ( 0 == pInstanceData->swResetCount )
        {
            if ( pCfg->i2cCloseAtReset )
            {
                ChangeState( pInstanceData, MS_STATE_I2C_OPEN );
            }
            else
            {
                ChangeState( pInstanceData, MS_STATE_SW_RESET );
            }
        }
        else
        {
            ChangeState( pInstanceData, MS_STATE_ERROR );
        }
        return;
    }

    switch ( state )
    {
        case MS_STATE_I2C_OPEN:
        {
            switch ( event.id )
            {
                case MS_EVENT_NEXT:
                    ChangeState( pInstanceData, MS_STATE_SW_RESET );
                    break;
            }
        }
        break;
        case MS_STATE_SW_RESET:
        {
            switch ( event.id )
            {
                case MS_EVENT_NEXT:
                    if ( HandleStateSwReset( portIndex ) )
                    {
                        ChangeState( pInstanceData, MS_STATE_CONFIGURE );
                    }
                    else
                    {
                        ChangeState( pInstanceData, MS_STATE_ERROR );
                    }
                    break;
            }
        }
        break;
        case MS_STATE_CONFIGURE:
        {
            switch ( event.id )
            {
                case MS_EVENT_TX_DONE: // Sensor has received configuration
                    ChangeState( pInstanceData, MS_STATE_ACTIVE );
                    break;
            }
        }
        break;
        case MS_STATE_ACTIVE:
        {
            switch ( event.id )
            {
                case MS_EVENT_RX_DONE:
                    HandleStateActive( pInstanceData );

                    break;
                case MS_EVENT_TIMER:
                    if ( IDigitalOutput_Get( IDIGITALOUTPUTCFG_VDD_HAL ) )
                    {
                        (void)RxRequest( portIndex ); // Start new reading of sensor
                    }
                    else
                    {
                        ChangeState( pInstanceData, MS_STATE_OFF );
                    }
                    break;
            }
        }
        break;
        case MS_STATE_ERROR:
        {
            switch ( event.id )
            {
                case MS_EVENT_I2C_OPEN:
                    ChangeState( pInstanceData, MS_STATE_I2C_OPEN );
                    break;
                case MS_EVENT_SW_RESET:
                    ChangeState( pInstanceData, MS_STATE_SW_RESET );
                    break;
                case MS_EVENT_HW_RESET:
                    ChangeState( pInstanceData, MS_STATE_HW_RESET );
                    break;
                case MS_EVENT_FATAL_ERROR:
                    ChangeState( pInstanceData, MS_STATE_FATAL_ERROR );
                    break;
            }
        }
        break;
        case MS_STATE_HW_RESET:
        {
            switch ( event.id )
            {
                case MS_EVENT_TIMER:
                    ChangeState( pInstanceData, MS_STATE_I2C_OPEN );
                    break;
            }
        }
        break;
        case MS_STATE_OFF:
        {
            switch ( event.id )
            {
                case MS_EVENT_TIMER:
                    if ( IDigitalOutput_Get( IDIGITALOUTPUTCFG_VDD_HAL ) )
                    {
                        ChangeState( pInstanceData, MS_STATE_I2C_OPEN );
                    }
                    break;
            }
        }
        break;
        case MS_STATE_FATAL_ERROR:
        {
            switch ( event.id )
            {
                case MS_EVENT_TIMER:
                    if ( IDigitalOutput_Get( IDIGITALOUTPUTCFG_VDD_HAL ) )
                    {
                        ChangeState( pInstanceData, MS_STATE_BACKOFF );
                    }
                    else
                    {
                        ChangeState( pInstanceData, MS_STATE_OFF );
                    }
                    break;
            }
        }
        break;
        case MS_STATE_BACKOFF:
        {
            switch ( event.id )
            {
                static uint16 timerCounter;
                case MS_EVENT_TIMER:
                    if ( ( timerCounter * IMagnetic3dSensorTle493dCfg_Scheduler.timerTaskPeriod ) >= MAGNETIC_3D_SENSOR_BACKOFF_TIME )
                    {
                        if ( pCfg->hwResetEnabled )
                        {
                            ChangeState( pInstanceData, MS_STATE_HW_RESET );
                        }
                        else if ( pCfg->i2cCloseAtReset )
                        {
                            ChangeState( pInstanceData, MS_STATE_I2C_OPEN );
                        }
                        else
                        {
                            ChangeState( pInstanceData, MS_STATE_SW_RESET );
                        }
                        timerCounter = 0;
                    }
                    else
                    {
                        timerCounter += 1;
                    }
                    break;
            }
        }
        break;
    }
}
/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */

static void ChangeState( tMagnetic3dSensor_InstanceData* pInstanceData, tMagneticSensorState newState )
{
    const tMagneticSensorState             oldState  = pInstanceData->safeState; // state shall always be set to something
    const uint8                            portIndex = 0;
    tMagnetic3dSensorTle493d_Config const* pCfg      = GetConfig( portIndex );

    if ( oldState == newState )
    {
        return;
    }

    tEvent event;
    event.id   = MS_EVENT_NUMBER_OF_EVENTS;
    event.data = portIndex;

    /* Exit actions */
    switch ( oldState )
    {
        case MS_STATE_I2C_OPEN:
            II2c_Open( pCfg->i2cPortIndex, I2cEventCallback, portIndex );
            break;
        case MS_STATE_CONFIGURE:
            break;
        case MS_STATE_HW_RESET:
            IDigitalOutput_Set( IDIGITALOUTPUTCFG_VDD_HAL, true );
            break;
        case MS_STATE_ACTIVE:
            pInstanceData->validDataReceived = false;
            break;
        case MS_STATE_OFF:
            break;
        default:
            break;
    }

    pInstanceData->safeState = newState;

    /* Entry actions */
    switch ( newState )
    {
        case MS_STATE_I2C_OPEN:
            if ( !II2c_Open( pCfg->i2cPortIndex, I2cEventCallback, portIndex ) )
            {
                while ( 1 )
                    ;
            }
            event.id = MS_EVENT_NEXT; // Jump to next state
            break;
        case MS_STATE_SW_RESET:
            pInstanceData->swResetCount = 1;
            event.id                    = MS_EVENT_NEXT;
            break;
        case MS_STATE_CONFIGURE:
            HandleStateConfigure( portIndex );
            break;
        case MS_STATE_ACTIVE:
            pInstanceData->zeroFluxCount = 0;
            ErrorStatusUpdate( pInstanceData, false );
            /* No event as this state is moved forward by timer or interrupt from sensor */
            break;
        case MS_STATE_ERROR:
        {
            uint16 errorCount = vars.errorCount;
            vars.errorCount   = errorCount + 1;
            if ( pCfg->i2cCloseAtReset )
            {
                II2c_Close( pCfg->i2cPortIndex );
            }

            ErrorStatusUpdate( pInstanceData, true );

            uint16 failedResetCount         = pInstanceData->resetFailedCount;
            pInstanceData->resetFailedCount = failedResetCount + 1;
            if ( failedResetCount >= TLE493D_MAX_RESET_LOOPS )
            {
                event.id = MS_EVENT_FATAL_ERROR;
            }
            else if ( pCfg->hwResetEnabled )
            {
                pInstanceData->hwResetCount = 0;
                event.id                    = MS_EVENT_HW_RESET;
            }
            else
            {
                if ( pCfg->i2cCloseAtReset )
                {
                    event.id = MS_EVENT_I2C_OPEN;
                }
                else
                {
                    event.id = MS_EVENT_SW_RESET;
                }
            }
        }
        break;
        case MS_STATE_HW_RESET:
            IDigitalOutput_Set( IDIGITALOUTPUTCFG_VDD_HAL, false );
            break;
        case MS_STATE_OFF:
            II2c_Close( pCfg->i2cPortIndex );
            break;
        case MS_STATE_FATAL_ERROR:
            break;
        case MS_STATE_BACKOFF:
            break;
        default:

            break;
    }
    if ( event.id != MS_EVENT_NUMBER_OF_EVENTS )
    {
        EventHandler( event );
    }
}

static void HandleStateConfigure( tIMagnetic3dSensor_PortIndex portIndex )
{
    // all configuration is done in TxRequest. And we start with I2C peripheral restart
    tMagnetic3dSensorTle493d_Config const* pCfg   = GetConfig( portIndex );
    uint8*                                 txData = GetTxData( 0 );
    II2c_Close( portIndex );
    II2c_Open( pCfg->i2cPortIndex, I2cEventCallback, portIndex );
    /* A byte with a write command:
     * --7---6---5----4---3---2---1---0---
     * | TRIG BITS  | WRITE REGISTER ADDR|
     * -----------------------------------
     * 0b100 ADC starts after reading reg. 0x06
     * 0x10 as first register to write
     */
    txData[ 0 ] = 0x90;
    /* Register 0x10 contents:
     * --7----6----5---4----3----2---1----0---
     * | DT | AM |  TRIG  | X2 | TL_mag | CP |
     * ---------------------------------------
     * 0b0 temperature measurement is enabled
     * 0b0 Bz measurement is enabled
     * 0b10 as trigger bits, measurement starts after 0x06 register is read
     * 0b0 normal short range sensitivity (not doubled)
     * 0b00 no magnetic temperature compensation
     * 0b1 as wake-up and configuration parity. should be adjusted depending on configured values
     */
    txData[ 1 ] = 0x21;
    /* Register 0x11 contents:
     * --7----6---5----4----3----2----1---0---
     * | FP | IICadr | PR | CA | INT|  MODE  |
     * ---------------------------------------
     * 0b1 as fuse parity (should be adjusted)
     * 0b00 default I2C address
     * 0b1 1 byte I2C read protocol (read always starts with register 0, do not provide address of read register)
     * 0b1 disable clock stretching to avoid communication when ADC is running
     * 0b1 disable interrupt after completed measurement and ADC conversion
     * 0b01 Master Controlled Mode. Measurement is done every time after reading certain register
     */
    txData[ 2 ] = 0x9d;
    (void)TxRequest( portIndex, 3 ); // send factory reset data
}

static void HandleStateActive( tMagnetic3dSensor_InstanceData* pInstanceData )
{
    const uint8 portIndex = pInstanceData->portIndex;
    uint16      uFlux[ 3 ];
    uint8       stuckFrameCount;
    uint8*      rxData = GetRxData( 0 );

    uFlux[ 0 ] = ( rxData[ 0 ] << 4 ) | ( rxData[ 4 ] >> 4 );
    uFlux[ 1 ] = ( rxData[ 1 ] << 4 ) | ( rxData[ 4 ] & 0xf );
    uFlux[ 2 ] = ( rxData[ 2 ] << 4 ) | ( rxData[ 5 ] & 0xf );

    const uint8  currentFrame = ( rxData[ 6 ] & 0x03 );
    const uint16 lastFrame    = pInstanceData->frameCount;
    pInstanceData->frameCount = currentFrame;

    const sint16 tFluxX = ConvertUint12ToSint16( uFlux[ 0 ] );
    const sint16 tFluxY = ConvertUint12ToSint16( uFlux[ 1 ] );
    const sint16 tFluxZ = ConvertUint12ToSint16( uFlux[ 2 ] );

    const sint16 lastX = pInstanceData->safeCurFlux.x;
    const sint16 lastY = pInstanceData->safeCurFlux.y;
    const sint16 lastZ = pInstanceData->safeCurFlux.z;

    pInstanceData->safeCurFlux.x = tFluxX;
    pInstanceData->safeCurFlux.y = tFluxY;
    pInstanceData->safeCurFlux.z = tFluxZ;

    if ( currentFrame == lastFrame )
    {
        if ( lastX == tFluxX &&
             lastY == tFluxY &&
             lastZ == tFluxZ )
        {
            stuckFrameCount = pInstanceData->safeStuckFrameCount;
            stuckFrameCount++;
            pInstanceData->safeStuckFrameCount = stuckFrameCount;
            if ( stuckFrameCount >= TLE493D_MAX_FRAME_LIMIT )
            {
                tEvent event;
                event.id   = MS_EVENT_ERROR;
                event.data = portIndex;
                EventHandler( event );
                return;
            }
        }
        else
        {
            pInstanceData->safeStuckFrameCount = 0;
        }
    }

    if ( !pInstanceData->validDataReceived )
    {
        if ( 0 == tFluxX &&
             0 == tFluxY &&
             0 == tFluxZ )
        {
            const uint16 zeroFluxCnt = pInstanceData->zeroFluxCount;
            if ( zeroFluxCnt > 10 )
            {
                pInstanceData->zeroFluxCount = 0;
                return;
            }
            else
            {
                pInstanceData->zeroFluxCount = zeroFluxCnt + 1;
            }
        }
        else
        {
            pInstanceData->zeroFluxCount     = 0;
            pInstanceData->validDataReceived = true;
            pInstanceData->resetFailedCount  = 0;
            pInstanceData->swResetCount      = 0;
        }
    }
    tEvent eventUpdatedFlux;
    eventUpdatedFlux.id   = IMAGNETIC3DSENSOR_EVENT_FLUX_UPDATED;
    eventUpdatedFlux.data = portIndex;
    if ( pInstanceData->validDataReceived && currentFrame != lastFrame )
    {
        StartWatch();
        for ( uint8 i = 0; i < TLE493D_MAX_NO_EVENT_CB; i++ )
        {
            tEventCallback cb = (tEventCallback)pInstanceData->safeEventCb[ i ];
            if ( cb != NULL )
            {
                if ( Magnetic3dSensor_GetFluxEventEnabled( portIndex ) )
                {
                    cb( eventUpdatedFlux );
                }
            }
        }
        StopWatch();
    }
}

static bool HandleStateSwReset( tIMagnetic3dSensor_PortIndex portIndex )
{
    bool commandStatus = true;
    // I2C restart
    tMagnetic3dSensorTle493d_Config const* pCfg = GetConfig( 0 );
    II2c_Close( portIndex );
    II2c_Open( pCfg->i2cPortIndex, I2cEventCallback, portIndex );
    /* According to user manual for TLE493D-W2B6 reset is done
     * by sending a sequence of:
     * start, 0xFF, stop, start, 0xFF, stop, start, 0x00, stop, start, 0x00, stop
     * and waiting 30 us. */
    static uint8 resetData[ 4 ] = { 0xFF, 0xFF, 0x00, 0x00 };
    for ( uint16 i = 0; i < 4; i++ )
    {
        commandStatus &= II2C_PutByte( portIndex, resetData[ i ] );
    }

    if ( commandStatus )
    {
        /* The delay has to be at least 30us based on TLE493D user manual, 2.3 Sensor reset by I2C */
        BusyWait( pCfg->sdaHoldDuringResetDuration );
        return true;
    }
    else
    {
        return false;
    }
}

static sint16 ConvertUint12ToSint16( uint16 u12 )
{
    if ( u12 & 0x800 )
    {
        return -2048 + ( 0x7ff & u12 );
    }
    else
    {
        return u12;
    }
}

/* NOTE This function is called in interrupt context*/
static void I2cEventCallback( tEvent event )
{
    const tIMagnetic3dSensor_PortIndex     portIndex = event.data;
    tEvent notifyEvent;
    notifyEvent.data = portIndex;
    switch ( event.id )
    {
        case II2C_EVENT_RX_DONE:
            notifyEvent.id = MS_EVENT_RX_DONE;
            EventHandler( notifyEvent );

            break;
        case II2C_EVENT_TX_DONE:
            /* Data transmitted */
            notifyEvent.id = MS_EVENT_TX_DONE;
            EventHandler( notifyEvent );
            break;
        case II2C_EVENT_ERROR:
            /* Handle error */
            notifyEvent.id = MS_EVENT_ERROR;
            EventHandler( notifyEvent );
            break;
    }
}

static void BusyWait( uint32 microsec )
{
    uint64 deltaT = 0;
    while ( deltaT < microsec * 2700 )
    {
        deltaT++;
    }
}

static bool RxRequest( tIMagnetic3dSensor_PortIndex index )
{
    // todo: here, the rxBuffer is emptied, so if this is called during an already active RxRequest, it will probably cause problems....
    uint8*                                 rxData = GetRxData( index );
    tMagnetic3dSensorTle493d_Config const* pCfg   = GetConfig( index );
    for ( uint32 i = 0; i < TLE493D_RX_SIZE; i++ )
    {
        rxData[ i ] = 0;
    }
    bool status = false;

    // Probably blocking below
    for ( uint32 i = 0; i < TLE493D_MAX_RETRANSMIT; i++ )
    {
        if ( II2C_RxRequest( pCfg->i2cPortIndex,
                             pCfg->i2cAddress,
                             TLE493D_RX_SIZE,
                             rxData ) )
        {
            status = true;
            i      = TLE493D_MAX_RETRANSMIT;
        }
        else
        {
            BusyWait( TLE493D_WAIT_RETRANSMIT_MICROSEC );
        }
    }
    return status;
}

static bool RxRequest_Startup( tIMagnetic3dSensor_PortIndex index )
{
    uint8*                                 rxData = GetRxData( index );
    tMagnetic3dSensorTle493d_Config const* pCfg   = GetConfig( index );

    for ( uint32 i = 0; i < TLE493D_RX_SIZE; i++ )
    {
        rxData[ i ] = 0;
    }
    bool status = false;
    for ( uint32 i = 0; i < TLE493D_MAX_RETRANSMIT; i++ )
    {
        if ( II2C_RxRequest( pCfg->i2cPortIndex,
                             pCfg->i2cAddress,
                             TLE493D_RX_SIZE,
                             rxData ) )
        {
            status = true;
            i      = TLE493D_MAX_RETRANSMIT;
        }
        else
        {
            BusyWait( TLE493D_WAIT_RETRANSMIT_MICROSEC );
        }
    }
    return status;
}

static bool TxRequest( tIMagnetic3dSensor_PortIndex index, uint16 len )
{
    uint8*                                 txData = GetTxData( index );
    tMagnetic3dSensorTle493d_Config const* pCfg   = GetConfig( index );
    bool                                   status = false;
    for ( uint32 i = 0; i < TLE493D_MAX_RETRANSMIT; i++ )
    {
        if ( II2C_TxRequest( pCfg->i2cPortIndex,
                             pCfg->i2cAddress,
                             len,
                             txData ) )
        {
            status = true;
            i      = TLE493D_MAX_RETRANSMIT;
        }
        else
        {
            BusyWait( TLE493D_WAIT_RETRANSMIT_MICROSEC );
        }
    }
    return status;
}
static void ErrorStatusUpdate( tMagnetic3dSensor_InstanceData* pInstanceData, bool newStatus )
{
    const bool   oldStatus     = pInstanceData->safeErrorStatus.error;
    const uint16 newComErrCode = II2c_GetStatus( pInstanceData->portIndex );
    const bool   newComFailure = newComErrCode != 0 ? true : false;

    pInstanceData->safeErrorStatus.error        = newStatus;
    pInstanceData->safeErrorStatus.comFailure   = newComFailure;
    pInstanceData->safeErrorStatus.comErrorCode = newComErrCode;

    if ( newStatus != oldStatus )
    {
        tEvent event;
        event.id   = IMAGNETIC3DSENSOR_EVENT_ERROR_STATUS_UPDATE;
        event.data = pInstanceData->portIndex;

        for ( uint8 i = 0; i < TLE493D_MAX_NO_EVENT_CB; i++ )
        {
            tEventCallback cb = (tEventCallback)pInstanceData->safeEventCb[ i ];
            if ( cb != NULL )
            {
                cb( event );
            }
        }
    }
}

static tMagnetic3dSensorTle493d_Config const* GetConfig( tIMagnetic3dSensor_PortIndex index )
{
    return &IMagnetic3dSensorTle493dCfg_Port[ index ];
}

static uint8* GetRxData( tIMagnetic3dSensor_PortIndex index )
{
    return i2cData[ index ].rxData;
}
static uint8* GetTxData( tIMagnetic3dSensor_PortIndex index )
{
    return i2cData[ index ].txData;
}

static void StartWatch( void )
{
    // nop
}
static void StopWatch( void )
{
    // nop
}
