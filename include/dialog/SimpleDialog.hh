#ifndef __SMF_SIMPLEDIALOG_HH
#define __SMF_SIMPLEDIALOG_HH

/**
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: December 9, 2008
 *
 * File: SimpleDialog.hh
 *
 * A dialog which displays text only, useful for loading
 * screens, etc.
 *
 */

// Standard Library headers

// Simple Directmedia Library headers
#if defined(__APPLE__)
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

// SMF headers
#include "base/Control.hh"
#include "controls/StaticTextLabel.hh"
#include "dialog/DialogBase.hh"
#include "geometry/Rect.hh"

// SMF constants
#include "constants/ColorConstants.hh"

class SimpleDialog : public DialogBase
  {

  public:

    /** Constructors. Same as any lower tier Control base class. @{ */
    SimpleDialog(const Rect& = default_rect, const string& = "");
    SimpleDialog(const string&, const string& = "", const string& = "OK", const Rect& = default_rect, const string& = "");
    SimpleDialog(const SimpleDialog&);
    virtual ~SimpleDialog();

    /** Assignment operator */
    SimpleDialog& operator=(const SimpleDialog&);

    /** Text properties. @{ */
    virtual std::string getDialogText();
    virtual void setDialogText(std::string);
    virtual std::string getDialogTitle();
    virtual void setDialogTitle(std::string);
    /** @} */

    /** Keyboard event handlers, all defined to return true to prevent underlying
       *  controls from capturing input. @{ */
    virtual bool onKeyDown(const SDL_KeyboardEvent&);	/// Key pressed down.
    virtual bool onKeyUp(const SDL_KeyboardEvent&);	/// Key released.
    /** @} */

    /** Joystick event handlers, all defined to return true to prevent underlying
     *  controls from capturing input. @{ */
    virtual bool onJoystickButtonDown(const SDL_JoyButtonEvent&);	/// Joystick button pressed down.
    virtual bool onJoystickButtonUp(const SDL_JoyButtonEvent&);	/// Joystick button released.
    virtual bool onJoystickMove(const SDL_JoyAxisEvent&);	/// Joystick movement.
    virtual bool onJoystickHat(const SDL_JoyHatEvent&);	/// Joystick movement.
    virtual bool onJoystickBall(const SDL_JoyBallEvent&);	/// Joystick ball movement.
    /** @} */

    /** Mouse evet handlers, all defined to return true to prevent underlying
     *  controls from capturing input. @{ */
    virtual bool onMouseMove(const SDL_MouseMotionEvent&);	/// Mouse move.
    virtual bool onMouseButtonDown(const SDL_MouseButtonEvent&);	/// Mouse button down.
    virtual bool onMouseButtonUp(const SDL_MouseButtonEvent&);	/// Mouse button up.
    /** @} */

    /** Initialization of the control and cleanup. */
    virtual void init();
    virtual void cleanup();

    /** Draw the frame. */
    virtual void draw();

  private:

    TextLabel dialog_text, dialog_title;

  };	// class Control

#endif // __SMF_DIALOGBASE_HH
