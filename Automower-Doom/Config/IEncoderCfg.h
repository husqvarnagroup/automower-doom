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

#ifndef IENCODERCFH_H
#define IENCODERCFH_H

/*
 -------------------------
 Include files
 -------------------------
 */
#include "EncoderCfgDefs.h"
#include <stm32_hal.h>

/*
 -------------------------
 Type definitions
 -------------------------
 */

/*
 -------------------------
 Defines
 -------------------------
 */

static const tEncoderCfg IEncoderCfg =
    {
        .TimHandle   = &htim1,
        .TimInit     = MX_TIM1_Init,
        .TimChannels = TIM_CHANNEL_ALL };

#endif /* IENCODERCFH_H */
