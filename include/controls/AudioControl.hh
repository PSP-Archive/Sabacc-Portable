#ifndef __SMF_AUDIOCONTROL_HH
#define __SMF_AUDIOCONTROL_HH

/*
 * Sabacc Media Framework
 *
 * Author: goumba
 * Date: September 11, 2008
 *
 * File: MediaControl.hh
 *
 * A control for playing audio files.
 */

// SMF headers
#include "base/Object.hh"
#include "geometry/Rect.hh"

typedef double media_time_t;

class AudioControl : public Object {
public:
    /** Constructors @{
      * @param position - the position from which to start playing the audio.
      * @param property_string - any Object or AudioControl property (to be implemented).
      * @param audio_file - the file to load.
      *
      * Like the lower level Widget base classes, position and properties are specified first,
      * allowing a consistent interface for calling the base class constructor without having
      * to pass all parameters. The top level classes may move these parameters around
      * for easier access to the end user. */
    AudioControl(const media_time_t& = 0, const std::string& = "", const string& = "");
    AudioControl(const AudioControl& src);
    virtual ~AudioControl();
    /** @} */

    virtual AudioControl& operator=(const AudioControl&);	/// Gives this AudioControl's members the values of another AudioControl.

    /** File and media properties @{ */
    virtual std::string getFileName();
    virtual void setFileName(std::string);
    virtual unsigned long getBitrate();
    virtual void setBitrate(unsigned long);
    /** Todo: complete interface @} */

    /** Position retrieval and modification functions. @{ */
    virtual const media_time_t getPosition();	/// Get the position of the AudioControl, as a Rect.
    virtual void setPosition(const media_time_t);	/// Set the position @param @c position of the AudioControl.
    /** @} */

    /** Playback controls. @{ */
    virtual void pause();	/// Pause: playback to be resumed from the same point as before being paused.
    virtual void play();	/// Start playback from the specified point.
    virtual void stop();;	/// Stop: playback will resume from the specified point.

    virtual void toBeginning();	/// Go to the beginning to media.
    virtual void toPositionkip(media_time_t);	/// Go forward the specified number of steps (default: 1).

    /** Initialization and cleanup functions which must be overloaded by derived classes. @{ */
    virtual void init() = 0;
    virtual void cleanup() = 0;
    /** @} */

    /** Initialization status functions. @{ */
    virtual bool isInitialized();
    virtual void setInitialized();
    virtual void setNotInitialized();
    /** @} */

    /** All derived classes must have a play() method for consistency. */
    virtual void play() = 0;

private:

    std::string	audio_file;

    media_time_t	start_position, pause_position;

    bool	initialized;

};

#endif // __SMF_AUDIOCONTROL_HH
