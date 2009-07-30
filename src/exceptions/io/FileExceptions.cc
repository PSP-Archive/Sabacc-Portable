/**
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: August 28, 2008
 *
 * File: FileExceptions.cc
 *
 * File IO related exceptions.
 */

// Standard library headers
#include <string>
using std::string;

// SMF Exception headers
#include "exceptions/base/ExceptionBase.hh"
#include "exceptions/io/FileExceptions.hh"

namespace Exceptions {

    namespace File {

        FileNotFound::FileNotFound(const string& message, const char* file, int line) :
                ExceptionBase(message, file, line) { }
        FileNotFound::FileNotFound(const string& message, const string& details, const char* file, int line)  :
                ExceptionBase(message, details, file, line) { }
        FileNotFound::FileNotFound(const string& message, const string& details, const string& data, const char* file, int line)  :
                ExceptionBase(message, details, data, file, line) { }

        string FileNotFound::getExceptionName() {
            return("File::FileNotFound");
        }

        ZeroLengthFileName::ZeroLengthFileName(const string& message, const char* file, int line) :
                ExceptionBase(message, file, line) { }
        ZeroLengthFileName::ZeroLengthFileName(const string& message, const string& details, const char* file, int line)  :
                ExceptionBase(message, details, file, line) { }
        ZeroLengthFileName::ZeroLengthFileName(const string& message, const string& details, const string& data, const char* file, int line)  :
                ExceptionBase(message, details, data, file, line) { }

        string ZeroLengthFileName::getExceptionName() {
            return("File::ZeroLengthFileName");
        }


    }	// namespace LoadError

}	// namespace Exceptions
