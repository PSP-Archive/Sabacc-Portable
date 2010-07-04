/**
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: December 1, 2008
 *
 * File: SingleButtonDialog.hh (Rewrite)
 *
 * A dialog which displays one button. When dismissed, returns
 * event_dialog_dismissed.
 *
 * Rewritten to replace original, accidentally deleted, version.
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
#include "controls/StaticTextLabel.hh"
#include "dialog/DialogBase.hh"
#include "dialog/SingleButtonDialog.hh"
#include "geometry/Rect.hh"

// SMF Button, conditionally included
#if defined(__PSP__)
#include "controls/StaticPSPButton.hh"
#else
#include "controls/StaticButton.hh"
#endif

// SMF constants
#include "constants/ColorConstants.hh"
#include "constants/EventConstants.hh"
#include "constants/PSPControlConstants.hh"

// SMF Exceptions
#include "exceptions/events/EventExceptions.hh"

// SMF Debugging
#if defined(_DEBUG) || defined(_DEBUGSINGLEBUTTONDIALOG)
#include <cstdlib>
#include "utility/Log.hh"
#endif

SingleButtonDialog::SingleButtonDialog(const Rect& position,
                                       const string& property_string) :
    DialogBase(position, property_string),
    dialog_text(""),
    dialog_title(""),
    dialog_button("")
{
}

SingleButtonDialog::SingleButtonDialog(const string& text,
                                       const string& title,
                                       const string& button_text,
                                       const Rect& position,
                                       const string& property_string) :
    DialogBase(position, property_string),
    dialog_text(text),
    dialog_title(title),
    dialog_button(button_text)
{ }

SingleButtonDialog::SingleButtonDialog(const SingleButtonDialog& src) :
    DialogBase(src),
    dialog_text(src.dialog_text),
    dialog_title(src.dialog_title),
    dialog_button(src.dialog_button)
{ }

SingleButtonDialog::~SingleButtonDialog()
{ }

SingleButtonDialog
&SingleButtonDialog::operator=(const SingleButtonDialog& src)
{
  if (&src != this)
    {

      SingleButtonDialog::operator=(src);

      dialog_text = src.dialog_text;
      dialog_title = src.dialog_title;
      dialog_button = src.dialog_button;

    }	// if(not self)

  return *this;
}	// operator=

string
SingleButtonDialog::getDialogText()
{
  return(dialog_text.getText());
}
void
SingleButtonDialog::setDialogText(std::string text)
{
  dialog_text.setText(text);
  setNotInitialized();
}

string
SingleButtonDialog::getDialogTitle()
{
  return(dialog_title.getText());
}

void
SingleButtonDialog::setDialogTitle(std::string title)
{
  dialog_title.setText(title);
  setNotInitialized();
}

string
SingleButtonDialog::getDialogButtontext()
{
  return(dialog_button.getText());
}

void
SingleButtonDialog::setDialogButtonText(std::string button_text)
{
  dialog_button.setText(button_text);
  setNotInitialized();
}

bool
SingleButtonDialog::onKeyDown(const SDL_KeyboardEvent& key)
{

#if defined(_DEBUG) || defined(_DEBUGSINGLEBUTTONDIALOG)
  logAppend("Handling keyboard event.");
#endif

  if (SDLK_RETURN == key.keysym.sym)
    {

      SDL_Event selection_made;
      selection_made.type = SDL_USEREVENT;
      selection_made.user.code = event_dialog_dismiss;

      if (-1 == SDL_PushEvent(&selection_made))
        {

          throw Exceptions::Events::PushFailed("Push failed.",
                                               "The event for menu selection could not be pushed into the event queue.",
                                               SDL_GetError(),
                                               __FILE__,
                                               __LINE__);

        }	// if(SDL_PushEvent)

    }	// Enter/Return

  /* We processed events with this control.
   * A dialog takes precendence over all others. */
  return(true);

}
bool
SingleButtonDialog::onKeyUp(const SDL_KeyboardEvent&)
{
  return(true);
}

