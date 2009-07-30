#ifndef __SMF_WIDGETEXCEPTIONS_HH
#define __SMF_WIDGETEXCEPTIONS_HH

/*
 * Sabacc Media Framework
 *
 * Author: goumba
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

namespace Exceptions {

    namespace Widget {

        class BlitError : public ExceptionBase {

        public:

            /** Constructors. See ExceptionBase for details. @{ */
            BlitError(const string&, const char* = "", int = 0);	/// Short message, file (default: zero length) and line (default: -1)
            BlitError(const string&, const string&, const char* = "", int = 0);	/// Short message, long message, file (default: zero length) and line (default: -1)
            BlitError(const string&, const string&, const string&, const char* = "", int = 0);	/// Short message, long message, extra data, file (default: zero length) and line (default: -1)
            virtual ~BlitError() { }
            /** @} */

            /** These functions get exception information. @{ */
            virtual string getExceptionName();

        };	// class BlitError

        class InvalidSurfaceForBlit : public ExceptionBase {

        public:

            /** Constructors. See ExceptionBase for details. @{ */
            InvalidSurfaceForBlit(const string&, const char* = "", int = 0);	/// Short message, file (default: zero length) and line (default: -1)
            InvalidSurfaceForBlit(const string&, const string&, const char* = "", int = 0);	/// Short message, long message, file (default: zero length) and line (default: -1)
            InvalidSurfaceForBlit(const string&, const string&, const string&, const char* = "", int = 0);	/// Short message, long message, extra data, file (default: zero length) and line (default: -1)
            virtual ~InvalidSurfaceForBlit() { }
            /** @} */

            /** These functions get exception information. @{ */
            virtual string getExceptionName();

        };	// class LoadError

    }	// namespace Widget

}	// namespace Exceptions

#endif // __SMF_WIDGETEXCEPTIONS_HH
