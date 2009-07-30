#ifndef __SMF_SMARTPOINTER_HH
#define __SMF_SMARTPOINTER_HH

/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: October 8, 2008
 *
 * File: SmartPointer.hh
 *
 * A smart pointer implementation, utilizing copy-on-write.
 * The #1 purpose is to replace the CachedGlyph class, hopefully it will succeed,
 */

template<class C> class SmartPointer 
{
public:
  typedef C ElementType;
  typedef C* PointerType;
  typedef C& ReferenceType;
  
  explicit SmartPointer(PointerType new_ptr = 0) : pointer_data(0) 
  {
    if(new_ptr) pointer_data = new PointerImpl(new_ptr);
  }
  ~SmartPointer();
  
#endif // __SMF_SMARTPOINTER_HH
