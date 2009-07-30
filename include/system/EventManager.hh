#ifndef __SMF_EVENTMANAGER_HH
#define __SMF_EVENTMANAGER_HH

/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: September 30, 2008
 *
 * File: EventManager.hh
 *
 * Stores widgets for event calls.
 * The RenderManager class used to handle this functionality, except
 * that it always cast a Widget* to a Control* costing time.
 *
 * Some functions will take Widget* and the Widget will be cast to a Control.
 */

// Standard Library headers
#include <std::vector>


// Simple Directmedia Library headers
#if defined(__APPLE__)
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

// Sabacc Media Framework headers
#include "base/Control.hh"
#include "base/Widget.hh"
#include "constants/EventConstants.hh"

class EventManager {

public:

  /** @{ */
  EventManager();
  virtual ~EventManager();
  /** @} */

    /** EventManager operators @{ */
    virtual EventManager& operator<<(Control*);
  virtual EventManager& operator<<(Widget*);
    virtual Control* operator[](std::vector<Control*>::size_type);	/// Allow for EM[] = x.
    virtual Control* operator[](std::vector<Control*>::size_type) const;	/// Read only.
    /** @} */

    /** Event queue operations. @{ */
    virtual void clear();	/// Clear the event queue.

  virtual void push(Control*);	/// Add a Control to the end of the queue.
  virtual void push(Widget*);  /// Add a Widget, conversion done in function
    virtual void pop();	/// Remove the last Widget from the queue.

    virtual void erase(std::vector<Control*>::iterator);	/// Erase the Widget indicated by iterator
    /** @} */

    /** Event queue status. @{ */
    virtual std::vector<Control*>::size_type size();
    virtual bool empty();
    /** @} */

    /** Draw the Widgets in the queue. */
    virtual smf_event_t doEvents();

  /** Set up user event handlers. @{ */
  //  virtual void addUserEventHandler(user_fn*(int));

private:

    std::vector<Control*>	event_queue;

    SDL_Event	system_event;

  //  std::list<user_fn*(int)> user_event_handlers;

    // Copying not allowed
    EventManager(const EventManager&);
    EventManager& operator=(const EventManager&);

};	// class EventManager

#endif // __SMF_EVENTMANAGER_HH
