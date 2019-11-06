#include "leApp.hpp"

namespace le4 {

void leApp::run() {

    SDL_version version;
    SDL_GetVersion(&version);
    LELOG("SDL version: %d.%d.%d '%s'", version.major, version.minor, version.patch, SDL_GetRevision());

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
      LELOG("Failed to initialise SDL");
      return;
    }
    LELOG("Hooray it works!\n");

    void* d = leMalloc(4096);
    leFree(d);

    Data d2;
    d2.init((u8*)&d2, 4);
    LELOG("yay")
    d2.deinit();

    Data d3 = fileLoadResource("resources/testbutton.png");
    LELOG("loaded %ld bytes", d3.size);
    d3.deinit();


    leMemDumpLog();

    SDL_Quit();
}


void leApp::startup() {

}

void leApp::update() {

}

void leApp::shutdown() {

}

}

