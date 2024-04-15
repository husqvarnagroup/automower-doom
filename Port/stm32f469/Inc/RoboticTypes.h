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

#ifndef RoboticTypes_H
#define RoboticTypes_H

#include <stdint.h>

/* Max value for sint16 = 32767 */
#define MAX_sint16 (sint16)0x7fff
/* Max value for sint32 = 2147483647 */
#define MAX_sint32 (sint32)0x7fffffff
/* Max value for sint8 = 127 */
#define MAX_sint8 (sint8)0x7f
/* Max value for sint64 = 9,223,372,036,854,775,807 */
#define MAX_sint64 (sint64)0x7FFFFFFFFFFFFFFF
/* Max value for uint16 = 65535 */
#define MAX_uint16 (uint16)0xffff
/* Max value for uint32 = 4294967295  */
#define MAX_uint32 (uint32)0xffffffff
/* Max value for uint8 = 255  */
#define MAX_uint8 (uint8)0xff
/* Min value for sint16 = -32767  */
#define MIN_sint16 (sint16)0x8000
/* Min value for sint32 = -2147483647  */
#define MIN_sint32 (sint32)0x80000000
/* Min value for sint8 = -127  */
#define MIN_sint8 (sint8)0x80
/* Min value for uint16 = 0   */
#define MIN_uint16 (uint16)0x0000
/* Min value for uint32 = 0  */
#define MIN_uint32 (uint32)0x00000000
/* Min value for uint8 = 0  */
#define MIN_uint8 (uint8)0x00
/* Minimal guaranteed precision for double */
#define PRECISION_double (double)1e-15
/* Minimal guaranteed precision for float */
#define PRECISION_float (float)1e-6f
#ifndef _sint16
#define _sint16
typedef int16_t sint16;
#endif
#ifndef _sint32
#define _sint32
typedef int32_t sint32;
#endif
#ifndef _sint64
#define _sint64
typedef int64_t sint64;
#endif
#ifndef _sint8
#define _sint8
typedef int8_t sint8;
#endif
#ifndef _uint16
#define _uint16
typedef uint16_t uint16;
#endif
#ifndef _uint32
#define _uint32
typedef uint32_t uint32;
#endif
#ifndef _uint64
#define _uint64
typedef uint64_t uint64;
#endif
#ifndef _uint8
#define _uint8
typedef unsigned char uint8;
#endif
/* All need to use stdbool.h to get the same definition of bool. Since VS2010 doesn't support bool handle it separately */
#if defined(_MSC_VER) && _MSC_VER == 1600 /* VS2010 */
#define BOOLDEF
typedef unsigned char bool;
#else
#include <stdbool.h>
#endif

#ifndef FALSE
#define FALSE 0
#endif
#ifndef false
#define false 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef true
#define true 1
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#if defined(__GNUC__) || defined(__clang__) || defined(__TI_COMPILER_VERSION__)
/* For GCC, clang and TI compilers */
#ifndef __weak
#define __weak __attribute__((weak))
#endif
#elif defined(__ICCARM__) || defined(__CC_ARM)
/* For IAR ARM and Keil MDK-ARM compilers
 * __weak is already defined and requires no __attribute__ syntax which isn't supported */
#else
/* Unknown compiler */
#ifndef __weak
#define __weak "__weak keyword is not supported by the compiler"
#endif
#endif

#ifndef tEvent
typedef struct
{
    union {
        uint32 id; // Note: Little Endian  // 0x12345678
        struct
        {
            uint16 moduleSpecificEventId; // 0x    5678
            uint16 moduleId;              // 0x1234
        };
    };
    union {
        uint32 data;
        void *pData;
    };
} tEvent;
typedef void (*tEventCallback)(tEvent event);
#endif

#ifndef RETURN_OK
#define RETURN_OK OK
#endif

#ifndef _tUnixTime
#define _tUnixTime
typedef uint32 tUnixTime;
#endif

#ifndef PI
#define PI (3.14159265358979323846)
#endif

#ifndef PI_F
#define PI_F (3.14159265f)
#endif

#ifndef RAD_TO_DEG_10_F
#define RAD_TO_DEG_10_F (572.957795f) // 180 * 10 / Pi
#endif

#ifndef DEG_10_TO_RAD_F
#define DEG_10_TO_RAD_F (0.00174532925f) // Pi / ( 180 * 10 )
#endif

#endif
