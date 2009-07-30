#if !defined(__SMF_OKCANCELDIALOG_HH)
#define __SMF_OKCANCELDIALOG_HH

/**
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: December 3, 2008
 *
 * File: OKCancelDialog.hh
 *
 * A dialog which displays text and gives the user two choices.
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
#include "dialog/DialogBase.hh"
#include "geometry/Rect.hh"

// SMF Button, conditionally included
#if defined(__PSP__)
#include "controls/StaticPSPButton.hh"
#else
#include "controls/StaticButton.hh"
#endif

// SMF Forward decl
class TextLabel;

// SMF constants
#include "constants/ColorConstants.hh"

class OKCancelDialog : public DialogBase {

public:

    /** Constructors. Same as any lower tier Control base class. @{ */
  OKCancelDialog(const Rect& = default_rect, const string& = "");
  //  OKCancelDialog(const string&, const string& = "", const Rect& = default_rect, const string& = "");
  OKCancelDialog(const string&, const string& = "", const string& = "OK", const string& = "Cancel", const Rect& = default_rect, const string& = "");
    OKCancelDialog(const OKCancelDialog&);
    virtual ~OKCancelDialog();

    /** Assignment operator */
    OKCancelDialog& operator=(const OKCancelDialog&);

  /** Text properties. @{ */
  virtual std::string getDialogText();
  virtual void setDialogText(std::string);
  virtual std::string getDialogTitle();
  virtual void setDialogTitle(std::string);
  virtual std::string getDialogOKButtontext();
  virtual void setDialogOKButtonText(std::string);
  virtual std::string getDialogCancelButtontext();
  virtual void setDialogCancelButtonText(std::string);
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

#if defined(__PSP__)
  PSPButton ok_button, cancel_button;
#else
  Button ok_button, cancel_button;
#endif


};	// class Control

#endif // __SMF_OKCANCELDIALOG_HH
