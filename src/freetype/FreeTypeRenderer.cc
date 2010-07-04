/*
 * Sabacc Media Framework
 *
s * Author: Anthony Thomasel
 * Date: September 5, 2008
 *
 * File: FreeTypeRenderer.cc
 *
 * The main FreeType text rendering class.
 */

// Standard Library headers
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>
using std::string;
using std::stringstream;
using std::vector;

// SDL headers
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

// FreeType2 headers
#include <ft2build.h>

#if (_PSP_FW_VERSION >= 100)
#include <freetype/internal/ftobjs.h>
#endif

#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_STROKER_H
#include FT_SYNTHESIS_H

// #define FT_FLOOR(X)	((X & -64) / 64)
// #define FT_CEIL(X)	(((X + 63) & -64) / 64)

inline int ftFloor(int value)
{
  return((value & -64) / 64);
}
inline int ftCeil(int value)
{
  return(((value + 63) & -64) / 64);
}

// SMF headers
#include "types/TypeDefinitions.hh"
#include "utility/StringTokenizer.hh"

// SMF FreeType headers
#include "constants/FreeTypeRendererConstants.hh"
#include "freetype/FontState.hh"
#include "freetype/FreeTypeRenderer.hh"
#include "freetype/Glyph.hh"

// SMF default font
#include "freetype/fonts/DejaVuSans.h"

// SMF Exceptions
#include "exceptions/font/FreeTypeExceptions.hh"

// SMF Managers
#include "system/SystemManager.hh"

// SMF Debugging headers
#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERER) || defined(_DEBUGFREETYPEPOSITIONING) || defined(_DEBUGGLYPHCACHE) || defined(_DEBUGFREETYPEALPHA)
#include "utility/Log.hh"
#endif

FreeTypeRenderer::FreeTypeRenderer() :
    freetype2(), load_flags(FT_LOAD_DEFAULT), typeface(), font_data(NULL),
    lib_open(false), lib_initialized(false), using_stream(false), result(0), current_state(), states(), glyph_cache(), stroke_cache() { }
FreeTypeRenderer::FreeTypeRenderer(const FontState& state) :
    freetype2(), load_flags(FT_LOAD_DEFAULT), typeface(), font_data(NULL),
    lib_open(false), lib_initialized(false), using_stream(false), result(0), current_state(state), states(), glyph_cache(), stroke_cache() { }
FreeTypeRenderer::FreeTypeRenderer(const string& font_file, int font_size, int font_style, SDL_Color color, const string& property_string) :
    freetype2(), load_flags(FT_LOAD_DEFAULT), typeface(), font_data(NULL),
    lib_open(false), lib_initialized(false), using_stream(false), result(0), current_state(font_size, font_style, color, property_string),
    states(), glyph_cache(), stroke_cache() { }
FreeTypeRenderer::FreeTypeRenderer(const char* font_file, int font_size, int font_style, SDL_Color color, const string& property_string) :
    freetype2(), load_flags(FT_LOAD_DEFAULT), typeface(), font_data(NULL),
    lib_open(false), lib_initialized(false), using_stream(false), result(0), current_state(font_size, font_style, color, property_string),
    states(), glyph_cache(), stroke_cache() { }
FreeTypeRenderer::FreeTypeRenderer(const FreeTypeRenderer& src) :
    freetype2(), load_flags(FT_LOAD_DEFAULT), typeface(), font_data(NULL),
    lib_open(false), lib_initialized(false), using_stream(false), result(0), current_state(src.current_state), states(),
    glyph_cache(), stroke_cache() { }
FreeTypeRenderer::~FreeTypeRenderer()
{
  cleanup();
  close();
}	// ~FreeTypeRenderer

FreeTypeRenderer& FreeTypeRenderer::operator=(const FreeTypeRenderer& src)
{
  cleanup();

  current_state = src.current_state;
  freetype2 = FT_Library();

  init();

  return(*this);
}	// operator=
bool FreeTypeRenderer::operator<(const FreeTypeRenderer& other)
{
  // return(other.current_state.getSize() < getSize());
  return(false);
}	// operator<
bool FreeTypeRenderer::operator>(const FreeTypeRenderer& other)
{
  // return(other.current_state.getSize() > getSize());
  return(false);
}	// operator>

void FreeTypeRenderer::setFileStream(FT_Byte* stream_data, int stream_size)
{
  initFromStream(stream_data, stream_size);
}
string FreeTypeRenderer::getProperty(string property_key)
{
  return(current_state.getProperty(property_key));
}	// getProperty
void FreeTypeRenderer::setProperty(string property_key, string property_value)
{
  current_state.setProperty(property_key, property_value);
}	// setProperty
string FreeTypeRenderer::getProperties()
{
  return(current_state.getProperties());
}	// getProperties
void FreeTypeRenderer::setProperties(string property_string)
{
  current_state.setProperties(property_string);
}	// setProperties

