// This odd looking header guard ensures:
// 1. The usual: make sure this file has not been included.
// 2. We only build this file under a true PSP build
//    (__PSP__ allows UI simulation)
// The requirement that the standard dialog is not included
// has been removed to allow the co-existence of SMF and
// PSP utility dialogs.
#if !defined(__SMF_PSP_DIALOGBASE_HH) &&		\
  (_PSP_FW_VERSION > 100)
#define __SMF_PSP_DIALOGBASE_HH

/**
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: January 8, 2009.
 *
 * File: DialogBase.hh
 *
 * A class for displaying dialogs. This is the base class which
 * sets up the dialog. It is incapable of displaying a dialog
 * and so draw is a pure virtual function. It will always return
 * true from the event handling functions as the true widgets do
 * so that all other controls are blocked from handling them.
 *
 * As with the standard SMF dialogs this really only works effectively
 * if it is the last to be added to the render queue.
 *
 * It would also be a good idea not to override the event handlers
 * as there really is no point in doing so.
 *
 */

// SMF headers
#include "base/Control.hh"
#include "geometry/Rect.hh"

// SMF constants
#include "constants/ColorConstants.hh"

// PSP SDK headers
#include <psputility.h>

// The PSP dialogs have been moved to the PSPUtility namespace
// so that both SMF dialogs and PSP dialogs may be used together
// in the same application.
namespace PSPUtility {

  /**
   * @class DialogBase
   * Draws the dialog frame and renders the dialog elements.
   */
  class DialogBase : public Control {
    
  public:
    
    /** Constructors. These are the same as standard SMF dialogs, however,
	as the SDK dialogs do not allow positioning nor do they allow colors,
	all they do is set up the dialog properties. @{ */
    DialogBase(const Rect&, const string&, const SDL_Color& = default_frame_forecolor, const SDL_Color& = default_frame_backcolor, const SDL_Color& = default_frame_titlecolor, const SDL_Color& = default_frame_bordercolor);
    DialogBase(const DialogBase&);
    virtual ~DialogBase();
    
    /** Assignment operator */
    DialogBase& operator=(const DialogBase&);
    
    /** Retrieve and set dialog specific properties. @{ */
    /** The SDK dialogs do not allow colors, so these functions
	are only placeholders to keep API compatibility. @{ */
    virtual const SDL_Color& getBackgroundColor();
    virtual void setBackgroundColor(const SDL_Color& = default_frame_backcolor);
    virtual const SDL_Color& getBorderColor();
    virtual void setBorderColor(const SDL_Color& = default_frame_bordercolor);
    virtual const SDL_Color& getTextColor();
    virtual void setTextColor(const SDL_Color& = default_frame_forecolor);
    virtual const SDL_Color& getTitleColor();
    virtual void setTitleColor(const SDL_Color& = default_frame_titlecolor);
    /*@ @} */
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
    
    /** Get PSP utility parameters. */
    pspUtilityMsgDialogParams& getPSPDialogParameters();
    
    /** Initialization of the control and cleanup. */
    virtual void init();
    virtual void cleanup();
    
    /** Change statuis of the dialog. @{ */
    virtual bool buttonPressed();
    virtual void resetButtonPress();
    virtual void setButtonPress();
    
    /** Draw the frame. Pure virtual so that a derived object must be created. */
    virtual void draw() = 0;
    
  private:
    
    pspUtilityMsgDialogParams dialog_parameters;
    
    bool button_pressed;
    
  };	// class Control
  
};// namespace PSPUtility

#endif // __SMF_PSP_DIALOGBASE_HH
  
