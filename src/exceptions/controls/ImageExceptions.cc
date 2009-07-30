/**
 * Sabacc Media Framework
 *
 * Author: goumba
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
#include "exceptions/controls/ImageExceptions.hh"

namespace Exceptions {

    namespace Image {

        LoadError::LoadError(const string& message, const char* file, int line) :
                ExceptionBase(message, file, line) { }
        LoadError::LoadError(const string& message, const string& details, const char* file, int line)  :
                ExceptionBase(message, details, file, line) { }
        LoadError::LoadError(const string& message, const string& details, const string& data, const char* file, int line)  :
                ExceptionBase(message, details, data, file, line) { }

        string LoadError::getExceptionName() {
            return("Image::LoadError");
        }

    }	// namespace Image

}	// namespace Exceptions
