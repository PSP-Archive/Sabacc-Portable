/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: September 10, 2008
 *
 * File:FreeTypeControl.cc
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
#include "base/FreeTypeControl.hh"

// SMF FreeType headers
#include "freetype/FontManager.hh"
#include "freetype/FontState.hh"
#include "freetype/FreeTypeRenderer.hh"

// SMF Constants
#include "constants/ColorConstants.hh"
#include "constants/FreeTypeRendererConstants.hh"

FreeTypeControl::FreeTypeControl(const Rect& position, string property_string, const string& font_file, int font_size, int font_style, SDL_Color font_color) :
        Control(position, property_string), widget_font_renderer(font_file, font_size, font_style, font_color, property_string) { }
FreeTypeControl::FreeTypeControl(const Rect& position, const FontState& new_state) :
        Control(position), widget_font_renderer(new_state) { }
FreeTypeControl::FreeTypeControl(Widget* guardian, const Rect& position, string property_string, const string& font_file, int font_size, int font_style, SDL_Color font_color) :
        Control(guardian, position, property_string), widget_font_renderer(font_file, font_size, font_style, font_color, property_string) { }
FreeTypeControl::FreeTypeControl(Widget* guardian, const Rect& position, const FontState& new_state) :
        Control(guardian, position),widget_font_renderer(new_state) { }
FreeTypeControl::FreeTypeControl(const FreeTypeControl& src) :
        Control(src), widget_font_renderer(src.widget_font_renderer) { }
FreeTypeControl::~FreeTypeControl() { }

FreeTypeControl& FreeTypeControl::operator=(const FreeTypeControl& src) {

    if (&src != this) {

        Control::operator=(src);

        widget_font_renderer = src.widget_font_renderer;

    }	// if(not the same instance)

    return(*this);
}	// operator=

int FreeTypeControl::getTextSize() {
    return(widget_font_renderer.getSize());
}
void FreeTypeControl::setTextSize(int font_size) {
    widget_font_renderer.setSize(font_size);
}
int FreeTypeControl::getTextStyle() {
    return(widget_font_renderer.getStyle());
}
void FreeTypeControl::setTextStyle(int font_style) {
    widget_font_renderer.setStyle(font_style);
}
const SDL_Color& FreeTypeControl::getTextColor() {
    return(widget_font_renderer.getColor());
}
void FreeTypeControl::setTextColor(SDL_Color font_color) {
    widget_font_renderer.setColor(font_color);
}
void FreeTypeControl::setTextColor(int r, int g, int b, int a) {
    widget_font_renderer.setColor(r, g, b, a);
}
string FreeTypeControl::getFontProperties() {
    return(widget_font_renderer.getProperties());
}
void FreeTypeControl::setFontProperties(string property_string) {
    widget_font_renderer.setProperties(property_string);
}

int FreeTypeControl::getFontAscent() {
    return(widget_font_renderer.getAscent());
}
int FreeTypeControl::getFontDescent() {
    return(widget_font_renderer.getDescent());
}
int FreeTypeControl::getFontHeight() {
    return(widget_font_renderer.getFontHeight());
}
int FreeTypeControl::getFontLineHeight() {
    return(widget_font_renderer.getLineHeight());
}

void FreeTypeControl::popState() {
    widget_font_renderer.popState();
}
void FreeTypeControl::pushState() {
    widget_font_renderer.pushState();
}
void FreeTypeControl::clearState() {
    widget_font_renderer.clearStates();
}
const FontState& FreeTypeControl::getFontState() {
    return(widget_font_renderer.getState());
}
void FreeTypeControl::setFontState(const FontState& new_state) {
    widget_font_renderer.setState(new_state);
}

int FreeTypeControl::lineWidth(const string& text) {
    return(widget_font_renderer.lineWidth(text));
}
int FreeTypeControl::lineWidth(const char* text) {
    return(widget_font_renderer.lineWidth(string(text)));
}
int FreeTypeControl::textWidth(const string& text) {
    return(widget_font_renderer.textWidth(text));
}
int FreeTypeControl::textWidth(const char* text) {
    return(widget_font_renderer.textWidth(string(text)));
}
int FreeTypeControl::textHeight(const string& text) {
    return(widget_font_renderer.textHeight(text));
}
int FreeTypeControl::textHeight(const char* text) {
    return(widget_font_renderer.textHeight(string(text)));
}

void FreeTypeControl::render(const string& text, SDL_Surface* destination, int dx, int dy) {
    widget_font_renderer.render(text, destination, dx, dy);
}	// render
void FreeTypeControl::render(const char* text, SDL_Surface* destination, int dx, int dy) {
    widget_font_renderer.render(string(text), destination, dx, dy);
}	// render
