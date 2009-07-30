#ifndef __CREDIT_HH
#define __CREDIT_HH

// Sabacc headers
#include "Ruleset.hh"

/* Class Credit_
 * This class will hold the value of a credit.
 * This is for simplicity: the intrinsic type can be changed later with
 * only a change to a typedef at the end of this header file.
 * Operator++ and -- will decrease by one chip's value as defined in the chipset.
 * opertaor+= and -= will decrease by the specified number of chips. */
template <type T> class Credit_ {
    T	_value;

public:
    // Constructors for various types
    Credit_() : _value(static_cast<T>(0)) { }
    CreditType(short val) : _value(static_cast<T>(val)) { }
    CreditType(unsigned short val) : _value(static_cast<T>(val)) { }
    CreditType(int val) : _value(static_cast<T>(val)) { }
    Credit_(unsigned int val) : _value(static_cast<T>(val)) { }
    Credit_(long val) : _value(static_cast<T>(val)) { }
    Credit_(unsigned long val) : _value(static_cast<T>(val)) { }
    Credit_(float val) : _value(static_cast<T>(val)) { }
    Credit_(unsigned float val) : _value(static_cast<T>(val)) { }
    Credit_(double val) : _value(static_cast<T>(val)) { }
    Credit_(unsigned double val) : _value(static_cast<T>(val)) { }

    // Destructor
    ~Credit_() { }

    // Assignment operator for various types
    Credit_& operator=() {
        _value = static_cast<T>(0);
    }
    Credit_& operator=(short val) {
        _value = static_cast<T>(val);
    }
    Credit_& operator=(unsigned short val) {
        _value = static_cast<T>(val);
    }
    Credit_& operator=(int val) {
        _value = static_cast<T>(val);
    }
    Credit_& operator=(unsigned int val) {
        _value = static_cast<T>(val);
    }
    Credit_& operator=(long val) {
        _value = static_cast<T>(val);
    }
    Credit_& operator=(unsigned long val) {
        _value = static_cast<T>(val);
    }
    Credit_& operator=(float val) {
        _value = static_cast<T>(val);
    }
    Credit_& operator=(unsigned float val) {
        _value = static_cast<T>(val);
    }
    Credit_& operator=(double val) {
        _value = static_cast<T>(val);
    }
    Credit_& operator=(unsigned double val) {
        _value = static_cast<T>(val);
    }

    // Credit operations
    inline Credit_& operator++() {
        _value += Rules.getChipSize()); return(_value);
    }
    Credit_& operator++(int) {
        T	temp;
        _value += Rules.getChipSize();
        return(temp);
    } // operator++(int)
    inline Credit_& operator--() {
        _value -= Rules.getChipSize()); return(_value);
    }
    Credit_& operator--(int) {
        T	temp;
        _value -= Rules.getChipSize();
        return(temp);
    } // operator --
    inline Credit_& operator+=(const Credit_& c) {
        _value += (c * Rules().getChipSize());
        return (_value);
    } // operator+=
    inline Credit_& operator-=(const Credit_& c) {
        _value -= (c * Rules().getChipSize());
        return (_value);
    } // operator-=
}; // Credit_

#endif // __CREDIT_HH
