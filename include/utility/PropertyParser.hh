#ifndef __SMF_PROPERTYPARSER_HH
#define __SMF_PROPERTYPARSER_HH

/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 27, 2008
 *
 * File: PropertyParser.hh
 *
 * Parses a string based list of properties into a map for easy access.
 */

// Standard Library headers
#include <string>
using std::string;

// SMF Headers
#include "types/TypeDefinitions.hh"

PropertyMap parseProperties(string);
string buildPropertyString(const PropertyMap&);

#endif // __SMF_PROPERTYPARSER_HH
