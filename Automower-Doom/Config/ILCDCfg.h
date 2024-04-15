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

#ifndef ILCDCFG_H
#define ILCDCFG_H

/*
-------------------------
   Include files
-------------------------
*/
#include "LCD_ST7789VICfgDef.h"
#include "ISpiCfg.h"
#include "IDigitalOutputCfg.h"
/*
-------------------------
   Defines
-------------------------
*/

/*
-------------------------
   Type definitions
-------------------------
*/

tLcdBacklightCfg lcdBacklightConfig  = { GPIOB, GPIO_PIN_12 };
tLcdST7789VICfg  lcdControllerConfig = { ISPICFG_DEVICE_SPI_LCD, IDIGITALOUTPUTCFG_LCD_RESET };

#endif /* ILCDCFG_H */
