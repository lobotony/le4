#include <iostream>

#include "leApp.h"

using std::cout;
using namespace le4;
int main(int argc, const char * argv[])
{
    App app;
    app.run("Testing", 640, 480, "com.lobotony", "le4");
    return 0;
}
