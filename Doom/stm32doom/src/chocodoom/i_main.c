//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
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
// DESCRIPTION:
//	Main program, simply calls D_DoomMain high level loop.
//

#include "config.h"

#include <stdio.h>

#ifdef ORIGCODE
#include "SDL.h"
#endif

#include "doomtype.h"
#include "i_system.h"
#include "m_argv.h"

#define ARM_EMBEDDED 1

//
// D_DoomMain()
// Not a globally visible function, just included for source reference,
// calls all startup code, parses command line options.
//

void D_DoomMain (void);

#if defined(_WIN32_WCE)

// Windows CE?  I doubt it even supports SMP..

static void LockCPUAffinity(void)
{
}

#elif defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef BOOL (WINAPI *SetAffinityFunc)(HANDLE hProcess, DWORD mask);

// This is a bit more complicated than it really needs to be.  We really
// just need to call the SetProcessAffinityMask function, but that
// function doesn't exist on systems before Windows 2000.  Instead,
// dynamically look up the function and call the pointer to it.  This
// way, the program will run on older versions of Windows (Win9x, etc.)

static void LockCPUAffinity(void)
{
    HMODULE kernel32_dll;
    SetAffinityFunc SetAffinity;

    // Find the kernel interface DLL.

    kernel32_dll = LoadLibrary("kernel32.dll");

    if (kernel32_dll == NULL)
    {
        // This should never happen...

//        fprintf(stderr, "Failed to load kernel32.dll\n");
        return;
    }
    // Find the SetProcessAffinityMask function.

    SetAffinity = (SetAffinityFunc)GetProcAddress(kernel32_dll, "SetProcessAffinityMask");

    // If the function was not found, we are on an old (Win9x) system
    // that doesn't have this function.  That's no problem, because
    // those systems don't support SMP anyway.

    if (SetAffinity != NULL)
    {
        if (!SetAffinity(GetCurrentProcess(), 1))
        {
//            fprintf(stderr, "Failed to set process affinity (%d)\n",
//                            (int) GetLastError());
        }
    }
}

#elif defined(HAVE_SCHED_SETAFFINITY)

#include <unistd.h>
#include <sched.h>

// Unix (Linux) version:

static void LockCPUAffinity(void)
{
#ifdef CPU_SET
    cpu_set_t set;

    CPU_ZERO(&set);
    CPU_SET(0, &set);

    sched_setaffinity(getpid(), sizeof(set), &set);
#else
    unsigned long mask = 1;
    sched_setaffinity(getpid(), sizeof(mask), &mask);
#endif
}

#elif defined(ARM_EMBEDDED)

#else

#warning No known way to set processor affinity on this platform.
#warning You may experience crashes due to SDL_mixer.

static void LockCPUAffinity(void)
{
    fprintf(stderr, 
    "WARNING: No known way to set processor affinity on this platform.\n"
    "         You may experience crashes due to SDL_mixer.\n");
}

#endif

#ifdef ORIGCODE
int main(int argc, char **argv)
{
    // save arguments

    myargc = argc;
    myargv = argv;

    // Only schedule on a single core, if we have multiple
    // cores.  This is to work around a bug in SDL_mixer.

    LockCPUAffinity();

    M_FindResponseFile();

    // start doom

    D_DoomMain ();

    return 0;
}
#endif
