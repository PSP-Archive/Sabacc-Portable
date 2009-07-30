#ifndef SMF_FONTMANAGER_HH
#define SMF_FONTMANAGER_HH

/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: September 2, 2008
 *
 * File: FontManager.hh
 *
 * A few functions for managing a single instance of a font.
 * This is not a manager in the sense of SystemManager and RenderManager
 * in that it is not a class.
 */

// SMF FreeType headers
#include "freetype/FontState.hh"
#include "freetype/FreeTypeRenderer.hh"

FreeTypeRenderer& getSystemFont();
FreeTypeRenderer& getPSPFont();
FreeTypeRenderer& getCardFont();
const FontState& getDefaultFontState();

#endif // SMF_FONTMANAGER_HH
