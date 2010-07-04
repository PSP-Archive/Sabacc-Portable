/**
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 28, 2008
 *
 * File: StaticImage.cc
 *
 * Exceptions related to Image releated Widgets.
 */

// Standard library headers
#include <string>
using std::string;

// SMF Exception headers
#include "exceptions/base/ExceptionBase.hh"
#include "exceptions/system/SystemManagerExceptions.hh"

namespace Exceptions
  {

  namespace SystemManager
    {

    InitFailed::InitFailed(const string& message, const char* file, int line) :
        ExceptionBase(message, file, line) { }
    InitFailed::InitFailed(const string& message, const string& details, const char* file, int line)  :
        ExceptionBase(message, details, file, line) { }
    InitFailed::InitFailed(const string& message, const string& details, const string& data, const char* file, int line)  :
        ExceptionBase(message, details, data, file, line) { }

    string InitFailed::getExceptionName()
    {
      return("Image::InitFailed");
    }

  }	// namespace SystemManager

}	// namespace Exceptions
