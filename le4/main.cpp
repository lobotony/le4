#include <iostream>

#include "le4.h"

using std::cout;
int main(int argc, const char * argv[])
{
  void* d = le4::leMalloc(4096);
  le4::leFree(d);

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    LELOG("Failed to initialise SDL");
    return -1;
  }
  LELOG("Hooray it works!\n");

  le4::leMemDumpLog();


  SDL_Quit();
  return 0;
}
