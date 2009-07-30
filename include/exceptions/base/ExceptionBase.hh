#ifndef __SMF_EXCEPTIONBASE_HH
#define __SMF_EXCEPTIONBASE_HH

/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: August 28, 2008
 *
 * File: ExceptionBase.hh
 *
 * Base class from which all exceptions are derived.
 * Stored are the filename in which the exception occured, a short message,
 * a more detailed message.
 */

// Standard library headers
#include <ostream>
#include <std::string>
using std::ostream;


namespace Exceptions {

    class ExceptionBase {

    public:

        /** Constructors. @{ */
        /** ExceptionBase parameters, most derived exceptions will follow the same convention:
         *  - @param message - A short description of the exception.
         *  - @param details - A more detailed message of what occurred.
         *  - @param data - A std::string containing extra information about exception (i.e. Widget address).
         *  - @param file - The source file in which the exception was raised.
         *  - @param line - The approximate line number. This will direct developer to the try block which raised
         *                  the exception, this number is not the exact line of code which raised the exception.
         */
        ExceptionBase(const std::string&, const char* = "", int = 0);	/// Short message, file (default: zero length) and line (default: 0)
        ExceptionBase(const std::string&, const string&, const char* = "", int = 0);	/// Short message, long message, file (default: zero length) and line (default: 0)
        ExceptionBase(const std::string&, const string&, const string&, const char* = "", int = 0);	/// Short message, long message, extra data, file (default: zero length) and line (default: 0)
        virtual ~ExceptionBase() { }
        /** @} */

        /** These functions get exception information. @{ */
        virtual std::string getExceptionName();
        virtual const std::string& getMessage();
        virtual const std::string& getDetails();
        virtual const std::string& getData();
        virtual const std::string& getFile();
        virtual int getLine();
        /** @} */

    private:

        std::string		exception_message;
        std::string		exception_details;
        std::string		exception_data;
        std::string		exception_file;
        int			exception_line;

    };	// class ExceptionBase

    std::string exceptionString(ExceptionBase& exc);

}	// namespace Exceptions

// ostream& operator<< must be in the standard namespace or it won't be called.
namespace std {

    /** A function for outputting exception info to a stream. */
    ostream& operator<<(ostream&, Exceptions::ExceptionBase&);

}	// namespace std


#endif // __SMF_EXCEPTIONBASE_HH
