#include <iostream>
#include "leApp.h"
#include "SokolGl3Renderer.h"

using std::cout;
using namespace le4;


struct TestApp : App {
    Bitmap bmp;
    SokolGl3Renderer tr;

    void startup() {
        bmp.init(fileLoadResource("resources/testbutton.png"));
        tr.init();
    }

    void update() {

        tr.draw();
    }

    void shutdown() {
        bmp.deinit();
    }
};

int main(int argc, const char * argv[])
{
    TestApp app;
    app.run("Testing", 640, 480, "com.lobotony", "le4");
    return 0;
}
