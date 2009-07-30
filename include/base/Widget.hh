#ifndef __SMF_WIDGET_H
#define __SMF_WIDGET_H

/*
 * Sabacc Media Framework
 *
 * Author: goumba
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
    Widget(Widget*, const Rect& = default_rect, std::string = "autosize: true;");
    Widget(const Widget& other);
    virtual ~Widget();
    /** @} */

    virtual Widget& operator=(const Widget&);	/// Gives this Widget's members the values of another Widget.

    /** Parentage functions. @{ */
    Widget* const getParent();
    void setParent(Widget*);
    /** @} */

    /** position retrieval and modification functions. @{ */
    virtual inline const Rect& getPosition();	/// Get the position of the Widget, as a Rect.
    virtual inline const Rect::rect_type getLeft();	/// Get the leftmost boundary of the Widget.
    virtual inline const Rect::rect_type getTop();	/// Get the topmost boundary of the Widget.
    virtual inline const Rect::rect_type getRight();	/// Get the rightmost boundary of the Widget.
    virtual inline const Rect::rect_type getBottom();	/// Get the bottommost boundary of the Widget.
    virtual inline const Rect::rect_type getWidth();	/// Get the width of the Widget.
    virtual inline const Rect::rect_type getHeight();	/// Get the height of the Widget.
    virtual void setposition(const Rect&);	/// Set the position @param @c position of the Widget.
    virtual void setLeft(Rect::rect_type);	/// Make @param @c left the leftmost boundary of the Widget.
    virtual void setTop(Rect::rect_type);	/// Make @param @c top the topmost boundary of the Widget.
    virtual void setRight(Rect::rect_type);	/// Make @param @c right the rightmost boundary of the Widget.
    virtual void setBottom(Rect::rect_type);	/// Make @param @c bottom the bottomtmost boundary of the Widget.
    virtual void setWidth(Rect::rect_type);	/// Set the width of the Widget to @param @c width.
    virtual void setHeight(Rect::rect_type);	/// Set the height of the Widget to @param @c height.
    /** @} */

    /** Initialization and cleanup functions which must be overloaded by derived classes. @{ */
    virtual void init() = 0;
    virtual void cleanup() = 0;
    /** @} */

    /** Initialization status functions. @{ */
    virtual inline bool isInitialized();
    virtual inline void setInitialized();
    virtual inline void setNotInitialized();
    /** @} */

    /** Resize the widget: changes the dimensions of @c Widget::position to those of surface in most cases. */
    virtual void resize();

    /** All derived classes must have a draw method, this is for drawing when called, i.e. from a render queue. */
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
