#ifndef _SMF_PSPCONTROLCONSTANTS_HH
#define _SMF_PSPCONTROLCONSTANTS_HH

/**
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: October 5, 2008
 *
 * File: PSPControlConstants.hh
 *
 * Constants defining the PSP buttons (all but analog control).
 */

// Directional pad, clockwise
static const int psp_dpad_up = 8;
static const int psp_dpad_right = 9;
static const int psp_dpad_down = 6;
static const int psp_dpad_left = 7; 

// Action buttons, clockwise
static const int psp_triangle = 0;
static const int psp_circle = 1;
static const int psp_cross = 2;
static const int psp_square = 3;

// Shoulder (Trigger) buttons
static const int psp_trigger_left = 4;
static const int psp_trigger_right = 5;

// Lower buttons (along button of the screen)
static const int psp_home = 12;
static const int psp_select = 10;
static const int psp_start = 11;

// Why does SDL_psp define this and not other lower buttons or switches?
static const int psp_switch_hold = 13;

#endif// _SMF_PSPCONTROLCONSTANTS_HH
