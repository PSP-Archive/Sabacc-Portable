/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: September 3, 2008
 *
 * File: FontState.cc
 *
 * Font infomation used for managing font states.
 */

// Standard Library headers
#include <string>
using std::string;

// SDL headers
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

// FreeType2 headers
#include <ft2build.h>
#include FT_FREETYPE_H

// SMF headers
#include "freetype/FontState.hh"
#include "utility/PropertyParser.hh"
#include "types/TypeDefinitions.hh"

// SMF Debugging headers
#if defined(_DEBUG) || defined(_DEBUGFONTSTATE)
#include <cstdio>
#include "utility/Log.hh"
#endif

FontState::FontState() : size(default_font_size), style(default_font_style),
    color(default_forecolor), stroke_color(default_forecolor), size_index(0), scale(1), ascent(0),
    descent(0), height(0), line_height(0), underline_offset(0), underline_height(1), italic_offset(0),
    bold_offset(0), font_properties() { }
FontState::FontState(int font_size, int font_style, SDL_Color font_color, string property_string) :
    size(font_size), style(font_style), color(font_color), stroke_color(default_forecolor),
    size_index(0), scale(1), ascent(0), descent(0), height(0), line_height(0),
    underline_offset(0), underline_height(1), italic_offset(0),
    bold_offset(0), font_properties(parseProperties(property_string)) { }
FontState::FontState(const FontState& src) :
    size(src.size), style(src.style), color(src.color), stroke_color(src.stroke_color),
    size_index(0), scale(src.scale), ascent(0), descent(0), height(0), line_height(0),
    underline_offset(0), underline_height(1), italic_offset(0),
    bold_offset(0), font_properties(src.font_properties) { }
FontState::~FontState() { }

FontState& FontState::operator=(const FontState& src)
{

  size = src.size;
  style = src.style;
  color = src.color;
  stroke_color = src.stroke_color;

  size_index = 0;

  scale = src.scale;

  ascent = descent = height = line_height = 0;
  underline_offset = 0;
  underline_height = 1;

  italic_offset = 0;
  bold_offset = 0;

  font_properties = src.font_properties;

  return(*this);
}	// operator=

int FontState::getSize()
{
  return(size);
}
void FontState::setSize(int font_size)
{
  size = font_size;

#if defined(_DEBUG) || defined(_DEBUGFONTSTATE)
  char debug_string[128];
  sprintf(debug_string, "Changing font size to %d.", size);
  logAppend(debug_string);
#endif

}
int FontState::getStyle()
{
  return(style);
}
void FontState::setStyle(int font_style)
{
  style = font_style;

#if defined(_DEBUG) || defined(_DEBUGFONTSTATE)
  char debug_string[128];
  sprintf(debug_string, "Changing font style to %d.", style);
  logAppend(debug_string);
#endif

}
string FontState::getProperty(string property_key)
{
  return(font_properties[property_key]);
}
void FontState::setProperty(string property_key, string property_value)
{
  font_properties[property_key] = property_value;

#if defined(_DEBUG) || defined(_DEBUGFONTSTATE)
  char debug_string[128];
  sprintf(debug_string, "Changing font property %s to %s.", property_key.c_str(), property_value.c_str());
  logAppend(debug_string);
#endif

}	// setProperty
const SDL_Color& FontState::getColor()
{
  return(color);
}
void FontState::setColor(SDL_Color new_color)
{
  color = new_color;
}
void FontState::setColor(int red, int green, int blue, int alpha)
{
  color.r = red;
  color.g = green;
  color.b = blue;
  color.unused = alpha;
}	// setColor
const SDL_Color& FontState::getStrokeColor()
{
  return(stroke_color);
}
void FontState::setStrokeColor(SDL_Color new_stroke_color)
{
  stroke_color = new_stroke_color;
}
void FontState::setStrokeColor(int red, int green, int blue, int alpha)
{
  stroke_color.r = red;
  stroke_color.g = green;
  stroke_color.b = blue;
  stroke_color.unused = alpha;
}	// setStrokeColor
string FontState::getProperties()
{
  return(buildPropertyString(font_properties));
}	// getProperties
void FontState::setProperties(string property_string)
{
  font_properties = parseProperties(property_string);

#if defined(_DEBUG) || defined(_DEBUGFONTSTATE)
  char debug_string[128];
  sprintf(debug_string, "Changing font properties to %s.", property_string.c_str());
  logAppend(debug_string);
#endif

}	// setProperties

int FontState::getSizeIndex()
{
  return(size_index);
}
void FontState::setSizeIndex(int new_index)
{
  size_index = new_index;
}
FT_Fixed FontState::getScale()
{
  return(scale);
}
void FontState::setScale(FT_Fixed new_scale)
{
  scale = new_scale;
}
int FontState::getAscent()
{
  return(ascent);
}
void FontState::setAscent(int new_ascent)
{
  ascent = new_ascent;
}
int FontState::getDescent()
{
  return(descent);
}
void FontState::setDescent(int new_descent)
{
  descent = new_descent;
}
int FontState::getHeight()
{
  return(height);
}
void FontState::setHeight(int new_height)
{
  height = new_height;
}
int FontState::getLineHeight()
{
  return(line_height);
}
void FontState::setLineHeight(int new_line_height)
{
  line_height = new_line_height;
}
int FontState::getUnderlineOffset()
{
  return(underline_offset);
}
void FontState::setUnderlineOffset(int new_offset)
{
  underline_offset = new_offset;
}
int FontState::getUnderlineHeight()
{
  return(underline_height);
}
void FontState::setUnderlineHeight(int new_underline_height)
{
  underline_height = new_underline_height;
}
float FontState::getItalicOffset()
{
  return(italic_offset);
}
void FontState::setItalicOffset(float new_offset)
{
  italic_offset = new_offset;
}
int FontState::getBoldOffset()
{
  return(bold_offset);
}
void FontState::setBoldOffset(int new_offset)
{
  bold_offset = new_offset;
}
