// This odd looking header guard ensures:
// 1. The usual: make sure this file has not been included.
// 2. We only build this file under a true PSP build
//    (__PSP__ allows UI simulation)
// The requirement that the standard dialog is not included
// has been removed to allow the co-existence of SMF and
// PSP utility dialogs.
#if !defined(__SMF_PSP_OKCANCELDIALOG_HH) &&		\
  (_PSP_FW_VERSION > 100)
#define __SMF_PSP_OKCANCELDIALOG_HH

/**
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: January 9, 2009.
 *
 * File: OKCancelDialog.hh
 *
 * A dialog which displays OK and Cancel buttons on PSP.
 *
 */

// Standard Library headers
#include <string>

// SMF headers
#include "psputility/DialogBase.hh"
#include "geometry/Rect.hh"

// PSP SDK headers
#include <psputility.h>

// The PSP dialogs have been moved to the PSPUtility namespace
// so that both SMF dialogs and PSP dialogs may be used together
// in the same application.
namespace PSPUtility
  {

  class OKCancelDialog : public DialogBase
    {

    public:

      /** Constructors. Same as any lower tier Control base class. @{ */
      OKCancelDialog(const Rect& = default_rect, const string& = "");
      OKCancelDialog(const std::string&, const std::string& = "",
                     const std::string& = "OK", const std::string& = "Cancel",
                     const Rect& = default_rect,
                     const std::string& = "");
      OKCancelDialog(const OKCancelDialog&);
      virtual ~OKCancelDialog();

      /** Assignment operator */
      OKCancelDialog& operator=(const OKCancelDialog&);

      /** Text properties. @{ */
      virtual std::string getDialogText();
      virtual void setDialogText(std::string);
      virtual std::string getDialogTitle();
      virtual void setDialogTitle(std::string);
      virtual std::string getDialogOKButtonText();
      virtual void setDialogOKButtonText(std::string);
      virtual std::string getDialogCancelButtonText();
      virtual void setDialogCancelButtonText(std::string);
      /** @} */

      /** Initialization of the control and cleanup. */
      virtual void init();
      virtual void cleanup();

      /** Draw the frame. */
      virtual void draw();

    protected:

      std::string dialog_text, dialog_title, ok_button, cancel_button;

    };	// class OKCancelDialog

};// namespace PSPUtility

#endif // __SMF_PSP_OKCANCELDIALOG_HH
