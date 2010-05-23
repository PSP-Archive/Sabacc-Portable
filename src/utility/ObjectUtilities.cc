/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 20, 2008
 *
 * File: ObjectUtilities.cc
 *
 * Utility functions mostly used with the base Object class.
 */

// SMF headers
#include "utility/ObjectUtilities.hh"

object_id_type generateObjectID() {
    static object_id_type new_object_id = 0;
    return(++new_object_id);
} // generateObjectID
