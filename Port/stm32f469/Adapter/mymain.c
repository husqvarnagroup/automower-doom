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
/*
 ------------------------------------------------------------------------------
    Include files
 ------------------------------------------------------------------------------
 */

#include <stdint.h>
#include <ctype.h>
#include "w_file.h"

#include "IDraw.h"

#include "main.h"

#include "myff.h"
#include "mylcd.h"

#include "IJoystick.h"
#include "ISystemTick.h"

#define MYMAIN_TRIGGER_APPLICATION_TIMEOUT_MS ( 2000 )

/*
 ------------------------------------------------------------------------------
    Type definitions
 ------------------------------------------------------------------------------
 */

/*
 ------------------------------------------------------------------------------
    Private data
 ------------------------------------------------------------------------------
 */
static bool isInitalized = false; /**< true in case the module has been initialized */

/*---------------------------------------------------------------------*
 *  global data                                                        *
 *---------------------------------------------------------------------*/

wad_file_class_t win32_wad_file;
extern void D_DoomMain( void );

/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void IDoom_Init( void )
{
    if ( !isInitalized )
    {
        isInitalized = true;

        // adapter code initialize
        ff_init();
        lcd_init();

        IDraw_Init();
        IJoystick_Init();
    }
}
/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void IDoom_Start( void )
{
    // adapter code start
    ff_start();
    lcd_start();

    IDraw_Start();
    IJoystick_Start();
}
/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void IDoom_Run( void )
{
    D_DoomMain();
}
/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
uint32_t my_get_systime( void )
{
    return (uint32_t)ISystemTick_GetTick();
}
/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
char* strupr( char* s )
{
    char* p = s;
    while ( ( *p = toupper( *p ) ) )
        p++;
    return s;
}
/*
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void sleep_ms( uint32_t ms )
{
    const uint32 abortMs = ( my_get_systime() + ms );
    while ( my_get_systime() < abortMs )
    {
        ;
    }
}
