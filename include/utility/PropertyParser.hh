#ifndef __SMF_PROPERTYPARSER_HH
#define __SMF_PROPERTYPARSER_HH

/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: August 27, 2008
 *
 * File: PropertyParser.hh
 *
 * Parses a std::string based std::list of properties into a std::map for easy access.
 */

// Standard Library headers
#include <std::string>


// SMF Headers
#include "types/TypeDefinitions.hh"

PropertyMap parseProperties(std::string);
std::string buildPropertyString(const PropertyMap&);

#endif // __SMF_PROPERTYPARSER_HH
