/**
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: September 8, 2008
 *
 * File: StaticMenu.cc
 *
 * A control in which choices will be displayed from which the user
 * can select one.
 *
 * To do:
 * - When menu item is chosen (Enter, button, etc.) push an event on to the event
 *   stack that will be processed, such as type = MENU_ITEM_SELECTED. This will
 *   allow us to use the normal event processing loop for handling custom cases.
 */

// Standard Library headers
#include <string>
#include <utility>
#include <vector>
using std::pair;
using std::string;
using std::vector;

// SMF headers
#include "base/FreeTypeControl.hh"
#include "base/Widget.hh"
#include "controls/StaticMenu.hh"
#include "geometry/Rect.hh"

// SMF Constants
#include "constants/EventConstants.hh"
#include "constants/PSPControlConstants.hh"

// SMF FreeType headers
#include "freetype/FontManager.hh"
#include "freetype/FreeTypeRenderer.hh"

// SMF Exceptions
#include "exceptions/events/EventExceptions.hh"

// SMF Debugging
#if defined(_DEBUG) || defined(_DEBUGSTATICMENU) || defined(_DEBUGEVENTS)
#include <cstdlib>
#include "utility/Log.hh"
#endif

Menu::Menu(string title, const Rect& position, string property_string) :
    FreeTypeControl(position, property_string), menu_title(title), items(),
    selector() { }
Menu::Menu(vector<ItemType> initial_items, string title, const Rect& position, string property_string) :
    FreeTypeControl(position, property_string), menu_title(title), items(initial_items),
    selector() { }
Menu::Menu(Widget* guardian, string title, const Rect& position, string property_string) :
    FreeTypeControl(guardian, position, property_string), menu_title(title), items(),
    selector() { }
Menu::Menu(Widget* guardian, vector<ItemType> initial_items, string title, const Rect& position, string property_string) :
    FreeTypeControl(guardian, position, property_string), menu_title(title), items(initial_items),
    selector() { }
Menu::Menu(const Menu& src) : FreeTypeControl(src), menu_title(src.menu_title), items(src.items),
    selector() { }
Menu::~Menu() { }

Menu& Menu::operator=(const Menu& src)
{

  if (&src != this)
    {

      FreeTypeControl::operator=(src);

      menu_title = src.menu_title;
      items = src.items;

      selector = items.begin();
    }	// if(not the same instance)

  return(*this);
}	// operator=

string Menu::getTitle()
{
  return(menu_title);
}
void Menu::setTitle(string new_title)
{
  menu_title = new_title;
}

void Menu::add(string item_text, string help_text)
{
  if (1 > item_text.length()) return;	// No need for error.

  pair<string, string> temp_item(item_text, help_text);
  items.push_back(temp_item);

  selector = items.begin();
}	// add
void Menu::insert(const string& item_to_follow, const string& item_text, const string& item_help)
{
  vector<ItemType>::iterator following = items.begin();
  for (vector<ItemType>::iterator follow_iter = items.begin(); items.end() != follow_iter; ++follow_iter)
    {
      if (item_to_follow == follow_iter->first)
        {
          following = follow_iter;
          break;
        }//if
    }// for

  insert(following, item_text, item_help);
}// insert(string, string, string)
void Menu::insert(vector<ItemType>::iterator item_following, const string& item_text, const string& item_help)
{
  insert(item_following, ItemType(item_text, item_help));
}//insert(iter)
void Menu::insert(vector<ItemType>::iterator item_following, ItemType new_item)
{
  // First check to see if the item exists
  // Fix this to use find algorithm
  for (vector<ItemType>::iterator finder = items.begin(); items.end() != finder; ++finder)
    if ((finder->first == new_item.first) && (finder->second == new_item.second)) return;

  // We want to add the item after the one specified. vector::insert inserts it
  // before, so we will adjust, making sure we can, first.
  if (items.end() != item_following + 1) ++item_following;
  items.insert(item_following, new_item);
}//insert(iter, ItemType)

