/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: September 13, 2008
 *
 * File: StaticPSPPSPButton.hh
 *
 * A button type control. Displays a PSP button glyph.
 */

// Standard Library headers
#include <string>
using std::string;

// Simple Directmedia headers
#if defined(__APPLE__)
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#else
#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#endif

// SMF headers
#include "base/Control.hh"
#include "base/FreeTypeControl.hh"
#include "base/SmartSurface.hh"
#include "base/Widget.hh"
#include "controls/StaticPSPButton.hh"
#include "freetype/FreeTypeRenderer.hh"
#include "freetype/FontManager.hh"
#include "geometry/Rect.hh"

// SMF Manager
#include "system/SystemManager.hh"

// SMF Exceptions
#include "exceptions/controls/ImageExceptions.hh"
#include "exceptions/io/FileExceptions.hh"

// SMF System Font
#include "freetype/FontManager.hh"

// SMF Debugging
#if defined(_DEBUG) || defined(_DEBUGSTATICBUTTON)
#include "utility/Log.hh"
#endif

PSPButton::PSPButton(string text, const Rect& position, string property_string) :
  FreeTypeControl(position, property_string), label_text(text),
  psp_key("X") {

}
PSPButton::PSPButton(Widget* guardian, string text, const Rect& position, string property_string) :
  FreeTypeControl(guardian, position, property_string), label_text(text),
  psp_key("X") {

}
PSPButton::PSPButton(const PSPButton& src) : FreeTypeControl(src), label_text(src.label_text), psp_key(src.psp_key) {

}
PSPButton::~PSPButton() {

    if (isInitialized()) cleanup();

}

PSPButton& PSPButton::operator=(const PSPButton& src) {

    if (&src != this) {

        FreeTypeControl::operator=(src);
        label_text = src.label_text;
	psp_key = src.psp_key;

        setNotInitialized();

    }

    return(*this);
}	// operator=

string PSPButton::getText() {
    return(label_text);
}	// getText
void PSPButton::setText(string text) {
    if (isInitialized()) {
        cleanup();
        setNotInitialized();
    }	// if(isInitialized)

    label_text = text;
}	// setText
string PSPButton::getPSPKey() { return(psp_key); }
void PSPButton::setPSPKey(char key) {
    if (isInitialized()) {
        cleanup();
        setNotInitialized();
    }	// if(isInitialized)

    psp_key = key;
}

void PSPButton::init() {
#if defined(_DEBUG) || defined(_DEBUGSTATICBUTTON)
    logAppend("Setting up text label: " + label_text );
#endif

    int glyph_width = getPSPFont().textWidth(psp_key);
    int glyph_height = getPSPFont().textHeight(psp_key);

    // Store the previous state in the internal text renderer
    pushState();

    // Propogate widget properties to text renderer
    setFontProperties(getProperties());

    int surface_width = glyph_width + textWidth(label_text) + 11;

#if defined(_DEBUG) || defined(_DEBUGSTATICBUTTON)
    logAppend("Got width.");
#endif

    int text_height = textHeight(label_text);
    int surface_height = text_height + 1;

#if defined(_DEBUG) || defined(_DEBUGSTATICBUTTON)
    logAppend("Got height.");
#endif

    setSurface(SDL_AllocSurface(SDL_SWSURFACE, surface_width, surface_height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000));

    // Draw button background
    boxRGBA(getSurface(), 0, 0, surface_width, surface_height, default_button_backcolor.r, default_button_backcolor.g, default_button_backcolor.b, 0xFF);
    rectangleRGBA(getSurface(), 0, 0, surface_width - 1, surface_height - 1, default_button_bordercolor.r, default_button_bordercolor.g, default_button_bordercolor.b, 0xFF);

    getPSPFont().render(psp_key, getSurface(), 3, (surface_height - glyph_height) / 2);
    render(label_text, getSurface(), glyph_width+8, (surface_height - text_height) / 2);

    // After redering, restore previous state
    popState();

#if defined(_DEBUG) || defined(_DEBUGSTATICBUTTON)
    logAppend("Rendered.");
#endif

    // Automatic adjustment of Widget size
    if (string::npos != getProperty("autosize").find("true")) {
        setWidth(surface_width);
        setHeight(surface_height);
    }	// getProperty("all")

#if defined(_DEBUG) || defined(_DEBUGSTATICBUTTON)
    logAppend("Converting rendered surface to display alpha format.");
#endif

    setSurface(SDL_DisplayFormatAlpha(getSurface()));

    setInitialized();

}	// init
void PSPButton::cleanup() {
    setSurface(0);

    setNotInitialized();

}	// cleanup

void PSPButton::draw() {

#if defined(_DEBUG) || defined(_DEBUGSTATICBUTTON)
    logAppend("PSPButton: Drawing.");
#endif

    FreeTypeControl::draw();

}	// draw

