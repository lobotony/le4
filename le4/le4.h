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
  inline u32 nextPowerOf2(u32 inVal) {
    u32 result = 1;

    while(result < inVal) {
      result <<= 1;
    }

    return result;
  }

  inline f32 clamp(f32 val, f32 lower, f32 upper) {
    return fminf(fmaxf(lower, val), upper);
  }

  inline f32 clamp01(f32 v) {
    return clamp(v, 0.f, 1.f);
  }

#pragma mark - Hash -

inline u32 hashDjb2(const char* data) {
    u32 hash = 5381;
    s32 c;

    const unsigned char* str = (const unsigned char*)data;
    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + (u32)c;
    }

    return hash;
}

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
    inline vec2 scale(f32 s) const { return vec2(s*x, s*y); }
    inline vec2 add(const vec2& r) const { return vec2(x+r.x, y+r.y); }
    inline vec2 sub(const vec2& r) const { return vec2(x-r.x, y-r.y); }
    inline f32 sqMag() const { return x*x + y*y; }
    inline f32 mag() const { return sqrtf(sqMag()); }
    inline vec2 normalize() const { return scale(1.f/mag()); }
    inline f32 distance(const vec2& r) const { return r.sub(*this).mag(); }
    inline f32 dot(const vec2& r) const { return x*r.x + y*r.y; }

    bool isInside(const rect& r) {
        return ((x >= r.x) && (x<(r.x+r.w)) && (y >= r.y) && (y<(r.y+r.h)));
    }
};

inline bool operator==(const vec2& l, const vec2& r) {
    return (l.x == r.x) && (l.y == r.y);
}

struct vec3 {
union {
    struct { f32 data[3]; };
    struct { f32 x; f32 y; f32 z; };
    vec2 xy;
};

    vec3() { x= 0; y = 0; z=0; }
    vec3(f32 v) { x = y = z = v; }
    vec3(f32 inX, f32 inY, f32 inZ) { x = inX; y = inY; z = inZ; }

    inline vec3 scale(f32 s) const { return vec3(s*x, s*y, s*z); }
    inline vec3 add(const vec3& r) const { return vec3(x+r.x, y+r.y, z+r.z); }
    inline vec3 sub(const vec3& r) const { return vec3(x-r.x, y-r.y, z-r.z); }
    inline f32 sqMag() const { return x*x + y*y + z*z; }
    inline f32 mag() const { return sqrtf(sqMag()); }
    inline vec3 normalize() const { return scale(1.f/mag()); }
    inline f32 distance(const vec3& r) const { return r.sub(*this).mag(); }
    inline f32 dot(const vec3& r) const { return x*r.x + y*r.y + z*r.z; }
};

struct vec4 {
union {
    struct { f32 data[4]; };
    struct { f32 x; f32 y; f32 z; f32 w; };
    vec2 xy;
    vec3 xyz;
};
    vec4() { x= 0; y = 0; z = 0; w = 0; }
    vec4(f32 v) { x = y= z = w = v; }
    vec4(f32 inX, f32 inY, f32 inZ, f32 inW) { x = inX; y = inY; z = inZ; w = inW; }

    inline vec4 scale(f32 s) const { return vec4(s*x, s*y, s*z, s*w); }
    inline vec4 add(const vec4& r) const { return vec4(x+r.x, y+r.y, z+r.z, w+r.w); }
    inline vec4 sub(const vec4& r) const { return vec4(x-r.x, y-r.y, z-r.z, w-r.w); }
    inline f32 sqMag() const { return x*x + y*y + z*z + w*w; }
    inline f32 mag() const { return sqrtf(sqMag()); }
    inline vec4 normalize() const { return scale(1.f/mag()); }
    inline f32 distance(const vec4& r) const { return r.sub(*this).mag(); }
    inline f32 dot(const vec4& r) const { return x*r.x + y*r.y + z*r.z + w*r.w; }
};

}

