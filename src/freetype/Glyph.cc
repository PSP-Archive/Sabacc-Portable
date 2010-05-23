/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 30, 2008
 *
 * File: Object.hh
 *
 * A SmartGlyph class for near automatic management of glyph bitmaps.
 */

// Standard library headers
#include <iostream>
#include <stdexcept>
using std::bad_alloc;
using std::cerr;
using std::cout;
using std::endl;

// SMF headers
#include "freetype/Glyph.hh"

#if defined(_DEBUG) || defined(_DEBUGCACHEDGLYPH)
#include <cstdio>
#include "utility/Log.hh"
#endif

CachedGlyphImpl::CachedGlyphImpl() : index(0), bitmap(), metrics(), advance(),
				     x_min(0), x_max(0), x_adv(0), y_min(0), y_max(0), y_offset(0) {

#if defined(_DEBUGCACHEDGLYPH)
    logAppend("Constructed from nothing.");
#endif

}
CachedGlyphImpl::CachedGlyphImpl(const CachedGlyphImpl& src) : index(src.index), bitmap(src.bitmap),
							       metrics(src.metrics), advance(src.advance), x_min(src.x_min), x_max(src.x_max), x_adv(src.x_adv), y_min(src.y_min), y_max(src.y_max), y_offset(src.y_offset) {

#if defined(_DEBUGCACHEDGLYPH)
    logAppend("Constructed via copy.");
#endif

}

CachedGlyphImpl::~CachedGlyphImpl() {

    if (bitmap.buffer) {

      delete[](bitmap.buffer); // deleted via the smart pointer wrapper

#if defined(_DEBUG) || defined(_DEBUGCACHEDGLYPH)
        char debug_string[128];
        sprintf(debug_string, "Buffer of %d @ 0x%x freed.", index, bitmap.buffer);
        logAppend(debug_string);
#endif

    }

}	// ~CachedGlyphImpl

CachedGlyphImpl& CachedGlyphImpl::operator=(const CachedGlyphImpl& src) {

    index = src.index;
    bitmap = src.bitmap;
    metrics = src.metrics;
    advance = src.advance;

    return(*this);

}
bool CachedGlyphImpl::operator==(const CachedGlyphImpl& rhs) {

    if ((rhs.index == this->index) && (rhs.bitmap.buffer == this->bitmap.buffer)) {
        return(true);
    }

    return(false);

}	// operator==

bool CachedGlyphImpl::operator!=(const CachedGlyphImpl& rhs) {

    return(!operator==(rhs));

}	// operator!=

unsigned char* CachedGlyphImpl::allocGlyph(size_t size) {

  try {
    bitmap.buffer = new unsigned char[size];
  } catch(bad_alloc& a) {

    cout << "bad_alloc thrown while allocating glyph memory." << endl;
    cerr << "bad_alloc thrown while allocating glyph memory." << endl;

#if defined(_DEBUG) || defined(_DEBUGCACHEDGLYPH)
    logAppend("bad_alloc thrown while allocating glyph memory.");
#endif

  }

#if defined(_DEBUGCACHEDGLYPH)
    char debug_string[128];
    sprintf(debug_string, "Allocated buffer 0x%x.", bitmap.buffer);
    logAppend(debug_string);
#endif

    return(bitmap.buffer);

}	// allocGlyph
