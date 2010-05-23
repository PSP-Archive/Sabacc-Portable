#ifndef __SMF_CONTROL_HH
#define __SMF_CONTROL_HH

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
#include "base/SmartSurface.hh"
#include "base/SurfaceWidget.hh"
#include "geometry/Rect.hh"

class Control : public SurfaceWidget {
public:
    /** Constructors @{ */
    Control(const Rect& = default_rect, string = "");
    Control(Widget*, const Rect& = default_rect, string = "");
    Control(const Control&);
    virtual ~Control();
    /** @} */

    virtual Control& operator=(const Control&);	/// Gives this Control's members the values of another Control.

    /** Keyboard event handlers @{ */
    virtual bool onKeyDown(const SDL_KeyboardEvent&);	/// Key pressed down.
    virtual bool onKeyUp(const SDL_KeyboardEvent&);	/// Key released.
    /** @} */

    /** Joystick event handlers @{ */
    virtual bool onJoystickButtonDown(const SDL_JoyButtonEvent&);	/// Joystick button pressed down.
    virtual bool onJoystickButtonUp(const SDL_JoyButtonEvent&);	/// Joystick button released.
    virtual bool onJoystickMove(const SDL_JoyAxisEvent&);	/// Joystick movement.
    virtual bool onJoystickHat(const SDL_JoyHatEvent&);	/// Joystick movement.
    virtual bool onJoystickBall(const SDL_JoyBallEvent&);	/// Joystick ball movement.
    /** @} */

    /** Mouse evet handlers @{ */
    virtual bool onMouseMove(const SDL_MouseMotionEvent&);	/// Mouse move.
    virtual bool onMouseButtonDown(const SDL_MouseButtonEvent&);	/// Mouse button down.
    virtual bool onMouseButtonUp(const SDL_MouseButtonEvent&);	/// Mouse button up.
    /** @} */

    /** Draw the widget to screen. */
    virtual void draw();

};	// class Control

#endif // __SMF_CONTROL_HH
