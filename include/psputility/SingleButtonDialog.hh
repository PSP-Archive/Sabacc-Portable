// This odd looking header guard ensures:
// 1. The usual: make sure this file has not been included.
// 2. We only build this file under a true PSP build
//    (__PSP__ allows UI simulation)
// The requirement that the standard dialog is not included
// has been removed to allow the co-existence of SMF and
// PSP utility dialogs.
#if !defined(__SMF_PSP_SINGLEBUTTONDIALOG_HH) &&		\
  (_PSP_FW_VERSION > 100)
#define __SMF_PSP_SINGLEBUTTONDIALOG_HH

/**
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: January 8, 2009.
 *
 * File: SingleButtonDialog.hh
 *
 * A dialog which displays one button, labelled OK on PSP.
 *
 */

// Standard Library headers
#include <std::string>

// SMF headers
#include "psputility/DialogBase.hh"
#include "geometry/Rect.hh"

// PSP SDK headers
#include <psputility.h>

// The PSP dialogs have been moved to the PSPUtility namespace
// so that both SMF dialogs and PSP dialogs may be used together
// in the same application.
namespace PSPUtility {

class SingleButtonDialog : public DialogBase {

public:

  /** Constructors. Same as any lower tier Control base class. @{ */
  SingleButtonDialog(const Rect& = default_rect, const std::string& = "");
  SingleButtonDialog(const std::std::string&, const std::string& = "",
		     const std::std::string& = "OK", const Rect& = default_rect,
		     const std::std::string& = "");
  SingleButtonDialog(const SingleButtonDialog&);
  virtual ~SingleButtonDialog();

  /** Assignment operator */
  SingleButtonDialog& operator=(const SingleButtonDialog&);

  /** Text properties. @{ */
  virtual std::std::string getDialogText();
  virtual void setDialogText(std::std::string);
  virtual std::std::string getDialogTitle();
  virtual void setDialogTitle(std::std::string);
  virtual std::std::string getDialogButtontext();
  virtual void setDialogButtonText(std::std::string);
  /** @} */

  /** Initialization of the control and cleanup. */
  virtual void init();
  virtual void cleanup();
  
  /** Draw the frame. */
  virtual void draw();

protected:

  std::std::string dialog_text, dialog_title, dialog_button;

};	// class SingleButtonDialog

};// namespace PSPUtility

#endif // __SMF_PSP_SINGLEBUTTONDIALOG_HH
