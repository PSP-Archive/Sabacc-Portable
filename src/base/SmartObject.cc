/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: August 26, 2008
 *
 * File: SmartObject.cc
 *
 * A utility class for automatic creation and deletion
 * of heap based objects.
 */

// SMF headers
#include "base/Object.hh"
#include "base/SmartObject.hh"

SmartObject::~SmartObject() {
    release();
}	// ~SmartObject
SmartObject::SmartObject(const SmartObject& src) : pointer_data(0) {
    set(src.pointer_data);
}	// SmartObject copy
SmartObject& SmartObject::operator=(const SmartObject& src) {

    if (this != &src) {

        release();
        set(src.pointer_data);

    }

    return(*this);
}

Object* SmartObject::operator->() const {
    return(pointer_data->pointer);
}	// operator->
Object& SmartObject::operator*() const {
    return(*pointer_data->pointer);
}	// operator*

bool SmartObject::operator!() const {
    if (!pointer_data) return true;

    return(0 == pointer_data->pointer);
}

bool SmartObject::operator==(const SmartObject& rhs) {

    return(rhs.pointer_data == this->pointer_data);

}
bool SmartObject::operator!=(const SmartObject& rhs) {

    return(!operator==(rhs));

}

Object* SmartObject::get() const {
    return(pointer_data->pointer);
}	// get
bool SmartObject::unique() {
    return ((pointer_data) ? (pointer_data->count == 1) : true);
}	// unique

void SmartObject::set(PointerData* p) {

    pointer_data = p;
    if (p) ++p->count;

}	// set
void SmartObject::release() {

    if (pointer_data) {

        if (0 == --pointer_data->count) {

            delete(pointer_data->pointer);
            delete(pointer_data);

        }	// if(0)

    }	// if(pointer_data)

}	// release