void Menu::erase(const string& item_text)
{
  for (vector<ItemType>::iterator it = items.begin(); items.end() != it; ++it)
    {
      if (item_text == it->first)
        {
          items.erase(it);
          break;
        }	// if(found)
    }	// for(it)
}	// erase
void Menu::erase(vector<ItemType>::iterator item_iterator)
{
  items.erase(item_iterator);
}	// erase
void Menu::clear()
{
  items.clear();
}

void Menu::previousItem()
{

#if defined(_DEBUG) || defined(_DEBUGSTATICMENU)
  logAppend("Selecting previous item");
#endif

  if (2 > items.size()) return;

  if (items.begin() > --selector) selector = items.begin();
  setNotInitialized();
}	// previousItem
void Menu::nextItem()
{

#if defined(_DEBUG) || defined(_DEBUGSTATICMENU)
  logAppend("Selecting next item");
#endif

  if (2 > items.size()) return;

  if (items.end() <= ++selector) selector = items.end() - 1;
  setNotInitialized();
}	// nextItem
const string& Menu::getSelectedItemText()
{
  return(selector->first);
}
vector<ItemType>::const_iterator Menu::getSelectedIterator()
{
  return(selector);
}

bool Menu::onKeyDown(const SDL_KeyboardEvent& key)
{

#if defined(_DEBUG) || defined(_DEBUGSTATICMENU) || defined(_DEBUGEVENTS)
  logAppend("Handling keyboard event.");
#endif

  if (SDLK_UP == key.keysym.sym) previousItem();
  if (SDLK_DOWN == key.keysym.sym) nextItem();
  if (SDLK_RETURN == key.keysym.sym)
    {

      SDL_Event selection_made;
      selection_made.type = SDL_USEREVENT;
      selection_made.user.code = event_menu_item_selected;

      if (-1 == SDL_PushEvent(&selection_made))
        {

          throw(Exceptions::Events::PushFailed("Push failed.", "The event for menu selection could not be pushed into the event queue.",
                                               SDL_GetError(), __FILE__, __LINE__));

        }	// if(SDL_PushEvent)

    }	// Enter/Return

  /* We processed events with this control.
   * If a menu is present, it is likely the only one to process events, so
   * prevent other controls from processing it as well. */
  return(true);

}	// onKeyDown
bool Menu::onKeyUp(const SDL_KeyboardEvent&)
{
  return(false);
}

bool Menu::onJoystickButtonDown(const SDL_JoyButtonEvent& joystick_event)
{

  if (1 > SDL_NumJoysticks()) return(false);// Don't process if none found
#if defined(__PSP__)
  const Uint8& button = joystick_event.button;

  if (psp_dpad_up == button) previousItem();
  if (psp_dpad_down == button) nextItem();

  if ((psp_cross == button) || (psp_start == button))
    {

      SDL_Event selection_made;
      selection_made.type = SDL_USEREVENT;
      selection_made.user.code = event_menu_item_selected;

      if (-1 == SDL_PushEvent(&selection_made))
        {

          throw(Exceptions::Events::PushFailed("Push failed.", "The event for menu selection could not be pushed into the event queue.",
                                               SDL_GetError(), __FILE__, __LINE__));

        }	// if(SDL_PushEvent)

    }	// Enter/Return

  /* We processed events with this control.
   * If a menu is present, it is likely the only one to process events, so
   * prevent other controls from processing it as well. */
  return(true);
#endif

  /* Right now (and for the forseeable future), SMF does not support
     game controllers other than that of the PSP, so ignore this event. */
  return(false);

}
bool Menu::onJoystickButtonUp(const SDL_JoyButtonEvent&)
{
  return(false);
}
bool Menu::onJoystickMove(const SDL_JoyAxisEvent& joystick_event)
{

  /* if(1 > SDL_NumJoysticks()) return(false);// Don't process if none found

  #if defined(__PSP__)

  const Sint16& axis_position = joystick_event.value;
  if(-100 > axis_position) previousItem();
  if(100 < axis_position) nextItem();

  return(true);
  #endif

  // See note in joystick button down */
  return(false);

}

