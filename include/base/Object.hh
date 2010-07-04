#ifndef __SMF_OBJECT_HH__
#define __SMF_OBJECT_HH__

/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 20, 2008
 *
 * File: Object.hh
 *
 * The definition of the object on which all interface
 * objects (widgets, etc) will be based.
 */

// Standard library headers
#include <string>
using std::string;

// SMF headers
#include "types/TypeDefinitions.hh"
#include "utility/ObjectUtilities.hh"
#include "utility/PropertyParser.hh"

class Object
  {

  public:

    /** Constructors. @{ */
    Object();
    Object(string);
    Object(const Object&);
    virtual ~Object();
    /** @} */

    virtual Object& operator=(const Object&);

    /** Get the Object's ID */
    object_id_type getID();

    /** String based property related methods. @{ */
    virtual string getProperty(string);	/// Get property with key @param key.
    virtual void setProperty(string, string);	/// Set (or change) property with key @param key to @param value
    virtual string getProperties();	/// Get all properties in one string.
    virtual void setProperties(string);	/// Set (or create) properties from a string containing @c key: @c value pairs
    /** @} */

  private:

    object_id_type	my_id;
    PropertyMap		my_properties;

  }; // class Object

#endif // __SMF_OBJECT_HH__

