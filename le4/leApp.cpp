#include "leApp.h"
#include "legl.h"

namespace le4 {

void App::run(const char* windowName, u16 windowWidth, u16 windowHeight, const char* prefsOrg, const char* prefsProduct) {

    patchSDLMemoryFuncs();
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
//                              SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_RESIZABLE|SDL_WINDOW_MAXIMIZED);
                              SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_RESIZABLE);
    LEASSERTM(window != NULL, SDL_GetError());

    glContext = SDL_GL_CreateContext(window);
    LEASSERTM(glContext != NULL, SDL_GetError());

    int w, h;
    SDL_GL_GetDrawableSize(window, &w, &h);
    LELOG("drawable size: %d %d", w, h);
    windowSize = vec2(w, h);
    dt = 0;
    tnow = SDL_GetTicks();
    tprev = tnow;

/*    leZoneInit(&app->temp, 1024*1024);
    le2DRendererInit(&app->r2d, &app->windowSize);
    leGuiInit(&app->gui, &app->r2d);
    leInputInit();
    leAudioInit(&app->audio);
    leAppStartup(app);*/

    SDL_Event e;
    running = true;
    while(running) {
        tnow = SDL_GetTicks();
        dt = ((f32)(tnow - tprev))/1000.0f;

        while(SDL_PollEvent(&e))
        {
          switch(e.type)
          {
            case SDL_QUIT:running = false;break;
            //case SDL_MOUSEMOTION:leInputMouseMoved(e.motion.x, e.motion.y);break;
            //case SDL_MOUSEBUTTONDOWN:leInputMouseDown(e.button.button, e.button.x, e.button.y);break;
            //case SDL_MOUSEBUTTONUP:leInputMouseUp(e.button.button, e.button.x, e.button.y);break;
            //case SDL_KEYDOWN:leInputKeyDown(e.key.keysym.sym);break;
            //case SDL_KEYUP:leInputKeyUp(e.key.keysym.sym);break;
            //case SDL_DROPFILE:leInputAddDropPath(e.drop.file);break;
            case SDL_WINDOWEVENT:
              switch(e.window.event)
              {
                case SDL_WINDOWEVENT_RESIZED:
                  LELOG("resized to %d %d", e.window.data1, e.window.data2);
                  windowSize.x = e.window.data1;
                  windowSize.y = e.window.data2;
                  break;
              }
              break;
           }
        }
        update();
        glClearColor(0.f, 1.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        //leAudioUpdate(&app->audio);
        //leInputReset();
        SDL_GL_SwapWindow(window);
        tprev = tnow;
        //leZoneFree(&app->temp);
    }
    shutdown();
    //leAudioDeinit(&app->audio);
    //leGuiDeinit(&app->gui);
    //le2DRendererDeinit(&app->r2d);
    SDL_free(prefsPath);
    // nothing to deinit for input

    dumpMemoryLog();
    LELOG("stopped");
    SDL_Quit();
}


void App::startup() {
/*    void* d = leMalloc(4096);
    leFree(d);

    Data d2;
    d2.init((u8*)&d2, 4);
    LELOG("yay")
    d2.deinit();

    Data d3 = fileLoadResource("resources/testbutton.png");
    LELOG("loaded %ld bytes", d3.size);
    d3.deinit(); */
}

void App::update() {

}

void App::shutdown() {
}

}

