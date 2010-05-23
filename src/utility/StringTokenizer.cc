/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 26, 2008
 *
 * File: StringStringTokenizer.cc
 *
 * A simple string tokenizer
 */

// Standard Library deahers
#include <deque>
#include <string>
using std::deque;
using std::string;

// Sabacc headers
#include "utility/StringTokenizer.hh"

// Debugging headers
#if defined(_DEBUG) && defined(_DEBUG_TOKENIZER)
#include "utility/Log.hh"
#endif

StringTokenizer::StringTokenizer(string str) : tokens(), current_token(), delimiter("\n"),
        delim_single(false) {
    tokenize(str);

#if defined(_DEBUG) && defined(_DEBUG_TOKENIZER)
    logAppend("StringTokenizer: string");
#endif
}	// ctor(string)

StringTokenizer::StringTokenizer(string str, bool single) : tokens(), current_token(),
        delimiter("\n"), delim_single(single) {
    tokenize(str);

#if defined(_DEBUG) && defined(_DEBUG_TOKENIZER)
    logAppend("StringTokenizer: string, bool");
#endif
}	// ctor(string, bool)

StringTokenizer::StringTokenizer(string str, string delims) : tokens(), current_token(),
        delimiter(delims), delim_single(false) {
    tokenize(str);

#if defined(_DEBUG) && defined(_DEBUG_TOKENIZER)
    logAppend("StringTokenizer: string, string");
#endif
}	// ctor(string, string)

StringTokenizer::StringTokenizer(string str, string delims, bool single) :
        tokens(), current_token(), delimiter(delims), delim_single(single) {
    tokenize(str);

#if defined(_DEBUG) && defined(_DEBUG_TOKENIZER)
    logAppend("StringTokenizer: string, string, bool");
#endif
}	// ctor(string, string, bool)

void StringTokenizer::tokenize(string str) {

#if defined(_DEBUG) && defined(_DEBUG_TOKENIZER)
    logAppend("StringTokenizer: Starting '" + str + "'.");
#endif

    if (0 == str.length()) {
#if defined(_DEBUG) && defined(_DEBUG_TOKENIZER)
        logAppend("StringTokenizer: zero length string, aborting.");
#endif

        tokens.clear();
        current_token = tokens.begin();
        return;
    }	// if(0 == length)

    string::size_type	begin = str.find_first_not_of(delimiter);
    string::size_type	end = 0;

#if defined(_DEBUG) && defined(_DEBUG_TOKENIZER)
    logAppend("StringTokenizer: Set limits.");
#endif

    while (string::npos != end) {

#if defined(_DEBUG) && defined(_DEBUG_TOKENIZER)
        logAppend("StringTokenizer: Creating token.");
#endif

        end = str.find_first_of(delimiter, begin);
        // if(0 < str.substr(begin, end - begin).length())
        tokens.push_back(str.substr(begin, end - begin));

        if (delim_single) {
            begin = str.find_first_not_of(delimiter, end);
        } else {
            begin = end + 1;
        }	// if(delim_single
    }	// while

    current_token = tokens.begin();
}	// tokenize

deque<string>::size_type StringTokenizer::size() {
    return(tokens.size());
}	// size

string StringTokenizer::next() {
    if (tokens.end() < current_token) current_token = tokens.begin();
    return(*(current_token++));
}	// next

bool StringTokenizer::end() {
    return(tokens.end() == current_token);
}	// end
