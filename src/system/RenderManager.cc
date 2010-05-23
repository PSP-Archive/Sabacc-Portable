
/**
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 29, 2008
 *
 * File: RenderManager.cc
 *
 * A class for automatic rendering of Widgets.
 *
 * ToDo:
 * - Create a SmartRenderer which will be the preferred method of
 * rendering as it will automatically
 * - A virtual function for user defined event processing:
 *   (bool)(userEventFn*)(const SDL_UserEvent&)
 */

// Standard Library headers
#include <algorithm>
#include <functional>
#include <vector>
using std::mem_fun;
using std::vector;

// Simple Directmedia Layer headers
#if defined(__APPLE__)
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

// SMF headers
#include "base/Widget.hh"
#include "base/Control.hh"
#include "system/RenderManager.hh"

// SMF Exception headers
#include "exceptions/base/ExceptionBase.hh"
#include "exceptions/system/RenderManagerExceptions.hh"

// SMF Debugging headers
#if defined(_DEBUG) || defined(_DEBUGRENDERMANAGER) || defined(_DEBUGRENDERMANAGERFPS) || defined(_DEBUGEVENTS)
#include <cstdlib>
#include "utility/Log.hh"

#if defined(__APPLE__)
#include <SDL/SDL_gfxPrimitives.h>
#else
#include <SDL_gfxPrimitives.h>
#endif	// __APPLE__

#endif	// Debug

RenderManager::RenderManager() : render_queue()
#if defined(_DEBUG) || defined(_DEBUGRENDERMANAGER) || defined(_DEBUGRENDERMANAGERFPS)
        , frame_0(0), frame_1(0), fps_counter_0(0), fps_counter_1(0), delta(0), fps_calculated(0.0f)
#endif
{ }
RenderManager::~RenderManager() { }

RenderManager& RenderManager::operator<<(Widget* w) {
    render_queue.push_back(w);
    return(*this);
}	// operator<<
Widget* RenderManager::operator[](vector<Widget*>::size_type index) {
    return(render_queue[index]);
}
Widget* RenderManager::operator[](vector<Widget*>::size_type index) const {
    return(render_queue[index]);
}

void RenderManager::clear() {
    render_queue.clear();
}

void RenderManager::push(Widget* w) {
    render_queue.push_back(w);
}
void RenderManager::pop() {
    render_queue.pop_back();
}

void RenderManager::erase(Widget* to_erase)
{
  // Fix this to use find algo
  for(vector<Widget*>::iterator finder = render_queue.begin(); render_queue.end() != finder; ++finder)
    {
      if(to_erase == *finder) erase(finder);
      break;
    }
  return;
}// erase(Widget*)
void RenderManager::erase(vector<Widget*>::iterator widget_iter) {
    render_queue.erase(widget_iter);
}

vector<Widget*>::size_type RenderManager::size() {
    return(render_queue.size());
}
bool RenderManager::empty() {
    return(render_queue.empty());
}

void RenderManager::draw() {
#if defined(_DEBUG) || defined(_DEBUGRENDERMANAGER) || defined(_DEBUGRENDERMANAGERFPS)
    char renderer_debug[128];
#endif

    /* Blank screen before each frame, sometimes needed. If this slows down rendering
     * too much, may be removed at a future date. */
    SDL_FillRect(SDL_GetVideoSurface(), NULL, 0x0);
//    for (std::vector<Widget*>::size_type n = 0; render_queue.size() > n; ++n) {
//		SDL_FillRect(SDL_GetVideoSurface(), &getSDLRect(const_cast<Rect&>(render_queue[n]->getPosition())), 0x00000000);
//    }	// for(black_rectangle)

        /* Why do the most recently added widgets get rendered first?
         * Most likely things like menus and buttons will be added last and these are the
         * widgets we want the user to interact with. It seems more natural this way
         * than adding things in reverse in the ui building code (i.e. adding the backdrop last.) */

    try {

      for_each(render_queue.begin(), render_queue.end(), mem_fun(&Widget::draw));

      } catch (Exceptions::ExceptionBase& exc) {

#if defined(_DEBUG) || defined(_DEBUGRENDERMANAGER)
      logAppend(exceptionString(exc));
#endif

      } catch (...) {	// Everything else propogates
            throw;
      }// try

    if (-1 == SDL_Flip(SDL_GetVideoSurface())) {

#if defined(_DEBUG) || defined(_DEBUGRENDERMANAGER)
        logAppend("Could not swap screen buffers.");
        logAppend(SDL_GetError());
#endif

        throw(Exceptions::RenderManager::FlipFailed("Could not swap buffers.", SDL_GetError(), "", __FILE__, __LINE__));

    }	// if(!Flip)

#if defined(_DEBUG) || defined(_DEBUGRENDERMANAGER) || defined(_DEBUGRENDERMANAGERFPS)

    if (0 == fps_counter_0) fps_counter_0 = SDL_GetTicks();
    fps_counter_1 = SDL_GetTicks();
    delta = fps_counter_1 - fps_counter_0;
    if ((0 < fps_counter_1) && (0 < delta)) {
        fps_calculated = static_cast<float>(frame_1 - frame_0) / static_cast<float>(delta / 1000);
    }	// if(...)

    char fps_formatted[128];

    sprintf(fps_formatted, "%d (%.2f)", frame_1, fps_calculated);

    if (1.0f < fps_calculated) {

      stringColor(SDL_GetVideoSurface(), 200, 0, fps_formatted, 0xFFFFFFFF);

        if (0 == (delta % 100)) {
            logAppend(fps_formatted);
        }	// if(frames % 100)

    }	// if(fps)

    frame_1++;

#endif	// _DEBUGFPS

}	// draw
