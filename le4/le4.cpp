#include "le4.h"

namespace le4 {

void leLog(const char* file, int line, const char* func, ...) {
  const char* filename = strrchr(file, '/');
  if(!filename) {
    filename = file;
  }
  else {
    filename++;
  }

  va_list va;

  va_start(va, func);
  const char* fmt = va_arg(va, const char*);

  char* logmsg;
  vasprintf(&logmsg, fmt, va);
  printf("%8d - %s : %d : %s : %s\n", SDL_GetTicks(), filename, line, func, logmsg);
  free(logmsg);
  va_end(va);
}

  static u32 numMalloc = 0;
  static u32 numRealloc = 0;
  static u32 numFree = 0;

  void* leMalloc(size_t size) {
    numMalloc++;
    return SDL_malloc(size);
  }

  void* leRealloc(void* ptr, size_t size) {
    numRealloc++;
    return SDL_realloc(ptr, size);
  }

  void leFree(void* ptr) {
    numFree++;
    SDL_free(ptr);
  }

  void leMemDumpLog() {
    LELOG("mallocs: %d", numMalloc);
    LELOG("reallocs: %d", numRealloc);
    LELOG("frees: %d", numFree);
  }

}
