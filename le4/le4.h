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

    void patchSDLMemoryFuncs();
    void dumpMemoryLog();


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

#pragma mark - Bitmap -

  enum BitmapFormat {
      Undefined,
      A,
      RGB,
      RGBA
  };

  struct Bitmap {
      u8*             data;   // points to the raw pixel data
      u16             width;  // width in pixels
      u16             height; // height in pixels
      BitmapFormat    format; // format of bitmap (rgb, rgba)
      bool            premultiplied; // true if alpha was premlultiplied, false otherwise
      bool            loaded; // true if the image was loaded with the image library and data must be freed by it.
      // false if data is just a chunk of memory and can simply be deleted

      void init(u16 inWidth, u16 inHeight, BitmapFormat inFormat);
      void init(const Data& data);
      void deinit();

      void write(const char* path);
      void flip();
      void premultiply();
      void clear(u32 clearColor);
      void setPixel(u16 x, u16 y, u32 color);

  private:
      void destroy(); // throw away and dealloc all data
  };

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
    inline vec3 cross(const vec3& r) { return vec3(y*r.z - z*r.y, z*r.x - x*r.z, x*r.y - y*r.x); }
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

struct mat44 {
    f32 data[16];

    mat44() {}

    inline f32 get(int row, int column)
    {
      LEASSERT((row >= 0) && (row < 4));
      LEASSERT((column >= 0) && (column < 4));

      return data[row*4+column];
    }

    inline void set(int row, int column, f32 v)
    {
      LEASSERT((row >= 0) && (row < 4));
      LEASSERT((column >= 0) && (column < 4));

      data[row*4+column] = v;
    }

    void setZero() {
        for(int i=0; i<16; ++i)
        {
          data[i] = .0f;
        }
    }

    void setIdentity() {
        setZero();
        data[0] = 1.f;
        data[5] = 1.f;
        data[10] = 1.f;
        data[15] = 1.f;
    }

    void setTranslate(vec3 v) {
        data[12] = v.x;
        data[13] = v.y;
        data[14] = v.z;
    }

    void setScale(f32 sx, f32 sy, f32 sz) {
        data[0] = sx;
        data[5] = sy;
        data[10] = sz;
    }

