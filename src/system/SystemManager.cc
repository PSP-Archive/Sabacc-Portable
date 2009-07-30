/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: August 29, 2008
 *
 * File: SystemManager.cc
 *
 * System Management class.
 */

// Standard library headers
#include <string>
using std::string;

// SDL headers
#ifdef __APPLE__
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#else
#include <SDL.h>
#include <SDL_mixer.h>
#endif

// SMF
#include "geometry/Rect.hh"
#include "system/EventManager.hh"
#include "system/RenderManager.hh"
#include "system/SystemManager.hh"
#include "system/VideoManager.hh"

// SMF Exception headers
#include "exceptions/system/SystemManagerExceptions.hh"

// Debug headers
#if defined(_DEBUG) || defined(_DEBUGSYSTEMMANAGER)
#include "utility/Log.hh"
#endif

// PSP specific headers
#if (_PSP_FW_VERSION >= 100)
#include <pspfpu.h>
#endif

SystemManager::SystemManager() : path_base("Data"), path_delimiter("/"),
        font_path("Fonts"), sound_path("Sounds"), image_path("Images"), window_title("Untitled SMF Window"),
	video(NULL), renderer(NULL), events(NULL), stick(NULL), stick_number(0) {

    // Initialize everything
    init();
}	// ctor
SystemManager::~SystemManager() {

    // Shut everything down
    quit();
}	// dtor

void SystemManager::init() {

    // SystemManager::set up the subsystem "managers".

    // First remove existing instances from memory.
    if (video) delete video;
    if (renderer) delete renderer;

    SDL_WM_SetCaption(window_title.c_str(), "cb.bmp");

    // Create new instances.
    video = new VideoManager;
    renderer = new RenderManager;
    events = new EventManager;

    if (!video) throw(Exceptions::SystemManager::InitFailed("SystemManager::init could not create video manager", SDL_GetError(), "", __FILE__));
    if (!renderer) throw(Exceptions::SystemManager::InitFailed("SystemManager::init could not create rendering manager", SDL_GetError(), "", __FILE__));

    if (0 != SDL_Init(SDL_INIT_JOYSTICK)) {
#if defined(__PSP__)	// Joystick only critical for PSP
        throw(Exceptions::SystemManager::InitFailed("Could not initialize joystick", SDL_GetError(), "", __FILE__));
#endif
    } else {
        SDL_JoystickEventState(SDL_ENABLE);
        stick = SDL_JoystickOpen(0);
    }   // if(SDL_INIT_JOYSTICK)

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {

#ifdef _DEBUGSYSTEMMANAGER
        logAppend("Could not initialize audio.");
#endif

        throw(Exceptions::SystemManager::InitFailed("Could not initialize audo file support.", Mix_GetError(), "", __FILE__, __LINE__));

    }

    // Some PSP specific SystemManager::set-up
#if (_PSP_FW_VERSION >= 100)
    pspfpu_set_enable(0);
#endif
}	// init

void SystemManager::quit() {

    // Mix_CloseAudio();

    // Remove the individual managers from memory
  if(video) delete(video);
    if(renderer) delete(renderer);
    if(events) delete(events);

    SDL_Quit();
}	// quit

std::string SystemManager::getBasePath() {
    return(path_base);
}	// getBasePath
void SystemManager::setBasePath(std::string path) {
    path_base = path;
}	// setBasePath

std::string SystemManager::getPathDelimiter() {
    return(path_delimiter);
}	// getPathDelimiter
void SystemManager::setPathDelimiter(std::string delimiter) {
    path_delimiter = delimiter;
}	// setPathDelimiter

std::string SystemManager::getFontPath() {
    return(path_base + path_delimiter + font_path);
}	// getFontPath
std::string SystemManager::getFontPath(std::string file) {
    return(path_base + path_delimiter + font_path + path_delimiter + file);
}	// getFontPath
void SystemManager::setFontPath(std::string path) {
    font_path = path;
}	// setFontPath
std::string SystemManager::getSoundPath() {
    return(path_base + path_delimiter + sound_path);
}	// getFontPath
std::string SystemManager::getSoundPath(std::string file) {
    return(path_base + path_delimiter + sound_path + path_delimiter + file);
}	// getFontPath
void SystemManager::setSoundPath(std::string path) {
    sound_path = path;
}	// setFontPath

std::string SystemManager::getImagePath() {
    return(path_base + path_delimiter + image_path);
}	// getFontPath
std::string SystemManager::getImagePath(std::string file) {
    return(path_base + path_delimiter + image_path + path_delimiter + file);
}	// getFontPath
void SystemManager::setImagePath(std::string path) {
    image_path = path;
}	// setFontPath

std::string SystemManager::getWindowTitle() {
    return(window_title);
}	// getWindowTitle
void SystemManager::setWindowTitle(std::string title) {
    window_title = title;
    SDL_WM_SetCaption(window_title.c_str(), NULL);
}	// setWindowTitle

int SystemManager::getActiveJoystick() {
    return(stick_number);
}	// getActiveJoystick
void SystemManager::setActiveJoystick(int number) {
    stick_number = number;
}	// setActiveJoystick

VideoManager& SystemManager::getVideo() {
    if (!video) video = new VideoManager;
    //    if(!video) throw(Exceptions::System::Init("Could not create an instance of VideoManager"));
    return(*video);
}	// getVideo()
RenderManager& SystemManager::getRenderer() {
    if (!renderer) renderer = new RenderManager;
    //    if(!renderer) throw(Exceptions::System::Init("Could not create an instance of VideoManager"));
    return(*renderer);
}	// getRenderer()
EventManager& SystemManager::getEventManager() {
  if(!events) events = new EventManager;
  //  if(!events) throw(Exceptions::System::Init("Could not create an instance of EventManager"));
  return(*events);
} // getEventManager
SystemManager& getSystemManager() {
    static SystemManager sys_mgr;

    return(sys_mgr);
}	// SystemManager::getSystemManager
