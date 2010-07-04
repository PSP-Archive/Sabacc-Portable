#if (_PSP_FW_VERSION >= 100)

/**
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: January 9, 2008
 *
 * File: OKCancelDialog.hh (PSP)
 *
 * A dialog which displays two choices to the player.
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
#include "psputility/OKCancelDialog.hh"

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

  OKCancelDialog::OKCancelDialog(const Rect& position, const string& property_string) :
      DialogBase(position, property_string), dialog_text(""), dialog_title(""),
      ok_button("OK"), cancel_button("Cancel") { }
  OKCancelDialog::OKCancelDialog(const string& text, const string& title,
                                 const string& ok_text, const string& cancel_text,
                                 const Rect& position, const string& property_string) :
      DialogBase(position, property_string), dialog_text(text), dialog_title(title),
      ok_button(ok_text), cancel_button(cancel_text) { }
  OKCancelDialog::OKCancelDialog(const OKCancelDialog& src) :
      DialogBase(src), dialog_text(src.dialog_text), dialog_title(src.dialog_title),
      ok_button(src.ok_button), cancel_button(src.cancel_button) { }
  OKCancelDialog::~OKCancelDialog() { }

  OKCancelDialog& OKCancelDialog::operator=(const OKCancelDialog& src)
  {

    if (&src != this)
      {

        OKCancelDialog::operator=(src);

        dialog_text = src.dialog_text;
        dialog_title = src.dialog_title;
        ok_button = src.ok_button;
        cancel_button = src.cancel_button;

      }	// if(not self)

    return(*this);

  }	// operator=

  string OKCancelDialog::getDialogText()
  {
    return(dialog_text);
  }
  void OKCancelDialog::setDialogText(std::string text)
  {

    dialog_text = text;
    setNotInitialized();

  }
  string OKCancelDialog::getDialogTitle()
  {
    return(dialog_title);
  }
  void OKCancelDialog::setDialogTitle(std::string title)
  {
    dialog_title = title;
    setNotInitialized();
  }
  string OKCancelDialog::getDialogOKButtonText()
  {
    return(ok_button);
  }
  void OKCancelDialog::setDialogOKButtonText(std::string button)
  {
    ok_button = button;
    setNotInitialized();
  }
  string OKCancelDialog::getDialogCancelButtonText()
  {
    return(cancel_button);
  }
  void OKCancelDialog::setDialogCancelButtonText(std::string button)
  {
    cancel_button = button;
    setNotInitialized();
  }

  void OKCancelDialog::init()
  {

    DialogBase::init();

#if defined(_DEBUG) || defined(_DEBUGSINGLEBUTTONDIALOG)
    logAppend("Creating PSP OK dialog.");
#endif

    strncpy(getPSPDialogParameters().message, dialog_text.c_str(), dialog_text.length());

    getPSPDialogParameters().options = PSP_UTILITY_MSGDIALOG_OPTION_TEXT |
                                       PSP_UTILITY_MSGDIALOG_OPTION_YESNO_BUTTONS | PSP_UTILITY_MSGDIALOG_OPTION_DEFAULT_NO;

#if defined(_DEBUG) || defined(_DEBUGSINGLEBUTTONDIALOG)
    logAppend("Created PSP Ok dialog.");
#endif

  }	// init
  void OKCancelDialog::cleanup()
  {
    DialogBase::cleanup();
  }	// cleanup

  void OKCancelDialog::draw()
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
            dialog_result.user.code =
              (PSP_UTILITY_MSGDIALOG_RESULT_YES == getPSPDialogParameters().buttonPressed)
              ? event_dialog_ok : event_dialog_cancel;

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
