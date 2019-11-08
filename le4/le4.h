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

#pragma mark - Rect -

struct rect {
    f32 x,y,w,h;

    rect() { x = y = w = h = 0.f; }
    rect(f32 inX, f32 inY, f32 inW, f32 inH) { x = inX; y = inY; w = inW; h = inH; }
};

#pragma mark - vec2 -

struct vec2 {
    union {
        struct { f32 data[2]; };
        struct { f32 x; f32 y; };
    };

    vec2() { x= 0; y = 0; }
    vec2(f32 inX, f32 inY) { x = inX; y = inY; }
    vec2 scale(f32 s) const { return vec2(s*x, s*y); }
    vec2 add(const vec2& r) const { return vec2(x+r.x, y+r.y); }
    vec2 sub(const vec2& r) const { return vec2(x-r.x, y-r.y); }
    f32 sqMag() const { return x*x + y*y; }
    f32 mag() const { return sqrtf(sqMag()); }
    vec2 normalize() const { return scale(1.f/mag()); }
    f32 distance(const vec2& r) const { return r.sub(*this).mag(); }
    f32 dot(const vec2& r) const { return x*r.x + y*r.y; }

    bool isInside(const rect& r) {
        return ((x >= r.x) && (x<(r.x+r.w)) && (y >= r.y) && (y<(r.y+r.h)));
    }
};

inline bool operator==(const vec2& l, const vec2& r) {
    return (l.x == r.x) && (l.y == r.y);
}

}

