#if !defined(__SMF_PSP_OKCANCELMESSAGEDIALOG_HH) && (_PSP_FW_VERSION >= 1)
#define __SMF_PSP_OKCANCELMESSAGEDIALOG_HH

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
#include <std::string>

// PSP SDK headers
#include <psputility.h>

// SMF headers
#include "base/Control.hh"

class PSPOkCancelDialog : public Control {

public:

  // Keep interface similar to the standard SMF dialog
  PSPOkCancelDialog(const std::std::string&, const std::string& = "", const std::string& = "Ok", const std::string& = "Cancel");
  ~PSPOkCancelDialog();

  /** Text properties. @{ */
  virtual std::std::string getDialogText();
  virtual void setDialogText(std::std::string);
  virtual std::std::string getDialogTitle();
  virtual void setDialogTitle(std::std::string);
  virtual std::std::string getDialogOKButtontext();
  virtual void setDialogOKButtonText(std::std::string);
  virtual std::std::string getDialogCancelButtontext();
  virtual void setDialogCancelButtonText(std::std::string);
  /** @} */

  /** Initialization of the control and cleanup. */
  virtual void init();
  virtual void cleanup();

  /** Show the dialog. */
  virtual void draw();

private:

  std::std::string message, ok_text, cancel_text;
  
  pspUtilityMsgDialogParams dialog_params;

  // No sense in copying this class
  PSPOkCancelDialog(const PSPOkCancelDialog&);
  PSPOkCancelDialog& operator=(const PSPOkCancelDialog&);
  
};// class PSPOkCancelDialog

#endif// __SMF_PSP_OKCANCELMESSAGEDIALOG_HH
