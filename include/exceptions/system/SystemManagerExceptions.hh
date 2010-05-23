#ifndef __SMF_SYSTEMMANAGEREXCEPTIONS_HH
#define __SMF_SYSTEMMANAGEREXCEPTIONS_HH

/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 29, 2008
 *
 * File: SystemManagerExceptions.hh
 *
 * Exceptions related to SystemManager functions.
 */

// Standard library headers
#include <string>
using std::string;

// SMF Exception headers
#include "exceptions/base/ExceptionBase.hh"

namespace Exceptions {

    namespace SystemManager {

        class InitFailed : public ExceptionBase {

        public:

            /** Constructors. @{ */
            /** InitFailed parameters, most derived exceptions will follow the same convention:
             *  - @param message - A short description of the exception.
             *  - @param details - A more detailed message of what occurred.
             *  - @param data - A string containing extra information about exception (i.e. Widget address).
             *  - @param file - The source file in which the exception was raised.
             *  - @param line - The approximate line number. This will direct developer to the try block which raised
             *                  the exception, this number is not the exact line of code which raised the exception.
             */
            InitFailed(const string&, const char* = "", int = 0);	/// Short message, file (default: zero length) and line (default: -1)
            InitFailed(const string&, const string&, const char* = "", int = 0);	/// Short message, long message, file (default: zero length) and line (default: -1)
            InitFailed(const string&, const string&, const string&, const char* = "", int = 0);	/// Short message, long message, extra data, file (default: zero length) and line (default: -1)
            virtual ~InitFailed() { }
            /** @} */

            /** These functions get exception information. @{ */
            virtual string getExceptionName();

        };	// class FlipFailed

    }	// namespace SystemManager

}	// namespace Exceptions

#endif // __SMF_SYSTEMMANAGEREXCEPTIONS_HH
