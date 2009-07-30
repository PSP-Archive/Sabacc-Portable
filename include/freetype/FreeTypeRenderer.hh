#ifndef SMF_FREETYPERENDERER_HH
#define SMF_FREETYPERENDERER_HH

/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: September 4, 2008
 *
 * File: FreeTypeRenderer.hh
 *
 * The main FreeType text rendering class.
 */

// Standard Library headers
#include <map>
#include <string>
#include <vector>
using std::map;
using std::string;
using std::vector;

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
#include "base/SmartSurface.hh"
#include "types/TypeDefinitions.hh"

// SMF FreeType headers
#include "constants/ColorConstants.hh"
#include "constants/FreeTypeRendererConstants.hh"
#include "freetype/FontState.hh"

// SMF Forward declarations
class CachedGlyph;

class FreeTypeRenderer {

public:

    FreeTypeRenderer();
    FreeTypeRenderer(const FontState&);
    FreeTypeRenderer(const string&, int = default_font_size, int = default_font_style, SDL_Color = default_forecolor, const string& = "");
    FreeTypeRenderer(const char*, int = default_font_size, int = default_font_style, SDL_Color = default_forecolor, const string& = "");
    FreeTypeRenderer(const FreeTypeRenderer&);
    virtual ~FreeTypeRenderer();

    /** Operators @{ */
    FreeTypeRenderer& operator=(const FreeTypeRenderer&);
    virtual bool operator<(const FreeTypeRenderer&);	/// Compare by font size and style
    virtual bool operator>(const FreeTypeRenderer&);	/// Compare by font size and style
    /** @} */

    /** Retrieve and set font properties. @{ */
  virtual void setFileStream(FT_Byte*, int);
    virtual string getProperty(string);
    virtual void setProperty(string, string);
    virtual string getProperties();
    virtual void setProperties(string);
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
    /** @} */

    /** Read-only properties. @{ */
    virtual int getAscent();
    virtual int getDescent();
    virtual int getFontHeight();
    virtual int getLineHeight();
    /** @} */

    /** State operatoions. @{ */
    virtual void pushState();	/// Saves state for future retrieval.
    virtual void popState();	/// Retrieves the most recently saved state.
    virtual void clearStates();
    virtual const FontState& getState();	/// Gets the data of the current font state.
    virtual void setState(FontState);	/// Replaces the current state with the specified data.
    /** @} */

    /** Text measurement. @{ */
    virtual int lineWidth(const string&);
    virtual int lineWidth(const char*);
    virtual int textWidth(const string&);
    virtual int textWidth(const char*);
    virtual int textHeight(const string&);
    virtual int textHeight(const char*);
    /** @} */

    /** Creates a new surface and renders to it. @{ */
    virtual SmartSurface render(const string&);
    virtual SmartSurface render(const char*);
    /** @} */


    /** These methods render to an existing surface at the specified position within the surface. @{ */
    virtual void render(const string&, SDL_Surface*, int = 0, int = 0);
    virtual void render(const char*, SDL_Surface*, int = 0, int = 0);
    /** @} */

private:

    // Freetype objects
    FT_Library			freetype2;
    int					load_flags;
    FT_Face				typeface;
    FT_Byte*			font_data;

    // Library status
    bool				lib_open;
    bool				lib_initialized;
  bool using_stream;

    // error code
    int					result;

    // Set up the library
    virtual void init();
  virtual void initFromStream(FT_Byte*, int);
    virtual void cleanup();
    virtual void close();
    virtual void open();

    // Font information
    FontState			current_state;
    vector<FontState>		states;

    // Glyph retrieval and cache
    map<char, CachedGlyph>	glyph_cache, stroke_cache;	// char, glyph index, bitmap
    virtual CachedGlyph loadGlyph(char);

  // Do not use this under PSP as the newer FreeType lib works with stroker
  // differently. To be fixed, and dependent on FreeType lib version rather
  // than if SMF is being compiled for PSP or not.
    virtual CachedGlyph loadStrokedGlyph(char);

    void clearCache();

};	// class FreeTypeRenderer

#endif // SMF_FREETYPERENDERER_HH
