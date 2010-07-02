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
    Object(properties),
    parent(0),
    position(position),
    initialized(false)
{ }

Widget::Widget(Widget* guardian,
	       const Rect& position,
	       std::string properties) :
    Object(properties),
    parent(guardian),
    position(position),
    initialized(false)
{ }

Widget::Widget(const Widget& src) :
    Object(src),
    parent(src.parent),
    position(src.position),
    initialized(false)
{ }

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
    return *this;
}

Rect const
&Widget::Position() const
{
    return position;
}

typename Rect::rect_type
Widget::Left() const
{
    return position.Left();
}

typename Rect::rect_type
Widget::Right() const
{
    return position.Right();
}

typename Rect::rect_type
Widget::Width() const
{
    return position.Width();
}

typename Rect::rect_type
Widget::Top() const
{
    return position.Top();
}

typename Rect::rect_type
Widget::Bottom() const
{
    return position.Bottom();
}

typename Rect::rect_type
Widget::Height() const
{
    return position.Height();
}

Widget
&Widget::Position(Rect const &val)
{
    position = val;
    return *this;
}

Widget
&Widget::Left(typename Rect::rect_type val)
{
    position.Left(val);
    return *this;
}

Widget
&Widget::Right(typename Rect::rect_type val)
{
    position.Right(val);
    return *this;
}

Widget
&Widget::Width(typename Rect::rect_type val)
{
    position.Width(val);
    return *this;
}

Widget
&Widget::Top(typename Rect::rect_type val)
{
    position.Top(val);
    return *this;
}

Widget
&Widget::Bottom(typename Rect::rect_type val)
{
    position.Bottom(val);
    return *this;
}

Widget
&Widget::Height(typename Rect::rect_type val)
{
    position.Height(val);
    return *this;
}

bool
Widget::Initialized()
{
    return initialized;
}

Widget
&Widget::Initialized(bool state)
{
    initialized = state;
    return *this;
}

void
Widget::resize()
{
    // return *this;
}

#if defined(_DEBUG) || defined(_DEBUGWIDGETBOUNDARY)
void
Widget::draw() {
    rectangleColor(SDL_GetVideoSurface(),
		   Left(),
		   Top(),
		   Right(),
		   Bottom(),
		   0xFFFFFFFF);

    char debug_string[128];
    sprintf(debug_string, "Boundary: %d,%dx%dw,%dh",
	    Left(),
	    Top(),
	    Width(),
	    Height());

  logAppend(debug_string);

}
#endif
