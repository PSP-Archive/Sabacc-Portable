#if (_PSP_FW_VERSION >= 200) // Only build for true PSP>2.00

/**
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: January 4, 2008
 *
 * File: MessageDialog.hh
 *
 * An interface to the PSP message dialog.
 *
 */

// Standard Library headers
#include <cstring>
#include <string>
using std::string;				\

// PSP SDK headers
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspkerneltypes.h>
#include <psputility.h>

// SMF headers
#include "base/Control.hh"
#include "psputility/MessageDialog.hh"

// SMF Constants
#include "constants/EventConstants.hh"

// SMF Exceptions
#include "exceptions/events/EventExceptions.hh"

// Debugging
#if defined(_DEBUG) || defined(_DEBUGPSPOKCANCELDLG) || defined(_DEBUGEVENTS)
#include <cstdio>
#include "utility/Log.hh"
#endif

PSPDialog::PSPDialog(const string& msg, const string&,
				     const string& btn) :
  Control(), message(msg), button_text(btn), dialog_params()
{
}
PSPDialog::~PSPDialog() 
{
}

string PSPDialog::getDialogText()
{
  return(message);
}
void PSPDialog::setDialogText(string new_message)
{
  message = new_message;
  cleanup();
}
string PSPDialog::getDialogTitle()
{
  return(string());
}
void PSPDialog::setDialogTitle(string)
{
}
string PSPDialog::getDialogButtontext()
{
  return(button_text);
}
void PSPDialog::setDialogButtonText(string new_button_text)
{
  button_text = new_button_text;
  cleanup();
}

void PSPDialog::init()
{

  size_t dialog_size = sizeof(dialog_params);
  
  memset(&dialog_params, 0, dialog_size);

  dialog_params.base.size = dialog_size;
  sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE,
			      &dialog_params.base.language); // Prompt language
  sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN,
			      &dialog_params.base.buttonSwap); // X/O button swap
  
  dialog_params.base.graphicsThread = 0x11;
  dialog_params.base.accessThread = 0x13;
  dialog_params.base.fontThread = 0x12;
  dialog_params.base.soundThread = 0x10;

  dialog_params.mode = PSP_UTILITY_MSGDIALOG_MODE_TEXT;
  dialog_params.options = PSP_UTILITY_MSGDIALOG_OPTION_TEXT;
	
  strncpy(dialog_params.message, message.c_str(), message.length());

  sceUtilityMsgDialogInitStart(&dialog_params);  

  setInitialized();
  
}//init
void PSPDialog::cleanup()
{
  setNotInitialized();
}//cleanup

void PSPDialog::draw()
{

  if(!isInitialized()) init();

  sceUtilityMsgDialogInitStart(&dialog_params);
  
  for(;;)
    {
    if(PSP_UTILITY_DIALOG_VISIBLE == sceUtilityMsgDialogGetStatus())
      {
	sceUtilityMsgDialogUpdate(1);
      }
    else if(PSP_UTILITY_DIALOG_QUIT == sceUtilityMsgDialogGetStatus())
      {
	sceUtilityMsgDialogShutdownStart();
      }
    else if(PSP_UTILITY_DIALOG_NONE == sceUtilityMsgDialogGetStatus())
      {
	break;
      }// if(sceUtilityMsgDialogGetStatus)

    sceDisplayWaitVblankStart();
    sceGuSwapBuffers();
    
    }// if(isActive)

#if defined(_DEBUG) || defined(_DEBUGPSPOKCANCELDLG) || defined(_DEBUGEVENTS)
  logAppend("Finsihed drawing PSP Utility Dialog.");
#endif

  SDL_Event dialog_result;
  dialog_result.type = SDL_USEREVENT;
    
  dialog_result.user.code = event_dialog_dismiss;

#if defined(_DEBUG) || defined(_DEBUGPSPOKCANCELDLG) || defined(_DEBUGEVENTS)
  char debug_string[64];
  sprintf(debug_string, "Dialog event: %d.", dialog_result.user.code);
  logAppend(debug_string);
#endif

  if (-1 == SDL_PushEvent(&dialog_result)) {

#if defined(_DEBUG) || defined(_DEBUGPSPOKCANCELDLG) || defined(_DEBUGEVENTS)
    logAppend("Dialog event error:");
    logAppend(SDL_GetError());
#endif

    throw(Exceptions::Events::PushFailed("Push failed.",
					 "The event for dialog selection could not be pushed into the event queue.",
					 SDL_GetError(), __FILE__, __LINE__));
      
    }	// if(SDL_PushEvent)

}// draw

#endif//PSP_FW_VERSION
