/**
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: August 28, 2008
 *
 * File: EventExceptions.cc
 *
 * Exceptions related to events.
 */

// Standard library headers
#include <string>
using std::string;

// SMF Exception headers
#include "exceptions/base/ExceptionBase.hh"
#include "exceptions/events/EventExceptions.hh"

namespace Exceptions {

    namespace Events {

        PushFailed::PushFailed(const string& message, const char* file, int line) :
                ExceptionBase(message, file, line) { }
        PushFailed::PushFailed(const string& message, const string& details, const char* file, int line)  :
                ExceptionBase(message, details, file, line) { }
        PushFailed::PushFailed(const string& message, const string& details, const string& data, const char* file, int line)  :
                ExceptionBase(message, details, data, file, line) { }

        string PushFailed::getExceptionName() {
            return("Events::PushFailed");
        }

    }	// namespace Events

}	// namespace Exceptions
