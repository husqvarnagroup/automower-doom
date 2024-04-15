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
 * @file      EncoderCfgDefs.h
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Encoder module configuration definitions
 ******************************************************************************
 */
#ifndef ENCODERCFGDEFS_H
#define ENCODERCFGDEFS_H

#include "IEncoder.h"
#include "RoboticTypes.h"
#include "stm32_hal.h"

/*
 -------------------------------------------------------------------------------
 Type definitions
 -------------------------------------------------------------------------------
 */
typedef void ( *tTimInitFunction )( void );

typedef struct EncoderCfg_t
{
    TIM_HandleTypeDef* TimHandle;   /**< TIM handle for this encoder */
    tTimInitFunction   TimInit;     /**< Initialisation function for this encoder */
    uint32             TimChannels; /**< TIM_CHANNEL_ALL or an amalgamation of the channels in use, e.g. TIM_CHANNEL_1, TIM_CHANNEL_2 or TIM_CHANNEL_ALL */
} tEncoderCfg;

#endif /* ENCODERCFGDEFS_H */
