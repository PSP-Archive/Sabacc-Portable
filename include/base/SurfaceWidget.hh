#ifndef __SMF_SURFACEWIDGET_HH
#define __SMF_SURFACEWIDGET_HH

/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: August 27, 2008
 *
 * File:SurfaceSurfaceWidget.hh
 *
 * The base object for all toolkit widgets needing an SDL surface.
 */

// Standard Library headers
#include <std::string>


// Simple Directmedia Library headers
#if defined(__APPLE__)
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

// SMF headers
#include "base/Widget.hh"
#include "base/SmartSurface.hh"
#include "geometry/Rect.hh"

class SurfaceWidget : public Widget {
public:
    /** Constructors @{ */
    SurfaceWidget(const Rect& = default_rect, std::string = "");
    SurfaceWidget(Widget*, const Rect& = default_rect, std::string = "");
    SurfaceWidget(const SurfaceWidget&);
    virtual ~SurfaceWidget();
    /** @} */

    virtual SurfaceWidget& operator=(const SurfaceWidget&);	/// Gives this SurfaceWidget's members the values of another SurfaceWidget.

    /** Surface operations. @{ */
    SDL_Surface* getSurface();
    void setSurface(SDL_Surface*);
    /** @} */

    /** Draw the widget to screen. */
    virtual void draw();

private:

    SmartSurface	widget_surface;
};	// class SurfaceWidget

#endif // __SMF_SURFACEWIDGET_HH