    void setOrtho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar) {
        LEASSERT(fabs(zNear - FLT_EPSILON) > 0.f);

        setIdentity();
        data[0] = 2.f / (right - left);
        data[5] = 2.f / (top - bottom);
        data[10] = - 2.f / (zFar - zNear);
        data[12] = - (right + left) / (right - left);
        data[13] = - (top + bottom) / (top - bottom);
        data[14] = - (zFar + zNear) / (zFar - zNear);
    }

    void setPerspective(f32 fovy, f32 aspect, f32 zNear, f32 zFar)
    {
        LEASSERT(fabs(aspect - FLT_EPSILON) > 0.f);

        f32 tanHalfFovy = tanf(fovy / 2.f);

        setZero();
        data[0] = 1.f / (aspect * tanHalfFovy);
        data[5] = 1.f / tanHalfFovy;
        data[10] = - (zFar + zNear) / (zFar - zNear);
        data[11] = - 1.f;
        data[14] = - (2.f * zFar * zNear) / (zFar - zNear);
    }

    void setLookAt(const vec3& eye, const vec3& center, const vec3& up) {
        vec3 f = center.sub(eye).normalize();
        vec3 s = f.sub(up).normalize();
        vec3 u = f.cross(s);

        setIdentity();
        data[0] = s.x;
        data[4] = s.y;
        data[8] = s.z;
        data[1] = u.x;
        data[5] = u.y;
        data[9] = u.z;
        data[2] = -f.x;
        data[6] = -f.y;
        data[10] = -f.z;
        data[12] = -s.dot(eye);
        data[13] = -u.dot(eye);
        data[14] = f.dot(eye);
    }

    inline void setRotate(f32 angle, const vec3& v)
    {
        f32 a = angle;
        f32 c = cosf(a);
        f32 s = sinf(a);

        vec3 axis = v.normalize();
        vec3 temp = axis.scale(1.f-c);

        setIdentity();
        set(0, 0, c + temp.x * axis.x);
        set(0, 1, 0 + temp.x * axis.y + s * axis.z);
        set(0, 2, 0 + temp.x * axis.z - s * axis.y);

        set(1, 0, 0 + temp.y * axis.x - s * axis.z);
        set(1, 1, c + temp.y * axis.y);
        set(1, 2, 0 + temp.y * axis.z + s * axis.x);

        set(2, 0, 0 + temp.z * axis.x + s * axis.y);
        set(2, 1, 0 + temp.z * axis.y - s * axis.x);
        set(2, 2, c + temp.z * axis.z);
    }

    vec4 column(u16 i) const {
        vec4 result;
        switch(i)
        {
          case 0:result = vec4(data[0], data[1], data[2], data[3]);break;
          case 1:result = vec4(data[4], data[5], data[6], data[7]);break;
          case 2:result = vec4(data[8], data[9], data[10], data[11]);break;
          case 3:result = vec4(data[12], data[13], data[14], data[15]);break;
          default:result=vec4(0);LEASSERTM(false, "Matrix column index out of bounds");break;
        }
        return result;
    }

    vec4 row(u16 i) const {
        vec4 result;

        switch(i)
        {
          case 0:result = vec4(data[0], data[4], data[8], data[12]);break;
          case 1:result = vec4(data[1], data[5], data[9], data[13]);break;
          case 2:result = vec4(data[2], data[6], data[10], data[14]);break;
          case 3:result = vec4(data[3], data[7], data[11], data[15]);break;
          default:result=vec4(0);LEASSERTM(false, "Matrix row index out of bounds");break;
        }

        return result;
    }


    mat44 transpose() const {
        mat44 result = *this;
        std::swap(result.data[1], result.data[4]);
        std::swap(result.data[2], result.data[8]);
        std::swap(result.data[6], result.data[9]);
        std::swap(result.data[3], result.data[12]);
        std::swap(result.data[7], result.data[13]);
        std::swap(result.data[11], result.data[14]);
        return result;
    }

    mat44 mult(const mat44& r) {
        mat44 result;
        vec4 lr0 = row(0);
        vec4 lr1 = row(1);
        vec4 lr2 = row(2);
        vec4 lr3 = row(3);

        vec4 t = column(0);
        result.data[0] = lr0.dot(t);
        result.data[1] = lr1.dot(t);
        result.data[2] = lr2.dot(t);
        result.data[3] = lr3.dot(t);

        t = column(1);
        result.data[4] = lr0.dot(t);
        result.data[5] = lr1.dot(t);
        result.data[6] = lr2.dot(t);
        result.data[7] = lr3.dot(t);

        t = column(2);
        result.data[8] = lr0.dot(t);
        result.data[9] = lr1.dot(t);
        result.data[10] = lr2.dot(t);
        result.data[11] = lr3.dot(t);

        t = column(3);
        result.data[12] = lr0.dot(t);
        result.data[13] = lr1.dot(t);
        result.data[14] = lr2.dot(t);
        result.data[15] = lr3.dot(t);

        return result;
    }

    static inline mat44 zero() { mat44 r; r.setZero(); return r; }
    static inline mat44 identity() { mat44 r; r.setIdentity(); return r; }
    static inline mat44 translate(const vec3& v) { mat44 r; r.setTranslate(v); return r; }
    static inline mat44 scale(f32 sx, f32 sy, f32 sz) { mat44 r; r.setScale(sx, sy, sz); return r; }
    static inline mat44 ortho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar) {
        mat44 result;
        result.setOrtho(left, right, bottom, top, zNear, zFar);
        return result;
    }
    static inline mat44 ortho2d(f32 left, f32 right, f32 bottom, f32 top) {
        mat44 result;
        result.setOrtho(left, right, bottom, top, -1.f, 1.f);
        return result;
    }
    static inline mat44 perspective(f32 fovy, f32 aspect, f32 zNear, f32 zFar) {
        mat44 result;
        result.setPerspective(fovy, aspect, zNear, zFar);
        return result;
    }
    static inline mat44 lookAt(const vec3& eye, const vec3& center, const vec3& up) {
        mat44 result;
        result.setLookAt(eye, center, up);
        return result;
    }
    static inline mat44 rotate(f32 angle, const vec3& v) {
        mat44 result;
        result.setRotate(angle, v);
        return result;
    }

    vec4 transform(const vec4& v) {
        vec4 result;

        result.x = row(0).dot(v);
        result.y = row(1).dot(v);
        result.z = row(2).dot(v);
        result.w = row(3).dot(v);

        return result;
    }

    vec3 transform(const vec3& v) {
        vec4 tmp;
        tmp.xyz = v;
        tmp.w = 1.0;
        vec4 r = transform(tmp);
        return r.xyz;
    }
};

struct mat33 {
    f32 data[9];

    inline f32 get(int row, int column)
    {
      LEASSERT((row >= 0) && (row < 3));
      LEASSERT((column >= 0) && (column < 3));

      return data[row*3+column];
    }

    inline void set(int row, int column, f32 v)
    {
      LEASSERT((row >= 0) && (row < 3));
      LEASSERT((column >= 0) && (column < 3));

      data[row*3+column] = v;
    }

    void setZero() {
        for(int i=0; i<9; ++i)
        {
          data[i] = .0f;
        }
    }

    void setIdentity() {
        setZero();
        data[0] = 1.f;
        data[4] = 1.f;
        data[8] = 1.f;
    }

