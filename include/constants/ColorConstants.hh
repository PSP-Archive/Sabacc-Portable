#ifndef __SMF_COLORCONSTANTS_HH
#define __SMF_COLORCONSTANTS_HH

/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: August 30, 2008
 *
 * File: ColorConstants.hh
 *
 * Font infomation used for managing font states.
 */

// SDL headers
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

// Foreground and background
static const SDL_Color  default_forecolor = { 0xFF, 0xFF, 0xFF, 0xFF };
static const SDL_Color  default_backcolor = { 0x00, 0x00, 0x00, 0xFF };

// Frame colors
static const SDL_Color	default_frame_titlecolor = { 0xFF, 0xCC, 0xCC, 0xFF };
static const SDL_Color  default_frame_forecolor = { 0x80, 0x80, 0x80, 0xFF };
static const SDL_Color  default_frame_backcolor = { 0x20, 0x20, 0x40, 0xFF };
static const SDL_Color  default_frame_bordercolor = { 0x80, 0x80, 0x80, 0xA0 };

// Menu colors
static const SDL_Color  default_menu_forecolor = { 0xFF, 0xFF, 0x00, 0xFF };
static const SDL_Color  default_menu_backcolor = { 0x00, 0x00, 0x00, 0x88 };

// Button colors
static const SDL_Color  default_button_forecolor = { 0xFF, 0xFF, 0xFF, 0xFF };
static const SDL_Color  default_button_backcolor = { 0x44, 0x44, 0x66, 0x00 };
static const SDL_Color  default_button_bordercolor = { 0xAA, 0xAA, 0xCC, 0x00 };

// Text entry colors
static const SDL_Color  default_entry_forecolor = { 0x00, 0x00, 0x44, 0xFF };
static const SDL_Color  default_entry_backcolor = { 0xBB, 0xBB, 0xBB, 0xFF };

#endif // __SMF_COLORCONSTANTS_HH