bool
SingleButtonDialog::onJoystickButtonDown(const
    SDL_JoyButtonEvent& joystick_event)
{

  if (1 > SDL_NumJoysticks()) return(false);// Don't process if none found

#if defined(__PSP__)
  const Uint8& button = joystick_event.button;

  if ((psp_cross == button) || (psp_start == button))
    {

      SDL_Event selection_made;
      selection_made.type = SDL_USEREVENT;
      selection_made.user.code = event_dialog_dismiss;

      if (-1 == SDL_PushEvent(&selection_made))
        {

          throw Exceptions::Events::PushFailed("Evt push failed.",
                                               "The event for menu selection could not be pushed into the event queue.",
                                               SDL_GetError(),
                                               __FILE__,
                                               __LINE__);

        }	// if(SDL_PushEvent)

    }	// Enter/Return

#endif // __PSP__

  /* We processed events with this control.
   * A dialog takes precendence over all others. */
  return(true);

}

bool
SingleButtonDialog::onJoystickButtonUp(const SDL_JoyButtonEvent&)
{
  return(true);
}

bool
SingleButtonDialog::onJoystickMove(const SDL_JoyAxisEvent&)
{
  return(true);
}

bool
SingleButtonDialog::onJoystickHat(const SDL_JoyHatEvent&)
{
  return(true);
}

bool
SingleButtonDialog::onJoystickBall(const SDL_JoyBallEvent&)
{
  return(true);
}

bool
SingleButtonDialog::onMouseMove(const SDL_MouseMotionEvent&)
{
  return(true);
}

bool
SingleButtonDialog::onMouseButtonDown(const SDL_MouseButtonEvent&)
{
  return(true);
}

bool
SingleButtonDialog::onMouseButtonUp(const SDL_MouseButtonEvent&)
{
  return(true);
}

void
SingleButtonDialog::init()
{

  DialogBase::init();

#if defined(_DEBUG) || defined(_DEBUGSINGLEBUTTONDIALOG)
  logAppend("Creating single button dialog.");
#endif

  // Set up child controls

  dialog_title.setProperties("autosize: true;");
  dialog_title.init();
  dialog_title.Left(3 + Left() + ((Width() - dialog_title.Width()) / 2));
  dialog_title.Top(Top() + 3);

  dialog_button.setProperties("autosize: true;");
  dialog_button.init();
  // dialog_button.Left(3 + Left() + ((Width() - dialog_button.Width()) / 2));
  dialog_button.Left(Left() + Width() - dialog_button.Width() - 13);
  dialog_button.Top((Top() + Height() - 3) - dialog_button.Height());

  dialog_text.setProperties("justify: center; autosize: true;");

  dialog_text.init();

  // Although we used autosize, resulting text must be smaller than
  // the dialog, so clip accordingly

  if ((Width() - 6) < dialog_text.Width())
    {
      dialog_text.Width(Width() - 6);
    }

  int text_max_height =
    Height() - dialog_title.Height() - dialog_button.Height() - 9;
  if (text_max_height < dialog_text.Height())
    {
      dialog_text.Height(text_max_height);
    }

  dialog_text.Left(3 + Left() + ((Width() - dialog_text.Width()) / 2));
  dialog_text.Top(Top() + ((Height() - dialog_text.Height()) / 2));

  setInitialized();

#if defined(_DEBUG) || defined(_DEBUGSINGLEBUTTONDIALOG)
  logAppend("Created single button dialog.");
#endif

}	// init

void
SingleButtonDialog::cleanup()
{
  DialogBase::cleanup();
}	// cleanup

void
SingleButtonDialog::draw()
{

  DialogBase::draw();

  dialog_title.draw();
  dialog_text.draw();
  dialog_button.draw();

}	// draw

