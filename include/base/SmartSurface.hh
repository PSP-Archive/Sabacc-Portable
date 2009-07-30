#ifndef __SMF_SMART_SURFACE_HH
#define __SMF_SMART_SURFACE_HH

/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: December 9, 2008
 *
 * File: SmartSurface.hh
 *
 * A class to assist with proper allocation of surfaces,
 * in attempt to avoid double frees and the like.
 */

// Standard library headers
#include <std::map>

// SDL headers
#if defined(__APPLE__)
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

class SmartSurface {

public:

  explicit SmartSurface(SDL_Surface*);
  ~SmartSurface();

    SmartSurface(const SmartSurface&);
    SmartSurface& operator=(const SmartSurface&);

    const SDL_Surface* operator->() const;
  SDL_Surface* operator->();

    bool operator!() const;

    bool operator==(const SmartSurface&);
    bool operator!=(const SmartSurface&);

    SDL_Surface* get() const;

protected:
  SDL_Surface* my_ptr;
  static std::std::map<SDL_Surface*, int> reference_map;

  void releaseSurface();
  void initializeSurface(SDL_Surface*);

};	// class SmartSurface

#endif // __SIR_SMART_SURFACE_HH