    void setRotate(f32 angle, const vec3& v)
    {
      f32 a = angle;
      f32 c = cosf(a);
      f32 s = sinf(a);

      vec3 axis = v.normalize();
      vec3 temp = axis.scale(1.f-c);

      setIdentity();
      set(0, 0, c + temp.x * axis.x);
      set(0, 1, 0 + temp.x * axis.y + s * axis.z);
      set(0, 2, 0 + temp.x * axis.z - s * axis.y);

      set(1, 0, 0 + temp.y * axis.x - s * axis.z);
      set(1, 1, c + temp.y * axis.y);
      set(1, 2, 0 + temp.y * axis.z + s * axis.x);

      set(2, 0, 0 + temp.z * axis.x + s * axis.y);
      set(2, 1, 0 + temp.z * axis.y - s * axis.x);
      set(2, 2, c + temp.z * axis.z);
    }

    void setScale(f32 sx, f32 sy, f32 sz)
    {
      data[0] = sx;
      data[4] = sy;
      data[8] = sz;
    }

    static inline mat33 from(const mat44& r) {
        mat33 result;

        result.data[0] = r.data[0];
        result.data[1] = r.data[1];
        result.data[2] = r.data[2];

        result.data[3] = r.data[4];
        result.data[4] = r.data[5];
        result.data[5] = r.data[6];

        result.data[6] = r.data[8];
        result.data[7] = r.data[9];
        result.data[8] = r.data[10];

        return result;
    }

    vec3 column(u16 i) const {
        vec3 result;
        switch(i)
        {
          case 0:result = vec3(data[0], data[1], data[2]);break;
          case 1:result = vec3(data[3], data[4], data[5]);break;
          case 2:result = vec3(data[6], data[7], data[8]);break;
            default:result = vec3(0);LEASSERTM(false, "Matrix column index out of bounds");break;
        }
        return result;
    }

    vec3 row(u16 i) const {
        vec3 result;
        switch(i)
        {
          case 0:result = vec3(data[0], data[3], data[6]);break;
          case 1:result = vec3(data[1], data[4], data[7]);break;
          case 2:result = vec3(data[2], data[5], data[8]);break;
          default:result = vec3(0);LEASSERTM(false, "Matrix row index out of bounds");break;
        }
        return result;
    }

    mat33 transpose() const {
        mat33 result = *this;
        std::swap(result.data[1], result.data[3]);
        std::swap(result.data[2], result.data[6]);
        std::swap(result.data[5], result.data[7]);
        return result;
    }

    mat33 mul(const mat33& r) const {
        mat33 result;
        vec3 lr0 = row(0);
        vec3 lr1 = row(1);
        vec3 lr2 = row(2);

        vec3 t = column(0);
        result.data[0] = lr0.dot(t);
        result.data[1] = lr1.dot(t);
        result.data[2] = lr2.dot(t);

        t = column(1);
        result.data[3] = lr0.dot(t);
        result.data[4] = lr1.dot(t);
        result.data[5] = lr2.dot(t);

        t = column(2);
        result.data[6] = lr0.dot(t);
        result.data[7] = lr1.dot(t);
        result.data[8] = lr2.dot(t);
        return result;
    }

    // calculates the determinant
    f32 det() const {
        f32 result = 0.f;
        const f32* m = data;

        result = + m[0] * (m[4] * m[8] - m[7] * m[5])
        - m[3] * (m[1] * m[8] - m[7] * m[2])
        + m[6] * (m[1] * m[5] - m[4] * m[2]);

        return result;
    }

    mat33 inverse() const
    {
      f32 OneOverDeterminant = 1.f / det();

      mat33 result;

      const f32* m = data;

      result.data[0] = + (m[4] * m[8] - m[7] * m[5]) * OneOverDeterminant;
      result.data[3] = - (m[3] * m[8] - m[6] * m[5]) * OneOverDeterminant;
      result.data[6] = + (m[3] * m[7] - m[6] * m[4]) * OneOverDeterminant;
      result.data[1] = - (m[1] * m[8] - m[7] * m[2]) * OneOverDeterminant;
      result.data[4] = + (m[0] * m[8] - m[6] * m[2]) * OneOverDeterminant;
      result.data[7] = - (m[0] * m[7] - m[6] * m[1]) * OneOverDeterminant;
      result.data[2] = + (m[1] * m[5] - m[4] * m[2]) * OneOverDeterminant;
      result.data[5] = - (m[0] * m[5] - m[3] * m[2]) * OneOverDeterminant;
      result.data[8] = + (m[0] * m[4] - m[3] * m[1]) * OneOverDeterminant;

      return result;
    }

    vec3 transform(const vec3& v) const {
        vec3 result;

        result.x = row(0).dot(v);
        result.y = row(1).dot(v);
        result.z = row(2).dot(v);

        return result;
    }

    static inline mat33 scale(f32 sx, f32 sy, f32 sz) {
        mat33 result;
        result.setIdentity();
        result.setScale(sx, sy, sz);
        return result;
    }

    static inline mat33 rotate(f32 angle, const vec3& v) {
        mat33 result;
        result.setRotate(angle, v);
        return result;
    }

};

}

