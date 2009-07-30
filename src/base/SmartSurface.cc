/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: December 9, 2008
 *
 * File: SmartSurface.cc
 *
 * A utility class for automatic creation and deletion
 * of heap based objects.
 */

// Standard library headers
#include <map>
using std::map;

// SDL headers
#if defined(__APPLE__)
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

// SMF headers
#include "base/SmartSurface.hh"

// SMF Debugging headers
#if defined(_DEBUG) || defined(_DEBUGSMARTSURFACE)
#include <cstdlib>
#include "utility/Log.hh"
#endif

// Redefine map
map<SDL_Surface*, int> SmartSurface::reference_map;

SmartSurface::SmartSurface(SDL_Surface* surf) : my_ptr(0) { initializeSurface(surf); }
SmartSurface::SmartSurface(const SmartSurface& other) : my_ptr(0) {
  initializeSurface(other.my_ptr);
}

SmartSurface::~SmartSurface() {
    releaseSurface();
}	// ~SmartSurface

SmartSurface& SmartSurface::operator=(const SmartSurface& other) {

    if (this != &other) {
        releaseSurface();
        initializeSurface(other.my_ptr);
    }

    return(*this);
}

const SDL_Surface* SmartSurface::operator->() const 
{
  return(my_ptr);
}
SDL_Surface* SmartSurface::operator->() {
    return(my_ptr);
}	// operator->

bool SmartSurface::operator!() const {
    if (!my_ptr) return true;

    return(0 == my_ptr);
}

bool SmartSurface::operator==(const SmartSurface& rhs) {

    return(rhs.my_ptr == this->my_ptr);

}
bool SmartSurface::operator!=(const SmartSurface& rhs) {

    return(!operator==(rhs));

}

SDL_Surface* SmartSurface::get() const {
	if(!my_ptr) return(NULL);
    return(my_ptr);
}	// get

void SmartSurface::releaseSurface()
{
  if(reference_map.end() == reference_map.find(my_ptr))
    {
#if defined(_DEBUG) || defined(_DEBUGSMARTSURFACE)
      logAppend("Missing entry in surface reference map.");
#endif
      return;
    }
  --reference_map[my_ptr];
  if(0 == reference_map[my_ptr])
    {
      reference_map.erase(my_ptr);
      SDL_FreeSurface(my_ptr);
    }
}// releaseSurface

void SmartSurface::initializeSurface(SDL_Surface* new_surface)
{
  my_ptr = new_surface;
  if(reference_map.end() == reference_map.find(my_ptr))
    {
      reference_map[my_ptr] = 1;
    }
  else
    {
      ++reference_map[my_ptr];
    }
}// initializeSurface
