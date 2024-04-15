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

#include "IApplication.h"
#include "IDraw.h"
#include "IDigitalInput.h"
#include "IDigitalOutput.h"
#include "IDigitalOutputCfg.h"
#include "IEncoder.h"
#include "ISystemTick.h"
#include "ISystemPower.h"
#include "IInterrupt.h"
#include "IMagnetic3dSensor.h"
#include "IRestart.h"
#include "II2c.h"
#include "IWatchdog.h"

#include "RoboticTypes.h"
#include "IDoom.h"

#include "stm32_hal.h"

/*
-------------------------------------------------------------------------------
   Interface functions
-------------------------------------------------------------------------------
*/

void IApplication_Init()
{
    uint16* pTmp = (uint16*)0x20000000;
    *pTmp        = 0x0000;

    IInterrupt_Init();
    ISystemTick_Init();
    IDigitalOutput_Init();
    IDraw_Init();
    IDigitalInput_Init();
    IEncoder_Init();

    II2c_Init();
    IMagnetic3dSensor_Init();
    IRestart_Init();
    ISystemPower_Init();
    IWatchdog_Init();

    IDoom_Init();
}

void IApplication_Start()
{
    IInterrupt_Start();
    ISystemTick_Start();
    IInterrupt_GlobalEnable();

    IDigitalOutput_Start();
    IDraw_Start();
    IDigitalInput_Start();
    IDigitalOutput_Set( IDIGITALOUTPUTCFG_POWERSTATE_LATCH_R, TRUE );
    IEncoder_Start();

    II2c_Start();
    IMagnetic3dSensor_Start();
    IRestart_Start();
    ISystemPower_Start();

    IWatchdog_Start();
    IWatchdog_Activate();

    IDoom_Start();

    IDigitalOutput_Set( IDIGITALOUTPUTCFG_POWERSTATE_LATCH_R, FALSE );
}

void IApplication_Run()
{
    IDoom_Run(); // Should not return
    Error_Handler();
}