int FreeTypeRenderer::getSize()
{
  return(current_state.getSize());
}
void FreeTypeRenderer::setSize(int font_size)
{

  if (!lib_initialized) init();

  current_state.setSize(font_size);

  if (FT_IS_SCALABLE(typeface))
    {
      if (FT_Set_Char_Size(typeface, 0, current_state.getSize() * 64, 0, 0))
        {
          throw(Exceptions::FreeType::GeneralError(__FILE__, "Could not set font size."));
        }	// if(FT_Set_Char_Size)

      // Scalable font informations
      current_state.setScale(typeface->size->metrics.y_scale);
      current_state.setAscent(ftCeil(FT_MulFix(typeface->bbox.yMax, current_state.getScale())));
      current_state.setDescent(ftCeil(FT_MulFix(typeface->bbox.yMin, current_state.getScale())));
      current_state.setHeight(current_state.getAscent() - current_state.getDescent() + 1);
      current_state.setLineHeight(ftCeil(FT_MulFix(typeface->height, current_state.getScale())));
      current_state.setUnderlineOffset(ftFloor(FT_MulFix(typeface->underline_position, current_state.getScale())));

      int underline_height = (0 < ftFloor(FT_MulFix(typeface->underline_thickness, current_state.getScale()))) ? ftFloor(FT_MulFix(typeface->underline_thickness, current_state.getScale())) : 1;
      current_state.setUnderlineHeight(underline_height);

    }
  else
    {

      if (current_state.getSize() >= typeface->num_fixed_sizes)
        {
          current_state.setSize(typeface->num_fixed_sizes - 1);
        }	// if(current_state.size >= num_sizes)

      current_state.setSizeIndex(current_state.getSize());

      if (FT_Set_Pixel_Sizes(typeface, typeface->available_sizes[current_state.getSize()].height, typeface->available_sizes[current_state.getSize()].width))
        {
          throw(Exceptions::FreeType::GeneralError(__FILE__, "Unable to get fixed size font information."));
        }	// if(FT_Set_Pixel_Sizes)

      current_state.setAscent(typeface->available_sizes[current_state.getSize()].height);
      current_state.setDescent(0);
      current_state.setHeight(typeface->available_sizes[current_state.getSize()].height);
      current_state.setLineHeight(ftCeil(current_state.getHeight()));
      current_state.setUnderlineOffset(ftFloor(typeface->underline_position));
      current_state.setUnderlineHeight(ftFloor(typeface->underline_thickness));

    }	// if(FT_IS_SCALABLE)

  /* x offset = cos(((90.0-12)/360)*2*M_PI), or 12 degree angle */
  current_state.setItalicOffset(0.207f * current_state.getHeight());
  current_state.setBoldOffset(typeface->size->metrics.y_ppem / 10);

  clearCache();
}	// setSize
int FreeTypeRenderer::getStyle()
{
  return(current_state.getStyle());
}
void FreeTypeRenderer::setStyle(int font_style)
{
  current_state.setStyle(font_style);

  clearCache();
}	// setStyle
const SDL_Color& FreeTypeRenderer::getColor()
{
  return(current_state.getColor());
}
void FreeTypeRenderer::setColor(SDL_Color font_color)
{
  current_state.setColor(font_color);
}
void FreeTypeRenderer::setColor(int red, int green, int blue, int alpha)
{
  SDL_Color font_color =
  {
    static_cast<Uint8>(red),
    static_cast<Uint8>(green),
    static_cast<Uint8>(blue),
    static_cast<Uint8>(alpha)
  };
  current_state.setColor(font_color);
}	// setColor
const SDL_Color& FreeTypeRenderer::getStrokeColor()
{
  return(current_state.getStrokeColor());
}
void FreeTypeRenderer::setStrokeColor(SDL_Color font_color)
{
  current_state.setStrokeColor(font_color);
}
void FreeTypeRenderer::setStrokeColor(int red, int green, int blue, int alpha)
{
  SDL_Color font_color =
  {
    static_cast<Uint8>(red),
    static_cast<Uint8>(green),
    static_cast<Uint8>(blue),
    static_cast<Uint8>(alpha)
  };
  current_state.setStrokeColor(font_color);
}	// setColor

int FreeTypeRenderer::getAscent()
{
  return(current_state.getAscent());
}
int FreeTypeRenderer::getDescent()
{
  return(current_state.getDescent());
}
int FreeTypeRenderer::getFontHeight()
{
  return(current_state.getHeight());
}
int FreeTypeRenderer::getLineHeight()
{
  return(current_state.getLineHeight());
}

void FreeTypeRenderer::pushState()
{
#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERER)
  logAppend("Stored state.");
#endif
  states.push_back(current_state);
}	// pushState
void FreeTypeRenderer::popState()
{

  if (!states.empty())
    {

      clearCache();

      FontState new_state = states.back();
      states.pop_back();

      clearCache();

      setSize(new_state.getSize());
      setStyle(new_state.getStyle());
      setColor(new_state.getColor());
      setProperties(new_state.getProperties());

    }
  else
    {

#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERER)
      logAppend("No state to restore.");
#endif
    }	// if(states.empty()

}	// popState
void FreeTypeRenderer::clearStates()
{

  states.clear();

#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERER)
  logAppend("States clear.");
#endif

}	// clearStates
const FontState& FreeTypeRenderer::getState()
{
  return(current_state);
}
void FreeTypeRenderer::setState(FontState new_state)
{
  if (&new_state != &current_state)
    {

      clearCache();

#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERER)
      logAppend("Setting state via setState(...)");
#endif

      setSize(new_state.getSize());
      setStyle(new_state.getStyle());
      setColor(new_state.getColor());
      setProperties(new_state.getProperties());

    }	// if(!=)
}	// setState

int FreeTypeRenderer::lineWidth(const string& text)
{

  if (!lib_initialized) init();

  unsigned int line_width = 0;
  unsigned int previous_index = 0;

  FT_Bool use_kerning = FT_HAS_KERNING(typeface);

  /* Save previous state to work with, this way we don't permanently
   * change the state. */
  pushState();

  // For keeping track of formatting characters
  bool formatting_active = false;
  string format_string = "";

  for (string::const_iterator char_iter = text.begin(); text.end() != char_iter; ++char_iter)
    {
      // Process formatting characters
      if ('\\' == *char_iter)
        {
          formatting_active = true;
          format_string = "";

#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERER)
          logAppend("Activating formatting of text.");
#endif

          continue;
        }

      if (formatting_active)
        {
          if ('b' == *char_iter)  	// bold
            {

              setStyle(getStyle() ^ font_style_bold);

            }
          else if ('i' == *char_iter)  	// italic
            {

              setStyle(getStyle() ^ font_style_italic);

            }
          else if ('u' == *char_iter)  	// underline
            {

              setStyle(getStyle() ^ font_style_underline);

            }
          else if ('s' == *char_iter)  	// strikethrough
            {

              setStyle(getStyle() ^ font_style_strikethrough);

            }
          else if ('c' == *char_iter)  	// color
            {

              if (text.end() > char_iter + 6)
                {
                  char_iter += 6;
                }
              else if (text.end() > char_iter + 3)
                {
                  char_iter += 3;
                }	// if(determined len gth of color format string)

            }

          formatting_active = false;

          continue;

        }	// if(formatting_active)

      CachedGlyph glyph = loadGlyph(*char_iter);

      if (use_kerning && previous_index && glyph->index)
        {
          FT_Vector delta;
          FT_Get_Kerning(typeface, previous_index, glyph->index, FT_KERNING_DEFAULT, &delta);
          line_width += delta.x >> 6;
        }	// if(use_kerning)

      line_width += glyph->advance.x >> 6;

      previous_index = glyph->index;

    }	// for(char_iter)

  // Restore previous state
  popState();

  return(line_width);
}	// lineWidth
int FreeTypeRenderer::lineWidth(const char* text)
{
  return(lineWidth(string(text)));
}	// lineWidth
int FreeTypeRenderer::textWidth(const string& text)
{
  int largest_line = 0;

  StringTokenizer tokens(text);

  while (!tokens.end())
    {

      int line_length = lineWidth(tokens.next());
      if (line_length > largest_line) largest_line = line_length;

#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERER)
      char debug_string[128];
      sprintf(debug_string, "Line = %d.", line_length);
#endif

    }	// while

#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERER)
  char debug_string[128];
  sprintf(debug_string, "Text width = longest line = %d.", largest_line);
#endif

  return(largest_line);

}	// textWidth
int FreeTypeRenderer::textWidth(const char* text)
{
  return(textWidth(string(text)));
}	// textWidth
int FreeTypeRenderer::textHeight(const string& text)
{

  if (!lib_initialized) init();

  StringTokenizer tokens(text);

  int text_height = (tokens.size() * current_state.getLineHeight()) - current_state.getDescent();

#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERER)
  char debug_string[128];
  sprintf(debug_string, "Text height = %d.", text_height);
#endif

  return(text_height);

}	// textHeight
int FreeTypeRenderer::textHeight(const char* text)
{
  return(textHeight(string(text)));
}	// textHeight

