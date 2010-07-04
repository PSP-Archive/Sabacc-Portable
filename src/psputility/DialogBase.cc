#if (_PSP_FW_VERSION >= 100)
/**
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: January 8, 2009.
 *
 * File: DialogBase.cc
 *
 * A class for displaying dialogs. See DialogBase.hh for notes.
 */

// Standard Library headers
#include <cstring>
#include <string>
using std::string;
\

// PSP SDK headers
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspkerneltypes.h>
#include <psputility.h>

// SMF headers
#include "base/Control.hh"
#include "psputility/DialogBase.hh"

// SMF Constants
#include "constants/EventConstants.hh"

// SMF Exceptions
#include "exceptions/events/EventExceptions.hh"

#if defined(_DEBUG) || defined(_DEBUGDIALOGBASE) || defined(_DEBUGEVENTS)
#include <cstdio>
#include "utility/Log.hh"
#endif

// The PSP dialogs have been moved to the PSPUtility namespace
// so that both SMF dialogs and PSP dialogs may be used together
// in the same application.
namespace PSPUtility
  {

  DialogBase::DialogBase(const Rect& position, const string& property_string,
                         const SDL_Color& fore_color, const SDL_Color& back_color,
                         const SDL_Color& title_color, const SDL_Color& border_color) :
      Control(position, property_string), dialog_parameters(),
      button_pressed(false)
  { }
  DialogBase::DialogBase(const DialogBase& src) : Control(src),
      dialog_parameters(),
      button_pressed(false)
  {
  }
  DialogBase::~DialogBase() { }

  DialogBase& DialogBase::operator=(const DialogBase& src)
  {

    if (&src != this)
      {

        memcpy(&dialog_parameters, &src.dialog_parameters, sizeof(dialog_parameters));
        button_pressed = false;

      }	// if(not self)

    return(*this);

  }	// operator=

  const SDL_Color& DialogBase::getBackgroundColor()
  {
    return(default_frame_backcolor);
  }
  void DialogBase::setBackgroundColor(const SDL_Color&)
  {
  }
  const SDL_Color& DialogBase::getBorderColor()
  {
    return(default_frame_bordercolor);
  }
  void DialogBase::setBorderColor(const SDL_Color&)
  {
  }
  const SDL_Color& DialogBase::getTextColor()
  {
    return(default_frame_forecolor);
  }
  void DialogBase::setTextColor(const SDL_Color&)
  {
  }
  const SDL_Color& DialogBase::getTitleColor()
  {
    return(default_frame_titlecolor);
  }
  void DialogBase::setTitleColor(const SDL_Color&)
  {
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

  pspUtilityMsgDialogParams& DialogBase::getPSPDialogParameters()
  {
    return(dialog_parameters);
  }

  void DialogBase::init()
  {

#if defined(_DEBUG) || defined(_DEBUGPSPDIALOGBASE)
    logAppend("Initializing PSP dialog.");
#endif

    size_t dialog_size = sizeof(dialog_parameters);

    memset(&dialog_parameters, 0, dialog_size);

    dialog_parameters.base.size = dialog_size;
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE,
                                &dialog_parameters.base.language);
    // X/O button swap
    sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN,
                                &dialog_parameters.base.buttonSwap);

    dialog_parameters.base.graphicsThread = 0x11;
    dialog_parameters.base.accessThread = 0x13;
    dialog_parameters.base.fontThread = 0x12;
    dialog_parameters.base.soundThread = 0x10;

    dialog_parameters.mode = PSP_UTILITY_MSGDIALOG_MODE_TEXT;
    dialog_parameters.options = PSP_UTILITY_MSGDIALOG_OPTION_TEXT;

    sceUtilityMsgDialogInitStart(&dialog_parameters);

    resetButtonPress();

    setInitialized();

#if defined(_DEBUG) || defined(_DEBUGDIALOGBASE)
    logAppend("Finished initializing PSP dialog.");
#endif

  }	// init
  void DialogBase::cleanup()
  {

    button_pressed = false;
    setNotInitialized();

  }	// cleanup

  bool DialogBase::buttonPressed()
  {
    return(button_pressed);
  }
  void DialogBase::resetButtonPress()
  {
    button_pressed = false;
  }
  void DialogBase::setButtonPress()
  {
    button_pressed = true;
  }

};// namespace PSPUtility

#endif// _PSP_FW_VERSION
