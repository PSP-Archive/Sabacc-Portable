/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: August 20, 2008
 *
 * File: Object.cc
 *
 * The implementation of the object on which all interface
 * objects (widgets, etc) will be based.
 */

// Standard Library headers
#include <string>
using std::string;

// SMF headers
#include "base/Object.hh"

// SMF Utilities
#include "utility/ObjectUtilities.hh"
#include "utility/PropertyParser.hh"

// Debug
#ifdef _DEBUG
#include "utility/Log.hh"
#endif

/** Creates an instance of the base object class, and uses the utility function
 *  generateObjectID() to assign an object id. */
Object::Object() : my_id(generateObjectID()), my_properties() { }
Object::Object(string property_string) :
        my_id(generateObjectID()), my_properties(parseProperties(property_string)) { }
Object::Object(const Object& src) : my_id(generateObjectID()), my_properties(src.my_properties) { }
Object::~Object() { }

Object& Object::operator=(const Object& src) {

    if (&src != this) {
        my_id = generateObjectID();
        // my_properties = src.my_properties;
    }	// if(&src != this)

    return(*this);
}	// operator=

object_id_type Object::getID() {
    return(my_id);
}	// getID

string Object::getProperty(string key) {
  if(my_properties.end() != my_properties.find(key))
    {
      return(my_properties.at(key));
    } else {
    return("");
  }
}	// getProperty
void Object::setProperty(string key, string property) {

#ifdef _DEBUG
  logAppend("Setting object properties.");
#endif

  // if((my_properties.end() != my_properties.find(key))
  //   && (property != my_properties.at(key)))
  //  my_properties[key] = property;
}	// setProperty
string Object::getProperties() {
    return(buildPropertyString(my_properties));
}	// getProperties
void Object::setProperties(string property_string) {
    my_properties = parseProperties(property_string);
}	// setProperties
