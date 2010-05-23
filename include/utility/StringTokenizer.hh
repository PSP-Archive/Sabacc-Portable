#ifndef __SMF_STRING_TOKENIZER_HH
#define __SMF_STRING_TOKENIZER_HH

/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 26, 2008
 *
 * File: StringTokenizer.hh
 *
 * A simple string tokenizer
 */

// Standard library headers
#include <deque>
#include <string>
using std::deque;
using std::string;

class StringTokenizer {
    deque<string>		tokens;
    deque<string>::iterator	current_token;
    string					delimiter;

    bool						delim_single;	// Adjecent delimeters create empty string if true

    StringTokenizer();	// Can not create instance without string to parse
    void tokenize(string);	// do it

public:
    StringTokenizer(string);
    StringTokenizer(string, bool);
    StringTokenizer(string, string);
    StringTokenizer(string, string, bool);
    ~StringTokenizer() { }

    deque<string>::size_type size();
    string next();
    bool end();
};	// class StringTokenizer

#endif	// __SMF_STRING_TOKENIZER_HH
