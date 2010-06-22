/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 27, 2008
 *
 * File:SurfaceSurfaceWidget.hh
 *
 * The base object for all toolkit widgets needing an SDL surface.
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
#include "base/Widget.hh"
#include "base/SurfaceWidget.hh"
#include "geometry/Rect.hh"

// SMF Exceptions
#include "exceptions/base/WidgetExceptions.hh"

// SMF Debugging headers
#if defined(_DEBUG) || defined(_DEBUGSURFACEWIDGET)
#include <cstdlib>
#include "utility/Log.hh"
#endif

SurfaceWidget::SurfaceWidget(const Rect& position, string property_string ) :
        Widget(position, property_string), widget_surface(0) { }
SurfaceWidget::SurfaceWidget(Widget* guardian, const Rect& position, string property_string) :
        Widget(guardian, position, property_string), widget_surface(0) { }
SurfaceWidget::SurfaceWidget(const SurfaceWidget& src) :
        Widget(src), widget_surface(src.widget_surface) { }
SurfaceWidget::~SurfaceWidget() { }

SurfaceWidget& SurfaceWidget::operator=(const SurfaceWidget& src) {

    if (&src != this) {
        Widget::operator=(src);
        widget_surface = src.widget_surface;
    }

    return(*this);

}	// operator=

SDL_Surface* SurfaceWidget::getSurface() {
    return(widget_surface.get());
}
void SurfaceWidget::setSurface(SDL_Surface* surface) {
    widget_surface = SmartSurface(surface);
}

void SurfaceWidget::draw() {

    if (!isInitialized() || !getSurface()) {

#if defined(_DEBUG) || defined(_DEBUGSURFACEWIDGET)
        logAppend("SurfaceWidget: Not initialized. Calling init()");
#endif

        init();

        if (!getSurface()) {

            throw(Exceptions::Widget::InvalidSurfaceForBlit("Invalid surface held by SurfaceWidget.",
                    "SurfaceWidget or a derived class never successfully created a surface to be drawn to the screen surface, therefore, no drawing can be done.", "",
                    __FILE__, __LINE__));

        }
    }	// if(!init)

    SDL_Rect position = getSDLRect(const_cast<Rect&>(Position()));
    SDL_Rect draw_rect = {
	0,
	0,
	static_cast<Uint16>(Width()),
	static_cast<Uint16>(Height())
    };

#if defined(_DEBUG) || defined(_DEBUGSURFACEWIDGET)
    char debug_string[128];
    sprintf(debug_string, "Drawing 0x%x @ %d,%d %dw %dh", this, Left(), Top(), Width(), Height());
    logAppend(debug_string);
#endif

    if (-1 == SDL_BlitSurface(getSurface(), &draw_rect, SDL_GetVideoSurface(), &position)) {

#if defined(_DEBUG) || defined(_DEBUGSURFACEWIDGET)
        logAppend("SurfaceWidget: Blit failed.");
#endif

        throw(Exceptions::Widget::BlitError("SDL_Blit failed for Widget.",
                                            "SurfaceWidget could not blit the internal widget surface to the screen. See additional data for the error returned by SDL, if any.",
					    SDL_GetError(), __FILE__, __LINE__));
    }	// Blit

#if defined(_DEBUG) || defined(_DEBUGWIDGETBOUNDARY)
    Widget::draw();
#endif

}	// draw
