#ifndef __SMF_STATICIMAGE_HH
#define __SMF_STATICIMAGE_HH

/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: August 28, 2008
 *
 * File: StaticImage.hh
 *
 * The definition of the object on which all interface
 * objects (widgets, etc) will be based.
 */

// Standard Library headers
#include <string>
using std::string;

// SMF headers
#include "base/SurfaceWidget.hh"
#include "geometry/Rect.hh"

class StaticImage : public SurfaceWidget {

public:

    /** StaticImage constructors:
     * @param buffer
     * @param buf_size - size of the buffer.
     * @param file - the file to load.
     * @param position - the position to display the image.
     * @param property_string - properties for displaying the image.
     *        Can be any of the startdard Object and Widget properties,
     *        as well as the following StaticImage specifics:
     *        - @c format: @c noconversion, @c display, @c displayalpha
     * @{ */
    StaticImage(unsigned char*, unsigned long, const Rect& = default_rect, const string& = "autosize: true; format: display");
    StaticImage(string, const Rect& = default_rect, const string& = "autosize: true; format: display");
    StaticImage(const StaticImage&);
    ~StaticImage();
    /** @} */

    /** Assignment operator */
    StaticImage& operator=(const StaticImage&);

    /** Retrieve or set the image file. @{ */
  virtual void setImageBuffer(unsigned char*, unsigned long);
    virtual string getFileName();
    virtual void setFileName(string);
    /** @} */

    /** Initialization and cleanup functions which must be overloaded by derived classes. @{ */
    virtual void init();
    virtual void cleanup();
    /** @} */

    /** Draw image to screen. */
    virtual void draw();

private:

  unsigned char* image_buffer;
  unsigned long image_buffer_size;
    string	image_file;

};	// class StaticImage

#endif // __SMF_STATICIMAGE_HH
