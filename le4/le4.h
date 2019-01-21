#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <SDL2/SDL.h>
#include <SDL2/SDL_stdinc.h>
#pragma clang diagnostic pop

#include <float.h>
#include <stdint.h>
#include <stdbool.h>

namespace le4 {

  typedef uint64_t u64;
  typedef uint32_t u32;
  typedef uint16_t u16;
  typedef uint8_t u8;
  typedef int64_t s64;
  typedef int32_t s32;
  typedef int16_t s16;
  typedef int8_t s8;
  typedef float f32;
  typedef double f64;
  typedef f64 TimeInterval;
  typedef u32 ResourceId;

#pragma mark - Log -

  void leLog(const char* file, int line, const char* func, ...);

#define LELOG(...) { le4::leLog(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); }

#pragma mark - Assert -

#define LEASSERT(cond) { s64 LOST_ASSERT_COND = (s64)(cond); if(!LOST_ASSERT_COND) {LELOG("%s", #cond); abort();} }
#define LEASSERTM(cond, ...) { int LOST_ASSERT_COND = (int)(cond); if(!LOST_ASSERT_COND) {LELOG(__VA_ARGS__); abort();} }

  // assert without abort
#define LEVERIFYM(cond, ...) { int LOST_ASSERT_COND = (int)(cond); if(!LOST_ASSERT_COND) {LELOG(__VA_ARGS__);} }

#pragma mark - Math -

#define LE_DEG2RAD_F ((2.f*M_PI)/360.f)
#define LE_RAD2DEG_F (360.f/(2.f*M_PI))

  // calculate the next value that is >= inVal and also a power of two
  static inline u32 leNextPowerOf2(u32 inVal) {
    u32 result = 1;

    while(result < inVal) {
      result <<= 1;
    }

    return result;
  }

  static inline f32 leClamp(f32 val, f32 lower, f32 upper) {
    return fminf(fmaxf(lower, val), upper);
  }

  static inline f32 leClamp01(f32 v) {
    return leClamp(v, 0.f, 1.f);
  }

#pragma mark - Hash -

  u32 hashDjb2(const char* data);

#pragma mark - Memory -

  void* leMalloc(size_t size);
  void* leRealloc(void* ptr, size_t size);
  void leFree(void* ptr);
  void leMemDumpLog();

#pragma mark - Data -

  struct Data {
    u8* bytes;
    u32 size;

    Data* init(const u8* bytes, u32 size);
    void deinit();
  };

#pragma mark - File -

  Data fileLoad(const char* spath);
  Data fileLoadResource(const char* relativeFilePath);
  void fileSave(const char* path, Data data);
  void fileSaveResource(const char* relativeFilePath, Data data);

}
