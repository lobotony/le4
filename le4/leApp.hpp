#pragma once

#include "le4.h"

namespace le4 {

struct leApp {
    SDL_GLContext   glContext;
    SDL_Window*     window;

    void run();

    void startup();
    void update();
    void shutdown();
};

}

