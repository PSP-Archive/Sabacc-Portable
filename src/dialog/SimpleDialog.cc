/**
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: December 9, 2008
 *
 * File: SimpleDialog.cc
 *
 * A dialog which displays text only, useful for loading
 * screens, etc.
 *
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
#include "controls/StaticButton.hh"
#include "controls/StaticTextLabel.hh"
#include "dialog/DialogBase.hh"
#include "dialog/SimpleDialog.hh"
#include "geometry/Rect.hh"

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

SimpleDialog::SimpleDialog(const Rect& position, const string& property_string) :
  DialogBase(position, property_string), dialog_text(""), dialog_title("") { }
SimpleDialog::SimpleDialog(const string& text, const string& title, const string& button_text, const Rect& position, const string& property_string) :
  DialogBase(position, property_string), dialog_text(text), dialog_title(title) { }
SimpleDialog::SimpleDialog(const SimpleDialog& src) :
  DialogBase(src), dialog_text(src.dialog_text), dialog_title(src.dialog_title)
  { }
SimpleDialog::~SimpleDialog() { }

SimpleDialog& SimpleDialog::operator=(const SimpleDialog& src) {

	if(&src != this) {

		SimpleDialog::operator=(src);

		dialog_text = src.dialog_text;
		dialog_title = src.dialog_title;

	}	// if(not self)

	return(*this);

}	// operator=

string SimpleDialog::getDialogText() { return(dialog_text.getText()); }
void SimpleDialog::setDialogText(std::string text) { 
  dialog_text.setText(text);
  setNotInitialized();
}
string SimpleDialog::getDialogTitle() { return(dialog_title.getText()); }
void SimpleDialog::setDialogTitle(std::string title) { 
  dialog_title.setText(title); 
  setNotInitialized();
}

bool SimpleDialog::onKeyDown(const SDL_KeyboardEvent& key) {

#if defined(_DEBUG) || defined(_DEBUGSINGLEBUTTONDIALOG)
    logAppend("Handling keyboard event.");
#endif

    if (SDLK_RETURN == key.keysym.sym) {

        SDL_Event selection_made;
        selection_made.type = SDL_USEREVENT;
        selection_made.user.code = event_dialog_dismiss;

        if (-1 == SDL_PushEvent(&selection_made)) {

            throw(Exceptions::Events::PushFailed("Push failed.", "The event for menu selection could not be pushed into the event queue.",
                                                 SDL_GetError(), __FILE__, __LINE__));

        }	// if(SDL_PushEvent)

    }	// Enter/Return

    /* We processed events with this control.
     * A dialog takes precendence over all others. */
    return(true);

}
bool SimpleDialog::onKeyUp(const SDL_KeyboardEvent&) {
    return(true);
}

bool SimpleDialog::onJoystickButtonDown(const SDL_JoyButtonEvent& joystick_event) {

  if(1 > SDL_NumJoysticks()) return(false);// Don't process if none found
#if defined(__PSP__)
  const Uint8& button = joystick_event.button;

  if((psp_cross == button) || (psp_start == button)) {

        SDL_Event selection_made;
        selection_made.type = SDL_USEREVENT;
        selection_made.user.code = event_dialog_dismiss;

        if (-1 == SDL_PushEvent(&selection_made)) {

            throw(Exceptions::Events::PushFailed("Push failed.", "The event for menu selection could not be pushed into the event queue.",
                                                 SDL_GetError(), __FILE__, __LINE__));

        }	// if(SDL_PushEvent)

    }	// Enter/Return

#endif // __PSP__

    /* We processed events with this control.
     * A dialog takes precendence over all others. */
    return(true);

}
bool SimpleDialog::onJoystickButtonUp(const SDL_JoyButtonEvent&) {
    return(true);
}
bool SimpleDialog::onJoystickMove(const SDL_JoyAxisEvent&) {
    return(true);
}
bool SimpleDialog::onJoystickHat(const SDL_JoyHatEvent&) {
    return(true);
}
bool SimpleDialog::onJoystickBall(const SDL_JoyBallEvent&) {
    return(true);
}

bool SimpleDialog::onMouseMove(const SDL_MouseMotionEvent&) {
    return(true);
}
bool SimpleDialog::onMouseButtonDown(const SDL_MouseButtonEvent&) {
    return(true);
}
bool SimpleDialog::onMouseButtonUp(const SDL_MouseButtonEvent&) {
    return(true);
}

void SimpleDialog::init() {

  DialogBase::init();

#if defined(_DEBUG) || defined(_DEBUGSINGLEBUTTONDIALOG)
	logAppend("Creating single button dialog.");
#endif

	// Set up child controls

	dialog_title.setProperties("autosize: true;");
	dialog_title.init();
	dialog_title.Left(3 + Left() + ((Width() - dialog_title.Width()) / 2));
	dialog_title.Top(Top() + 3);

	dialog_text.setProperties("justify: center; autosize: true;");

	dialog_text.init();

	// Although we used autosize, resulting text must be smaller than
	// the dialog, so clip accordingly

	if((Width() - 6) < dialog_text.Width()) dialog_text.Width(Width() - 6);

	int text_max_height = Height() - dialog_title.Height() - 9;
	if(text_max_height < dialog_text.Height()) dialog_text.Height(text_max_height);

	dialog_text.Left(3 + Left() + ((Width() - dialog_text.Width()) / 2));
	dialog_text.Top(Top() + ((Height() - dialog_text.Height()) / 2));
	
	setInitialized();

#if defined(_DEBUG) || defined(_DEBUGSINGLEBUTTONDIALOG)
	logAppend("Created single button dialog.");
#endif

}	// init
void SimpleDialog::cleanup() {
  DialogBase::cleanup();
}	// cleanup

void SimpleDialog::draw() {

  DialogBase::draw();

  dialog_title.draw();
  dialog_text.draw();

}	// draw

