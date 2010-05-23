/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
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

// Simple Directmedia headers
#if defined(__APPLE__)
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#endif

// SMF headers
#include "base/SmartSurface.hh"
#include "base/SurfaceWidget.hh"
#include "controls/StaticImage.hh"
#include "geometry/Rect.hh"

// SMF Manager
#include "system/SystemManager.hh"

// SMF Exceptions
#include "exceptions/controls/ImageExceptions.hh"
#include "exceptions/io/FileExceptions.hh"

// SMF Debugging
#if defined(_DEBUG) || defined(_DEBUGSTATICIMAGE)
#include <cstdlib>
#include "utility/Log.hh"
#endif

StaticImage::StaticImage(unsigned char* buffer, unsigned long buf_size, const Rect& position, const string& property_string) : SurfaceWidget(position, property_string), image_buffer(buffer), image_buffer_size(buf_size), image_file() { }
StaticImage::StaticImage(string file_name, const Rect& position, const string& property_string) :
  SurfaceWidget(position, property_string), image_buffer(0), image_buffer_size(0), image_file(file_name) { }
StaticImage::StaticImage(const StaticImage& src) : SurfaceWidget(src), image_buffer(src.image_buffer), image_buffer_size(src.image_buffer_size), image_file(src.image_file) { }
StaticImage::~StaticImage() {

    if (isInitialized()) cleanup();

}

StaticImage& StaticImage::operator=(const StaticImage& src) {

    if (&src != this) {

        SurfaceWidget::operator=(src);
	image_buffer = src.image_buffer;
	image_buffer_size = src.image_buffer_size;
        image_file = src.image_file;
        setNotInitialized();

    }

    return(*this);
}	// operator=

void StaticImage::setImageBuffer(unsigned char* buffer, unsigned long size) {
  image_buffer = buffer;
  image_buffer_size = size;
} // setImageBuffer
string StaticImage::getFileName() {
    return(image_file);
}
void StaticImage::setFileName(string file_name) {
    if (isInitialized()) {
        cleanup();
        setNotInitialized();
    }	// if(isInitialized)

    image_file = file_name;
}	// setFileName

void StaticImage::init() {
#if defined(_DEBUG) || defined(_DEBUGSTATICIMAGE)
    logAppend("StaticImage: Opening image.");

    logAppend(getSystemManager().getImagePath(image_file));
#endif

    SDL_RWops* image_stream = 0;

    if(image_buffer && (0 < image_buffer_size) && (image_file.empty())) 
      {
	image_stream = SDL_RWFromMem(image_buffer, image_buffer_size);
      }
    else {
      if(1 > image_file.length()) throw(Exceptions::File::ZeroLengthFileName("", "", "", __FILE__, __LINE__));

      image_stream = SDL_RWFromFile(getSystemManager().getImagePath(image_file).c_str(), "r");
    }

    SDL_Surface* image_surface = IMG_Load_RW(image_stream, true);
    
    cleanup();

    if (std::string::npos != getProperty("pixelformat").find("noconversion")) {

        setSurface(image_surface);

#if defined(_DEBUG) || defined(_DEBUGSTATICIMAGE)
        logAppend("StaticImage: using image data as stored.");
#endif

    } else if (std::string::npos != getProperty("pixelformat").find("displayalpha")) {

#if defined(_DEBUG) || defined(_DEBUGSTATICIMAGE)
        logAppend("StaticImage: converting image data to display format with alpha.");
#endif

        if (image_surface) {
            setSurface(SDL_DisplayFormatAlpha(image_surface));
	    SDL_FreeSurface(image_surface);
        } else {
            setSurface(0);
        }	// if(image_surface)
    } else {

#if defined(_DEBUG) || defined(_DEBUGSTATICIMAGE)
        logAppend("StaticImage: converting image data to display format.");
#endif

        if (image_surface) {
            setSurface(SDL_DisplayFormat(image_surface));
	    SDL_FreeSurface(image_surface);
        } else {
            setSurface(0);
        }	// if(image_surface)

    }	// convert to display format

    // Automatic adjustment of Widget size
    if (string::npos != getProperty("autosize").find("true")) {
        setWidth(getSurface()->w);
        setHeight(getSurface()->h);
    }	// getProperty("all")

    setInitialized();

}	// init
void StaticImage::cleanup() {
    setSurface(0);

    setNotInitialized();

}	// cleanup

void StaticImage::draw() {

#if defined(_DEBUG) || defined(_DEBUGSTATICIMAGE)
  char img_debug[256];
  if(!getFileName().empty()) sprintf(img_debug, "Drawing '%s' @ %d %d %d %d (%s)", getFileName().c_str(), getLeft(), getTop(), getWidth(), getHeight(), getProperties().c_str());
  else sprintf(img_debug, "Drawing internal buffer of size %u @ %d %d %d %d (%s)", image_buffer_size, getLeft(), getTop(), getWidth(), getHeight(), getProperties().c_str());
    logAppend(img_debug);
#endif

    SurfaceWidget::draw();

}	// draw
