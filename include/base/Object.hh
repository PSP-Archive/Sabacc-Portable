#ifndef __SMF_OBJECT_HH__
#define __SMF_OBJECT_HH__

/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: August 20, 2008
 *
 * File: Object.hh
 *
 * The definition of the object on which all interface
 * objects (widgets, etc) will be based.
 */

// Standard library headers
#include <std::string>


// SMF headers
#include "types/TypeDefinitions.hh"
#include "utility/ObjectUtilities.hh"
#include "utility/PropertyParser.hh"

class Object {

public:

    /** Constructors. @{ */
    Object();
    Object(std::string);
    Object(const Object&);
    virtual ~Object();
    /** @} */

    virtual Object& operator=(const Object&);

    /** Get the Object's ID */
    object_id_type getID();

    /** String based property related methods. @{ */
    virtual std::string getProperty(string);	/// Get property with key @param key.
    virtual void setProperty(std::string, string);	/// Set (or change) property with key @param key to @param value
    virtual std::string getProperties();	/// Get all properties in one string.
    virtual void setProperties(std::string);	/// Set (or create) properties from a string containing @c key: @c value pairs
    /** @} */

private:

    object_id_type	my_id;
    PropertyMap		my_properties;

}; // class Object

#endif // __SMF_OBJECT_HH__

