#ifndef __SMF_PIXELFUNCTIONS_HH
#define __SMF_PIXELFUNCTIONS_HH

/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: September 17, 2008
 *
 * File: PixelFunctions.hh
 *
 * Functions for setting and retrieving pixel data. Taken directly from the
 * SDL documentation
 */

// Simple Directmedia Library headers
#if defined(__APPLE__)
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

Uint32 getPixel(SDL_Surface*, int, int);
void putPixel(SDL_Surface*, int, int, Uint32);

#endif // __SMF_PIXELFUNCTIONS_HH
