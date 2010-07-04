/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 28, 2008
 *
 * File: WidgetExceptions.hh
 *
 * Exceptions related to the Widget and SurfaceWidget bases.
 */

// Standard library headers
#include <string>
using std::string;

// SMF Exception headers
#include "exceptions/base/ExceptionBase.hh"
#include "exceptions/base/WidgetExceptions.hh"

namespace Exceptions
  {

  namespace Widget
    {

    BlitError::BlitError(const string& message, const char* file, int line) :
        ExceptionBase(message, file, line) { }
    BlitError::BlitError(const string& message, const string& details, const char* file, int line)  :
        ExceptionBase(message, details, file, line) { }
    BlitError::BlitError(const string& message, const string& details, const string& data, const char* file, int line)  :
        ExceptionBase(message, details, data, file, line) { }

    string BlitError::getExceptionName()
    {
      return("Widget::BlitError");
    }

    InvalidSurfaceForBlit::InvalidSurfaceForBlit(const string& message, const char* file, int line) :
        ExceptionBase(message, file, line) { }
    InvalidSurfaceForBlit::InvalidSurfaceForBlit(const string& message, const string& details, const char* file, int line)  :
        ExceptionBase(message, details, file, line) { }
    InvalidSurfaceForBlit::InvalidSurfaceForBlit(const string& message, const string& details, const string& data, const char* file, int line)  :
        ExceptionBase(message, details, data, file, line) { }

    string InvalidSurfaceForBlit::getExceptionName()
    {
      return("Widget::InvalidSurfaceForBlit");
    }

  }	// namespace Widget

}	// namespace Exceptions
