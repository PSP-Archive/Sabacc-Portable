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
    Rect const &Position() const;
    typename Rect::rect_type Left() const;
    typename Rect::rect_type Right() const;
    typename Rect::rect_type Width() const;
    typename Rect::rect_type Top() const;
    typename Rect::rect_type Bottom() const;
    typename Rect::rect_type Height() const;

    Widget &Position(Rect const &);
    Widget &Left(typename Rect::rect_type);
    Widget &Right(typename Rect::rect_type);
    Widget &Width(typename Rect::rect_type);
    Widget &Top(typename Rect::rect_type);
    Widget &Bottom(typename Rect::rect_type);
    Widget &Height(typename Rect::rect_type);

    /** Initialization and cleanup functions which must be 
	overloaded by derived classes. @{ */
    virtual void init() = 0;
    virtual void cleanup() = 0;
    /** @} */

    /** Initialization status functions. @{ */
    bool Initialized();

    Widget &Initialized(bool);
    /** @} */

    // for backwards compat
    bool isInitialized() { return Initialized(); }
    void setInitialized() {  Initialized(true); }
    void setNotInitialized() { Initialized(false); }

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
