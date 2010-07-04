#ifndef __SMF_SINGLETON_HH
#define __SMF_SINGLETON_HH

/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 26, 2008
 *
 * File: StringTokenizer.cc
 *
 * A class for creating only one instance of another
 *
 * ToDo: Make thread safe
 */

template <class S> class Singleton
  {
  private:
    // Make ctor/dtor/assn inaccessable
    Singleton();
    ~Singleton();
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);

  public:
    static S& instance()
    {
      // Returns a reference to the instance of the class
      static S s_instance;
      return(s_instance);
    } // instance

  }; // template class Singleton

#endif	// __SMF_SINGLETON_HH
