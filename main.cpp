#include <iostream>

#include "leApp.h"

using std::cout;
using namespace le4;


struct TestApp : App {
    Bitmap bmp;

    void startup() {
        bmp.init(fileLoadResource("resources/testbutton.png"));
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
