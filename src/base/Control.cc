/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: September 7, 2008
 *
 * File: Control.hh
 *
 * The control base. This class will be responsible for handling events.
 */

// Standard Library headers
#include <string>
using std::string;

// Simple Directmedia Library headers
#if defined(__APPLE__)
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

// SMF headers
#include "base/Control.hh"
#include "base/SurfaceWidget.hh"
#include "geometry/Rect.hh"

// SMF Debugging headers
#if defined(_DEBUG) || defined(_DEBUGCONTROLBASE)
#include <cstdlib>
#include "utility/Log.hh"
#endif

Control::Control(const Rect& position, string property_string ) :
    SurfaceWidget(position, property_string) { }
Control::Control(Widget* guardian, const Rect& position, string property_string) :
    SurfaceWidget(guardian, position, property_string) { }
Control::Control(const Control& src) : SurfaceWidget(src) { }
Control::~Control() { }

Control& Control::operator=(const Control& src)
{

  if (&src != this)
    {
      SurfaceWidget::operator=(src);
    }

  return(*this);

}	// operator=

bool Control::onKeyDown(const SDL_KeyboardEvent&)
{
  return(false);
}
bool Control::onKeyUp(const SDL_KeyboardEvent&)
{
  return(false);
}

bool Control::onJoystickButtonDown(const SDL_JoyButtonEvent&)
{
  return(false);
}
bool Control::onJoystickButtonUp(const SDL_JoyButtonEvent&)
{
  return(false);
}
bool Control::onJoystickMove(const SDL_JoyAxisEvent&)
{
  return(false);
}
bool Control::onJoystickHat(const SDL_JoyHatEvent&)
{
  return(false);
}
bool Control::onJoystickBall(const SDL_JoyBallEvent&)
{
  return(false);
}

bool Control::onMouseMove(const SDL_MouseMotionEvent&)
{
  return(false);
}
bool Control::onMouseButtonDown(const SDL_MouseButtonEvent&)
{
  return(false);
}
bool Control::onMouseButtonUp(const SDL_MouseButtonEvent&)
{
  return(false);
}

void Control::draw()
{

#if defined(_DEBUG) || defined(_DEBUGCONTROLBASE)
  logAppend("Control doesn't draw, passing off to SurfaceWidget.");
#endif

  SurfaceWidget::draw();

}	// draw
