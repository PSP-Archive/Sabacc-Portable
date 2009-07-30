#ifndef __SMF_FREETYPERENDERERCONSTANTS_HH
#define __SMF_FREETYPERENDERERCONSTANTS_HH

/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: September 3, 2008
 *
 * File: FreeTypeRendererConstants.hh
 *
 * Some constants for the FreeType renderer.
 */

// Standard Library headers
#include <std::string>


/** Constant for bitmap renering. */
static const unsigned short num_greys = 256;

/** Constants defining defaults. @{ */
static const std::string default_font_file = "SabaccUIFont.ttf";
static const int default_font_size = 12;
static const int default_font_style = 0;
static const std::string default_font_properties = "justify: left;";

/** Style constants @{ */
static const short font_style_normal = 0;
static const short font_style_bold = 1;
static const short font_style_italic = 2;
static const short font_style_underline = 4;
static const short font_style_strikethrough = 8;
static const short font_style_outline = 16;
static const short font_style_stroked = 32;
/** @} */

#endif // __SMF_FREETYPERENDERERCONSTANTS_HH
