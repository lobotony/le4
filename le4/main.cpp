#include <iostream>

#include "le4.h"

using std::cout;
using namespace le4;
int main(int argc, const char * argv[])
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    LELOG("Failed to initialise SDL");
    return -1;
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
  return 0;
}
