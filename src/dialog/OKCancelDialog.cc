/**
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: December 1, 2008
 *
 * File: OKCancelDialog.hh
 *
 * A dialog which displays text and gives the user two choices.
 * This dialog is different from that offering two regular choices as:
 * OK will be tied to Enter/Return (cross on PSP)
 * Cancel will be tied to Escape (circle on PSP)
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
#include "controls/StaticTextLabel.hh"
#include "dialog/DialogBase.hh"
#include "dialog/OKCancelDialog.hh"
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
#if defined(_DEBUG) || defined(_DEBUGSINGLEBUTTONDIALOG) || defined(_DEBUGEVENTS)
#include <cstdlib>
#include "utility/Log.hh"
#endif

OKCancelDialog::OKCancelDialog(const Rect& position, const string& property_string) :
  DialogBase(position, property_string), dialog_text(""), dialog_title(""),
  ok_button("OK"), cancel_button("Cancel") { }
// OKCancelDialog::OKCancelDialog(const string& text, const string& title, const Rect& position, const string& property_string) :
//  DialogBase(position, property_string), dialog_text(text), dialog_title(title), ok_button("OK"), cancel_button("Cancel") { }
OKCancelDialog::OKCancelDialog(const string& text, const string& title, const string& ok_button_text, const string& cancel_button_text, const Rect& position, const string& property_string) :
  DialogBase(position, property_string), dialog_text(text), dialog_title(title), ok_button(ok_button_text), cancel_button(cancel_button_text) { }
OKCancelDialog::OKCancelDialog(const OKCancelDialog& src) :
  DialogBase(src), dialog_text(src.dialog_text), dialog_title(src.dialog_title),
  ok_button(src.ok_button), cancel_button(src.cancel_button) { }
OKCancelDialog::~OKCancelDialog() { }

OKCancelDialog& OKCancelDialog::operator=(const OKCancelDialog& src) {

	if(&src != this) {

		OKCancelDialog::operator=(src);

		dialog_text = src.dialog_text;
		dialog_title = src.dialog_title;
		ok_button = src.ok_button;
		cancel_button = src.cancel_button;

	}	// if(not self)

	return(*this);

}	// operator=

string OKCancelDialog::getDialogText() { return(dialog_text.getText()); }
void OKCancelDialog::setDialogText(std::string text) {
  dialog_text.setText(text);
  setNotInitialized();
}
string OKCancelDialog::getDialogTitle() { return(dialog_title.getText()); }
void OKCancelDialog::setDialogTitle(std::string title) { 
  dialog_title.setText(title); 
  setNotInitialized();
}
string OKCancelDialog::getDialogOKButtontext() { return(ok_button.getText()); }
void OKCancelDialog::setDialogOKButtonText(std::string button_text) { 
  ok_button.setText(button_text);
  setNotInitialized();
}
string OKCancelDialog::getDialogCancelButtontext() { return(cancel_button.getText()); }
void OKCancelDialog::setDialogCancelButtonText(std::string button_text) { 
  cancel_button.setText(button_text);
  setNotInitialized();
}

bool OKCancelDialog::onKeyDown(const SDL_KeyboardEvent& key) {

#if defined(_DEBUG) || defined(_DEBUGSINGLEBUTTONDIALOG)
    logAppend("Handling keyboard event.");
#endif

    if (SDLK_RETURN == key.keysym.sym) {

        SDL_Event dialog_result;
        dialog_result.type = SDL_USEREVENT;
        dialog_result.user.code = event_dialog_ok;

#if defined(_DEBUG) || defined(_DEBUGSINGLEBUTTONDIALOG) || defined(_DEBUGEVENTS)
	char debug_string[64];
	sprintf(debug_string, "Dialog event: %d.", dialog_result.user.code);
	logAppend(debug_string);
#endif

        if (-1 == SDL_PushEvent(&dialog_result)) {

            throw(Exceptions::Events::PushFailed("Push failed.", "The event for dialog selection could not be pushed into the event queue.",
                                                 SDL_GetError(), __FILE__, __LINE__));

        }	// if(SDL_PushEvent)

    } else if (SDLK_ESCAPE == key.keysym.sym) {

        SDL_Event dialog_result;
        dialog_result.type = SDL_USEREVENT;
        dialog_result.user.code = event_dialog_cancel;

#if defined(_DEBUG) || defined(_DEBUGSINGLEBUTTONDIALOG) || defined(_DEBUGEVENTS)
	char debug_string[64];
	sprintf(debug_string, "Dialog event: %d.", dialog_result.user.code);
	logAppend(debug_string);
#endif

        if (-1 == SDL_PushEvent(&dialog_result)) {

            throw(Exceptions::Events::PushFailed("Push failed.", "The event for dialog selection could not be pushed into the event queue.",
                                                 SDL_GetError(), __FILE__, __LINE__));

        }	// if(SDL_PushEvent)

    }	// Enter/Return/Escape

    /* We processed events with this control.
     * A dialog takes precendence over all others. */
    return(true);

}
bool OKCancelDialog::onKeyUp(const SDL_KeyboardEvent&) {
    return(true);
}

