#ifndef __SMF_FILEEXCEPTIONS_HH
#define __SMF_FILEEXCEPTIONS_HH

/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 28, 2008
 *
 * File: FileExceptions.hh
 *
 * File IO related exceptions.
 */

// Standard library headers
#include <string>
using std::string;

// SMF Exception headers
#include "exceptions/base/ExceptionBase.hh"

namespace Exceptions {

    namespace File {

        class FileNotFound : public ExceptionBase {

        public:

            /** Constructors. See documentation for ExceptionBase. @{ */
            FileNotFound(const string&, const char* = "", int = 0);	/// Short message, file (default: zero length) and line (default: -1)
            FileNotFound(const string&, const string&, const char* = "", int = 0);	/// Short message, long message, file (default: zero length) and line (default: -1)
            FileNotFound(const string&, const string&, const string&, const char* = "", int = 0);	/// Short message, long message, extra data, file (default: zero length) and line (default: -1)
            virtual ~FileNotFound() { }
            /** @} */

            /** These functions get exception information. @{ */
            virtual string getExceptionName();

        };	// class FileNotFound

        class ZeroLengthFileName : public ExceptionBase {

        public:

            /** Constructors. See documentation for ExceptionBase. @{ */
            ZeroLengthFileName(const string&, const char* = "", int = 0);	/// Short message, file (default: zero length) and line (default: -1)
            ZeroLengthFileName(const string&, const string&, const char* = "", int = 0);	/// Short message, long message, file (default: zero length) and line (default: -1)
            ZeroLengthFileName(const string&, const string&, const string&, const char* = "", int = 0);	/// Short message, long message, extra data, file (default: zero length) and line (default: -1)
            virtual ~ZeroLengthFileName() { }
            /** @} */

            /** These functions get exception information. @{ */
            virtual string getExceptionName();

        };	// class ZeroLengthFileName

    }	// namespace File

}	// namespace Exceptions

#endif // __SMF_FILEEXCEPTIONS_HH
