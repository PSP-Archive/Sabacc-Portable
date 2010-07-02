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
    rect_type Left() const
	{
	    return(left);
	}
    rect_type Top() const
	{
	    return(top);
	}
    rect_type Right() const
	{
	    return(left + width);
	}
    rect_type Bottom() const
	{
	    return(top + height);
	}
    rect_type Width() const
	{
	    return(width);
	}
    rect_type Height() const
	{
	    return(height);
	}
    /** @} */

    /** The following methods set the indiviual member's values. @{ */
    BasicRect &Left(rect_type left) {
        this->left = left;
	return *this;
    }
    BasicRect &Top(rect_type top) {
        this->top = top;
	return *this;
    }
    BasicRect &Right(rect_type right) {
        this->width = right - this->left;
	return *this;
    }
    BasicRect &Bottom(rect_type bottom) {
        this->height = bottom - this->top;
	return *this;
    }
    BasicRect &Width(rect_type width) {
        this->width = width;
	return *this;
    }
    BasicRect &Height(rect_type height) {
        this->height = height;
	return *this;
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
inline SDL_Rect SDLRect(Rect const &src_rect) {
    SDL_Rect temp_rect = {
	static_cast<Sint16>(src_rect.Left()),
	static_cast<Sint16>(src_rect.Top()),
	static_cast<Sint16>(src_rect.Width()),
	static_cast<Sint16>(src_rect.Height())
    };
    return(temp_rect);
}	// getSDLRect

#endif // __SMF_RECT_HH
