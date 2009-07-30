#ifndef __SMF_EVENTEXCEPTIONS_HH
#define __SMF_EVENTEXCEPTIONS_HH

/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: August 28, 2008
 *
 * File: eVENTExceptions.hh
 *
 * Exceptions related to events.
 */

// Standard library headers
#include <string>
using std::string;

// SMF Exception headers
#include "exceptions/base/ExceptionBase.hh"

namespace Exceptions {

    namespace Events {

        class PushFailed : public ExceptionBase {

        public:

            /** Constructors. See ExceptionBase. @{ */
            PushFailed(const string&, const char* = "", int = 0);	/// Short message, file (default: zero length) and line (default: -1)
            PushFailed(const string&, const string&, const char* = "", int = 0);	/// Short message, long message, file (default: zero length) and line (default: -1)
            PushFailed(const string&, const string&, const string&, const char* = "", int = 0);	/// Short message, long message, extra data, file (default: zero length) and line (default: -1)
            virtual ~PushFailed() { }
            /** @} */

            /** These functions get exception information. @{ */
            virtual string getExceptionName();

        };	// class PushFailed

    }	// namespace Events

}	// namespace Exceptions


#endif // __SMF_EVENTEXCEPTIONS_HH
