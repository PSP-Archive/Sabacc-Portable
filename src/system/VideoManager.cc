// Standard headers

// Sabacc headers
#include "exceptions/system/SystemManagerExceptions.hh"
#include "geometry/Rect.hh"
#include "system/VideoManager.hh"

#if defined(_DEBUG) || defined(_DEBUGVIDEOMANAGER)
#include "utility/Log.hh"
#endif

#ifdef __PSP__
VideoManager::VideoManager() : resolution(0, 0, 480, 272), color_depth(0),
        full_screen(true), video_flags(SDL_SWSURFACE), screen(NULL) {
    init();
}	// ctor-PSP
#else
VideoManager::VideoManager() : resolution(0, 0, 640, 480), color_depth(0),
        full_screen(false), video_flags(SDL_SWSURFACE), screen(NULL) {
    init();
}	// ctor everything else
#endif

VideoManager::VideoManager(int width, int height, int bpp) :
        resolution(0, 0, width, height), color_depth(bpp),
        full_screen(false), video_flags(SDL_SWSURFACE), screen(NULL) {
    init();
}	// ctor
VideoManager::VideoManager(int width, int height, int bpp, bool full_scr) :
        resolution(0, 0, width, height), color_depth(bpp),
        full_screen(full_scr), video_flags(SDL_SWSURFACE), screen(NULL) {
    init();
}	// ctor
VideoManager::VideoManager(Rect rez, int bpp) : resolution(rez), color_depth(bpp),
        full_screen(false), video_flags(SDL_SWSURFACE), screen(NULL) {
    init();
}	// ctor
VideoManager::VideoManager(Rect rez, int bpp, bool full_scr) : resolution(rez),
        color_depth(bpp), full_screen(full_scr), video_flags(SDL_SWSURFACE), screen(NULL) {
    init();
}	// ctor
VideoManager::VideoManager(const VideoManager& source) : resolution(source.resolution),
        color_depth(source.color_depth), full_screen(source.full_screen),
        video_flags(source.video_flags), screen(NULL) {
    init();
}	// ctor
VideoManager::~VideoManager() {
    if (SDL_WasInit(SDL_INIT_VIDEO)) SDL_Quit();
}	// dtor

// Operator overloads
VideoManager& VideoManager::operator=(const VideoManager& source) {
    resolution = source.resolution;
    color_depth = source.color_depth;
    full_screen = source.full_screen;
    video_flags = source.video_flags;
    screen = NULL;

    init();

    return(*this);
}	// operator=

// Window/screen properties
const Rect& VideoManager::getResolution() {
    return(resolution);
}
void VideoManager::setResolution(int width, int height) {
    resolution.setWidth(width);
    resolution.setHeight(height);
    init();
}	// setResolution
void VideoManager::setResolution(Rect rez) {
    resolution = rez;
    init();
}	// setResolution
int VideoManager::getColorDepth() {
    return(color_depth);
}
void VideoManager::setColorDepth(int bpp) {
    color_depth = bpp;
    init();
}	// setColorDepth
bool VideoManager::getFullScreen() {
    return(full_screen);
}
void VideoManager::setFullScreen(bool full_scr) {
    full_screen = full_scr;
    init();
}	// setFullScreen
int VideoManager::getVideoFlags() {
    return(video_flags);
}
void VideoManager::setVideoFlags(int flags) {
    video_flags = flags;
    init();
}	// setVideoFlags

SDL_Surface* VideoManager::getScreen() {
    return(screen);
}

void VideoManager::swapBuffers() {
    if (-1 == SDL_Flip(screen)) {
        // throw(Exceptions::Video::InvalidScreenSurface(__FILE__));
    }	// if(!SDL_Flip)
}	// swapBuffers

void VideoManager::init() {

    // Do not re-initialize video subsystem
    if (!SDL_WasInit(SDL_INIT_VIDEO) && (0 != SDL_Init(SDL_INIT_VIDEO))) {
        throw(Exceptions::SystemManager::InitFailed("Could not initialize video.", SDL_GetError(), "" , __FILE__, __LINE__));
    }   // if(SDL_INIT_VIDEO)

    // Set color depth if one was not specified
    if (0 == color_depth) {

#if defined(_DEBUG) || defined(_DEBUGVIDEOMANAGER)
        logAppend("No color depth specified, using default.");
#endif

        if (8 >= SDL_GetVideoInfo()->vfmt->BitsPerPixel) {
            color_depth = 8;
        } else {
            color_depth = 16;
        } // if (SDL_GetVideoInfo)
    } // if i_bpp

    if (full_screen) {
        video_flags |= SDL_FULLSCREEN;

#if defined(_DEBUG) || defined(_DEBUGVIDEOMANAGER)
        logAppend("Starting full screen.");
#endif
    } else {// if b_fullscreen
        color_depth = SDL_GetVideoInfo()->vfmt->BitsPerPixel;

#if defined(_DEBUG) || defined(_DEBUGVIDEOMANAGER)
        logAppend("Color depth set to display, not running full screen.");
#endif
    }

    screen = SDL_SetVideoMode(resolution.getWidth(), resolution.getHeight(), color_depth, video_flags);
    if (!screen) {
        throw(Exceptions::SystemManager::InitFailed("Could not set video mode.", SDL_GetError(), "", __FILE__, __LINE__));
    }   // if(SDL_SetVideoMode)

}	// init
