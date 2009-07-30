/*
 * Sabacc Media Framework
 *
 * Author: goumba
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

// SMF headers
#include "utility/PixelFunctions.hh"

// SMF debug
#if defined(_DEBUG) || defined(_DEBUGPIXELFUNCTIONS)
#include <cstdio>
#include "utility/Log.hh"
#endif

Uint32 getPixel(SDL_Surface* surface, int x, int y) {
    int Bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *pixel = static_cast<Uint8*>(surface->pixels) + (y * surface->pitch) + (x * Bpp);

    switch(Bpp) {
		case 1:
			return(*pixel);
			break;
		case 2:
			return(static_cast<Uint16>(*pixel));
			break;
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return(pixel[0] << 16 | pixel[1] << 8 | pixel[2]);
			else
				return(pixel[0] | pixel[1] << 8 | pixel[2] << 16);
			break;
		case 4:
			return(static_cast<Uint32>(*pixel));
			break;
		default:
			return(0);       /* shouldn't happen, but avoids warnings */
			break;
    }	// switch(Bpp)
}	// getPixel

/*
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */
void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel_color) {

#if defined(_DEBUG) || defined(_DEBUGPIXELFUNCTIONS)
  char pxl_debug[128];
  sprintf(pxl_debug, "Setting pixel %d,%d to %ul in surface %dwx%dh", x, y, pixel_color, surface->w, surface->h);
  logAppend(pxl_debug);
#endif

  int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel_color;
        break;

    case 2:
        *(Uint16 *)p = pixel_color;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel_color >> 16) & 0xff;
            p[1] = (pixel_color >> 8) & 0xff;
            p[2] = pixel_color & 0xff;
        } else {
            p[0] = pixel_color & 0xff;
            p[1] = (pixel_color >> 8) & 0xff;
            p[2] = (pixel_color >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel_color;
        break;
    }

}// putPixel_Color