bool OKCancelDialog::onJoystickButtonDown(const SDL_JoyButtonEvent& joystick_event) {

  if(1 > SDL_NumJoysticks()) return(false);// Don't process if none found
#if defined(__PSP__)
  const Uint8& button = joystick_event.button;

  if((psp_cross == button) || (psp_start == button)) {

        SDL_Event dialog_result;
        dialog_result.type = SDL_USEREVENT;
        dialog_result.user.code = event_dialog_ok;

#if defined(_DEBUG) || defined(_DEBUGSINGLEBUTTONDIALOG) || defined(_DEBUGEVENTS)
	char debug_string[64];
	sprintf(debug_string, "Dialog event: %d.", dialog_result.user.code);
	logAppend(debug_string);
#endif

        if (-1 == SDL_PushEvent(&dialog_result)) {

            throw(Exceptions::Events::PushFailed("Push failed.", "The event for dialog selection could not be pushed into the event queue.",
                                                 SDL_GetError(), __FILE__, __LINE__));

        }	// if(SDL_PushEvent)

    } else if((psp_circle == button) || (psp_home == button)) {

        SDL_Event dialog_result;
        dialog_result.type = SDL_USEREVENT;
        dialog_result.user.code = event_dialog_cancel;

#if defined(_DEBUG) || defined(_DEBUGSINGLEBUTTONDIALOG) || defined(_DEBUGEVENTS)
	char debug_string[64];
	sprintf(debug_string, "Dialog event: %d.", dialog_result.user.code);
	logAppend(debug_string);
#endif

        if (-1 == SDL_PushEvent(&dialog_result)) {

            throw(Exceptions::Events::PushFailed("Push failed.", "The event for dialog selection could not be pushed into the event queue.",
                                                 SDL_GetError(), __FILE__, __LINE__));

        }	// if(SDL_PushEvent)

    }	// Cross/Circle/Home

#endif // __PSP__

    /* We processed events with this control.
     * A dialog takes precendence over all others. */
    return(true);

}
bool OKCancelDialog::onJoystickButtonUp(const SDL_JoyButtonEvent&) {
    return(true);
}
bool OKCancelDialog::onJoystickMove(const SDL_JoyAxisEvent&) {
    return(true);
}
bool OKCancelDialog::onJoystickHat(const SDL_JoyHatEvent&) {
    return(true);
}
bool OKCancelDialog::onJoystickBall(const SDL_JoyBallEvent&) {
    return(true);
}

bool OKCancelDialog::onMouseMove(const SDL_MouseMotionEvent&) {
    return(true);
}
bool OKCancelDialog::onMouseButtonDown(const SDL_MouseButtonEvent&) {
    return(true);
}
bool OKCancelDialog::onMouseButtonUp(const SDL_MouseButtonEvent&) {
    return(true);
}

void OKCancelDialog::init() {

  DialogBase::init();

#if defined(_DEBUG) || defined(_DEBUGSINGLEBUTTONDIALOG)
	logAppend("Creating single button dialog.");
#endif

	// Set up child controls

	dialog_title.setProperties("autosize: true;");
	dialog_title.init();
	dialog_title.setTextColor(default_frame_titlecolor);
	dialog_title.setLeft(3 + getLeft() + ((getWidth() - dialog_title.getWidth()) / 2));
	dialog_title.setTop(getTop() + 3);

	// 20081208: The location of the OK and Cancel buttons reversed so OK is on the right.
	ok_button.setProperties("autosize: true;");
	ok_button.init();
	ok_button.setLeft(getLeft() + getWidth() - ok_button.getWidth() - 13);
	ok_button.setTop((getTop() + getHeight() - 3) - ok_button.getHeight());

#if defined(__PSP__)
	cancel_button.setPSPKey('C');
#endif

	cancel_button.setProperties("autosize: true;");
	cancel_button.init();
	cancel_button.setLeft(getLeft() + 13);
	cancel_button.setTop((getTop() + getHeight() - 3) - cancel_button.getHeight());

	dialog_text.setProperties("justify: center; autosize: true;");

	dialog_text.init();
	dialog_text.setTextColor(default_frame_forecolor);

	// Although we used autosize, resulting text must be smaller than
	// the dialog, so clip accordingly

	if((getWidth() - 6) < dialog_text.getWidth()) dialog_text.setWidth(getWidth() - 6);

	int text_max_height = getHeight() - dialog_title.getHeight() - ok_button.getHeight() - 9;
	if(text_max_height < dialog_text.getHeight()) dialog_text.setHeight(text_max_height);

	dialog_text.setLeft(3 + getLeft() + ((getWidth() - dialog_text.getWidth()) / 2));
	dialog_text.setTop(getTop() + ((getHeight() - dialog_text.getHeight()) / 2));
	
	setInitialized();

#if defined(_DEBUG) || defined(_DEBUGSINGLEBUTTONDIALOG)
	logAppend("Created single button dialog.");
#endif

}	// init
void OKCancelDialog::cleanup() {
  DialogBase::cleanup();
}	// cleanup

void OKCancelDialog::draw() {

  DialogBase::draw();

  dialog_title.draw();
  dialog_text.draw();
  ok_button.draw();
  cancel_button.draw();

}	// draw

