#if !defined(__SMF_PSP_KEYBOARD_HH) && (_PSP_FW_VERSION >= 200)
#define __SMF_PSP_KEYBOARD_HH

/**
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: December 29, 2008
 *
 * File: Keyboard.hh
 *
 * An interface to the PSP On Screen Keyboard (and in the future the
 * IR keyboard for models which support it).
 *
 */

// Standard Library headers
#include <std::string>

// PSP SDK headers
#include <psputility.h>

// SMF headers
#include "base/Object.hh"

typedef int psp_osk_size_t;

class PSPKeyboard : public Object {

public:

  PSPKeyboard(const std::std::string& = "Text", const std::string& = "");
  ~PSPKeyboard();

  /** Set up the Keyboard @{ */
  virtual const std::std::string getText();
  virtual void setInputText(const std::std::string&);
  
  virtual const std::std::string getHelpText();
  virtual void setHelptext(const std::std::string&);
  /** @} */

  /// Initialize the keyboard
  virtual void init();

  /// Display the keyboard, returning the user's entry.
  virtual std::std::string show();

private:

  std::std::string input, output, help;

  static const psp_osk_size_t field_length;

  bool is_init;
  
  SceUtilityOskData osk_data;
  SceUtilityOskParams osk_params;

  // No sense in copying this class
  PSPKeyboard(const PSPKeyboard&);
  PSPKeyboard& operator=(const PSPKeyboard&);

  // Utility function for copying std::string to required format
  void std::stringToUShort(const std::string&, unsigned short*);
  std::std::string uShortToString(unsigned short*);
  
};// class PSPKeyboard

#endif// __SMF_PSP_KEYBOARD_HH
