#if !defined(__SMF_DIALOGBASE_HH)
#define __SMF_DIALOGBASE_HH

/**
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: September 13, 2008
 *
 * File: DialogBase.hh
 *
 * A class for displaying dialogs. This is the base class which only draws
 * a frame to the display and renders all frame elements. It is derived from
 * Control so it may be added to the standard render queue and allow
 * previous elements to be rendered. It must be added last to the queue so that
 * it may capture all events.
 *
 * Derived dialogs will use the standard controls to display dialog elements,
 * overriding the event handlers to pass the event to the desired controls.
 * Any event not handled by the dialog MUST return true so that the underlying
 * interface does not capture those events.
 *
 * Derived dialogs must also provide their own draw method so that all necessary
 * elements are drawn. Original intent was to have a miniature render queue in the
 * base class, however this would remove authority over which controls would receive
 * specific events.
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
#include "geometry/Rect.hh"

// SMF constants
#include "constants/ColorConstants.hh"

/**
 * @class DialogBase
 * Draws the dialog frame and renders the dialog elements.
 */
class DialogBase : public Control
  {

  public:

    /** Constructors. Same as any lower tier Control base class. @{ */
    DialogBase(const Rect&,
	       const string&,
	       const SDL_Color& = default_frame_forecolor,
	       const SDL_Color& = default_frame_backcolor,
	       const SDL_Color& = default_frame_titlecolor,
	       const SDL_Color& = default_frame_bordercolor);
    DialogBase(const DialogBase&);
    virtual ~DialogBase();

    /** Assignment operator */
    DialogBase& operator=(const DialogBase&);

    /** Retrieve and set dialog specific properties. @{ */
    virtual const SDL_Color& getBackgroundColor();
    virtual void setBackgroundColor(const SDL_Color& = default_frame_backcolor);
    virtual const SDL_Color& getBorderColor();
    virtual void setBorderColor(const SDL_Color& = default_frame_bordercolor);
    virtual const SDL_Color& getTextColor();
    virtual void setTextColor(const SDL_Color& = default_frame_forecolor);
    virtual const SDL_Color& getTitleColor();
    virtual void setTitleColor(const SDL_Color& = default_frame_titlecolor);
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

    SDL_Color	background_color, frame_border_color, foreground_color, title_text_color;

  };	// class Control

#endif // __SMF_DIALOGBASE_HH
