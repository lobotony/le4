#pragma once

#include "le4.h"

namespace le4 {

struct App {
    SDL_GLContext   glContext;
    SDL_Window*     window;
    vec2            windowSize;
    bool            running;
    u32             tprev; // SDL_GetTicks
    u32             tnow;  // SDL_GetTicks
    f32             dt; // delta time since last frame, in seconds

    char*           prefsPath;

    // call this to actually run the app and start the main loop
    void run(const char* windowName,
             u16 windowWidth,
             u16 windowHeight,
             const char* prefsOrg,
             const char* prefsProduct);

    // override these to customise
    virtual void startup();
    virtual void update();
    virtual void shutdown();

    void quit(); // request application quit
};

}

