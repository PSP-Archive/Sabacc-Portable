#ifndef __SMF_FREETYPECONTROL_HH
#define __SMF_FREETYPECONTROL_HH

/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: September 9, 2008
 *
 * File:FreeTypeControl.hh
 *
 * The base object for all toolkit widgets needing acess to the FreeType renderer.
 */

// Standard Library headers
#include <string>
using std::string;

// Simple Directmedia Library headers
#if defined(__APPLE__)
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

// SMF headers
#include "base/Control.hh"
#include "base/Widget.hh"

// SMF FreeType headers
#include "freetype/FontManager.hh"
#include "freetype/FontState.hh"
#include "freetype/FreeTypeRenderer.hh"

// SMF Constants
#include "constants/ColorConstants.hh"
#include "constants/FreeTypeRendererConstants.hh"

class FreeTypeControl : public Control
  {

  public:

    FreeTypeControl(const Rect& = default_rect, string = "", const string& = default_font_file, int = default_font_size, int = default_font_style, SDL_Color = default_forecolor);
    FreeTypeControl(const Rect& = default_rect, const FontState& = getDefaultFontState());
    FreeTypeControl(Widget*, const Rect& = default_rect, string = "", const string& = default_font_file, int = default_font_size, int = default_font_style, SDL_Color = default_forecolor);
    FreeTypeControl(Widget*, const Rect& = default_rect, const FontState& = getDefaultFontState());
    FreeTypeControl(const FreeTypeControl&);
    virtual ~FreeTypeControl();

    FreeTypeControl& operator=(const FreeTypeControl&);

    /** Retrieve and set properties. @{ */
    virtual int getTextSize();
    virtual void setTextSize(int = default_font_size);
    virtual int getTextStyle();
    virtual void setTextStyle(int = default_font_style);
    virtual const SDL_Color& getTextColor();
    virtual void setTextColor(SDL_Color = default_forecolor);
    virtual void setTextColor(int, int, int, int);
    virtual string getFontProperties();
    virtual void setFontProperties(string);
    /** @} */

    /** Read-only properties. @{ */
    virtual int getFontAscent();
    virtual int getFontDescent();
    virtual int getFontHeight();
    virtual int getFontLineHeight();
    /** @} */

    /** Retrieve and set font state @{ */
    virtual void popState();
    virtual void pushState();
    virtual void clearState();
    virtual const FontState& getFontState();
    virtual void setFontState(const FontState&);
    /** @} */

    /** Get sizing data. @{ */
    virtual int lineWidth(const string&);
    virtual int lineWidth(const char*);
    virtual int textWidth(const string&);
    virtual int textWidth(const char*);
    virtual int textHeight(const string&);
    virtual int textHeight(const char*);
    /** @} */

    /** Render to the widget surface. @{ */
    virtual void render(const string&, SDL_Surface*, int = 0, int = 0);
    virtual void render(const char*, SDL_Surface*, int = 0, int = 0);
    /** @} */

  private:

    FreeTypeRenderer widget_font_renderer;

  };	// FreeTypeControl

#endif // __SMF_FREETYPECONTROL_HH
