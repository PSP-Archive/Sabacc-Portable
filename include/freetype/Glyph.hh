#ifndef __SMF_GLYPH_HH
#define __SMF_GLYPH_HH

/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 30, 2008
 *
 * File: Glyph.hh
 *
 * The SmartGlyph implementation, automatic creation and management
 * of glyph bitmaps.
 */

// Standard library headers
#include <iostream>
#include <stdexcept>

// FreeType2 headers
#include <ft2build.h>
#include FT_FREETYPE_H

// Debug headers
#if defined(_DEBUG) || defined(_DEBUGCACHEDGLYPH)
#include "utility/Log.hh"
#endif

struct CachedGlyphImpl {

    FT_Int							index;

    FT_Bitmap						bitmap;

    FT_Glyph_Metrics				metrics;
    FT_Vector						advance;

  FT_Int x_min, x_max, x_adv, y_min, y_max, y_offset;

    CachedGlyphImpl();
    CachedGlyphImpl(const CachedGlyphImpl&);
    ~CachedGlyphImpl();

    CachedGlyphImpl& operator=(const CachedGlyphImpl&);

    bool operator==(const CachedGlyphImpl&);
    bool operator!=(const CachedGlyphImpl&);

    unsigned char* allocGlyph(size_t);

};

// Sabacc Media Framework header which must be included at end.
#include "freetype/CachedGlyph.hh"

#endif // __GLYPH_HH
