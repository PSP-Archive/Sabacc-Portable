/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: August 26, 2008
 *
 * File: Log.cc
 *
 * A simple logging facility
 */

// Standard library headers
#include <ctime>
#include <cstring>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>
using std::cout;
using std::cerr;
using std::endl;
using std::ios;
using std::ofstream;
using std::string;
using std::stringstream;
using std::time_put;
using std::use_facet;

// Sabacc headers
#include "utility/Log.hh"

// For PSP
#ifdef __PSP__
#include <cstdio>
#endif

Log::Log() : debug_stream(), use_cerr(true) {
}   // Log

Log::~Log() {
    debug_stream.close();
}   // ~Log

tm Log::getNow() {
    // Return the current time as a tm struct
    time_t now = time(0);
    return(*(localtime(&now)));
} // getNow

string Log::date() {
    // Format the date and time for debugging timestamp
    stringstream t_out;
    const time_put<char>& dt = use_facet<time_put<char> >(t_out.getloc());
    tm now = getNow();
    const char* fmt = "%m-%d-%Y %H:%M:%S";

    dt.put(t_out, t_out, ' ', &now, fmt, fmt + strlen(fmt));

    return(t_out.str());
} // date

void Log::useFile(string filename = "") {
    // Append a timestamp at start of session
    stringstream log_header;
    log_header << endl << "= sit::Log Session started at " << date() << " **" << endl;

    // Set the file name and create stream if necessary
    if (0 == filename.length()) {
        use_cerr = true;

        // output timestamp
        cerr << log_header.str();
    } else {
        debug_stream.open(filename.c_str(), ios::out | ios::app); // Create strem

        // Unset cerr flag
        use_cerr = false;

        // output timestamp
        debug_stream << log_header.str();
    } // if !cerr
} // useFile

void Log::append(string output = "") {
    // Output text to the debug filename, automatically add newline
    stringstream out;

    if (0 == output.length()) {
        out << endl;
    } else {
        out << "(no file data): " << output << endl;
    }

    if (use_cerr) {
        cout << out.str();
    } else {
        debug_stream << out.str();
    }
}	// append(string)

void Log::append(string output = "", const char* file = "", int line = 0) {
    // Output text to the debug filename, automatically add newline
    stringstream out;

    string filestring(file);
    string::size_type dpos = filestring.find_last_of("/\\");
    if (string::npos == dpos) dpos = -1;
    filestring = filestring.substr(dpos + 1, filestring.length() - dpos);

    if (0 == output.length()) {
        out << endl;
    } else {
        out << filestring << "(" << line << "): " << output << endl;
    }

    if (use_cerr) {
        cout << out.str();
    } else {
        debug_stream << out.str();
    }

}	// append(string, const char*, int)