SmartSurface FreeTypeRenderer::render(const string& text)
{
  int surface_width = textWidth(text) + 3;
  int surface_height = textHeight(text) + 1;

  SDL_Surface* temp_surface = SDL_AllocSurface(SDL_SWSURFACE, surface_width, surface_height,
                              32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

  render(text, temp_surface, 0, 0);

  return(SmartSurface(temp_surface));
}
SmartSurface FreeTypeRenderer::render(const char* text)
{
  return(render(string(text)));
}	// render(const char*)

void FreeTypeRenderer::render(const string& text, SDL_Surface* destination, int dx, int dy)
{

#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERER) || defined(_DEBUGFREETYPEPOSITIONING)
  char debug_string[128];
#endif

  if (!destination)
    {

#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERER)
      logAppend("Render(s, s, i, i): NULL surface passed.");
#endif

      throw(Exceptions::FreeType::Render(__FILE__, "Can not render to a null surface."));

    }	// if(!destination)

  if (!lib_initialized) init();

  /* Save previous state to work with, this way we don't permanently
   * change the state. */
  pushState();

  // Get surface dimensions for future reference
  int surface_width = destination->w;
  int surface_height = destination->h;

#if defined(_DEBUG) || defined(_DEBUGFREETYPEPOSITIONING)
  sprintf(debug_string, "Drawing '%s' with a surface of %d,%d %dwx%dh", text.substr(0, 64).c_str(), dx, dy, surface_width, surface_height);
  logAppend(debug_string);
#endif

  // For kerning measurement
  unsigned int previous_index = 0;
  FT_Bool use_kerning = FT_HAS_KERNING(typeface);

  // Color can change, but we con't want to change that of the state, so save it here
  SDL_Color draw_color = current_state.getColor();
  SDL_Color previous_color = current_state.getColor();

  // Drawing location
  string justification = getProperty("justify");

  int y_pos = dy;

#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERERJUSTIFICATION)
  sprintf(debug_string, "J'st'n values: %d %d %d %d.", surface_width, surface_height, textWidth(text), textHeight(text));
  logAppend(debug_string);
#endif

  if (string::npos != justification.find("center"))
    {

      y_pos = (surface_height > textHeight(text)) ? dy + ((surface_height - textHeight(text)) >> 1) : 0;

#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERER)
      sprintf(debug_string, "Centered vertically with vertical offset of %d.", dy);
      logAppend(debug_string);
#endif

    }
  else if (string::npos != justification.find("bottom"))
    {

      y_pos = (surface_height > textHeight(text)) ? dy + (surface_height - textHeight(text)) : 0;

    }	// vertical justification

  // For keeping track of formatting characters
  bool formatting_active = false;
  string format_string = "";

  StringTokenizer tokens(text);

  while (!tokens.end())
    {

      string line = tokens.next();

      // Set horizontal drawing position, accounting for justification
      int x_pos = dx;

      if (string::npos != justification.find("center"))
        {

#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERER)
          logAppend("Justifying text: center.");
#endif

          x_pos = (surface_width - lineWidth(line)) >> 1;

        }
      else if (string::npos != justification.find("right"))
        {

#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERER)
          logAppend("Justifying text: right.");
#endif

          x_pos = surface_width - lineWidth(line);
        }	// if(justification)

#if defined(_DEBUG) || defined(_DEBUGFREETYPEPOSITIONING)
      sprintf(debug_string, "Drawing '%s' @ %d,%d", text.substr(0, 64).c_str(), x_pos, y_pos);
      logAppend(debug_string);
#endif


      for (string::const_iterator char_iter = line.begin(); line.end() != char_iter; ++char_iter)
        {
          // Process formatting characters
          if ('\\' == *char_iter)
            {
              formatting_active = true;
              format_string = "";

#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERER)
              logAppend("Activating formatting of text.");
#endif

              continue;
            }

          if (formatting_active)
            {
              if ('b' == *char_iter)  	// bold
                {

                  setStyle(getStyle() ^ font_style_bold);

                }
              else if ('i' == *char_iter)  	// italic
                {

                  setStyle(getStyle() ^ font_style_italic);

                }
              else if ('u' == *char_iter)  	// underline
                {

                  setStyle(getStyle() ^ font_style_underline);

                }
              else if ('s' == *char_iter)  	// strikethrough
                {

                  setStyle(getStyle() ^ font_style_strikethrough);

                }
              else if ('c' == *char_iter)  	// color
                {

                  /* Check that we don't go past the end of the string trying
                   * to grab the color code. */
                  if (line.end() > (char_iter + 6))
                    {

                      int red = 0;
                      int green = 0;
                      int blue = 0;

                      stringstream red_stream;
                      red_stream << *(char_iter + 1) << *(char_iter + 2);
                      red_stream >> std::hex >> red;

#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERER)
                      sprintf(debug_string, "Red: String: %s, value: %d.", red_stream.str().c_str(), red);
                      logAppend(debug_string);
#endif


                      stringstream green_stream;
                      green_stream << *(char_iter + 3) << *(char_iter + 4);
                      green_stream >> std::hex >> green;
#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERER)
                      sprintf(debug_string, "Green: String: %s, value: %d.", green_stream.str().c_str(),green);
                      logAppend(debug_string);
#endif

                      stringstream blue_stream;
                      blue_stream << *(char_iter + 5) << *(char_iter + 6);
                      blue_stream >> std::hex >> blue;
#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERER)
                      sprintf(debug_string, "Blue: String: %s, value: %d.", blue_stream.str().c_str(), blue);
                      logAppend(debug_string);
#endif

                      previous_color = draw_color;

                      draw_color.r = red;
                      draw_color.g = green;
                      draw_color.b = blue;

                      char_iter += 6;

                    }
                  else if ('C' == *char_iter)
                    {

                      // Uppercase C cancels color formatting
                      draw_color = current_state.getColor();

                    }	// check formatting character

                }	// check for formatting

              formatting_active = false;

              continue;

            }	// if(formatting_active)

          CachedGlyph glyph = loadGlyph(*char_iter);


          // Adjust for freetype sizing error
          int width = glyph->bitmap.width;
          /* if(width > (glyph->x_max - glyph->x_min))
            {
          width = (glyph->x_max - glyph->x_min);
          } */

          if (use_kerning && previous_index && glyph->index)
            {
              FT_Vector delta;
              FT_Get_Kerning(typeface, previous_index, glyph->index, FT_KERNING_DEFAULT, &delta);
              // x_pos += delta.x >> 6;
            }	// if(use_kerning)

          // Correct wraparound
          if (text.begin() == char_iter)
            {
              x_pos -= glyph->x_min;
            }

          // Draw the glyph to the surface
          int bpp = destination->format->BytesPerPixel;
          Uint32* dst_check = static_cast<Uint32*>(destination->pixels) + destination->pitch/bpp * destination->h;
          Uint32 pixel_value = SDL_MapRGB(destination->format, draw_color.r, draw_color.g, draw_color.b);

          if (SDL_MUSTLOCK(destination)) SDL_LockSurface(destination);

          // For computing drawing offset if using stroke
          int stroke_width = 0;
          int stroke_height = 0;

          if ((current_state.getStyle() & font_style_stroked) ||
              (current_state.getStyle() & font_style_outline))
            {

#if defined(_DEBUGFREETYPERENDERER)
              logAppend("Drawing test with stroke.");
#endif// _DEBUGFREETYPERENDERER

              CachedGlyph str_glyph = loadStrokedGlyph(*char_iter);
              stroke_width = str_glyph->bitmap.width + 1;
              stroke_height = str_glyph->bitmap.rows;

              // Copy stroke first

              SDL_Color stroke_color = current_state.getStrokeColor();

              for (int row = 0; str_glyph->bitmap.rows > row; ++row)
                {

                  Uint8* src = static_cast<Uint8*>(str_glyph->bitmap.buffer) + str_glyph->bitmap.pitch * row;
                  Uint32* dst = static_cast<Uint32*>(destination->pixels) + (row + y_pos + (current_state.getAscent() - ftFloor(glyph->metrics.horiBearingY))) * destination->pitch/bpp + (x_pos); // + glyph->x_min);

                  for (int col = 0; (str_glyph->bitmap.width > col) && (dst < dst_check); ++col)
                    {

                      Uint8 dst_red, dst_green, dst_blue, dst_alpha;

                      SDL_GetRGBA(*dst, destination->format, &dst_red, &dst_green, &dst_blue, &dst_alpha);

                      Uint8 src_alpha = *src++;

                      float src_a_ratio = (src_alpha / 256.0f);
                      float dst_a_ratio = (dst_alpha / 256.0f);

                      float alpha_0 = src_a_ratio + (dst_a_ratio * (1 - src_a_ratio));

                      Uint8 red = ((stroke_color.r * src_a_ratio) + ((dst_red * dst_a_ratio) * (1 - src_a_ratio))) / alpha_0;
                      Uint8 green = ((stroke_color.g * src_a_ratio) + ((dst_green * dst_a_ratio) * (1 - src_a_ratio))) / alpha_0;
                      Uint8 blue = ((stroke_color.b * src_a_ratio) + ((dst_blue * dst_a_ratio) * (1 - src_a_ratio))) / alpha_0;

                      Uint32 pixel_value = SDL_MapRGBA(destination->format, red, green, blue, alpha_0 * 256);

#if defined(_DEBUGFREETYPERENDERER)
                      char alpha_debug[256];	// Should be enough room
                      sprintf(alpha_debug, "Stroked: S%x,%x,%x,%x D%x,%x,%x,%x SR%2f DR%2f PV%x",
                              stroke_color.r, stroke_color.g, stroke_color.b, src_alpha, dst_red, dst_green, dst_blue, dst_alpha,
                              src_a_ratio, dst_a_ratio, pixel_value);
                      logAppend(alpha_debug);
#endif

                      *dst++ = pixel_value;

                    }	// col

                }	// row

            }	// (font_style_stroked)

          int stroke_offset_x = 0;
          int stroke_offset_y = 0;

          if (current_state.getStyle() & font_style_stroked)
            {
              stroke_offset_x = (stroke_width - glyph->bitmap.width) / 2;
              stroke_offset_y = (stroke_height - glyph->bitmap.rows) / 2;
            }

          if (!(current_state.getStyle() & font_style_outline))
            {
              for (int row = 0; glyph->bitmap.rows > row; ++row)
                {

                  Uint8* src = static_cast<Uint8*>(glyph->bitmap.buffer) + glyph->bitmap.pitch * row;
                  Uint32* dst = static_cast<Uint32*>(destination->pixels) + (row + (y_pos + stroke_offset_y) + (current_state.getAscent() - ftFloor(glyph->metrics.horiBearingY))) * destination->pitch/bpp + (x_pos + glyph->x_min + stroke_offset_x);

                  for (int col = 0; (width > col) && (dst < dst_check); ++col)
                    {

                      Uint8 dst_red, dst_green, dst_blue, dst_alpha;

                      SDL_GetRGBA(*dst, destination->format, &dst_red, &dst_green, &dst_blue, &dst_alpha);

                      Uint8 src_alpha = *src++;

                      float src_a_ratio = (src_alpha / 256.0f);
                      float dst_a_ratio = (dst_alpha / 256.0f);

                      float alpha_0 = src_a_ratio + (dst_a_ratio * (1 - src_a_ratio));

                      Uint8 red = ((draw_color.r * src_a_ratio) + ((dst_red * dst_a_ratio) * (1 - src_a_ratio))) / alpha_0;
                      Uint8 green = ((draw_color.g * src_a_ratio) + ((dst_green * dst_a_ratio) * (1 - src_a_ratio))) / alpha_0;
                      Uint8 blue = ((draw_color.b * src_a_ratio) + ((dst_blue * dst_a_ratio) * (1 - src_a_ratio))) / alpha_0;

                      Uint32 pixel_value = SDL_MapRGBA(destination->format, red, green, blue, alpha_0 * 256);

#if defined(_DEBUGFREETYPEALPHA)
                      char alpha_debug[256];	// Should be enough room
                      sprintf(alpha_debug, "Norm: S%x,%x,%x,%x D%x,%x,%x,%x SR%2f DR%2f PV%x",
                              draw_color.r, draw_color.g, draw_color.b, src_alpha, dst_red, dst_green, dst_blue, dst_alpha,
                              src_a_ratio, dst_a_ratio, pixel_value);
                      //                     logAppend(alpha_debug);
#endif

                      *dst++ = pixel_value;

                    }	// col

                }	// row

            } // font_style_outline

          if (current_state.getStyle() & font_style_underline)
            {

              int advance = glyph->advance.x >> 6;
              int row_start = (current_state.getAscent() - current_state.getUnderlineOffset());
              Uint32* dst = static_cast<Uint32*>(destination->pixels) + (y_pos + row_start) * destination->pitch/bpp + x_pos;

              for (int row = 0; current_state.getUnderlineHeight() > row; ++row)
                {
                  for (int col = x_pos; (x_pos + advance) > col; ++col)
                    {
                      *dst++ = pixel_value | 0xFF000000;
                    }	// col
                }	// row

            }	// if(underline)

          if (current_state.getStyle() & font_style_strikethrough)
            {

              int advance = glyph->advance.x >> 6;
              int row_start = (current_state.getLineHeight() * (2.0f / 3.0f));
              Uint32* dst = static_cast<Uint32*>(destination->pixels) + (y_pos + row_start) * destination->pitch/bpp + x_pos;

              for (int row = 0; current_state.getUnderlineHeight() > row; ++row)
                {
                  for (int col = x_pos; (x_pos + advance) > col; ++col)
                    {
                      *dst++ = pixel_value | 0xFF000000;
                    }	// col
                }	// row
            }	// if(strikethrough)


          if (SDL_MUSTLOCK(destination)) SDL_UnlockSurface(destination);

          x_pos += (glyph->advance.x >> 6);
          // x_pos += glyph->x_adv;

          previous_index = glyph->index;

        }	// for(char_iter)

      // Adjust vertical srawing position
      y_pos += current_state.getLineHeight();

    }	// while(!tokens.end())

  // Restore previous state
  popState();

}	// render(string&)
void FreeTypeRenderer::render(const char* text, SDL_Surface* destination, int dx, int dy)
{
  render(string(text), destination, dx, dy);
}	// render(const char*)

void FreeTypeRenderer::init()
{

  if (!lib_open) open();

#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERER)
  logAppend("Initializing (or reinitializing) FreeType Typeface.");
#endif

  cleanup();

  // Load font into memory
  // Comment all up to line 872, and remove comment from same,
  // if more memory is needed on the PSP. Also, if loading from
  // file instead of memory stream, reenable the glyph caches.

  using_stream = false;

  result = FT_New_Memory_Face(freetype2, dejavusans, size_dejavusans, 0, &typeface);

  if (FT_Err_Unknown_File_Format == result)
    {
      throw(Exceptions::FreeType::UnknownFormat(__FILE__));
    }
  else if (result)
    {
#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERER)
      char debug_string[32];
      sprintf(debug_string, "Error creating face: %d", result);
      logAppend(debug_string);
#endif

      throw(Exceptions::FreeType::FaceInit(__FILE__));
    }	// if(FT_New_Face)

  if (FT_IS_SCALABLE(typeface))
    {
      if (FT_Set_Char_Size(typeface, 0, current_state.getSize() * 64, 0, 0))
        {
          throw(Exceptions::FreeType::GeneralError(__FILE__, "Could not set font size."));
        }	// if(FT_Set_Char_Size)

      // Scalable font informations
      current_state.setScale(typeface->size->metrics.y_scale);
      current_state.setAscent(ftCeil(FT_MulFix(typeface->bbox.yMax, current_state.getScale())));
      current_state.setDescent(ftCeil(FT_MulFix(typeface->bbox.yMin, current_state.getScale())));
      current_state.setHeight(current_state.getAscent() - current_state.getDescent() + 1);
      current_state.setLineHeight(ftCeil(FT_MulFix(typeface->height, current_state.getScale())));
      current_state.setUnderlineOffset(ftFloor(FT_MulFix(typeface->underline_position, current_state.getScale())));

      int underline_height = (0 < ftFloor(FT_MulFix(typeface->underline_thickness, current_state.getScale()))) ? ftFloor(FT_MulFix(typeface->underline_thickness, current_state.getScale())) : 1;
      current_state.setUnderlineHeight(underline_height);

    }
  else
    {

      if (current_state.getSize() >= typeface->num_fixed_sizes)
        {
          current_state.setSize(typeface->num_fixed_sizes - 1);
        }	// if(current_state.size >= num_sizes)

      current_state.setSizeIndex(current_state.getSize());

      if (FT_Set_Pixel_Sizes(typeface, typeface->available_sizes[current_state.getSize()].height, typeface->available_sizes[current_state.getSize()].width))
        {
          throw(Exceptions::FreeType::GeneralError(__FILE__, "Unable to get fixed size font information."));
        }	// if(FT_Set_Pixel_Sizes)

      current_state.setAscent(typeface->available_sizes[current_state.getSize()].height);
      current_state.setDescent(0);
      current_state.setHeight(typeface->available_sizes[current_state.getSize()].height);
      current_state.setLineHeight(ftCeil(current_state.getHeight()));
      current_state.setUnderlineOffset(ftFloor(typeface->underline_position));
      current_state.setUnderlineHeight(ftFloor(typeface->underline_thickness));

    }	// if(FT_IS_SCALABLE)

  /* x offset = cos(((90.0-12)/360)*2*M_PI), or 12 degree angle */
  current_state.setItalicOffset(0.207f * current_state.getHeight());
  current_state.setBoldOffset(typeface->size->metrics.y_ppem / 10);

  lib_initialized = true;

}	// init
void FreeTypeRenderer::initFromStream(FT_Byte* stream_data, int stream_size)
{

  if (!lib_open) open();

#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERER)
  logAppend("Initializing (or reinitializing) FreeType Typeface.");
#endif

  cleanup();

  // Load font into memory
  // Comment all up to line 872, and remove comment from same,
  // if more memory is needed on the PSP. Also, if loading from
  // file instead of memory stream, reenable the glyph caches.

  using_stream = true;

  result = FT_New_Memory_Face(freetype2, stream_data, stream_size, 0, &typeface);

  if (FT_Err_Unknown_File_Format == result)
    {
      throw(Exceptions::FreeType::UnknownFormat(__FILE__));
    }
  else if (result)
    {
#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERER)
      char debug_string[32];
      sprintf(debug_string, "Error creating face: %d", result);
      logAppend(debug_string);
#endif

      throw(Exceptions::FreeType::FaceInit(__FILE__));
    }	// if(FT_New_Face)

  if (FT_IS_SCALABLE(typeface))
    {
      if (FT_Set_Char_Size(typeface, 0, current_state.getSize() * 64, 0, 0))
        {
          throw(Exceptions::FreeType::GeneralError(__FILE__, "Could not set font size."));
        }	// if(FT_Set_Char_Size)

      // Scalable font informations
      current_state.setScale(typeface->size->metrics.y_scale);
      current_state.setAscent(ftCeil(FT_MulFix(typeface->bbox.yMax, current_state.getScale())));
      current_state.setDescent(ftCeil(FT_MulFix(typeface->bbox.yMin, current_state.getScale())));
      current_state.setHeight(current_state.getAscent() - current_state.getDescent() + 1);
      current_state.setLineHeight(ftCeil(FT_MulFix(typeface->height, current_state.getScale())));
      current_state.setUnderlineOffset(ftFloor(FT_MulFix(typeface->underline_position, current_state.getScale())));

      int underline_height = (0 < ftFloor(FT_MulFix(typeface->underline_thickness, current_state.getScale()))) ? ftFloor(FT_MulFix(typeface->underline_thickness, current_state.getScale())) : 1;
      current_state.setUnderlineHeight(underline_height);

    }
  else
    {

      if (current_state.getSize() >= typeface->num_fixed_sizes)
        {
          current_state.setSize(typeface->num_fixed_sizes - 1);
        }	// if(current_state.size >= num_sizes)

      current_state.setSizeIndex(current_state.getSize());

      if (FT_Set_Pixel_Sizes(typeface, typeface->available_sizes[current_state.getSize()].height, typeface->available_sizes[current_state.getSize()].width))
        {
          throw(Exceptions::FreeType::GeneralError(__FILE__, "Unable to get fixed size font information."));
        }	// if(FT_Set_Pixel_Sizes)

      current_state.setAscent(typeface->available_sizes[current_state.getSize()].height);
      current_state.setDescent(0);
      current_state.setHeight(typeface->available_sizes[current_state.getSize()].height);
      current_state.setLineHeight(ftCeil(current_state.getHeight()));
      current_state.setUnderlineOffset(ftFloor(typeface->underline_position));
      current_state.setUnderlineHeight(ftFloor(typeface->underline_thickness));

    }	// if(FT_IS_SCALABLE)

  /* x offset = cos(((90.0-12)/360)*2*M_PI), or 12 degree angle */
  current_state.setItalicOffset(0.207f * current_state.getHeight());
  current_state.setBoldOffset(typeface->size->metrics.y_ppem / 10);

  lib_initialized = true;

}	// init
void FreeTypeRenderer::cleanup()
{

  clearCache();

  if (font_data && !using_stream) delete[](font_data);

  FT_Done_Face(typeface);

#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERER)
  logAppend("FT_Done_Face successful.");
#endif

}	// cleanup
void FreeTypeRenderer::close()
{

  FT_Done_FreeType(freetype2);
  lib_initialized = false;
  lib_open = false;

#if defined(_DEBUG) || defined(_DEBUGFREETYPERENDERER)
  logAppend("Cleaned up.");
#endif

}	// close
void FreeTypeRenderer::open()
{

  result = FT_Init_FreeType(&freetype2);
  if (result)
    {

#if defined(_DEBUG) || defined(_DEBUGFREERENDERER)
      logAppend("Could not open TrueType font file.");
#endif

      throw(Exceptions::FreeType::Init(__FILE__));
    }	// if(result)


  lib_open = true;

}	// open

CachedGlyph FreeTypeRenderer::loadGlyph(char glyph_char)
{

#if defined(_DEBUGFREETYPERENDERER) || defined(_DEBUGGLYPHCACHE)
  char debug_string[128];
  sprintf(debug_string, "Getting character glyph: %c.", glyph_char);
  logAppend(debug_string);
#endif

  if (CachedGlyph() != glyph_cache[glyph_char])
    {

#if defined(_DEBUGFREETYPERENDERER) || defined(_DEBUGGLYPHCACHE)
      logAppend("FTFont: Using a cached copy of the glyph.");
#endif

      return(glyph_cache[glyph_char]);

    }

#if defined(_DEBUGFREETYPERENDERER) || defined(_DEBUGGLYPHCACHE)
  logAppend("FTFont: Not using a cached copy of the glyph.");
#endif


  if (!typeface) throw(Exceptions::FreeType::InvalidHandle(__FILE__));

  FT_GlyphSlot glyph;

  CachedGlyph temp_cglyph(new CachedGlyphImpl);

  temp_cglyph->index = FT_Get_Char_Index(typeface, glyph_char);

#if defined(_DEBUGFREETYPERENDERER) || defined(_DEBUGGLYPHCACHE)
  sprintf(debug_string, "Retrieved character glyph index: %d.", temp_cglyph->index);
  logAppend(debug_string);
#endif

  result = FT_Load_Glyph(typeface, temp_cglyph->index, FT_LOAD_DEFAULT);
  if (result)
    {
      throw(Exceptions::FreeType::LoadGlyph(__FILE__));
    }

#if defined(_DEBUGFREETYPERENDERER) || defined(_DEBUGGLYPHCACHE)
  logAppend("Successfully loaded glyph.");
#endif

  glyph = typeface->glyph;

  temp_cglyph->metrics = glyph->metrics;
  temp_cglyph->advance = glyph->advance;

  if (current_state.getStyle() & font_style_bold)
    {
      for (int i = 0; i < current_state.getBoldOffset(); ++i) FT_GlyphSlot_Embolden(glyph);
    }

  if (current_state.getStyle() & font_style_italic)
    {
      FT_Matrix shear;
      FT_Outline* outline;

      outline = &glyph->outline;

      shear.xx = 1 << 16;
      shear.xy = static_cast<int>(current_state.getItalicOffset() * (1 << 16)) / current_state.getHeight();
      shear.yx = 0;
      shear.yy = 1 << 16;

      FT_Outline_Transform(outline, &shear);

#if defined(_DEBUGFREETYPERENDERER) || defined(_DEBUGGLYPHCACHE)
      logAppend("Applied italics style to glyph.");
#endif

    }

  result = FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);
  if (result)
    {

#if defined(_DEBUGFREETYPERENDERER) || defined(_DEBUGGLYPHCACHE)
      logAppend("Could not render glyph.");
#endif

      throw(Exceptions::FreeType::GeneralError(__FILE__, "Could not create italicized glyph."));
    }	// if(result)

#if defined(_DEBUGFREETYPERENDERER) || defined(_DEBUGGLYPHCACHE)
  logAppend("Rendered glyph.");
#endif

  FT_Bitmap* src = &glyph->bitmap;
  FT_Bitmap* dst = &temp_cglyph->bitmap;

  // while(*dst++ = *src++);
  memcpy(dst, src, sizeof(*dst));

  if (!FT_IS_SCALABLE(typeface))
    {
      dst->pitch *= 8;
    }

  // Leave the following two adjustments when the bitmap copying
  // code is moved to the CachedGlyph object.
  if (current_state.getStyle() & font_style_bold)
    {
      int bump = current_state.getBoldOffset();
      dst->pitch += bump;
      dst->width += bump;
    }
  if (current_state.getStyle() & font_style_italic)
    {
      int bump = static_cast<int>(ceil(current_state.getItalicOffset()));
      dst->pitch += bump;
      dst->width += bump;
    }

  if (dst->rows != 0)
    {

      dst->buffer = temp_cglyph->allocGlyph(dst->pitch * dst->rows);
      memset(dst->buffer, 0, dst->pitch * dst->rows);

      if (!dst->buffer)
        {
          throw(Exceptions::FreeType::InsufficientMemory(__FILE__));
        }	// if(!dst_buffer)

      memset( dst->buffer, 0, dst->pitch * dst->rows );

      for (int row = 0; row < src->rows; row++)
        {
          int src_offset = row * src->pitch;
          int dst_offset = row * dst->pitch;
          if (!FT_IS_SCALABLE(typeface))
            {
              unsigned char *srcp = src->buffer + src_offset;
              unsigned char *dstp = dst->buffer + dst_offset;
              for (int counter = 0; counter < src->width; counter += 8)
                {
                  unsigned char ch = *srcp++;
                  for (int counter2 = 0; counter2 < 8; ++counter2)
                    {
                      if ((ch & 0x80) >> 7)
                        {
                          *dstp++ = num_greys - 1;
                        }
                      else  	// if(ch)
                        {
                          *dstp++ = 0x00;
                        }	// if(ch)
                      ch <<= 1;
                    }	// for(counter2)
                }	//	for(counter)
            }
          else  	// if(!FT_IS_SCALABLE)
            {
              memcpy(dst->buffer + dst_offset,
                     src->buffer + src_offset, src->pitch);
            }	// if(!FT_IS_SCALABLE)
        }	// for(row)
    }	// if(dst->rows)

  // Now that all else is done, store miscellaneous metrics
  FT_Glyph_Metrics* mets = &temp_cglyph->metrics;

  if (FT_IS_SCALABLE(typeface))
    {
      temp_cglyph->x_min = ftFloor(mets->horiBearingX);
      temp_cglyph->x_max = temp_cglyph->x_min + ftCeil(mets->width);
      temp_cglyph->y_max = ftFloor(mets->horiBearingY);
      temp_cglyph->y_min = temp_cglyph->y_max - ftCeil(mets->height);
      temp_cglyph->y_offset = current_state.getAscent() - temp_cglyph->y_max;
      temp_cglyph->x_adv = ftCeil(mets->horiAdvance);
    }
  else
    {
      temp_cglyph->x_min = ftFloor(mets->horiBearingX);
      temp_cglyph->x_max = temp_cglyph->x_min + ftCeil(mets->horiAdvance);
      temp_cglyph->y_max = ftFloor(mets->horiBearingY);
      // temp_cglyph->y_min = temp_cglyph->y_max - ftCeil(typeface->available_sizes[current_state.getHeight()]);
      temp_cglyph->y_offset = 0;
      temp_cglyph->x_adv = ftCeil(mets->horiAdvance);
    }// if(FT_IS_SCALABLE)

  glyph_cache[glyph_char] = temp_cglyph;

  return(temp_cglyph);

}	// loadGlyph

