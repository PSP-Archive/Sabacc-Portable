#if !defined(__SMF_PSP_MESSAGEDIALOG_HH) && (_PSP_FW_VERSION >= 1)
#define __SMF_PSP_MESSAGEDIALOG_HH

/**
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: January 4, 2008
 *
 * File: MessageDialog.hh
 *
 * An interface to the PSP message dialog.
 *
 */

// Standard Library headers
#include <string>

// PSP SDK headers
#include <psputility.h>

// SMF headers
#include "base/Control.hh"

class PSPDialog : public Control {

public:

  // Keep interface similar to the standard SMF dialog
  PSPDialog(const std::string&, const std::string& = "", const std::string& = "Ok");
  ~PSPDialog();

  /** Text properties. @{ */
  virtual std::string getDialogText();
  virtual void setDialogText(std::string);
  virtual std::string getDialogTitle();
  virtual void setDialogTitle(std::string);
  virtual std::string getDialogButtontext();
  virtual void setDialogButtonText(std::string);
  /** @} */

  /** Initialization of the control and cleanup. */
  virtual void init();
  virtual void cleanup();

  /** Show the dialog. */
  virtual void draw();

private:

  std::string message, button_text;
  
  pspUtilityMsgDialogParams dialog_params;

  // No sense in copying this class
  PSPDialog(const PSPDialog&);
  PSPDialog& operator=(const PSPDialog&);
  
};// class PSPDialog

#endif// __SMF_PSP_MESSAGEDIALOG_HH
