#ifndef __SMF_STRING_TOKENIZER_HH
#define __SMF_STRING_TOKENIZER_HH

/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: August 26, 2008
 *
 * File: StringTokenizer.hh
 *
 * A simple std::string tokenizer
 */

// Standard library headers
#include <deque>
#include <std::string>
using std::deque;


class StringTokenizer {
    deque<std::string>		tokens;
    deque<std::string>::iterator	current_token;
    std::string					delimiter;

    bool						delim_single;	// Adjecent delimeters create empty std::string if true

    StringTokenizer();	// Can not create instance without std::string to parse
    void tokenize(std::string);	// do it

public:
    StringTokenizer(std::string);
    StringTokenizer(std::string, bool);
    StringTokenizer(std::string, string);
    StringTokenizer(std::string, string, bool);
    ~StringTokenizer() { }

    deque<std::string>::size_type size();
    std::string next();
    bool end();
};	// class StringTokenizer

#endif	// __SMF_STRING_TOKENIZER_HH
