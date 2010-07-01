/*
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: September 17, 2008
 *
 * File: StaticImageLabelLabel.hh
 *
 * A textLabel that also displays an image.
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
#include "base/Control.hh"
#include "base/FreeTypeControl.hh"
#include "base/SmartSurface.hh"
#include "base/Widget.hh"
#include "controls/StaticImageLabel.hh"
#include "geometry/Rect.hh"
#include "utility/CopySurface.hh"

// SMF Manager
#include "system/SystemManager.hh"

// SMF Exceptions
#include "exceptions/base/WidgetExceptions.hh"
#include "exceptions/controls/ImageExceptions.hh"
#include "exceptions/io/FileExceptions.hh"

// SMF FreeType headers
#include "freetype/FreeTypeRenderer.hh"

// SMF System Font
#include "freetype/FontManager.hh"

// SMF Debugging
#if defined(_DEBUG) || defined(_DEBUGSTATICIMAGELABEL)
#include "utility/Log.hh"
#endif

ImageLabel::ImageLabel(const string& text,
		       const string& file,
		       const Rect& position,
		       string property_string) :
    FreeTypeControl(position, property_string),
    label_text(text),
    image_file(file) { }

ImageLabel::ImageLabel(Widget* guardian,
		       const string& text,
		       const string& file,
		       const Rect& position,
		       string property_string) :
	FreeTypeControl(guardian, position, property_string),
	label_text(text),
	image_file(file) { }

ImageLabel::ImageLabel(const ImageLabel& src) :
	FreeTypeControl(src),
	label_text(src.label_text),
	image_file(src.image_file) { }

ImageLabel::~ImageLabel() {

    if (isInitialized()) cleanup();

}

ImageLabel& ImageLabel::operator=(const ImageLabel& src) {

    if (&src != this) {

        FreeTypeControl::operator=(src);
        label_text = src.label_text;
        image_file = src.image_file;

        setNotInitialized();

    }

    return(*this);
}	// operator=

string ImageLabel::getText() {
    return(label_text);
}	// getText
void ImageLabel::setText(const string& text) {
    if (isInitialized()) {
        cleanup();
        setNotInitialized();
    }	// if(isInitialized)

    label_text = text;
}	// setText
string ImageLabel::getFileName() {
    return(image_file);
}
void ImageLabel::setFileName(const string& file_name) {
    if ((isInitialized()) && (image_file != file_name)) {
        cleanup();
        setNotInitialized();
    }	// if(isInitialized)

    image_file = file_name;
}	// setFileName

void ImageLabel::init() {
#if defined(_DEBUG) || defined(_DEBUGSTATICIMAGELABEL)
    logAppend("Setting up image label: " + label_text + ", " + image_file);
#endif

    // Load image into a temporary surface first.
    SDL_RWops* image_ops =
	SDL_RWFromFile(getSystemManager().getImagePath(image_file).c_str(),
		       "r");
    SDL_Surface* image_surface = IMG_Load_RW(image_ops, true);

    if(!image_surface)
	throw Exceptions::Image::LoadError("The loaded image seems invalid.",
					   "Bad Image",
					   SDL_GetError(),
					   __FILE__,
					   __LINE__);

    // Store the previous state
    pushState();

    // Propogate widget properties to text renderer
    setFontProperties(getProperties());

#if defined(_DEBUG) || defined(_DEBUGSTATICIMAGELABEL)
    logAppend("Got width.");
#endif

    int text_width = textWidth(label_text);
    int text_height = textHeight(label_text);

    int surface_width = (text_width < image_surface->w)
	? image_surface->w
	: text_width;
    int surface_height = text_height + image_surface->h;

#if defined(_DEBUG) || defined(_DEBUGSTATICIMAGELABEL)
    logAppend("Got height.");
#endif

	// Image anchor
	int image_start_x = 45;
	int image_start_y = 9;

	string image_anchor = getProperty("image-anchor");

	if(string::npos != image_anchor.find("center"))
	{
	    image_start_x = (surface_width - image_surface->w) / 2;
	    image_start_y = (surface_height - image_surface->h) / 2;
	}

	if(string::npos != image_anchor.find("left"))
	    image_start_x = 0;
	if(string::npos != image_anchor.find("right"))
	    image_start_x = surface_width - image_surface->w;
	if(string::npos != image_anchor.find("top"))
	    image_start_y = 0;
	if(string::npos != image_anchor.find("bottom"))
	    image_start_y = surface_height - image_surface->h;

    SDL_Surface* text_surface =
	SDL_AllocSurface(SDL_SWSURFACE, surface_width, surface_height,
			 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

#if defined(_DEBUG) || defined(_DEBUGSTATICIMAGELABEL)
	char image_label_debug[256];
	sprintf(image_label_debug,
		"Created a surface of dimsensions %dx%d",
		surface_width,
		surface_height);
	logAppend(image_label_debug);
#endif

	// Copy image to same surface as text.

#if defined(_DEBUG) || defined(_DEBUGSTATICIMAGELABEL)
	sprintf(image_label_debug,
		"Blitting image (%dwx%dh) to surface @ %d,%d (%dwx%dh)",
		image_surface->w,
		image_surface->h,
		image_start_x,
		image_start_y,
		surface_width,
		surface_height);
	logAppend(image_label_debug);
#endif

	copySurface(image_surface, text_surface, image_start_x, image_start_y);

	// Text is rendered last in case it needs to overlay image.
	render(label_text, text_surface, 0, 0);
	
	// After redering, restore previous state
	popState();
	
#if defined(_DEBUG) || defined(_DEBUGSTATICIMAGELABEL)
	logAppend("Rendered.");
#endif
	
#if defined(_DEBUG) || defined(_DEBUGSTATICIMAGELABEL)
	logAppend("Converting rendered surface to display alpha format.");
#endif
	
	setSurface(SDL_DisplayFormatAlpha(text_surface));

	// Adjust size
	// Automatic adjustment of Widget size
	if (string::npos != getProperty("autosize").find("true")) {
	    Width(surface_width);
	    Height(surface_height);
	}	// getProperty("all")
	
	setInitialized();
	
}	// init

void ImageLabel::cleanup() {
    setSurface(0);
    setNotInitialized();

}	// cleanup

void ImageLabel::draw() {

#if defined(_DEBUG) || defined(_DEBUGSTATICIMAGELABEL)
    logAppend("ImageLabel: Drawing.");
#endif

    FreeTypeControl::draw();

}	// draw
