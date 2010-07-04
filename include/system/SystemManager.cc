#ifndef __SMF_SYSTEMMANAGER_HH
#define __SMF_SYSTEMMANAGER_HH

// Standard library headers
#include <string>
using std::string;

// SDL headers
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

// SMF headers
#include "geometry/Rect.hh"

class SystemManager
  {

  public:

    SystemManager();
    virtual ~SystemManager();

    /** System resource paths @{ */
    virtual string getBasePath();
    virtual void setBasePath(string);

    virtual string getPathDelimiter();
    virtual void setPathDelimiter(string);

    virtual string getFontPath();
    virtual string getFontPath(string);
    virtual void setFontPath(string);

    virtual string getSoundPath();
    virtual string getSoundPath(string);
    virtual void setSoundPath(string);

    virtual string getImagePath();
    virtual string getImagePath(string);
    virtual void setImagePath(string);
    /** @} @/

        /** Window manager settings. @{ */
    virtual string getWindowTitle();
    virtual void setWindowTitle(string);
    /** @} */

    /** Retrive and set the active joystick. @{ */
    virtual int getActiveJoystick();
    virtual void setActiveJoystick(int);
    /** @} */


  private:
    // Paths
    string		path_base;
    string		path_delimiter;
    string		font_path;
    string		sound_path;
    string		image_path;

    // Window title
    string		window_title;

    // Video Manager
    VideoManager*	video;

    // Rendering Manager
    RenderManager*	renderer;

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
