#ifndef __SMF_STATICMENU_HH
#define __SMF_STATICMENU_HH

/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: September 7, 2008
 *
 * File: StaticMenu.hh
 *
 * A control in which choices will be displayed from which the user
 * can select one.
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
#include "geometry/Rect.hh"

// SMF Type definitions
typedef pair<string, string> ItemType;

class Menu : public FreeTypeControl
  {

  public:

    /** Menu constructors:
     * @param title - the menu title.
     * @param position - the position to display the image.
     * @param property_string - properties for displaying the text.
     *        Can be any of the startdard Object, Widget and TextLabel properties.
     * @{ */
    Menu(string = "Menu", const Rect& = default_rect, string = "autosize: true; justify: center");
    Menu(vector<ItemType>, string = "Menu", const Rect& = default_rect, string = "autosize: true; justify: center");
    Menu(Widget*, string = "Menu", const Rect& = default_rect, string = "autosize: true; justify: center");
    Menu(Widget*, vector<ItemType>, string = "Menu", const Rect& = default_rect, string = "autosize: true; justify: center");
    Menu(const Menu&);
    virtual ~Menu();
    /** @} */

    /** Assignment operator */
    virtual Menu& operator=(const Menu&);

    /** Retrieve or set label text and properties. @{ */
    virtual string getTitle();
    virtual void setTitle(string);

    /** Item management. @{ */
    virtual void add(string, string = "");
    virtual void insert(const string&, const string&, const string& = "");
    virtual void insert(vector<ItemType>::iterator, const string&, const string&);
    virtual void insert(vector<ItemType>::iterator, ItemType);
    virtual void erase(const string&);
    virtual void erase(vector<ItemType>::iterator);
    virtual void clear();
    /** @} */

    /** Functions for changing and retrieving the selection. @{ */
    virtual void previousItem();
    virtual void nextItem();
    virtual const string& getSelectedItemText();
    virtual vector<ItemType>::const_iterator getSelectedIterator();
    /** @} */

    /** Control overloads @{ */
    /** Keyboard event handlers @{ */
    virtual bool onKeyDown(const SDL_KeyboardEvent&);	/// Key pressed down.
    virtual bool onKeyUp(const SDL_KeyboardEvent&);	/// Key released.
    /** @} */

    /** Joystick event handlers @{ */
    virtual bool onJoystickButtonDown(const SDL_JoyButtonEvent&);	/// Joystick button pressed down.
    virtual bool onJoystickButtonUp(const SDL_JoyButtonEvent&);	/// Joystick button released.
    virtual bool onJoystickMove(const SDL_JoyAxisEvent&);	/// Joystick movement.
    /** @} */

    /** Mouse evet handlers @{ */
    virtual bool onMouseButtonDown(const SDL_MouseButtonEvent&);	/// Mouse button down.
    virtual bool onMouseButtonUp(const SDL_MouseButtonEvent&);	/// Mouse button up.
    /** @} @} */

    /** Initialization and cleanup functions which must be overloaded by derived classes. @{ */
    virtual void init();
    virtual void cleanup();
    /** @} */

    /** Draw image to screen. */
    virtual void draw();

  private:
    string	menu_title;

    vector<ItemType>	items;
    vector<ItemType>::iterator selector;

  };	// class Menu

#endif // __SMF_STATICMENU_HH
