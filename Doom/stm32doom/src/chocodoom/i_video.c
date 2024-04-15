// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
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
// $Log:$
//
// DESCRIPTION:
//	DOOM graphics stuff for X11, UNIX.
//
//-----------------------------------------------------------------------------



static const char
rcsid[] = "$Id: i_x.c,v 1.6 1997/02/03 22:45:10 b1 Exp $";

#include "config.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_event.h"
#include "d_main.h"
#include "i_video.h"
#include "z_zone.h"

#include "tables.h"
#include "doomkeys.h"

#include <stdint.h>
#include <stdbool.h>
#include "mylcd.h"

#include "IJoystick.h"


extern boolean menuactive; // Menu overlayed?
extern boolean messageNeedsInput; // true when user is prompted for selecting yes/no
extern int key_menu_confirm; // Key pressed by user to select yes
extern int key_menu_abort;  // Key pressed by user to select no
extern boolean M_IsStartControlPanel (void); // returns true if the start menu is active

// The screen buffer; this is modified to draw things to the screen

byte *I_VideoBuffer = NULL;

// If true, game is running as a screensaver

boolean screensaver_mode = false;

// Flag indicating whether the screen is currently visible:
// when the screen isnt visible, don't render the screen

boolean screenvisible;

// Mouse acceleration
//
// This emulates some of the behavior of DOS mouse drivers by increasing
// the speed when the mouse is moved fast.
//
// The mouse input values are input directly to the game, but when
// the values exceed the value of mouse_threshold, they are multiplied
// by mouse_acceleration to increase the speed.

float mouse_acceleration = 2.0;
int mouse_threshold = 10;

// Gamma correction level to use

int usegamma = 0;

int usemouse = 0;

// If true, keyboard mapping is ignored, like in Vanilla Doom.
// The sensible thing to do is to disable this if you have a non-US
// keyboard.

int vanilla_keyboard_mapping = true;


typedef struct
{
	byte r;
	byte g;
	byte b;
} col_t;

// Palette converted to RGB565

static uint16_t rgb565_palette[256];



void I_InitGraphics (void)
{
	I_VideoBuffer = (byte*)Z_Malloc (SCREENWIDTH * SCREENHEIGHT, PU_STATIC, NULL);

	screenvisible = true;
}

void I_ShutdownGraphics (void)
{
	Z_Free (I_VideoBuffer);
}

void I_StartFrame (void)
{

}

void I_GetEvent (void)
{
    event_t event = { .data1 = -1, .data2 = -1, .data3 = -1, .data4 = -1, };

    static boolean last_jog_wheel_button_pressed = false;
    static boolean last_start_button_pressed = false;
    static boolean last_stop_button_pressed = false;
    static boolean last_escape_button_pressed = false;
    static boolean last_left = false;
    static boolean last_right = false;

    static int jog_wheel_button_pressed_ms = 0;

    const boolean jog_wheel_button_pressed = IJoystick_GetFire();
    const boolean start_button_pressed = IJoystick_GetUp();
    const boolean stop_button_pressed = IJoystick_GetDown();
    const boolean jog_wheel_left = IJoystick_GetLeft();
    const boolean jog_wheel_right = IJoystick_GetRight();

    // jog-wheel push button
    // =====================
    // - in menu; ENTER
    // - in game; FIRE
    // for simplicity we provide key down/up for both as this seems to work fine
    if ( last_jog_wheel_button_pressed != jog_wheel_button_pressed )
    {
        last_jog_wheel_button_pressed = jog_wheel_button_pressed;
        event.type = last_jog_wheel_button_pressed ? ev_keydown : ev_keyup;

        event.data1 = KEY_ENTER;
        D_PostEvent (&event);

        event.data1 = KEY_FIRE;
        D_PostEvent (&event);

    }
    // - after 5 seconds long press; ESCAPE
    boolean escape_button_pressed = false;
    if ( jog_wheel_button_pressed )
    {
        if ( 0 == jog_wheel_button_pressed_ms )
        {
            jog_wheel_button_pressed_ms = I_GetTimeMS();
        }
        if ( 5000 < ( I_GetTimeMS() - jog_wheel_button_pressed_ms ) )
        {
            escape_button_pressed = true;
        }
    }
    else
    {
        jog_wheel_button_pressed_ms = 0;
    }
    if ( last_escape_button_pressed != escape_button_pressed )
    {
        last_escape_button_pressed = escape_button_pressed;
        event.type = last_escape_button_pressed ? ev_keydown : ev_keyup;
        event.data1 = KEY_ESCAPE;
        D_PostEvent( &event );
    }

    // start button (a.k.a. UP)
    // ========================
    // - in menu, when user is prompted for selecting yes/no; CONFIRM
    // - in menu; ENTER
    // - in game; KEY_UPARROW
    if ( last_start_button_pressed != start_button_pressed )
    {
        last_start_button_pressed = start_button_pressed;
        event.type  = last_start_button_pressed ? ev_keydown : ev_keyup;
        event.data1 = messageNeedsInput ? key_menu_confirm : ( menuactive ? KEY_ENTER : KEY_UPARROW );
        D_PostEvent (&event);
    }
    // stop button (a.k.a. DOWN)
    // =========================
    // - in menu, when user is prompted for selecting yes/no; ABORT
    // - in menu; KEY_BACKSPACE
    // - in game; USE and ALT (ALT is handled in-game to enable STRAFE)
    if ( last_stop_button_pressed != stop_button_pressed )
    {
        last_stop_button_pressed = stop_button_pressed;
        event.type  = last_stop_button_pressed ? ev_keydown : ev_keyup;
        if ( messageNeedsInput )
        {
            event.data1 = key_menu_abort;
            D_PostEvent (&event);
        }
        else if ( menuactive )
        {
            // in the top menu we want the STOP to be ESC and not BACKSPACE...
            if ( M_IsStartControlPanel() )
            {
                event.data1 = KEY_ESCAPE;
            }
            else
            {
                event.data1 = KEY_BACKSPACE;
            }
            D_PostEvent (&event);
        }
        else
        {
            // note - both USE and ALT on STOP button!
            event.data1 = KEY_USE;
            D_PostEvent (&event);

            event.data1 = KEY_LALT;
            D_PostEvent (&event);
        }
    }
    // jog-wheel left/right
    // ====================
    // - in menu; UP/DOWN
    // - in game; LEFT/RIGHT
    if ( last_left != jog_wheel_left )
    {
        last_left = jog_wheel_left;
        event.type  = last_left ? ev_keydown : ev_keyup;
        event.data1 = menuactive ? KEY_UPARROW : KEY_LEFTARROW;
        D_PostEvent (&event);
    }
    if ( last_right != jog_wheel_right )
    {
        last_right = jog_wheel_right;
        event.type  = last_right ? ev_keydown : ev_keyup;
        event.data1 = menuactive ? KEY_DOWNARROW : KEY_RIGHTARROW;
        D_PostEvent (&event);
    }
}

