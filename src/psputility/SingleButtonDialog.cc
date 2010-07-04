#if (_PSP_FW_VERSION >= 100)

/**
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: January 9, 2008
 *
 * File: SingleButtonDialog.hh (PSP)
 *
 * A dialog which displays one button.
 */

// Standard Library headers
#include <cstring>
#include <string>
using std::string;

// PSP SDK headers
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspkerneltypes.h>
#include <psputility.h>

// SMF headers
#include "psputility/DialogBase.hh"
#include "psputility/SingleButtonDialog.hh"

// SMF Constants
#include "constants/EventConstants.hh"

// SMF Exceptions
#include "exceptions/events/EventExceptions.hh"

#if defined(_DEBUG) || defined(_DEBUGSINGLEBUTTONDIALOG) || defined(_DEBUGEVENTS)
#include <cstdio>
#include "utility/Log.hh"
#endif

// The PSP dialogs have been moved to the PSPUtility namespace
// so that both SMF dialogs and PSP dialogs may be used together
// in the same application.
namespace PSPUtility
  {

  SingleButtonDialog::SingleButtonDialog(const Rect& position, const string& property_string) :
      DialogBase(position, property_string), dialog_text(""), dialog_title(""),
      dialog_button("") { }
  SingleButtonDialog::SingleButtonDialog(const string& text, const string& title,
                                         const string& button_text, const Rect& position,
                                         const string& property_string) :
      DialogBase(position, property_string), dialog_text(text), dialog_title(title),
      dialog_button(button_text) { }
  SingleButtonDialog::SingleButtonDialog(const SingleButtonDialog& src) :
      DialogBase(src), dialog_text(src.dialog_text), dialog_title(src.dialog_title),
      dialog_button(src.dialog_button) { }
  SingleButtonDialog::~SingleButtonDialog() { }

  SingleButtonDialog& SingleButtonDialog::operator=(const SingleButtonDialog& src)
  {

    if (&src != this)
      {

        SingleButtonDialog::operator=(src);

        dialog_text = src.dialog_text;
        dialog_title = src.dialog_title;
        dialog_button = src.dialog_button;

      }	// if(not self)

    return(*this);

  }	// operator=

  string SingleButtonDialog::getDialogText()
  {
    return(dialog_text);
  }
  void SingleButtonDialog::setDialogText(std::string text)
  {

    dialog_text = text;
    setNotInitialized();

  }
  string SingleButtonDialog::getDialogTitle()
  {
    return(dialog_title);
  }
  void SingleButtonDialog::setDialogTitle(std::string title)
  {
    dialog_title = title;
    setNotInitialized();
  }
  string SingleButtonDialog::getDialogButtontext()
  {
    return(dialog_button);
  }
  void SingleButtonDialog::setDialogButtonText(std::string button)
  {
    dialog_button = button;
    setNotInitialized();
  }

  void SingleButtonDialog::init()
  {

    DialogBase::init();

#if defined(_DEBUG) || defined(_DEBUGSINGLEBUTTONDIALOG)
    logAppend("Creating PSP OK dialog.");
#endif

    strncpy(getPSPDialogParameters().message, dialog_text.c_str(), dialog_text.length());

#if defined(_DEBUG) || defined(_DEBUGSINGLEBUTTONDIALOG)
    logAppend("Created PSP Ok dialog.");
#endif

  }	// init
  void SingleButtonDialog::cleanup()
  {
    DialogBase::cleanup();
  }	// cleanup

  void SingleButtonDialog::draw()
  {

    if (!isInitialized()) init();

    for (;;)
      {
        switch (sceUtilityMsgDialogGetStatus())
          {

          case PSP_UTILITY_DIALOG_VISIBLE:
          {
            sceUtilityMsgDialogUpdate(1);
            break;
          }
          case PSP_UTILITY_DIALOG_QUIT:
          {
            sceUtilityMsgDialogShutdownStart();
            break;
          }
          case PSP_UTILITY_DIALOG_NONE:
          {

            SDL_Event dialog_result;

            dialog_result.type = SDL_USEREVENT;
            dialog_result.user.code = event_dialog_dismiss;

            if (-1 == SDL_PushEvent(&dialog_result))
              {
                throw(Exceptions::Events::PushFailed("Push failed.", "The event for dialog selection could not be pushed into the event queue.", SDL_GetError(), __FILE__, __LINE__));
              }	// if(SDL_PushEvent)

            return;// exit loop

          }// case PSP_DIALOG_NONE

          }// switch(dialog_status)

        sceDisplayWaitVblankStart();
        sceGuSwapBuffers();

      }// for(;;)

  }	// draw

};// namespace PSPUtility

#endif//_PSP_FW_VERSION
