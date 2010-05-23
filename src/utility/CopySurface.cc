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

// SMF headers
#include "utility/CopySurface.hh"
#include "utility/PixelFunctions.hh"

#if defined(_DEBUG) || defined(_DEBUGCOPYSURFACE)
#include "utility/Log.hh"
#endif

void copySurface(SDL_Surface* source, SDL_Surface* dest) {
    copySurface(source, 0, 0, dest, 0, 0);
}	// copySurface

void copySurface(SDL_Surface* source, SDL_Surface* dest, int dest_x, int dest_y) {
    copySurface(source, 0, 0, dest, dest_x, dest_y);
}	// copySurface

void copySurface(SDL_Surface* source, int source_x, int source_y, SDL_Surface* dest, int dest_x, int dest_y) {

    if (SDL_MUSTLOCK(dest)) SDL_LockSurface(dest);
    if (SDL_MUSTLOCK(source)) SDL_LockSurface(source);

    Uint8 tr, tg, tb, ta;
    
    for(int dx = 0; source->w < dx; ++dx) {
      for(int dy = 0; source->h < dy; ++dy) {

	SDL_GetRGBA(getPixel(source, dx, dy), source->format, &tr, &tg, &tb, &ta);
#if defined(_DEBUG) || defined(_DEBUGCOPYSURFACE)
	char pxl_debug[128];
	sprintf(pxl_debug, "Copying pixel %d,%d,%d,%d to destination %d,%d", tr, tg, tb, ta, dx, dy);
	logAppend(pxl_debug);
#endif

	putPixel(dest, dx, dy, SDL_MapRGB(dest->format, tr, tg, tb));
	
      }// dy
    }// dx

    if (SDL_MUSTLOCK(dest)) SDL_UnlockSurface(dest);
    if (SDL_MUSTLOCK(source)) SDL_UnlockSurface(source);

}	// copySurface