void I_StartTic (void)
{
	I_GetEvent();
}

void I_UpdateNoBlit (void)
{
}

void I_FinishUpdate (void)
{
	int x, y;
	byte index;
	uint16_t* pLcdFrameBuffer = ( uint16_t* )lcd_get_frame_buffer();

	for (y = 0; y < SCREENHEIGHT; y++)
	{
		for (x = 0; x < SCREENWIDTH; x++)
		{
			index = I_VideoBuffer[y * SCREENWIDTH + x];

			pLcdFrameBuffer[x * LCD_MAX_X + (LCD_MAX_X - y - 1)] = rgb565_palette[index];
		}
	}

	lcd_refresh ();
}

//
// I_ReadScreen
//
void I_ReadScreen (byte* scr)
{
    memcpy (scr, I_VideoBuffer, SCREENWIDTH * SCREENHEIGHT);
}

//
// I_SetPalette
//
void I_SetPalette (byte* palette)
{
	int i;
	col_t* c;

	for (i = 0; i < 256; i++)
	{
		c = (col_t*)palette;

		rgb565_palette[i] = LCD_RGB565(gammatable[usegamma][c->r],
									   gammatable[usegamma][c->g],
									   gammatable[usegamma][c->b]);

		palette += 3;
	}
}

// Given an RGB value, find the closest matching palette index.

int I_GetPaletteIndex (int r, int g, int b)
{
    int best, best_diff, diff;
    int i;
    col_t color;

    best = 0;
    best_diff = INT_MAX;

    for (i = 0; i < 256; ++i)
    {
    	color.r = LCD_RGB565_R(rgb565_palette[i]);
    	color.g = LCD_RGB565_G(rgb565_palette[i]);
    	color.b = LCD_RGB565_B(rgb565_palette[i]);

        diff = (r - color.r) * (r - color.r)
             + (g - color.g) * (g - color.g)
             + (b - color.b) * (b - color.b);

        if (diff < best_diff)
        {
            best = i;
            best_diff = diff;
        }

        if (diff == 0)
        {
            break;
        }
    }

    return best;
}

void I_BeginRead (void)
{
}

void I_EndRead (void)
{
}

void I_SetWindowTitle (char *title)
{
}

void I_GraphicsCheckCommandLine (void)
{
}

void I_SetGrabMouseCallback (grabmouse_callback_t func)
{
}

void I_EnableLoadingDisk (void)
{
}

void I_BindVideoVariables (void)
{
}

void I_DisplayFPSDots (boolean dots_on)
{
}

void I_CheckIsScreensaver (void)
{
}
