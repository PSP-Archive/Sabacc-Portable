/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: September 30, 2008
 *
 * File: EventManager.cc
 *
 * Stores widgets for event calls.
 * The RenderManager class used to handle this functionality, except
 * that it always cast a Widget* to a Control* costing time.
 *
 * Some functions will take Widget* and the Widget will be cast to a Control.
 */

// Standard library headers
#include <vector>
using std::vector;

// Sabacc Media Framework headers
#include "base/Control.hh"
#include "base/Widget.hh"
#include "constants/EventConstants.hh"
#include "system/EventManager.hh"

// Sabacc Media Framework debugging headers
#if defined(_DEBUG) || defined(_DEBUGEVENTS)
#include <cstdlib>
#include "utility/Log.hh"
#endif

EventManager::EventManager() : event_queue(), system_event() { }
EventManager::~EventManager() { }

EventManager& EventManager::operator<<(Control* ctl) {
    event_queue.push_back(ctl);
    return(*this);
}	// operator<<
EventManager& EventManager::operator<<(Widget* wgt) {
  Control* c_test = dynamic_cast<Control*>(wgt);
  if(c_test) {
    return(operator<<(c_test));
  } else {
    //    throw(Exceptions::EventManager::NotAControl("You tried to add a Widget to the event manager that is not a control."));
  }

  return(*this);
} // operator<<(Widget*)

Control* EventManager::operator[](vector<Control*>::size_type index) {
    return(event_queue[index]);
} // operator[]
Control* EventManager::operator[](vector<Control*>::size_type index) const 
{
  return(event_queue[index]);
} // operator[]

void EventManager::clear() 
{
  event_queue.clear();
} // clear
void EventManager::push(Control* ctl) 
{
  event_queue.push_back(ctl);
} // push(Control*)
void EventManager::push(Widget* wgt)
{
  Control* c_test = dynamic_cast<Control*>(wgt);
  if(c_test) {
    push(c_test);
  } else {
    //    throw(Exceptions::EventManager::NotAControl("You tried to add a Widget to the event manager that is not a control."));
  }
} // push(Widget*)
void EventManager::pop()
{
  event_queue.pop_back();
} // pop

void EventManager::erase(vector<Control*>::iterator ctl_iter)
{
  event_queue.erase(ctl_iter);
} // erase

vector<Control*>::size_type EventManager::size() 
{
  return(event_queue.size());
} // size
bool EventManager::empty() 
{
  return(event_queue.empty());
} // empty

smf_event_t EventManager::doEvents() 
{

    // Before rendering, capture any events.
    int got_event = SDL_PollEvent(&system_event);

    if(got_event) {

      if (SDL_QUIT == system_event.type) return(event_quit);

      for (vector<Control*>::reverse_iterator control_iter = event_queue.rbegin();
            event_queue.rend() != control_iter; ++control_iter) {

      Control* control = *control_iter;

            if (control) {

#if defined(_DEBUG) || defined(_DEBUGEVENTS)
                logAppend("Passing events to controls.");
#endif

                switch (system_event.type) {

                case SDL_KEYDOWN:

#if defined(_DEBUG) || defined(_DEBUGEVENTS)
                    logAppend("Key down.");
#endif

                    if (control->onKeyDown(system_event.key)) got_event = 0;
                    break;
                case SDL_KEYUP:

#if defined(_DEBUG) || defined(_DEBUGEVENTS)
                    logAppend("Key up.");
#endif

                    if (control->onKeyUp(system_event.key)) got_event = 0;
                    break;
                case SDL_MOUSEMOTION:

#if defined(_DEBUG) || defined(_DEBUGEVENTS)
                    logAppend("Mouse moved.");
#endif

                    if (control->onMouseMove(system_event.motion)) got_event = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:

#if defined(_DEBUG) || defined(_DEBUGEVENTS)
                    logAppend("Mouse button down.");
#endif

                    if (control->onMouseButtonDown(system_event.button)) got_event = 0;
                    break;
                case SDL_MOUSEBUTTONUP:

#if defined(_DEBUG) || defined(_DEBUGEVENTS)
                    logAppend("Mouse button up.");
#endif

                    if (control->onMouseButtonUp(system_event.button)) got_event = 0;
                    break;
                case SDL_JOYAXISMOTION:

#if defined(_DEBUG) || defined(_DEBUGEVENTS)
                    logAppend("Joystick movement.");
#endif

                    if (control->onJoystickMove(system_event.jaxis)) got_event = 0;
                    break;
                case SDL_JOYBALLMOTION:

#if defined(_DEBUG) || defined(_DEBUGEVENTS)
                    logAppend("Joystick ball movement.");
#endif

                    if (control->onJoystickBall(system_event.jball)) got_event = 0;
                    break;
                case SDL_JOYHATMOTION:

#if defined(_DEBUG) || defined(_DEBUGEVENTS)
                    logAppend("Joystick hat activated.");
#endif

                    if (control->onJoystickHat(system_event.jhat)) got_event = 0;
                    break;
                case SDL_JOYBUTTONDOWN:

#if defined(_DEBUG) || defined(_DEBUGEVENTS)
                    logAppend("Justick button down.");
#endif

                    if (control->onJoystickButtonDown(system_event.jbutton)) got_event = 0;
                    break;
                case SDL_JOYBUTTONUP:

#if defined(_DEBUG) || defined(_DEBUGEVENTS)
                    logAppend("Joystick button up.");
#endif

                    if (control->onJoystickButtonUp(system_event.jbutton)) got_event = 0;
                    break;
                case SDL_USEREVENT:

#if defined(_DEBUG) || defined(_DEBUGEVENTS)
                    char user_event_debug_string [128];
                    sprintf(user_event_debug_string, "User event: 0x%x.", system_event.user.code);
                    logAppend(user_event_debug_string);
#endif
		    
		    // Some user events we can handle here, but not all.
		    
		    // otherwise, return the code
		    return(system_event.user.code);

                    break;
                default:
                    break;
                }	// switch(event.type)

            }	// if(is a control)

      }	// for(widget iterator)

      // This moved to the end in case we want to allow a control to process Escape.
      // This presented a problem in diagnosing why a dialog would not return the proper
      // code when escape was pressed. Move this back to under "if(SDL_QUIT..." if being
      // here it presents a problem.
      if ((SDL_KEYDOWN == system_event.type) && (SDLK_ESCAPE == system_event.key.keysym.sym)) return(event_quit);

    }// if(got_event)

  return(event_none);
} // doEvents

/* void EventManager::addUserEventHandler((user_fn*)(int)) 
{
} // addUserEventHandler */

