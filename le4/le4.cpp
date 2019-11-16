#include "le4.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace le4 {

    void leLog(const char* file, int line, const char* func, ...) {
        const char* filename = SDL_strrchr(file, '/');
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
        printf("%8d - %s:%d : %s : %s\n", SDL_GetTicks(), filename, line, func, logmsg);
        free(logmsg);
        va_end(va);
    }

    // le memory functions are only intended as optional indirection layers for SDL_memory functions.
    // use the SDL ones rather than these.
    static u32 numMalloc = 0;
    static u32 numCalloc = 0;
    static u32 numRealloc = 0;
    static u32 numFree = 0;

    void* leMalloc(size_t size) {
        numMalloc++;
        return malloc(size);
    }

    void* leCalloc(size_t count, size_t size) {
        numCalloc++;
        return calloc(count, size);
    }

    void* leRealloc(void* ptr, size_t size) {
        numRealloc++;
        return realloc(ptr, size);
    }

    void leFree(void* ptr) {
        numFree++;
        free(ptr);
    }

    void dumpMemoryLog() {
        LELOG("mallocs: %d", numMalloc);
        LELOG("callocs: %d", numCalloc);
        LELOG("reallocs: %d", numRealloc);
        LELOG("frees: %d", numFree);
        LELOG("num allocs: %d", SDL_GetNumAllocations());
    }

    void patchSDLMemoryFuncs() {
        SDL_SetMemoryFunctions(leMalloc, leCalloc, leRealloc, leFree);
    }

    Data* Data::init(const u8* inBytes, u32 inSize) {
        SDL_memset(this, 0, sizeof(Data));
        if(inSize > 0) {
          bytes = (u8*)SDL_malloc(inSize);
          if(inBytes) {
            SDL_memcpy(bytes, inBytes, inSize);
          }
          size = inSize;
        }
        else {
          bytes = NULL;
          size = 0;
        }
        return this;
    }

    void Data::deinit() {
        if(bytes) {
          SDL_free(bytes);
        }
        SDL_memset(this, 0, sizeof(Data));
    }

    static const char* sep="/";
    char* pathCat(const char* l, const char* r)
    {
        LEASSERT(l);
        LEASSERT(r);

        size_t lsz = SDL_strlen(l);
        size_t rsz = SDL_strlen(r);
        size_t sepsz = SDL_strlen(sep);
        size_t sz = lsz + rsz + sepsz+1;

        char* result = (char*)SDL_malloc(sz);
        SDL_strlcpy(result, l, sz);
        SDL_strlcpy(result+lsz, sep, sz);
        SDL_strlcpy(result+lsz+sepsz, r, sz);

        return result;
    }

    static const char* _respath = NULL;

    const char* resPath()
    {
        if(!_respath)
        {
          _respath = SDL_GetBasePath();
        }
        return _respath;
    }

    const char* skipResourcePathPrefix(const char* path)
    {
        const char* respath = resPath();
        size_t pathlen = strlen(respath);
        const char* logpath = path;
        if(!SDL_strncmp(respath, path, pathlen))
        {
          logpath = path+pathlen+1;
        }
        return logpath;
    }

    Data fileLoad(const char* spath)
    {
        LEASSERT(spath);

        FILE* file;
        file = fopen(spath, "rb");
        if(file == NULL)
        {
          LELOG("couldn't open file %s",spath);
          Data result;
          result.bytes = NULL;
          result.size = 0;
          return result;
        }
        LEASSERTM(0 == fseek(file, 0, SEEK_END), "couldn't seek %s",spath);
        s64 size;
        size = ftell(file);
        LEASSERTM(size != -1, "couldn't get file pos %s",spath);
        Data result;
        result.init(NULL, (u32)size);
        LELOG("'%s' [%d bytes]", skipResourcePathPrefix(spath), size);
        LEASSERTM(0 == fseek(file, 0, SEEK_SET), "couldn't seek %s",spath);
        fread(result.bytes, (size_t)size, 1, file);
        LEASSERTM(0 == ferror(file), "couldn't read %s", spath);
        LEASSERTM(0 == fclose(file), "couldn't close %s", spath);

        return result;
    }

    void fileSave(const char* path, Data data)
    {
        LEASSERT(path);

        LELOG("%s [%d]", skipResourcePathPrefix(path), data.size);
        FILE* file;
        LEASSERTM((file = fopen(path, "wb")) != NULL, "couldn't open file: %s", path);
        size_t written = fwrite(data.bytes, data.size, 1, file);
        LELOG("wrote: %d", written);
        LEASSERTM(0 == ferror(file), "couldn't write %s", path);
        LEASSERTM(0 == fclose(file), "couldn't close %s", path);
    }

    Data fileLoadResource(const char* relativeFilePath)
    {
        const char* resourcePath = resPath();
        char* absoluteFilePath = pathCat(resourcePath, relativeFilePath);

        Data result = fileLoad(absoluteFilePath);

        SDL_free(absoluteFilePath);

        return result;
    }

    void fileSaveResource(const char* relativeFilePath, Data data)
    {
        const char* resourcePath = resPath();
        char* absoluteFilePath = pathCat(resourcePath, relativeFilePath);

        fileSave(absoluteFilePath, data);

        SDL_free(absoluteFilePath);
    }

    void Bitmap::init(u16 inWidth, u16 inHeight, BitmapFormat inFormat) {

    }

    void Bitmap::init(const Data& data) {

    }
    void Bitmap::deinit() {

    }

    void Bitmap::write(const char* path) {

    }

    void Bitmap::flip() {

    }
    void Bitmap::premultiply() {

    }
    void Bitmap::clear(u32 clearColor) {

    }
    void Bitmap::setPixel(u16 x, u16 y, u32 color) {

    }
}
