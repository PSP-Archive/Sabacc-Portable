#ifndef __SMF_LOG_HH
#define __SMF_LOG_HH

/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 26, 2008
 *
 * File: Log.hh
 *
 * A simple logging facility
 */

// Standard Library headers
#include <ctime>
#include <fstream>
#include <string>
using std::ofstream;
using std::string;

// Utility headers
#include "Singleton.hh"

class Log {
private:
    ofstream	debug_stream;
    bool			use_cerr;

    // Hide copy ctor/assignment
    Log(const Log&);
    Log& operator=(const Log&);

    // Functions for formatting timestamp
    tm getNow();
    string date();

public:
    // ctor/dtor
    Log();
    ~Log();

    void useFile(string);
    void append(string);
    void append(string, const char*, int);
}; // class Log

// An easy way to refer to the singleton
typedef Singleton<Log> Logger;

// A macro so that file and line numbers are automatically included in Log info
#define logAppend(o) Logger::instance().append(o, __FILE__, __LINE__)


#endif	// __SMF_LOG_HH
