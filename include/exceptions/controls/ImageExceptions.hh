#ifndef __SMF_IMAGEEXCEPTIONS_HH
#define __SMF_IMAGEEXCEPTIONS_HH

/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 28, 2008
 *
 * File: StaticImageExceptions.hh
 *
 * Exceptions related to Image releated Widgets.
 */

// Standard library headers
#include <string>
using std::string;

// SMF Exception headers
#include "exceptions/base/ExceptionBase.hh"

namespace Exceptions {

    namespace Image {

        class LoadError : public ExceptionBase {

        public:

            /** Constructors. @{ */
            /** LoadError parameters, most derived exceptions will follow the same convention:
             *  - @param message - A short description of the exception.
             *  - @param details - A more detailed message of what occurred.
             *  - @param data - A string containing extra information about exception (i.e. Widget address).
             *  - @param file - The source file in which the exception was raised.
             *  - @param line - The approximate line number. This will direct developer to the try block which raised
             *                  the exception, this number is not the exact line of code which raised the exception.
             */
            LoadError(const string&, const char* = "", int = 0);	/// Short message, file (default: zero length) and line (default: -1)
            LoadError(const string&, const string&, const char* = "", int = 0);	/// Short message, long message, file (default: zero length) and line (default: -1)
            LoadError(const string&, const string&, const string&, const char* = "", int = 0);	/// Short message, long message, extra data, file (default: zero length) and line (default: -1)
            virtual ~LoadError() { }
            /** @} */

            /** These functions get exception information. @{ */
            virtual string getExceptionName();

        };	// class LoadError

    }	// namespace Image

}	// namespace Exceptions

#endif // __SMF_IMAGEEXCEPTIONS_HH
