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

#include <string.h>
#include <RoboticTypes.h>
#include "ff.h"

/*
 ------------------------------------------------------------------------------
    Defines
 ------------------------------------------------------------------------------
 */
#define WAD_SIZE          ( 4196020 )
#define WAD_START_ADDRESS ( (char*)0xC0B00000 )
#define WAD_CRC           ( 377702509 )
/*
 ------------------------------------------------------------------------------
    Types
 ------------------------------------------------------------------------------
 */
// the RAM control block used by this adapter
typedef struct
{
    bool   inited;               // true if the adapter has been initialized
    bool   started;              // true if the adapter has been started
    bool   wadExists;            // true if the WAD file is valid in the RAM
    bool   wadOpen;              // true if the WAD file has been opened (and not closed)
    size_t wadCurrentReadOffset; // the current read offset in the WAD file
} tMyFfVars;

/*
 ------------------------------------------------------------------------------
    Private data
 ------------------------------------------------------------------------------
 */
static tMyFfVars myFfVars = { 0 };

// the only supported WAD file path we support, i.e. DOOM will think that this file exists and can be opened/read
static const char wadFPath[] = "0:/doom/doom1.wad";
// the fake file handle when opening the WAD file
static const uint32 wadFileHandle = 0x12345678;

/**
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void ff_init( void )
{
    if ( myFfVars.inited )
    {
        return;
    }
    myFfVars.inited = true;
}

/**
 ******************************************************************************
 * Function
 ******************************************************************************
 */
void ff_start( void )
{
    if ( myFfVars.started )
    {
        return;
    }
    myFfVars.started = true;

    // validate the WAD data with a simple checksum calculation
    uint8* pDataCrc = (uint8*)WAD_START_ADDRESS;

    uint32 crc = 0;
    while ( (char*)pDataCrc < ( WAD_START_ADDRESS + WAD_SIZE ) )
    {
        crc += ( *pDataCrc );
        ++pDataCrc;
    }
    // if the checksum is correct for the data, it is deemed correct
    if ( WAD_CRC != crc )
    {
        myFfVars.wadExists = false;
    }
    else
    {
        myFfVars.wadExists = true;
    }
}

/**
 ******************************************************************************
 * Function
 ******************************************************************************
 */
FRESULT f_open( FIL* const fp, const TCHAR* const path, const BYTE mode )
{
    if ( NULL == fp )
    {
        return FR_INVALID_OBJECT;
    }
    if ( 0 != strncmp( wadFPath, path, sizeof( wadFPath ) ) )
    {
        return FR_DENIED;
    }
    if ( !myFfVars.wadExists )
    {
        return FR_DENIED;
    }
    if ( myFfVars.wadOpen )
    {
        return FR_DENIED;
    }
    *fp                           = wadFileHandle;
    myFfVars.wadOpen              = true;
    myFfVars.wadCurrentReadOffset = 0;
    return FR_OK;
}

/**
 ******************************************************************************
 * Function
 ******************************************************************************
 */
FRESULT f_close( FIL* const fp )
{
    if ( NULL == fp )
    {
        return FR_INVALID_OBJECT;
    }
    if ( wadFileHandle != *fp )
    {
        return FR_DENIED;
    }
    myFfVars.wadOpen = false;
    return FR_OK;
}

/**
 ******************************************************************************
 * Function
 ******************************************************************************
 */
FRESULT f_read( FIL* const fp, void* const buff, const UINT btr, UINT* br )
{
    if ( NULL == fp )
    {
        return FR_INVALID_OBJECT;
    }
    if ( wadFileHandle != *fp )
    {
        return FR_DENIED;
    }
    if ( !myFfVars.wadOpen )
    {
        return FR_DENIED;
    }
    size_t bytesToRead = ( WAD_SIZE - myFfVars.wadCurrentReadOffset );
    if ( btr < bytesToRead )
    {
        bytesToRead = btr;
    }
    if ( 0 == bytesToRead )
    {
        return FR_DENIED;
    }
    memcpy( buff, WAD_START_ADDRESS + myFfVars.wadCurrentReadOffset, bytesToRead );
    *br = bytesToRead;

    myFfVars.wadCurrentReadOffset = bytesToRead;
    if ( WAD_SIZE < myFfVars.wadCurrentReadOffset )
    {
        myFfVars.wadCurrentReadOffset = WAD_SIZE;
    }
    return FR_OK;
}

/**
 ******************************************************************************
 * Function
 ******************************************************************************
 */
FRESULT f_readn( FIL* const fp, void* const buff, const UINT btr, UINT* br )
{
    return f_read( fp, buff, btr, br );
}

/**
 ******************************************************************************
 * Function
 ******************************************************************************
 */
FRESULT f_writen( FIL* const fp, const void* const buff, const UINT btw, UINT* const bw )
{
    return FR_DENIED;
}

/**
 ******************************************************************************
 * Function
 ******************************************************************************
 */
FRESULT f_lseek( FIL* const fp, const MY_DWORD ofs )
{
    if ( NULL == fp )
    {
        return FR_INVALID_OBJECT;
    }
    if ( wadFileHandle != *fp )
    {
        return FR_DENIED;
    }
    if ( !myFfVars.wadOpen )
    {
        return FR_DENIED;
    }
    size_t newOffset = ofs;
    if ( WAD_SIZE < newOffset )
    {
        newOffset = WAD_SIZE;
    }
    myFfVars.wadCurrentReadOffset = newOffset;
    return FR_OK;
}

/**
 ******************************************************************************
 * Function
 ******************************************************************************
 */
FRESULT f_mkdir( const TCHAR* const path )
{
    return FR_OK;
}

/**
 ******************************************************************************
 * Function
 ******************************************************************************
 */
FRESULT f_unlink( const TCHAR* const path )
{
    return FR_DENIED;
}

/**
 ******************************************************************************
 * Function
 ******************************************************************************
 */
FRESULT f_rename( const TCHAR* const path_old, const TCHAR* const path_new )
{
    return FR_DENIED;
}

/**
 ******************************************************************************
 * Function
 ******************************************************************************
 */
FRESULT f_stat( const TCHAR* const path, FILINFO* const fno )
{
    // Only really used for file existence check!

    if ( 0 != strncmp( wadFPath, path, sizeof( wadFPath ) ) )
    {
        return FR_DENIED;
    }
    if ( !myFfVars.wadExists )
    {
        return FR_DENIED;
    }
    fno->fsize = WAD_SIZE;
    return FR_OK;
}

/**
 ******************************************************************************
 * Function
 ******************************************************************************
 */
uint32_t f_tell( const FIL* const fp )
{
    return 0;
}

/**
 ******************************************************************************
 * Function
 ******************************************************************************
 */
uint32_t f_size( const FIL* const fp )
{
    if ( wadFileHandle != *fp )
    {
        return 0;
    }
    if ( !myFfVars.wadExists )
    {
        return 0;
    }
    if ( !myFfVars.wadOpen )
    {
        return 0;
    }
    return WAD_SIZE;
}
