#ifndef __SMF_EXCEPTIONBASE_HH
#define __SMF_EXCEPTIONBASE_HH

/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
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
#include <string>
using std::ostream;
using std::string;

namespace Exceptions
  {

  class ExceptionBase
    {

    public:

      /** Constructors. @{ */
      /** ExceptionBase parameters, most derived exceptions will follow the same convention:
       *  - @param message - A short description of the exception.
       *  - @param details - A more detailed message of what occurred.
       *  - @param data - A string containing extra information about exception (i.e. Widget address).
       *  - @param file - The source file in which the exception was raised.
       *  - @param line - The approximate line number. This will direct developer to the try block which raised
       *                  the exception, this number is not the exact line of code which raised the exception.
       */
      ExceptionBase(const string&, const char* = "", int = 0);	/// Short message, file (default: zero length) and line (default: 0)
      ExceptionBase(const string&, const string&, const char* = "", int = 0);	/// Short message, long message, file (default: zero length) and line (default: 0)
      ExceptionBase(const string&, const string&, const string&, const char* = "", int = 0);	/// Short message, long message, extra data, file (default: zero length) and line (default: 0)
      virtual ~ExceptionBase() { }
      /** @} */

      /** These functions get exception information. @{ */
      virtual string getExceptionName();
      virtual const string& getMessage();
      virtual const string& getDetails();
      virtual const string& getData();
      virtual const string& getFile();
      virtual int getLine();
      /** @} */

    private:

      string		exception_message;
      string		exception_details;
      string		exception_data;
      string		exception_file;
      int			exception_line;

    };	// class ExceptionBase

  string exceptionString(ExceptionBase& exc);

}	// namespace Exceptions

// ostream& operator<< must be in the standard namespace or it won't be called.
namespace std
  {

  /** A function for outputting exception info to a stream. */
  ostream& operator<<(ostream&, Exceptions::ExceptionBase&);

}	// namespace std


#endif // __SMF_EXCEPTIONBASE_HH
