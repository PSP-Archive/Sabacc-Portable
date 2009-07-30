#ifndef __VIDEOMANAGER_HH
#define __VIDEOMANAGER_HH

// Standard headers

// Sabacc headers
#include "geometry/Rect.hh"

class VideoManager {

public:

    VideoManager();
    VideoManager(int, int, int);
    VideoManager(int, int, int, bool);
    VideoManager(Rect, int);
    VideoManager(Rect, int, bool);
    VideoManager(const VideoManager&);
    virtual ~VideoManager();

    // Operator overloads
    virtual VideoManager& operator=(const VideoManager&);

    // Window/screen properties
    virtual const Rect& getResolution();
    virtual void setResolution(int, int);
    virtual void setResolution(Rect);
    virtual int getColorDepth();
    virtual void setColorDepth(int);
    virtual bool getFullScreen();
    virtual void setFullScreen(bool);
    virtual int getVideoFlags();
    virtual void setVideoFlags(int);

    virtual SDL_Surface* getScreen();

    virtual void swapBuffers();

private:
    Rect			resolution;
    int				color_depth;

    bool			full_screen;

    int				video_flags;

    SDL_Surface*	screen;

    void init();

};	// class VideoManager

#endif // __VIDEOMANAGER_HH
