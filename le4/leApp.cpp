#include "leApp.hpp"

namespace le4 {

void App::run(const char* windowName, u16 windowWidth, u16 windowHeight, const char* prefsOrg, const char* prefsProduct) {

    SDL_version version;
    SDL_GetVersion(&version);
    LELOG("SDL version: %d.%d.%d '%s'", version.major, version.minor, version.patch, SDL_GetRevision());

    LEASSERTM(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) == 0, SDL_GetError());

    // FIXME: prefsPath

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 32);
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow(windowName,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              windowWidth, windowHeight,
                              SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_RESIZABLE|SDL_WINDOW_MAXIMIZED);
    LEASSERTM(window != NULL, SDL_GetError());

    glContext = SDL_GL_CreateContext(window);
    LEASSERTM(glContext != NULL, SDL_GetError());

    int w, h;
    SDL_GL_GetDrawableSize(window, &w, &h);
    LELOG("drawable size: %d %d", w, h);

    leMemDumpLog();
    LELOG("stopped");
    SDL_Quit();
}


void App::startup() {
    void* d = leMalloc(4096);
    leFree(d);

    Data d2;
    d2.init((u8*)&d2, 4);
    LELOG("yay")
    d2.deinit();

    Data d3 = fileLoadResource("resources/testbutton.png");
    LELOG("loaded %ld bytes", d3.size);
    d3.deinit();
}

void App::update() {

}

void App::shutdown() {
}

}

