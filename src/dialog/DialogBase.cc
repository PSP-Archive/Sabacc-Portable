/**
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: September 16, 2008
 *
 * File: DialogBase.cc
 *
 * A class for displaying dialogs. See DialogBase.hh for notes.
 */

// Standard Library headers

// Simple Directmedia Library headers
#if defined(__APPLE__)
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#else
#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#endif

// SMF headers
#include "base/Control.hh"
#include "dialog/DialogBase.hh"
#include "geometry/Rect.hh"

// SMF constants
#include "constants/ColorConstants.hh"

// SMF Debugging
#if defined(_DEBUG) || defined(_DEBUGDIALOGBASE)
#include <cstdlib>
#include "utility/Log.hh"
#endif

DialogBase::DialogBase(const Rect& position, const string& property_string, const SDL_Color& fore_color, const SDL_Color& back_color, const SDL_Color& title_color, const SDL_Color& border_color) :
    Control(position, property_string), background_color(back_color), frame_border_color(border_color),
    foreground_color(fore_color), title_text_color(title_color) { }
DialogBase::DialogBase(const DialogBase& src) : Control(src), background_color(src.background_color),
    frame_border_color(src.frame_border_color), foreground_color(src.foreground_color),
    title_text_color(src.title_text_color) { }
DialogBase::~DialogBase() { }

DialogBase& DialogBase::operator=(const DialogBase& src)
{

  if (&src != this)
    {

      DialogBase::operator=(src);

      background_color = src.background_color;
      frame_border_color = src.frame_border_color;
      foreground_color = src.foreground_color;
      title_text_color = src.title_text_color;

    }	// if(not self)

  return(*this);

}	// operator=

const SDL_Color& DialogBase::getBackgroundColor()
{
  return(background_color);
}
void DialogBase::setBackgroundColor(const SDL_Color& backcolor)
{
  background_color = backcolor;
}
const SDL_Color& DialogBase::getBorderColor()
{
  return(frame_border_color);
}
void DialogBase::setBorderColor(const SDL_Color& bordercolor)
{
  frame_border_color = bordercolor;
}
const SDL_Color& DialogBase::getTextColor()
{
  return(foreground_color);
}
void DialogBase::setTextColor(const SDL_Color& forecolor)
{
  foreground_color = forecolor;
}
const SDL_Color& DialogBase::getTitleColor()
{
  return(title_text_color);
}
void DialogBase::setTitleColor(const SDL_Color& titlecolor)
{
  title_text_color = titlecolor;
}

bool DialogBase::onKeyDown(const SDL_KeyboardEvent&)
{
  return(true);
}
bool DialogBase::onKeyUp(const SDL_KeyboardEvent&)
{
  return(true);
}

bool DialogBase::onJoystickButtonDown(const SDL_JoyButtonEvent&)
{
  return(true);
}
bool DialogBase::onJoystickButtonUp(const SDL_JoyButtonEvent&)
{
  return(true);
}
bool DialogBase::onJoystickMove(const SDL_JoyAxisEvent&)
{
  return(true);
}
bool DialogBase::onJoystickHat(const SDL_JoyHatEvent&)
{
  return(true);
}
bool DialogBase::onJoystickBall(const SDL_JoyBallEvent&)
{
  return(true);
}

bool DialogBase::onMouseMove(const SDL_MouseMotionEvent&)
{
  return(true);
}
bool DialogBase::onMouseButtonDown(const SDL_MouseButtonEvent&)
{
  return(true);
}
bool DialogBase::onMouseButtonUp(const SDL_MouseButtonEvent&)
{
  return(true);
}

void DialogBase::init()
{

#if defined(_DEBUG) || defined(_DEBUGDIALOGBASE)
  logAppend("Creating dialog frame.");
#endif

  // If the dimensions are 0 in width or height, use a default size
  if ((0 == Width()) or (0 == Height()))
    {
      float frame_ratio = 3.0f / 4.0f;
      int width = SDL_GetVideoSurface()->w;
      int height = SDL_GetVideoSurface()->h;

      Width(static_cast<int>(static_cast<float>(width) * frame_ratio));
      Height(static_cast<int>(static_cast<float>(height) * frame_ratio));

      Left((width - Width()) / 2);
      Top((height - Height()) / 2);
    }	// (zero dimension)

  if (string::npos != getProperty("autosize").find("true"))
    {

    }	// autosize

  setSurface(SDL_AllocSurface(SDL_SWSURFACE, Width(), Height(), 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000));

  Sint16 l = 0;
  Sint16 t = 0;
  Sint16 w = Width() - 1;
  Sint16 h = Height() - 1;
  Sint16 m = 10;

  Sint16 poly_x[8] =
  {
    static_cast<Sint16>(l),
    static_cast<Sint16>(l + m),
    static_cast<Sint16>(w - m),
    static_cast<Sint16>(w),
    static_cast<Sint16>(w),
    static_cast<Sint16>(w - m),
    static_cast<Sint16>(l + m),
    static_cast<Sint16>(l)
  };
  Sint16 poly_y[8] =
  {
    static_cast<Sint16>(t + m),
    static_cast<Sint16>(t),
    static_cast<Sint16>(t),
    static_cast<Sint16>(t + m),
    static_cast<Sint16>(h - m),
    static_cast<Sint16>(h),
    static_cast<Sint16>(h),
    static_cast<Sint16>(h - m)
  };

  filledPolygonRGBA(getSurface(), poly_x, poly_y, 8, background_color.r,
                    background_color.g, background_color.b, background_color.unused);

  polygonRGBA(getSurface(), poly_x, poly_y, 8, frame_border_color.r, frame_border_color.g, frame_border_color.b, frame_border_color.unused);

#if defined(_DEBUG) || defined(_DEBUGDIALOGBASE)
  logAppend("Created frame.");
#endif

}	// init
void DialogBase::cleanup()
{
}	// cleanup

void DialogBase::draw()
{

  Control::draw();

}	// draw

