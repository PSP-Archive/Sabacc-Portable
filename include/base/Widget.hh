#ifndef __SMF_WIDGET_H
#define __SMF_WIDGET_H

/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 27, 2008
 *
 * File:Widget.hh
 *
 * The base object for all toolkit widgets.
 * TODO: CREATE ANIMATION CONTROLS
 */

// SMF headers
#include "base/Object.hh"
#include "geometry/Rect.hh"

class Widget : public Object {
public:
    /** Constructors @{ */
    Widget(const Rect& = default_rect, std::string = "autosize: true;");
    Widget(Widget*, const Rect& = default_rect,
	   std::string = "autosize: true;");
    Widget(const Widget& other);
    virtual ~Widget();
    /** @} */

    virtual Widget& operator=(const Widget&); 

    /** Parentage functions. @{ */
    Widget const *Parent();
    Widget &Parent(Widget*);
    /** @} */

    /** position retrieval and modification functions. @{ */
    Rect const
    &Position() const
	{
	    return position;
	}

    typename Rect::rect_type
    Left() const
	{
	    return position.Left();
	}

    typename Rect::rect_type
    Right() const
	{
	    return position.Right();
	}

    typename Rect::rect_type
    Width() const
	{
	    return position.Width();
	}

    typename Rect::rect_type
    Top() const
	{
	    return position.Top();
	}

    typename Rect::rect_type
    Bottom() const
	{
	    return position.Bottom();
	}

    typename Rect::rect_type
    Height() const
	{
	    return position.Height();
	}

    Widget
    &Position(Rect const &val)
	{
	    position = val;
	    return *this;
	}

    Widget
    &Left(typename Rect::rect_type val)
	{
	    position.Left(val);
	    return *this;
	}

    Widget
    &Right(typename Rect::rect_type val)
	{
	    position.Right(val);
	    return *this;
	}

    Widget
    &Width(typename Rect::rect_type val)
	{
	    position.Width(val);
	    return *this;
	}

    Widget
    &Top(typename Rect::rect_type val)
	{
	    position.Top(val);
	    return *this;
	}

    Widget
    &Bottom(typename Rect::rect_type val)
	{
	    position.Bottom(val);
	    return *this;
	}

    Widget
    &Height(typename Rect::rect_type val)
	{
	    position.Height(val);
	    return *this;
	}

    /** Initialization and cleanup functions which must be 
	overloaded by derived classes. @{ */
    virtual void init() = 0;
    virtual void cleanup() = 0;
    /** @} */

    /** Initialization status functions. @{ */
    bool
    isInitialized() { return initialized; }

    void
    setInitialized() { initialized = true; }
    
    void
    setNotInitialized() { initialized = false; }
    /** @} */

    /** Resize the widget: changes the dimensions of @c Widget::position 
	to those of surface in most cases. */
    virtual void resize();

    /** All derived classes must have a draw method, this is for drawing 
	when called, i.e. from a render queue. */
#if defined(_DEBUG)
    virtual void draw();
#else
    virtual void draw() = 0;
#endif
    
private:

    Widget*	parent;
    Rect	position;
    bool	initialized;
};

#endif	// __SMF_WIDGET_H
