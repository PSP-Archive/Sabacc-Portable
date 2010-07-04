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
#include "exceptions/system/RenderManagerExceptions.hh"

namespace Exceptions
  {

  namespace RenderManager
    {

    FlipFailed::FlipFailed(const string& message, const char* file, int line) :
        ExceptionBase(message, file, line) { }
    FlipFailed::FlipFailed(const string& message, const string& details, const char* file, int line)  :
        ExceptionBase(message, details, file, line) { }
    FlipFailed::FlipFailed(const string& message, const string& details, const string& data, const char* file, int line)  :
        ExceptionBase(message, details, data, file, line) { }

    string FlipFailed::getExceptionName()
    {
      return("Image::FlipFailed");
    }

  }	// namespace Image

}	// namespace Exceptions
