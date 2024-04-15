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

#ifndef __IAPPLICATION_H__
#define __IAPPLICATION_H__

/*
-------------------------------------------------------------------------------
    Interface function prototypes
-------------------------------------------------------------------------------
*/

/**
*******************************************************************************
* @brief        Initialize application
* @param        -
* @returns      -
*******************************************************************************
*/
void IApplication_Init( void );

/**
******************************************************************************
* @brief        Start application.
* @param        -
* @returns      -
******************************************************************************
*/
void IApplication_Start( void );

/**
 ******************************************************************************
 * @brief        Run application. Never returns
 * @param        -
 * @returns      -
 ******************************************************************************
 */
void IApplication_Run( void );

#endif /* __IAPPLICATION_H__ */
