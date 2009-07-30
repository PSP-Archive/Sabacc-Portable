#ifndef __SMF_FONTSTATE_HH
#define __SMF_FONTSTATE_HH

/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: September 3, 2008
 *
 * File: FontState.hh
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
#include "constants/ColorConstants.hh"
#include "constants/FreeTypeRendererConstants.hh"
#include "types/TypeDefinitions.hh"

class FontState {

public:

    /** Constructors @{  */
    FontState();
  FontState(int, int = default_font_style, SDL_Color = default_forecolor, string = "");
    FontState(const FontState&);
    FontState& operator=(const FontState&);
    virtual ~FontState();
    /** @} */

    /** Retrieve and set basic font properties. @{ */
    virtual int getSize();
    virtual void setSize(int);
    virtual int getStyle();
    virtual void setStyle(int);
    virtual const SDL_Color& getColor();
    virtual void setColor(SDL_Color = default_forecolor);
    virtual void setColor(int, int, int, int);
    virtual const SDL_Color& getStrokeColor();
    virtual void setStrokeColor(SDL_Color = default_forecolor);
    virtual void setStrokeColor(int, int, int, int);
    virtual string getProperty(string);
    virtual void setProperty(string, string);	/// Set an individual property's value.
    virtual string getProperties();	/// Get a list of properties as a string
    virtual void setProperties(string);	/// Set multiple properties together in a single string.
    /** @} */

    /** The following functions are provided as an interface to the FreeType renderer
      * so that no members need be public, and should not be used directly by the
      * developer. @{ */
    virtual int getSizeIndex();
    virtual void setSizeIndex(int);
    virtual FT_Fixed getScale();
    virtual void setScale(FT_Fixed);
    virtual int getAscent();
    virtual void setAscent(int);
    virtual int getDescent();
    virtual void setDescent(int);
    virtual int getHeight();
    virtual void setHeight(int);
    virtual int getLineHeight();
    virtual void setLineHeight(int);
    virtual int getUnderlineOffset();
    virtual void setUnderlineOffset(int);
    virtual int getUnderlineHeight();
    virtual void setUnderlineHeight(int);
    virtual float getItalicOffset();
    virtual void setItalicOffset(float);
    virtual int getBoldOffset();
    virtual void setBoldOffset(int);
    /** @} */

private:

    /** Basic font data. */
    int			size, style;
    SDL_Color	color;
    SDL_Color	stroke_color;

    /** Internal FreeType size index. */
    int			size_index;

    /** Font scaling. */
    FT_Fixed	scale;

    /** Glyph size. */
    int			ascent, descent, height, line_height;

    /** Underline position */
    int			underline_offset, underline_height;

    /** Style modifiers. */
    float		italic_offset;
    int			bold_offset;

    // Properties
    PropertyMap		font_properties;

};	// struct FontState


#endif // __SMF_FONTSTATE_HH
