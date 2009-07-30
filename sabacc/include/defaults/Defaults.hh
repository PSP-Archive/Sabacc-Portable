#ifndef __GAME_DEFAULTS_HH
#define __GAME_DEFAULTS_HH

// Simple Directmedia Layer headers
#if defined(__APPLE__)
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

// Sabacc Media Framework headers
#include "constants/ColorConstants.hh"

// Define some constants necessary for the game

// Card color constants
static const SDL_Color default_card_value_color = { 0x90, 0x90, 0xb0, 0xa0 };
static const SDL_Color default_card_value_stroke_color = { 0xff, 0xff, 0x40, 0xa0 };
static const SDL_Color default_card_suite_color = default_forecolor;
static const SDL_Color default_card_name_color = default_forecolor;

#endif // __GAME_DEFAULTS_HH
