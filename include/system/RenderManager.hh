#ifndef __RENDERMANAGER_HH
#define __RENDERMANAGER_HH

/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 29, 2008
 *
 * File: RenderManager.hh
 *
 * A class for automatic rendering of Widgets.
 */

// Standard Library headers
#include <vector>
using std::vector;

// Simple Directmedia Layer headers
#if defined(__APPLE__)
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

// SMF headers
#include "base/Widget.hh"

class RenderManager
  {

  public:

    RenderManager();
    virtual ~RenderManager();

    /** RenderManager operators @{ */
    virtual RenderManager& operator<<(Widget*);
    virtual Widget* operator[](vector<Widget*>::size_type);	/// Allow for RM[] = x.
    virtual Widget* operator[](vector<Widget*>::size_type) const;	/// Read only.
    /** @} */

    /** Render queue operations. @{ */
    virtual void clear();	/// Clear the render queue.

    virtual void push(Widget*);	/// Add a Widget to the end of the queue.
    virtual void pop();	/// Remove the last Widget from the queue.

    virtual void erase(Widget*);
    virtual void erase(vector<Widget*>::iterator);	/// Erase the Widget indicated by iterator
    /** @} */

    /** Render queue status. @{ */
    virtual vector<Widget*>::size_type size();
    virtual bool empty();
    /** @} */

    /** Draw the Widgets in the queue. */
    virtual void draw();

  private:

    vector<Widget*>	render_queue;

    // Copying not allowed
    RenderManager(const RenderManager&);
    RenderManager& operator=(const RenderManager&);

    // Debugging members, note fancy physics style notation of start and finish fps :)
#if defined(_DEBUG) || defined(_DEBUGRENDERMANAGER) || defined(_DEBUGRENDERMANAGERFPS)
    int	frame_0, frame_1, fps_counter_0, fps_counter_1, delta;
    float fps_calculated;
#endif

  };	// class RenderManager

#endif // __RENDERMANAGER_HH
