/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: August 28, 2008
 *
 * File: TextLabel.hh
 *
 * The definition of the object on which all interface
 * objects (widgets, etc) will be based.
 */

// Standard Library headers
#include <string>
using std::string;

// Simple Directmedia headers
#if defined(__APPLE__)
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#endif

// SMF headers
#include "base/Control.hh"
#include "base/FreeTypeControl.hh"
#include "base/SmartSurface.hh"
#include "base/Widget.hh"
#include "controls/StaticTextLabel.hh"
#include "geometry/Rect.hh"

// SMF Manager
#include "system/SystemManager.hh"

// SMF Exceptions
#include "exceptions/controls/ImageExceptions.hh"
#include "exceptions/io/FileExceptions.hh"

// SMF FreeType headers
#include "freetype/FreeTypeRenderer.hh"

// SMF System Font
#include "freetype/FontManager.hh"

// SMF Debugging
#if defined(_DEBUG) || defined(_DEBUGSTATICTEXTLABEL)
#include "utility/Log.hh"
#endif

TextLabel::TextLabel(const string& text, const Rect& position, string property_string) :
	FreeTypeControl(position, property_string), label_text(text) { }
TextLabel::TextLabel(Widget* guardian, const string& text, const Rect& position, string property_string) :
	FreeTypeControl(guardian, position, property_string), label_text(text) { }
TextLabel::TextLabel(const TextLabel& src) : FreeTypeControl(src), label_text(src.label_text) { }
TextLabel::~TextLabel() {

    if (isInitialized()) cleanup();

}

TextLabel& TextLabel::operator=(const TextLabel& src) {

    if (&src != this) {

        FreeTypeControl::operator=(src);
        label_text = src.label_text;

        setNotInitialized();

    }

    return(*this);
}	// operator=

string TextLabel::getText() {
    return(label_text);
}	// getText
void TextLabel::setText(string text) {
    if (isInitialized()) {
        cleanup();
        setNotInitialized();
    }	// if(isInitialized)

    label_text = text;
}	// setText

void TextLabel::init() {
#if defined(_DEBUG) || defined(_DEBUGSTATICTEXTLABEL)
    logAppend("Setting up text label: " + label_text );
#endif

    // Store the previous state
    pushState();

    // Propogate widget properties to text renderer
    setFontProperties(getProperties());

    int surface_width = textWidth(label_text) + 3;

#if defined(_DEBUG) || defined(_DEBUGSTATICTEXTLABEL)
    logAppend("Got width.");
#endif

    int surface_height = textHeight(label_text) + 1;

#if defined(_DEBUG) || defined(_DEBUGSTATICTEXTLABEL)
    logAppend("Got height.");
#endif

    setSurface(SDL_AllocSurface(SDL_SWSURFACE, surface_width, surface_height,
                                32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000));

    render(label_text, getSurface(), 0, 0);

    // After redering, restore previous state
    popState();

#if defined(_DEBUG) || defined(_DEBUGSTATICTEXTLABEL)
    logAppend("Rendered.");
#endif

    // Automatic adjustment of Widget size
    if (string::npos != getProperty("autosize").find("true")) {
        setWidth(surface_width);
        setHeight(surface_height);
    }	// getProperty("all")

#if defined(_DEBUG) || defined(_DEBUGSTATICTEXTLABEL)
    logAppend("Converting rendered surface to display alpha format.");
#endif

    setSurface(SDL_DisplayFormatAlpha(getSurface()));

    setInitialized();

}	// init
void TextLabel::cleanup() {
    setSurface(0);

    setNotInitialized();

}	// cleanup

void TextLabel::draw() {

#if defined(_DEBUG) || defined(_DEBUGSTATICTEXTLABEL)
    logAppend("TextLabel: Drawing.");
#endif

    FreeTypeControl::draw();

}	// draw
