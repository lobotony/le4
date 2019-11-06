#pragma once

#include "le4.h"

namespace le4 {

struct leApp {
    SDL_GLContext   glContext;
    SDL_Window*     window;

    void run(const char* windowName,
             u16 windowWidth,
             u16 windowHeight,
             const char* prefsOrg,
             const char* prefsProduct);

    void startup();
    void update();
    void shutdown();
};

}

