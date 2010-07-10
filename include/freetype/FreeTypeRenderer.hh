#ifndef SMF_FREETYPERENDERER_HH
#define SMF_FREETYPERENDERER_HH

/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
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
    ~FreeTypeRenderer();

    /** Operators @{ */
    FreeTypeRenderer& operator=(const FreeTypeRenderer&);
    bool operator<(const FreeTypeRenderer&);	/// Compare by font size and style
    bool operator>(const FreeTypeRenderer&);	/// Compare by font size and style
    /** @} */

    /** Retrieve and set font properties. @{ */
  void setFileStream(FT_Byte*, int);
    string getProperty(string);
    void setProperty(string, string);
    string getProperties();
    void setProperties(string);
    int getSize();
    void setSize(int);
    int getStyle();
    void setStyle(int);
    const SDL_Color& getColor();
    void setColor(SDL_Color = default_forecolor);
    void setColor(int, int, int, int);
    const SDL_Color& getStrokeColor();
    void setStrokeColor(SDL_Color = default_forecolor);
    void setStrokeColor(int, int, int, int);
    /** @} */

    /** Read-only properties. @{ */
    int getAscent();
    int getDescent();
    int getFontHeight();
    int getLineHeight();
    /** @} */

    /** State operatoions. @{ */
    void pushState();	/// Saves state for future retrieval.
    void popState();	/// Retrieves the most recently saved state.
    void clearStates();
    const FontState& getState();	/// Gets the data of the current font state.
    void setState(FontState);	/// Replaces the current state with the specified data.
    /** @} */

    /** Text measurement. @{ */
    int lineWidth(string);
    int textWidth(string);
    int textHeight(string);
    /** @} */

    /** Creates a new surface and renders to it. @{ */
    SmartSurface render(string);
    /** @} */


    /** These methods render to an existing surface at the specified position within the surface. @{ */
    void render(string, SDL_Surface*, int = 0, int = 0);
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
    void init();
    void initFromStream(FT_Byte*, int);
    void cleanup();
    void close();
    void open();

    // Font information
    FontState			current_state;
    vector<FontState>		states;

    // Glyph retrieval and cache
    map<char, CachedGlyph>	glyph_cache, stroke_cache;

    CachedGlyph loadGlyph(char);
    CachedGlyph loadStrokedGlyph(char);

    void clearCache();

};	// class FreeTypeRenderer

#endif // SMF_FREETYPERENDERER_HH
