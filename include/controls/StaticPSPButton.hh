#ifndef __SMF_STATICPSPBUTTON_HH
#define __SMF_STATICPSPBUTTON_HH

/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: December 10, 2008
 *
 * File: StaticPSPButton.hh
 *
 * A button type control, which also displays a PSP
 * button glyph alongside the text.
 */

// Standard Library headers
#include <string>
using std::string;

// SMF headers
#include "base/FreeTypeControl.hh"
#include "base/Widget.hh"
#include "geometry/Rect.hh"

class PSPButton : public FreeTypeControl {

public:

    /** PSPButton has only one constructor:
     * @param text - the file to load. Required.
     * @param position - the position to display the image.
     * @param property_string - properties for displaying the text.
     *        Can be any of the startdard Object and Widget properties,
     *        as well as the following PSPButton specifics:
     *        - @c justify: @c left, @c right, @c top,@c bottom, @c center.
     *        These justify the text in the confines of the label when it is lager than
     *        the text itself.
     * @{ */
    PSPButton(string, const Rect& = default_rect, string = "autosize: true");
    PSPButton(Widget*, string, const Rect& = default_rect, string = "autosize: true");
    PSPButton(const PSPButton&);
    ~PSPButton();
    /** @} */

    /** Assignment operator */
    PSPButton& operator=(const PSPButton&);

    /** Retrieve or set label text and properties. @{ */
    virtual string getText();
    virtual void setText(string);
  virtual string getPSPKey();
  virtual void setPSPKey(char);

    /** Initialization and cleanup functions which must be overloaded by derived classes. @{ */
    virtual void init();
    virtual void cleanup();
    /** @} */

    /** Draw image to screen. */
    virtual void draw();

private:
  string	label_text, psp_key;

};	// class PSPButton


#endif // __SMF_STATICBUTTON_HH
