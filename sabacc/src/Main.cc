// Standard library headers
#include <string>

// SDL headers
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

// SMF headers
#include "game/GameBoot.hh"
#include "system/SystemManager.hh"
#include "system/VideoManager.hh"
#include "geometry/Rect.hh"

#ifdef _DEBUG
#include "utility/Log.hh"
#endif

#if (_PSP_FW_VERSION > 200)
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspsdk.h>
#include <pspctrl.h>
#include <pspthreadman.h>

#define STDOUT_FILE "cout.txt"
#define STDERR_FILE "cerr.txt"

PSP_MODULE_INFO("SabaccPortable", 0, 1, 0);

// HEAP_SIZE_MAX removed for compatibility reasons
// but left here if it needs to be re-enabled.
// PSP_HEAP_SIZE_MAX();
PSP_HEAP_SIZE_KB(-512);

PSP_MAIN_THREAD_ATTR(0);
PSP_MAIN_THREAD_STACK_SIZE_KB(64);

int sdl_psp_exit_callback(int arg1, int arg2, void *common) {
    exit(0);
    return(0);
}

int sdl_psp_callback_thread(SceSize args, void *argp) {
    int cbid;
    cbid = sceKernelCreateCallback("Exit Callback",
                                   sdl_psp_exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return(0);
}

int sdl_psp_setup_callbacks(void) {
    int thid = 0;
    thid = sceKernelCreateThread("update_thread",
                                 sdl_psp_callback_thread, 0x11, 0xFA0, 0, 0);
    if (thid >= 0)
        sceKernelStartThread(thid, 0, 0);
    return(thid);
}
#endif//_PSP_FW_VERSION >= 200

extern "C" int main ( int argc, char** argv );

int main (int arg_count, char** argv) {

#if defined(_DEBUG) && defined(_DEBUGTOFILE)
    Logger::instance().useFile("./debug.log");
#endif

#if (_PSP_FW_VERSION > 200)
    pspDebugScreenInit();

    sdl_psp_setup_callbacks();

    atexit(sceKernelExitGame);
#endif	// __PSP__

    // Menu result
    VideoManager& video = getSystemManager().getVideo();

    if (1 < arg_count) {
      
    } else {
        // initialize SDL video
        video.setResolution(Rect(0, 0, 480, 272));
    }

    getSystemManager().setWindowTitle("Sabacc Portable");

    GameBoot* sabacc_game = new GameBoot();

    if(!sabacc_game) return(1);
    if(sabacc_game) delete(sabacc_game);

#if (_PSP_FW_VERSION > 200)
    sceKernelDelayThread(2500000);
#endif

    // all is well ;)
#if defined(_DEBUG)
    logAppend("If you see this message, SabaccPortable exited normally.");
#endif

    return(0);
}
