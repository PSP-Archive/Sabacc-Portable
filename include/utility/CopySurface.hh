#ifndef __SMF_COPYSURFACE_HH
#define __SMF_COPYSURFACE_HH

/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 28, 2008
 *
 * File: CopySurface.hh
 *
 * Functions for copying pixel data from one surface to another
 */

// SDL headers
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

void copySurface(SDL_Surface*, SDL_Surface*);
void copySurface(SDL_Surface*, SDL_Surface*, int, int);
void copySurface(SDL_Surface*, int, int, SDL_Surface*, int, int);

#endif // __SMF_COPYSURFACE_HH
