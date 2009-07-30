/*
 * Sabacc Media Framework
 *
 * Author: goumba
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

Widget& Widget::operator=(const Widget& src) {

    if (&src != this) {
        Object::operator=(src);

        this->parent = src.parent;
        this->position = src.position;
    }	// if(not self)

    return(*this);
}	// operator=

Widget* const Widget::getParent() {
    return(parent);
}
void Widget::setParent(Widget* guardian) {
    parent = guardian;
}

inline const Rect& Widget::getPosition() {
    return(position);
}
inline const Rect::rect_type Widget::getLeft() {
    return(const_cast<Rect&>(getPosition()).getLeft());
}
inline const Rect::rect_type Widget::getTop() {
    return(const_cast<Rect&>(getPosition()).getTop());
}
inline const Rect::rect_type Widget::getRight() {
    return(const_cast<Rect&>(getPosition()).getRight());
}
inline const Rect::rect_type Widget::getBottom() {
    return(const_cast<Rect&>(getPosition()).getBottom());
}
inline const Rect::rect_type Widget::getWidth() {
    return(const_cast<Rect&>(getPosition()).getWidth());
}
inline const Rect::rect_type Widget::getHeight() {
    return(const_cast<Rect&>(getPosition()).getHeight());
}
void Widget::setposition(const Rect& new_position) {
    position = new_position;
}
void Widget::setLeft(Rect::rect_type new_left) {
    position.setLeft(new_left);
}
void Widget::setTop(Rect::rect_type new_top) {
    position.setTop(new_top);
}
void Widget::setRight(Rect::rect_type new_right) {
    position.setRight(new_right);
}
void Widget::setBottom(Rect::rect_type new_bottom) {
    position.setBottom(new_bottom);
}
void Widget::setWidth(Rect::rect_type new_width) {
    position.setWidth(new_width);
}
void Widget::setHeight(Rect::rect_type new_height) {
    position.setHeight(new_height);
}

inline bool Widget::isInitialized() {
    return(initialized);
}
inline void Widget::setInitialized() {
    initialized = true;
}
inline void Widget::setNotInitialized() {
    initialized = false;
}

void Widget::resize() { }

#if defined(_DEBUG) || defined(_DEBUGWIDGETBOUNDARY)
void Widget::draw() {
  rectangleColor(SDL_GetVideoSurface(), getLeft(), getTop(), getRight(), getBottom(), 0xFFFFFFFF);

  char debug_string[128];
  sprintf(debug_string, "Boundary: %d,%dx%dw,%dh", getLeft(), getTop(), getWidth(), getHeight());
  logAppend(debug_string);
}
#endif
