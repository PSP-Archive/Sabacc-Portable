#ifndef __SMF_SYSTEM_HH
#define __SMF_SYSTEM_HH

/**
 * Sabacc Media Framework
 *
 * Author: Anthony Thomasel
 * Date: September 30, 2008
 *
 * File: System.hh
 *
 * A class for managing various aspects of the system, including
 * but not limited to:
 * - Display Settings (via Video Manager)
 * - Window manager (window title) (via Video Manager)
 * - Rendering ( via Render Manager)
 * - Event processing (not to be confused with event handling, which is
 *   done on a per control basis) (via Event Manager)
 */

// Standard library headers
#include <list>
#include <string>

// SDL headers
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

// SabaccMediaFramework headers
#include "base/Widget.hh"
#include "base/Control.hh"
#include "geometry/Rect.hh"

// SabaccmediaFramework system headers
#include "constants/system/SystemDefaults.hh"

// SabaccMediaFramework media managers
#include "system/EventManager.hh"
#include "system/RenderManager.hh"
#include "system/VideoManager.hh"

class SystemManager {

  /** Constructors.
      @arg video_res - A Rect used for setting video resolution. Only the width and height members are used. The default depends on platform: usually 640x480 for Desktop OSes. The resolution of 480x272 is the only available for PlayStation Portable.
      @arg video_color_depth - The bit depth to use for the display. If zero, the framework will use the current display color depth (default).
      @arg video_full_screen - If true, set display to full screen. The default is "false", using a window for display.
      @{ */
  SystemManager(const Rect& = default_video_resolution, Uint8 = default_color_depth, bool = false);
  virtual ~SystemManager();
  
  /** Retrieve and set system resource paths. @{ */
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
  /** @} */

  /** Retrieve and set Joystick input properties. @{ */
    virtual int getActiveJoystick();
    virtual void setActiveJoystick(int);
  /** @} */

  /** Retrieve and set Window Manager settings. @{ */
    virtual string getWindowTitle();
    virtual void setWindowTitle(string);
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

#endif // __SMF_SYSTEM_HH