bool Menu::onMouseButtonDown(const SDL_MouseButtonEvent&)
{
  return(false);
}
bool Menu::onMouseButtonUp(const SDL_MouseButtonEvent&)
{
  return(false);
}

void Menu::init()
{

  if ((items.begin() > selector) || (items.end() < selector)) selector = items.begin();

#if defined(_DEBUG) || defined(_DEBUGSTATICMENU)
  char debug_string[128];
  logAppend("Setting up menu.");
#endif

  int largest_width = 0;
  int largest_height = 0;

  for (vector<ItemType>::iterator size_iter = items.begin(); items.end() > size_iter; ++size_iter)
    {

      pushState();

      int help_height = textHeight(size_iter->second);
      int help_width = textWidth(size_iter->second);

      pushState();

      setTextSize(getTextSize() * 1.5f);

      int item_height = textHeight(size_iter->first) - (getFontAscent() * 0.25f);
      int item_width = textWidth(size_iter->first);

      popState();

      setTextStyle(font_style_bold | font_style_underline);
      int title_height = textHeight(menu_title) - (getFontAscent() * 0.25f);
      int title_width = textWidth(menu_title);

      popState();

      if (largest_width < help_width) largest_width = help_width;
      if (largest_width < item_width) largest_width = item_width;
      if (largest_width < title_width) largest_width = title_width;

      int total_height = help_height + item_height + title_height;
      if (largest_height < total_height) largest_height = total_height;

    }	// for(size_iter)


#if defined(_DEBUG) || defined(_DEBUGSTATICMENU)
  logAppend("Got dimensions.");
#endif

  // If we have set the size of the control manually, adjust here
  if (string::npos == getProperty("autosize").find("true"))
    {
      largest_width = (Width() > largest_width)
                      ? Width() : largest_width;
      largest_height = (Height() > largest_height)
                       ? Height() : largest_height;
    }

  setSurface(SDL_AllocSurface(SDL_SWSURFACE, largest_width, largest_height,
                              32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000));

#if defined(_DEBUG) || defined(_DEBUGSTATICMENU)
  sprintf(debug_string, "Created a surface of %dwx%dh", largest_width, largest_height);
  logAppend(debug_string);
#endif

  pushState();
  pushState();

  int y_pos = (largest_height * -0.33);

  setTextStyle(font_style_bold | font_style_underline);
  render(menu_title, getSurface(), 0, y_pos);

  popState();

#if defined(_DEBUG) || defined(_DEBUGSTATICMENU)
  logAppend("Rendered menu title: " + menu_title);
#endif

  y_pos += textHeight(menu_title) - (getFontAscent() * 0.25f);

  pushState();

  setTextSize(getTextSize() * 1.5f);

  render(selector->first, getSurface(), 0, y_pos);

#if defined(_DEBUG) || defined(_DEBUGSTATICMENU)
  logAppend("Rendered menu item: " + selector->first);
#endif

  y_pos += textHeight(selector->first) - (getFontAscent() * 0.25f);

  popState();

  render(selector->second, getSurface(), 0, y_pos);

#if defined(_DEBUG) || defined(_DEBUGSTATICMENU)
  logAppend("Rendered menu help: " + selector->second);
#endif

  popState();


#if defined(_DEBUG) || defined(_DEBUGSTATICMENU)
  logAppend("Rendered.");
#endif

  // Automatic adjustment of Widget size
  if (string::npos != getProperty("autosize").find("true"))
    {
      Width(getSurface()->w);
      Height(getSurface()->h);
    }	// getProperty("all")

#if defined(_DEBUG) || defined(_DEBUGSTATICMENU)
  logAppend("Converting rendered surface to display alpha format.");
#endif

  setSurface(SDL_DisplayFormatAlpha(getSurface()));

  setInitialized();

}	// init
void Menu::cleanup()
{

  setSurface(0);

  setNotInitialized();

}	// cleanup

void Menu::draw()
{

#if defined(_DEBUG) || defined(_DEBUGSTATICMENU)
  logAppend("Passing off drawing to Control.");
#endif

  FreeTypeControl::draw();

}	// draw
