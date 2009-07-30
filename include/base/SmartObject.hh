#ifndef __SMF_SMART_OBJECT_HH
#define __SMF_SMART_OBJECT_HH

/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: August 26, 2008
 *
 * File: SmartObject.hh
 *
 * A utility class for automatic creation and deletion
 * of heap based objects.
 */

// Forward decl to avoid including Object here
class Object;

class SmartObject {

public:

    explicit SmartObject(Object* p = 0) : pointer_data(0) {
        if (p) pointer_data = new PointerData(p);
    }	// SmartObject

    ~SmartObject();
    SmartObject(const SmartObject&);
    SmartObject& operator=(const SmartObject&);

    // Make the following two functions const when COW fixed
    Object* operator->() const;
    Object& operator*() const;

    bool operator!() const;

    bool operator==(const SmartObject&);
    bool operator!=(const SmartObject&);

    Object* get() const;
    bool unique();

private:
    struct PointerData {

        explicit PointerData(Object* ptr = 0, unsigned int ct = 1) : pointer(ptr), count(ct) { }

        Object*		pointer;
        unsigned int		count;

    }	*pointer_data;

    void set(PointerData*);
    void release();


};	// class SmartObject

#endif // __SMF_SMART_OBJECT_HH
