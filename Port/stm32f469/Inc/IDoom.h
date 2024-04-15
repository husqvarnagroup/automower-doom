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
 * @file      IDoom.h
 *
 * @copyright Copyright (c) Husqvarna AB
 *
 * @brief     Doom application interface
 ******************************************************************************
 */
#ifndef IDOOM_H
#define IDOOM_H
/*
 ------------------------------------------------------------------------------
    Include files
 ------------------------------------------------------------------------------
 */
/*
 ------------------------------------------------------------------------------
    Interface functions
 ------------------------------------------------------------------------------
 */
/**
 ******************************************************************************
 * @brief   Initialize module/application
 * @param   -
 * @returns -
 ******************************************************************************
 */
void IDoom_Init( void );
/**
 ******************************************************************************
 * @brief   Start module/application
 * @param   -
 * @returns -
 ******************************************************************************
 */
void IDoom_Start( void );
/**
 ******************************************************************************
 * @brief   Run module/application
 * @param   -
 * @returns -
 ******************************************************************************
 */
void IDoom_Run( void );

#endif // IDOOM_H
