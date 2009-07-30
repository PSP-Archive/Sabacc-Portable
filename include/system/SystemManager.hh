#ifndef __SMF_SYSTEMMANAGER_HH
#define __SMF_SYSTEMMANAGER_HH

// Standard library headers
#include <std::string>


// SDL headers
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

// SMF headers
#include "geometry/Rect.hh"
#include "system/EventManager.hh"
#include "system/RenderManager.hh"
#include "system/VideoManager.hh"

class SystemManager {

public:

    SystemManager();
    virtual ~SystemManager();

    virtual std::string getBasePath();
    virtual void setBasePath(std::string);

    virtual std::string getPathDelimiter();
    virtual void setPathDelimiter(std::string);

    virtual std::string getFontPath();
    virtual std::string getFontPath(string);
    virtual void setFontPath(std::string);

    virtual std::string getSoundPath();
    virtual std::string getSoundPath(string);
    virtual void setSoundPath(std::string);

    virtual std::string getImagePath();
    virtual std::string getImagePath(string);
    virtual void setImagePath(std::string);

    virtual std::string getWindowTitle();
    virtual void setWindowTitle(std::string);

    virtual int getActiveJoystick();
    virtual void setActiveJoystick(int);

    virtual VideoManager& getVideo();
    virtual RenderManager& getRenderer();
  virtual EventManager& getEventManager();

private:
    // Paths
    std::string		path_base;
    std::string		path_delimiter;
    std::string		font_path;
    std::string		sound_path;
    std::string		image_path;

    // Window title
    std::string		window_title;

    // Video Manager
    VideoManager*	video;

    // Rendering Manager
    RenderManager*	renderer;

  // Event Manager
  EventManager* events;
    // Audio Manager
    // AudioManager*	audio;

    // Input
    SDL_Joystick*	stick;
    int				stick_number;

    // Startup and shutdown
    virtual void init();
    virtual void quit();

    // Prevent copying and assignment
    SystemManager(const SystemManager&);
    SystemManager& operator=(const SystemManager&);

};	// SystemManager

SystemManager& getSystemManager();

#endif // __SMF_SYSTEMMANAGER_HH