CachedGlyph FreeTypeRenderer::loadStrokedGlyph(char glyph_char)
{

#if defined(_DEBUGFREETYPERENDERER) || defined(_DEBUGGLYPHCACHE)
  char debug_string[128];
  sprintf(debug_string, "Getting stroked character glyph: %c.", glyph_char);
  logAppend(debug_string);
#endif

  if (CachedGlyph() != stroke_cache[glyph_char])
    {

#if defined(_DEBUGFREETYPERENDERER) || defined(_DEBUGGLYPHCACHE)
      logAppend("Using a cached copy of the stroked glyph.");
#endif

      return(stroke_cache[glyph_char]);

    }

#if defined(_DEBUGFREETYPERENDERER) || defined(_DEBUGGLYPHCACHE)
  logAppend("FTFont: Not using a cached copy of the glyph.");
#endif


  if (!typeface) throw(Exceptions::FreeType::InvalidHandle(__FILE__));

  FT_GlyphSlot glyph;
  FT_Glyph s_glyph;
  FT_Stroker stroker = 0;

#if (_PSP_FW_VERSION >= 100)
  result = FT_Stroker_New(freetype2->memory, &stroker);
#else
  result = FT_Stroker_New(freetype2, &stroker);
#endif

  if (result)
    {
      char freetype_error[128];
      sprintf(freetype_error, "FreeType error code: 0x%2x", result);
      throw(Exceptions::FreeType::LoadGlyph("Could not open library routine to create stroke when stroke style requested", freetype_error, __FILE__, __LINE__));
    }

  FT_Stroker_Set(stroker, 64, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);


  CachedGlyph temp_cglyph(new CachedGlyphImpl);

  temp_cglyph->index = FT_Get_Char_Index(typeface, glyph_char);

#if defined(_DEBUGFREETYPERENDERER) || defined(_DEBUGGLYPHCACHE)
  sprintf(debug_string, "Retrieved character glyph index: %d.", temp_cglyph->index);
  logAppend(debug_string);
#endif

  result = FT_Load_Glyph(typeface, temp_cglyph->index, FT_LOAD_DEFAULT);
  if (result)
    {
      throw(Exceptions::FreeType::LoadGlyph(__FILE__));
    }

#if defined(_DEBUGFREETYPERENDERER) || defined(_DEBUGGLYPHCACHE)
  logAppend("Successfully loaded glyph.");
#endif

  glyph = typeface->glyph;

  temp_cglyph->metrics = glyph->metrics;
  temp_cglyph->advance = glyph->advance;

  if (current_state.getStyle() & font_style_bold)
    {
      for (int i = 1; i < current_state.getBoldOffset(); ++i) FT_GlyphSlot_Embolden(glyph);
    }

  if (current_state.getStyle() & font_style_italic)
    {
      FT_Matrix shear;
      FT_Outline* outline;

      outline = &glyph->outline;

      shear.xx = 1 << 16;
      shear.xy = static_cast<int>(current_state.getItalicOffset() * (1 << 16)) / current_state.getHeight();
      shear.yx = 0;
      shear.yy = 1 << 16;

      FT_Outline_Transform(outline, &shear);

#if defined(_DEBUGFREETYPERENDERER) || defined(_DEBUGGLYPHCACHE)
      logAppend("Applied italics style to glyph.");
#endif

    }

  // result = FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);
  if (result)
    {

#if defined(_DEBUGFREETYPERENDERER) || defined(_DEBUGGLYPHCACHE)
      logAppend("Could not render glyph.");
#endif

      throw(Exceptions::FreeType::GeneralError(__FILE__, "Could not create italicized glyph."));
    }	// if(result)

#if defined(_DEBUGFREETYPERENDERER) || defined(_DEBUGGLYPHCACHE)
  logAppend("Rendered glyph.");
#endif

  result = FT_Get_Glyph(glyph, &s_glyph);
  if (result)
    {
      char freetype_error[128];
      sprintf(freetype_error, "FreeType error code: 0x%2x", result);
      throw(Exceptions::FreeType::LoadGlyph("Could not load glyph create stroke when stroke style requested.", freetype_error, __FILE__, __LINE__));
    }

  result = FT_Glyph_Stroke(&s_glyph, stroker, 1);
  if (result)
    {
      char freetype_error[128];
      sprintf(freetype_error, "FreeType error code: 0x%2x", result);
      throw(Exceptions::FreeType::LoadGlyph("Could not create stroke when stroke style requested.", freetype_error, __FILE__, __LINE__));
    }

  result = FT_Glyph_To_Bitmap(&s_glyph, FT_RENDER_MODE_NORMAL, 0, 0);
  if (result)
    {
      char freetype_error[128];
      sprintf(freetype_error, "FreeType error code: 0x%2x", result);
      throw(Exceptions::FreeType::LoadGlyph("Could not create stroke bitmap when stroke style requested.", freetype_error, __FILE__, __LINE__));
    }

  if (FT_GLYPH_FORMAT_BITMAP != s_glyph->format) throw(Exceptions::FreeType::LoadGlyph("Could not create stroke bitmap when stroke style requested (@test).", "", __FILE__, __LINE__));

  FT_BitmapGlyph bitmap = (FT_BitmapGlyph)(s_glyph);

  FT_Bitmap* src = &bitmap->bitmap;
  FT_Bitmap* dst = &temp_cglyph->bitmap;

  // while(*dst++ = *src++);
  memcpy(dst, src, sizeof(*dst));

  if (!FT_IS_SCALABLE(typeface))
    {
      dst->pitch *= 8;
    }

  /*    if (current_state.getStyle() & font_style_bold) {
      int bump = current_state.getBoldOffset();
      dst->pitch += bump;
      dst->width += bump;
  } */
  if (current_state.getStyle() & font_style_italic)
    {
      int bump = (int)ceil(current_state.getItalicOffset());
      dst->pitch += bump;
      dst->width += bump;
    }

  if (dst->rows != 0)
    {

      dst->buffer = temp_cglyph->allocGlyph(dst->pitch * dst->rows);
      memset(dst->buffer, 0, dst->pitch * dst->rows);

      if (!dst->buffer)
        {
          throw(Exceptions::FreeType::InsufficientMemory(__FILE__));
        }	// if(!dst_buffer)

      memset( dst->buffer, 0, dst->pitch * dst->rows );

      for (int row = 0; row < src->rows; row++)
        {
          int src_offset = row * src->pitch;
          int dst_offset = row * dst->pitch;
          if (!FT_IS_SCALABLE(typeface))
            {
              unsigned char *srcp = src->buffer + src_offset;
              unsigned char *dstp = dst->buffer + dst_offset;
              for (int counter = 0; counter < src->width; counter += 8)
                {
                  unsigned char ch = *srcp++;
                  for (int counter2 = 0; counter2 < 8; ++counter2)
                    {
                      if ((ch & 0x80) >> 7)
                        {
                          *dstp++ = num_greys - 1;
                        }
                      else  	// if(ch)
                        {
                          *dstp++ = 0x00;
                        }	// if(ch)
                      ch <<= 1;
                    }	// for(counter2)
                }	//	for(counter)
            }
          else  	// if(!FT_IS_SCALABLE)
            {
              memcpy(dst->buffer + dst_offset,
                     src->buffer + src_offset, src->pitch);
            }	// if(!FT_IS_SCALABLE)
        }	// for(row)
    }	// if(dst->rows)

  stroke_cache[glyph_char] = temp_cglyph;

  return(temp_cglyph);

}	// loadStrokedGlyph

void FreeTypeRenderer::clearCache()
{
  glyph_cache.clear();
  stroke_cache.clear();

#if defined(_DEBUG) || defined(_DEBUGGLYPHCACHE)
  logAppend("Glyph cache emptied.");
#endif

}	// clearCache
