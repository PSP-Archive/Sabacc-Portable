#ifndef __SMF_RENDERMANAGEREXCEPTIONS_HH
#define __SMF_RENDERMANAGEREXCEPTIONS_HH

/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: August 29, 2008
 *
 * File: RenderManagerExceptions.hh
 *
 * Exceptions related to RenderManager.
 */

// Standard library headers
#include <string>
using std::string;

// SMF Exception headers
#include "exceptions/base/ExceptionBase.hh"

namespace Exceptions {

    namespace RenderManager {

        class FlipFailed : public ExceptionBase {

        public:

            /** Constructors. @{ */
            /** FlipFailed parameters, most derived exceptions will follow the same convention:
             *  - @param message - A short description of the exception.
             *  - @param details - A more detailed message of what occurred.
             *  - @param data - A string containing extra information about exception (i.e. Widget address).
             *  - @param file - The source file in which the exception was raised.
             *  - @param line - The approximate line number. This will direct developer to the try block which raised
             *                  the exception, this number is not the exact line of code which raised the exception.
             */
            FlipFailed(const string&, const char* = "", int = 0);	/// Short message, file (default: zero length) and line (default: -1)
            FlipFailed(const string&, const string&, const char* = "", int = 0);	/// Short message, long message, file (default: zero length) and line (default: -1)
            FlipFailed(const string&, const string&, const string&, const char* = "", int = 0);	/// Short message, long message, extra data, file (default: zero length) and line (default: -1)
            virtual ~FlipFailed() { }
            /** @} */

            /** These functions get exception information. @{ */
            virtual string getExceptionName();

        };	// class FlipFailed

    }	// namespace Image

}	// namespace Exceptions

#endif // __SMF_RENDERMANAGEREXCEPTIONS_HH
