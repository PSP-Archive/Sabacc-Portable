#ifndef __SMF_STATICBUTTON_HH
#define __SMF_STATICBUTTON_HH

/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: September 13, 2008
 *
 * File: StaticButton.hh
 *
 * A button type control. Standard UI fare.
 */

// Standard Library headers
#include <string>
using std::string;

// SMF headers
#include "base/FreeTypeControl.hh"
#include "base/Widget.hh"
#include "geometry/Rect.hh"

class Button : public FreeTypeControl {

public:

    /** Button has only one constructor:
     * @param text - the file to load. Required.
     * @param position - the position to display the image.
     * @param property_string - properties for displaying the text.
     *        Can be any of the startdard Object and Widget properties,
     *        as well as the following Button specifics:
     *        - @c justify: @c left, @c right, @c top,@c bottom, @c center.
     *        These justify the text in the confines of the label when it is lager than
     *        the text itself.
     * @{ */
    Button(string, const Rect& = default_rect, string = "autosize: true");
    Button(Widget*, string, const Rect& = default_rect, string = "autosize: true");
    Button(const Button&);
    ~Button();
    /** @} */

    /** Assignment operator */
    Button& operator=(const Button&);

    /** Retrieve or set label text and properties. @{ */
    virtual string getText();
    virtual void setText(string);

    /** Initialization and cleanup functions which must be overloaded by derived classes. @{ */
    virtual void init();
    virtual void cleanup();
    /** @} */

    /** Draw image to screen. */
    virtual void draw();

private:
    string	label_text;

};	// class Button


#endif // __SMF_STATICBUTTON_HH
