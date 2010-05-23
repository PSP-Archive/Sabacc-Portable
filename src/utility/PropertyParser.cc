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

#include <map>
#include <string>
using std::map;
using std::string;

// SMF headers
#include "types/TypeDefinitions.hh"
#include "utility/PropertyParser.hh"
#include "utility/StringTokenizer.hh"

#if defined(_DEBUG) && defined(_DEBUGPROPPARSER)
#include "utility/Log.hh"
#endif

PropertyMap parseProperties(string property_string) {

    // Temporary strings
    string key, value;

    // Create the map
    PropertyMap properties;

    // Parse the properties

    // First tokenize based on separator
    if (1 > property_string.length()) return(properties);	// returns an empty list be were given an empty string

    StringTokenizer property(property_string, string(";"));

    while (!property.end()) {

        StringTokenizer attribute(property.next(), string(":"));

        if (0 < attribute.size()) {
            key = attribute.next();

            // Remove spaces from key
            string::size_type space_finder = key.find_first_of(" ");
            while (string::npos != space_finder) {
                key.erase(space_finder, 1);

                space_finder = key.find_first_of(" ");
            }	// while(space_finder(

            if (!attribute.end()) {
                value = attribute.next();
            } else {
                value = "";
            }	// if(!empty())
            space_finder = value.find_first_of(" ");
            while (string::npos != space_finder) {
                value.erase(space_finder, 1);

                space_finder = value.find_first_of(" ");
            }	// while(space_finder(


            properties[key] = value;
        }	// if(attribute.size())

#if defined(_DEBUG) && defined(_DEBUGPROPPARSER)
        logAppend("Added property:");
        logAppend(key);
        logAppend(properties[key]);
#endif

        // Assign key to the first
    }	// while(tok(";"))

    return(properties);

}	// parse

string buildPropertyString(const PropertyMap& property_map) {
    string property_string = "";

    for (PropertyMap::const_iterator key_iter = property_map.begin(); property_map.end() != key_iter; ++key_iter) {
        if (0 < key_iter->second.length()) {	// map creates a key when value requested, and it will be blank. Don't add these to the string.
            property_string += key_iter->first;
            property_string += ": ";
            property_string += key_iter->second;
            property_string += "; ";
        }	// if(length)
    }	// for(iter)

    return(property_string);
}	// buildPropertyString
