/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 27, 2008
 *
 * File: Widget.cc
 *
 * The base object for all toolkit widgets.
 */

// SMF headers
#include "base/Widget.hh"
#include "geometry/Rect.hh"

// For debugging
#if defined(_DEBUG) || defined(_DEBUGWIDGETBOUNDARY)
#if defined(__APPLE__)
#include <SDL/SDL_gfxPrimtives.h>
#else
#include <SDL_gfxPrimitives.h>
#endif
#include <cstdlib>
#include "utility/Log.hh"
#endif

Widget::Widget(const Rect& position, std::string properties) :
        Object(properties), parent(0), position(position), initialized(false) { }
Widget::Widget(Widget* guardian, const Rect& position, std::string properties) :
        Object(properties), parent(guardian), position(position), initialized(false) { }
Widget::Widget(const Widget& src) :
        Object(src), parent(src.parent), position(src.position), initialized(false) { }
Widget::~Widget() { }

Widget
&Widget::operator=(const Widget& src) {

    if (&src != this) {
        Object::operator=(src);

        this->parent = src.parent;
        this->position = src.position;
    }	// if(not self)

    return(*this);
}	// operator=

Widget const 
*Widget::Parent() {
    return(parent);
}
Widget
&Widget::Parent(Widget* guardian) {
    parent = guardian;
}

void Widget::resize() { }

#if defined(_DEBUG) || defined(_DEBUGWIDGETBOUNDARY)
void
Widget::draw() {
  rectangleColor(SDL_GetVideoSurface(), getLeft(), getTop(), getRight(), getBottom(), 0xFFFFFFFF);

  char debug_string[128];
  sprintf(debug_string, "Boundary: %d,%dx%dw,%dh", getLeft(), getTop(), getWidth(), getHeight());
  logAppend(debug_string);
}
#endif
