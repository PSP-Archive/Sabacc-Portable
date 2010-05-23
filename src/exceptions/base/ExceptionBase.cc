/**
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 28, 2008
 *
 * File: ExceptionBase.cc
 *
 * Base class from which all exceptions are derived.
 * Stored are the filename in which the exception occured, a short message,
 * a more detailed message.
 */

// Standard library headers
#include <ostream>
#include <sstream>
#include <string>
using std::endl;
using std::ostream;
using std::stringstream;
using std::string;

// SMF Exception headers
#include "exceptions/base/ExceptionBase.hh"

namespace Exceptions {

    ExceptionBase::ExceptionBase(const string& message, const char* file, int line) :
            exception_message(message), exception_details(), exception_data(),
            exception_file(file), exception_line(line) { }
    ExceptionBase::ExceptionBase(const string& message, const string& details, const char* file, int line) :
            exception_message(message), exception_details(details), exception_data(),
            exception_file(file), exception_line(line) { }
    ExceptionBase::ExceptionBase(const string& message, const string& details, const string& data, const char* file, int line) :
            exception_message(message), exception_details(details), exception_data(data),
            exception_file(file), exception_line(line) { }

    string ExceptionBase::getExceptionName() {
        return("ExceptionBase - contact library author, this should be more specific.");
    }
    const string& ExceptionBase::getMessage() {
        return(exception_message);
    }
    const string& ExceptionBase::getDetails() {
        return(exception_details);
    }
    const string& ExceptionBase::getData() {
        return(exception_data);
    }
    const string& ExceptionBase::getFile() {
        return(exception_file);
    }
    int ExceptionBase::getLine() {
        return(exception_line);
    }

    /** A function for outputting exception info to a stream. */
    string exceptionString(ExceptionBase& exc) {

        stringstream except_str;
        string::size_type basename_pos = exc.getFile().find_last_of("/\\");
        basename_pos = (string::npos == basename_pos) ? 0 : basename_pos + 1;

        except_str << "*** An error has occured, available details follow. ***" << endl;
        except_str << "Please use the following information when reporting this problem." << endl;
        except_str << "=================================================================" << endl;
        except_str << "Exception: " << exc.getExceptionName() << endl;
        except_str << "Message: " << exc.getMessage() << endl;
        if (0 < exc.getDetails().length()) except_str << "Details: " << exc.getDetails() << endl;
        if (0 < exc.getFile().length()) except_str << "File: " << exc.getFile().substr(basename_pos, exc.getFile().length() - basename_pos) << "(" << exc.getLine() << ")" << endl;
        if (0 < exc.getData().length()) except_str << "Additional data: " << exc.getData() << endl;
        except_str << endl;

        return(except_str.str());

    }	// operator<<


}	// namespace Exceptions

// ostream& operator<< must be in the standard namespace or it won't be called.
namespace std {

    /** A function for outputting exception info to a stream, like cout */
    ostream& operator<<(ostream& output_stream, Exceptions::ExceptionBase& exc) {

        string::size_type basename_pos = exc.getFile().find_last_of("/\\");
        basename_pos = (string::npos == basename_pos) ? 0 : basename_pos + 1;

        output_stream << "*** An error has occured, available details follow. ***" << endl;
        output_stream << "Please use the following information when reporting this problem." << endl;
        output_stream << "=================================================================" << endl;
        output_stream << "Exception: " << exc.getExceptionName() << endl;
        output_stream << "Message: " << exc.getMessage() << endl;
        if (0 < exc.getDetails().length()) output_stream << "Details: " << exc.getDetails() << endl;
        if (0 < exc.getFile().length()) output_stream << "File: " << exc.getFile().substr(basename_pos, exc.getFile().length() - basename_pos) << "(" << exc.getLine() << ")" << endl;
        if (0 < exc.getData().length()) output_stream << "Additional data: " << exc.getData() << endl;
        output_stream << endl;

        return(output_stream);

    }	// operator<<

}	// namespace std
