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

/// Allow for easy changing of the internal type of the rectangle.
    typedef T	rect_type;	

    /** Create a rectangle with the default size and position 
	(0 wide, 0 high @ 0x0). */
    BasicRect() : x1(0), x2(0), y1(0), y2(0) { }
    /** Create a rectangle with the default size at @param left, @param top. */
    BasicRect(rect_type left, rect_type top) : 
	x1(left), x2(0), y1(top), y2(0) { }
    /** Create a rectangle at @param left, @param top,
	setting both width and height to @param size. */
    BasicRect(rect_type left, rect_type top, rect_type size) : 
	x1(left), x2(left + size), y1(top), y2(top + size) { }
    /** Create a rectangle at @param left, @param top, setting size to 
	@param width and @param height. */
    explicit BasicRect(rect_type left, rect_type top,
		       rect_type width, rect_type height) : 
	x1(left), x2(left + width), y1(top), y2(top + height) { }
    /** Copy a source Rect's member values to this Rect. */
    BasicRect(const BasicRect<T>& other) : 
	x1(other.x1), x2(other.x2), y1(other.y1), y2(other.y2) { }

    /** Assign another Rect's member values to this one. */
    BasicRect<rect_type>& operator=(const BasicRect<T>& other) {

        if (&other != this) {

            x1 = other.x1;
            x2 = other.x2;
            y1 = other.y1;
            y2 = other.y2;

        }	// if(not self)

        return(*this);
    }	// operator=

    const rect_type
    Left() const
	{
	    return x1;
	}

    const rect_type
    Right() const
	{
	    return x2;
	}

    const rect_type
    Width() const
	{
	    return x2 - x1;
	}

    const rect_type
    Top() const
	{
	    return y1;
	}

    const rect_type
    Bottom() const
	{
	    return y2;
	}

    const rect_type
    Height() const
	{
	    return y2 - y1;
	}

    BasicRect
    &Left(rect_type val)
	{
	    x1 = val;
	    return *this;
	}

    BasicRect
    &Right(rect_type val)
	{
	    x2 = val;
	    return *this;
	}

    BasicRect
    &Width(rect_type val)
	{
	    x2 = val - x1;
	    return *this;
	}

    BasicRect
    &Top(rect_type val)
	{
	    y1 = val;
	    return *this;
	}

    BasicRect
    &Bottom(rect_type val)
	{
	    x2 = val;
	    return *this;
	}

    BasicRect
    &Height(rect_type val)
	{
	    y2 = val - y1;
	    return *this;
	}

private:

    rect_type	x1, x2, y1, y2;

};// class BasicRect

/** Common rect types easily referenced. @{ */
typedef BasicRect<int> Rect;
typedef BasicRect<float> Rectf;

static const Rect default_rect;
static const Rectf default_rectf;
/** @} */

/** A helper function for getting an SDL compatible Rect from a BasicRect<int>. */
template <typename RectType>
SDL_Rect getSDLRect(BasicRect<RectType> const &src_rect)
{
    SDL_Rect temp_rect =
	{
	    static_cast<Sint16>(src_rect.Left()),
	    static_cast<Sint16>(src_rect.Top()),
	    static_cast<Sint16>(src_rect.Width()),
	    static_cast<Sint16>(src_rect.Height())
	};
    
    return temp_rect;
}	// getSDLRect

#endif // __SMF_RECT_HH
