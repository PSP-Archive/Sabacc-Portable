#ifndef __SMF_STATICTEXT_HH
#define __SMF_STATICTEXT_HH

/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: August 30, 2008
 *
 * File: StaticTextLabel.hh
 *
 * A control in which Text which will not be animated.
 */

// Standard Library headers
#include <string>
using std::string;

// SMF headers
#include "base/FreeTypeControl.hh"
#include "base/Widget.hh"
#include "geometry/Rect.hh"

class TextLabel : public FreeTypeControl {

public:

    /** TextLabel has only one constructor:
     * @param text - the file to load. Required.
     * @param position - the position to display the image.
     * @param property_string - properties for displaying the text.
     *        Can be any of the startdard Object and Widget properties,
     *        as well as the following TextLabel specifics:
     *        - @c justify: @c left, @c right, @c top,@c bottom, @c center.
     *        These justify the text in the confines of the label when it is lager than
     *        the text itself.
     * @{ */
    TextLabel(const string&, const Rect& = default_rect, string = "autosize: true");
    TextLabel(Widget*, const string&, const Rect& = default_rect, string = "autosize: true");
    TextLabel(const TextLabel&);
    ~TextLabel();
    /** @} */

    /** Assignment operator */
    TextLabel& operator=(const TextLabel&);

    /** Retrieve or set label text. @{ */
    virtual string getText();
    virtual void setText(string);
    /** @} */

    /** Initialization and cleanup functions which must be overloaded by derived classes. @{ */
    virtual void init();
    virtual void cleanup();
    /** @} */

    /** Draw image to screen. */
    virtual void draw();

private:
    string	label_text;

};	// class TextLabel

#endif // __SMF_STATICTEXT_HH
