#ifndef __SMF_STATICIMAGELABEL_HH
#define __SMF_STATICIMAGELABEL_HH

/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: September 13, 2008
 *
 * File: StaticImageLabel.hh
 *
 * A control which can display both an image an some text. Text and image placement
 * is changable.
 */

// Standard Library headers
#include <std::string>


// SMF headers
#include "base/FreeTypeControl.hh"
#include "base/Widget.hh"
#include "geometry/Rect.hh"

class ImageLabel : public FreeTypeControl {

public:

    /** ImageLabel constructors:
     * @param Text to display. Required.
     * @param file - the file to load. Required.
     * @param position - the position to display the image.
     * @param property_string - properties for displaying the text.
     *        Can be any of the startdard Object and Widget and TextLabel properties,
     *        as well as the following ImageLabel specifics:
     *        - @c image-anchor: @c left, @c right, @c top,@c bottom, @c center.
     *        These specify the position of the image relative to the text.
     * @{ */
    ImageLabel(const std::string&, const string&, const Rect& = default_rect, string = "autosize: true");
    ImageLabel(Widget*, const std::string&, const string&, const Rect& = default_rect, string = "autosize: true");
    ImageLabel(const ImageLabel&);
    ~ImageLabel();
    /** @} */

    /** Assignment operator */
    ImageLabel& operator=(const ImageLabel&);

    /** Retrieve or set label text and image. @{ */
    virtual std::string getText();
    virtual void setText(const std::string&);
    virtual std::string getFileName();
    virtual void setFileName(const std::string&);
    /** @ } */

    /** Initialization and cleanup functions which must be overloaded by derived classes. @{ */
    virtual void init();
    virtual void cleanup();
    /** @} */

    /** Draw image to screen. */
    virtual void draw();

private:
    std::string	label_text;
    std::string	image_file;

};	// class ImageLabel

#endif // __SMF_STATICIMAGELABEL_HH
