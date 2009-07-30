/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: September 13, 2008
 *
 * File: StaticButton.hh
 *
 * A button type control. Standard UI fare.
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
#include "controls/StaticButton.hh"
#include "geometry/Rect.hh"

// SMF Manager
#include "system/SystemManager.hh"

// SMF Exceptions
#include "exceptions/controls/ImageExceptions.hh"
#include "exceptions/io/FileExceptions.hh"

// SMF System Font
#include "freetype/FontManager.hh"

// SMF Debugging
#if defined(_DEBUG) || defined(_DEBUGSTATICTEXTLABEL)
#include "utility/Log.hh"
#endif

Button::Button(string text, const Rect& position, string property_string) :
        FreeTypeControl(position, property_string), label_text(text) { }
Button::Button(Widget* guardian, string text, const Rect& position, string property_string) :
        FreeTypeControl(guardian, position, property_string), label_text(text) { }
Button::Button(const Button& src) : FreeTypeControl(src), label_text(src.label_text) { }
Button::~Button() {

    if (isInitialized()) cleanup();

}

Button& Button::operator=(const Button& src) {

    if (&src != this) {

        FreeTypeControl::operator=(src);
        label_text = src.label_text;

        setNotInitialized();

    }

    return(*this);
}	// operator=

string Button::getText() {
    return(label_text);
}	// getText
void Button::setText(string text) {
    if (isInitialized()) {
        cleanup();
        setNotInitialized();
    }	// if(isInitialized)

    label_text = text;
}	// setText

void Button::init() {
#if defined(_DEBUG) || defined(_DEBUGSTATICTEXTLABEL)
    logAppend("Setting up text label: " + label_text );
#endif

    // Store the previous state
    pushState();

    // Propogate widget properties to text renderer
    setFontProperties(getProperties());

    int surface_width = textWidth(label_text) + 6;

#if defined(_DEBUG) || defined(_DEBUGSTATICTEXTLABEL)
    logAppend("Got width.");
#endif

    int surface_height = textHeight(label_text) + 1;

#if defined(_DEBUG) || defined(_DEBUGSTATICTEXTLABEL)
    logAppend("Got height.");
#endif

    setSurface(SDL_AllocSurface(SDL_SWSURFACE, surface_width, surface_height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000));

    // Draw button background
    boxRGBA(getSurface(), 0, 0, surface_width, surface_height, default_button_backcolor.r, default_button_backcolor.g, default_button_backcolor.b, 0xFF);
    rectangleRGBA(getSurface(), 0, 0, surface_width - 1, surface_height - 1, default_button_bordercolor.r, default_button_bordercolor.g, default_button_bordercolor.b, 0xFF);

    render(label_text, getSurface(), 3, 0);

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
void Button::cleanup() {
    setSurface(0);

    setNotInitialized();

}	// cleanup

void Button::draw() {

#if defined(_DEBUG) || defined(_DEBUGSTATICTEXTLABEL)
    logAppend("Button: Drawing.");
#endif

    FreeTypeControl::draw();

}	// draw
