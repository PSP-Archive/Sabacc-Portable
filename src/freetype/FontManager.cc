/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: September 2, 2008
 *
 * File: FontManager.cc
 *
 * A few functions for managing a single instance of a font.
 * This is not a manager in the sense of SystemManager and RenderManager
 * in that it is not a class.
 */

// SMF FreeType headers
#include "freetype/FontManager.hh"
#include "freetype/FontState.hh"
#include "freetype/FreeTypeRenderer.hh"

// PSP Glyph data
#include "freetype/fonts/PSPGlyphs.h"
#include "freetype/fonts/AureBesh.h"

FreeTypeRenderer& getSystemFont()
{
  static FreeTypeRenderer system_font;
  return(system_font);
}	// getSystemFont
FreeTypeRenderer& getPSPFont()
{
  static FreeTypeRenderer psp_font;
  psp_font.setFileStream(psp_glyph_font, size_psp_glyph_font);
  return(psp_font);
}
FreeTypeRenderer& getCardFont()
{
  static FreeTypeRenderer card_font;
  card_font.setFileStream(newaurebesh, size_newaurebesh);
  card_font.setSize(default_font_size * 0.75f);
  return(card_font);
}
const FontState& getDefaultFontState()
{
  static FontState default_state;
  return(default_state);
}	// getDefaultState
