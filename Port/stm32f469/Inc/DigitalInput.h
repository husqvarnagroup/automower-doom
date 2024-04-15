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
#ifndef DigitalInput_H
#define DigitalInput_H

#include <IDigitalInputCfg.h>
#include <RoboticTypes.h>
#include <IDigitalInput.h>

#define DIGITALINPUT_NUMBER_OF_INPUTS (sizeof(IDigitalInputCfg_Input) / sizeof(tDigitalInput_Input))
#define DIGITALINPUT_LOG_SIZE (5)

uint32 DigitalInput_Execute(void);
bool DigitalInput_GetJogWheelDown(void);
bool DigitalInput_GetStartButtonDown( void );

#endif
