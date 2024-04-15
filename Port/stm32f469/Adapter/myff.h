/**
 ******************************************************************************
 * Copyright (c) 2023 Husqvarna AB.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#ifndef __MY_FF_H__
#define __MY_FF_H__

/*
 ------------------------------------------------------------------------------
    Interface functions
 ------------------------------------------------------------------------------
 */
/**
 ******************************************************************************
 * @brief   Initializes file system adapter module
 ******************************************************************************
 */
extern void ff_init( void );

/**
 ******************************************************************************
 * @brief   Starts file system adapter module
 ******************************************************************************
 */
extern void ff_start( void );

#endif // __MY_FF_H__
