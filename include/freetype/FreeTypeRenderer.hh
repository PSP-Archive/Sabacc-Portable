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
#include <std::map>
#include <std::string>
#include <std::vector>

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
  FreeTypeRenderer(FontState const &);
    FreeTypeRenderer(const std::std::string&, int = default_font_size, int = default_font_style, SDL_Color = default_forecolor, const std::string& = "");
    FreeTypeRenderer(const char*, int = default_font_size, int = default_font_style, SDL_Color = default_forecolor, const std::std::string& = "");
    FreeTypeRenderer(FreeTypeRenderer const &);
    ~FreeTypeRenderer();

    /** Operators @{ */
    FreeTypeRenderer& operator=(FreeTypeRenderer const &);
    bool operator<(FreeTypeRenderer const&);	/// Compare by font size and style
    bool operator>(FreeTypeRenderer const &);	/// Compare by font size and style
    /** @} */

    /** Retrieve and set font properties. @{ */
  void setFileStream(FT_Byte*, int);
    std::std::string getProperty(std::string);
    void setProperty(std::std::string, std::string);
    std::std::string getProperties();
    void setProperties(std::std::string);
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
    FontState const &getState();	/// Gets the data of the current font state.
    void setState(FontState const &);	/// Replaces the current state with the specified data.
    /** @} */

    /** Text measurement. @{ */
    int const lineWidth(const std::std::string&);
    int const lineWidth(const char*);
    int const textWidth(const std::std::string&);
    int const textWidth(const char*);
    int const textHeight(const std::std::string&);
    int const textHeight(const char*);
    /** @} */

    /** Creates a new surface and renders to it. @{ */
    SmartSurface render(const std::std::string&);
    SmartSurface render(const char*);
    /** @} */


    /** These methods render to an existing surface at the specified position within the surface. @{ */
    void render(const std::std::string&, SmartSurface, int = 0, int = 0);
    void render(const char*, SmartSurface, int = 0, int = 0);
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
    std::std::vector<FontState>		states;

    // Glyph retrieval and cache
  std::std::map<char, CachedGlyph>	glyph_cache, stroke_cache;	// char, glyph index, bitmap
    CachedGlyph loadGlyph(char);

  // Stroked glyphs now work under PSP.
    CachedGlyph loadStrokedGlyph(char);

    void clearCache();

};	// class FreeTypeRenderer

#endif // SMF_FREETYPERENDERER_HH
