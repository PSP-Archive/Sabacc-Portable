#ifndef __SMF_RECT_HH
#define __SMF_RECT_HH

/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 27, 2008
 *
 * File: Rect.hh
 *
 * An object for storing position and size.
 */

// Simple Directmedia Library headers
// ToDo: Replace with header defining just SDL_Rect
#if defined(__APPLE__)
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 20, 2008
 *
 * File: Rect.hh
 *
 * A rectangle class for storing positions and sizes.
 */

template <typename T> class BasicRect {

public:

    typedef T	rect_type;	/// Allow for easy changing of the internal type of the rectangle.

    /** Create a rectangle with the default size and position (0 wide, 0 high @ 0x0). */
    BasicRect() : left(0), top(0), width(0), height(0) { }
    /** Create a rectangle with the default size at @param left, @param top. */
    explicit BasicRect(rect_type left, rect_type top) : left(left), top(top), width(0), height(0) { }
    /** Create a rectangle at @param left, @param top, setting both width and height to @param size. */
    explicit BasicRect(rect_type left, rect_type top, rect_type size) : left(left), top(top), width(size), height(size) { }
    /** Create a rectangle at @param left, @param top, setting size to @param width and @param height. */
    explicit BasicRect(rect_type left, rect_type top, rect_type width, rect_type height) : left(left), top(top), width(width), height(height) { }
    /** Copy a source Rect's member values to this Rect. */
    BasicRect(const BasicRect<T>& src) : left(src.left), top(src.top), width(src.width), height(src.height) { }

    /** Assign another Rect's member values to this one. */
    BasicRect<rect_type>& operator=(const BasicRect<T>& src) {

        if (&src != this) {

            left = src.left;
            top = src.top;
            width = src.width;
            height = src.height;

        }	// if(not self)

        return(*this);
    }	// operator=

    /** The following methods get the indiviual member's values. @{ */
    inline const rect_type getLeft() {
        return(left);
    }
    inline const rect_type getTop() {
        return(top);
    }
    inline const rect_type getRight() {
        return(left + width);
    }
    inline const rect_type getBottom() {
        return(top + height);
    }
    inline const rect_type getWidth() {
        return(width);
    }
    inline const rect_type getHeight() {
        return(height);
    }
    /** @} */

    /** The following methods set the indiviual member's values. @{ */
    inline void setLeft(rect_type left) {
        this->left = left;
    }
    inline void setTop(rect_type top) {
        this->top = top;
    }
    inline void setRight(rect_type right) {
        this->width = right - this->left;
    }
    inline void setBottom(rect_type bottom) {
        this->height = bottom - this->top;
    }
    inline void setWidth(rect_type width) {
        this->width = width;
    }
    inline void setHeight(rect_type height) {
        this->height = height;
    }
    /** @} */

private:

    rect_type	left, top, width, height;

};	// class BasicRect

/** Common rect types easily referenced. @{ */
typedef BasicRect<int> Rect;
typedef BasicRect<float> Rectf;

static const Rect default_rect;
static const Rectf default_rectf;
/** @} */

/** A helper function for getting an SDL compatible Rect from a BasicRect<int>. */
inline SDL_Rect getSDLRect(Rect& src_rect) {
    SDL_Rect temp_rect = { src_rect.getLeft(), src_rect.getTop(), src_rect.getWidth(),src_rect.getHeight() };
    return(temp_rect);
}	// getSDLRect

#endif // __SMF_RECT_HH
