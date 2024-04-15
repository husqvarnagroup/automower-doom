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

#ifndef __FF_H__
#define __FF_H__

#include <stdint.h>

#define FA_READ          0x01 /* Specifies read access to the object. Data can be read from the file. Combine with FA_WRITE for read-write access. */
#define FA_OPEN_EXISTING 0x00 /* Opens the file. The function fails if the file is not existing. (Default) */

#if !_FS_READONLY
#define FA_WRITE         0x02 /* Specifies write access to the object. Data can be written to the file. Combine with FA_READ for read-write access. */
#define FA_CREATE_NEW    0x04 /* Creates a new file. The function fails with FR_EXIST if the file is existing. */
#define FA_CREATE_ALWAYS 0x08 /* Creates a new file. If the file is existing, it will be truncated and overwritten. */
#define FA_OPEN_ALWAYS   0x10 /* Opens the file if it is existing. If not, a new file is created. */
#define FA__WRITTEN      0x20
#define FA__DIRTY        0x40
#endif

typedef enum
{
    FR_OK = 0,              /* (0) Succeeded */
    FR_DISK_ERR,            /* (1) A hard error occurred in the low level disk I/O layer */
    FR_INT_ERR,             /* (2) Assertion failed */
    FR_NOT_READY,           /* (3) The physical drive cannot work */
    FR_NO_FILE,             /* (4) Could not find the file */
    FR_NO_PATH,             /* (5) Could not find the path */
    FR_INVALID_NAME,        /* (6) The path name format is invalid */
    FR_DENIED,              /* (7) Access denied due to prohibited access or directory full */
    FR_EXIST,               /* (8) Access denied due to prohibited access */
    FR_INVALID_OBJECT,      /* (9) The file/directory object is invalid */
    FR_WRITE_PROTECTED,     /* (10) The physical drive is write protected */
    FR_INVALID_DRIVE,       /* (11) The logical drive number is invalid */
    FR_NOT_ENABLED,         /* (12) The volume has no work area */
    FR_NO_FILESYSTEM,       /* (13) There is no valid FAT volume */
    FR_MKFS_ABORTED,        /* (14) The f_mkfs() aborted due to any parameter error */
    FR_TIMEOUT,             /* (15) Could not get a grant to access the volume within defined period */
    FR_LOCKED,              /* (16) The operation is rejected according to the file sharing policy */
    FR_NOT_ENOUGH_CORE,     /* (17) LFN working buffer could not be allocated */
    FR_TOO_MANY_OPEN_FILES, /* (18) Number of open files > _FS_SHARE */
    FR_INVALID_PARAMETER    /* (19) Given parameter is invalid */
} FRESULT;

typedef uint8_t  BYTE;
typedef char     TCHAR;
typedef uint32_t UINT;
typedef uint32_t MY_DWORD;

typedef long FIL;
typedef struct
{
    MY_DWORD fsize;
} FILINFO;

typedef struct
{
    uint32_t dummy;
} DIR;

typedef struct
{
    uint32_t dummy;
} FATFS;

FRESULT f_open( FIL* const fp, const TCHAR* const path, const BYTE mode );           // Open or create a file
FRESULT f_close( FIL* const fp );                                                    // Close an open file object
FRESULT f_read( FIL* const fp, void* const buff, const UINT btr, UINT* br );         // Read data from a file
FRESULT f_readn( FIL* const fp, void* const buff, const UINT btr, UINT* br );        // Read data from a file without size limit
FRESULT f_writen( FIL* const fp, const void* const buff, const UINT btw, UINT* bw ); // Write data to a file without size limit
FRESULT f_lseek( FIL* const fp, const MY_DWORD ofs );                                // Move file pointer of a file object
FRESULT f_mkdir( const TCHAR* const path );                                          // Create a sub directory
FRESULT f_unlink( const TCHAR* const path );                                         // Delete an existing file or directory
FRESULT f_rename( const TCHAR* const path_old, const TCHAR* const path_new );        // Rename/Move a file or directory
FRESULT f_stat( const TCHAR* const path, FILINFO* const fno );                       // Get file status

uint32_t f_tell( const FIL* const fp );
uint32_t f_size( const FIL* const fp );

void my_ff_init( void );
void my_ff_start( void );

#endif // __FF_H__
