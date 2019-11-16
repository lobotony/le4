#include <iostream>
#include "leApp.h"
#include "TestRenderer.h

using std::cout;
using namespace le4;


struct TestApp : App {
    Bitmap bmp;
    TestRenderer tr;

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
