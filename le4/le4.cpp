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

    u8 bitmapFormatToBytesPerPixel(BitmapFormat v) {
        u8 result = 0;
        switch(v) {
            case Undefined:
                result = 0;
                LELOG("WARNING: can't determine size for undefined format");
                break;
            case A:result=1; break;
            case RGB:result=3; break;
            case RGBA:result=4; break;
        }
        return result;
    }

    void Bitmap::init(u16 inWidth, u16 inHeight, BitmapFormat inFormat) {
        deinit(); // get rid of old data if present
        u32 destBytesPerPixel = bitmapFormatToBytesPerPixel(format);
        u32 destSizeInBytes = destBytesPerPixel * width * height;
        data = (u8*)SDL_malloc(destSizeInBytes);
        loaded = false; // prevent stb_image from freeing
        width = inWidth;
        height = inHeight;
        format = inFormat;
    }

    void Bitmap::init(const Data& inData) {
        deinit(); // get rid of old data if present
        int bytesPerPixel, w, h = 0;
        data = stbi_load_from_memory(inData.bytes, (s32)(inData.size), &w, &h, &bytesPerPixel, 0);
        if(!data)
        {
            LELOG("ERROR: couldn't init image from memory: %s", stbi_failure_reason());
            LEASSERT(false);
        }
        width = (u16)w;
        height = (u16)h;

        switch(bytesPerPixel)
        {
            case 3:format = RGB;break;
            case 4:format = RGBA;break;
            default:
            LELOG("ERROR: couldn't init image, don't know what to do with bytesPerPixel: %d", bytesPerPixel);
                LEASSERT(false);
                break;
        }
        loaded = true;
    }

    void Bitmap::deinit() {
        if(data) {
            if(loaded) {
                stbi_image_free(data);
            } else {
                SDL_free(data);
            }
        }

        data = NULL;
        width = 0;
        height = 0;
        format = Undefined;
        premultiplied = false;
        loaded = false;
    }

    void Bitmap::write(const char* path) {
        int bpp = bitmapFormatToBytesPerPixel(format);
        if(!stbi_write_png(path, width, height, bpp, data, bpp*width)) {
            LELOG("screenshot save failed");
        }
    }

    void Bitmap::flip() {
        u16 pixelSizeBytes = bitmapFormatToBytesPerPixel(format);
        // flip vertically because OpenGL returns it the other way round
        u16 lineInBytes = width * pixelSizeBytes;
        u16 halfHeight = u16(height / 2); // deliberately round down if height is odd
        u8* dataBytes = data;
        for(u16 bottomLine=0; bottomLine<halfHeight; ++bottomLine) {
            u16 topLine = u16((height - 1) - bottomLine);
            for(u16 bi=0; bi<lineInBytes; ++bi) {
                u16 topLineByte = width*topLine*pixelSizeBytes+bi;
                u16 bottomLineByte = width*bottomLine*pixelSizeBytes+bi;
                u8 b = dataBytes[topLineByte];
                dataBytes[topLineByte] = dataBytes[bottomLineByte];
                dataBytes[bottomLineByte] = b;
            }
        }
    }

    void Bitmap::premultiply() {
        LEASSERTM(format == RGBA, "premutiply only supported for RGBA bitmaps");
        u32* pp = (u32*)data;
        f32 n = 1/255.0f;
        for(int y=0; y<height; ++y) {
            for(int x=0; x<width; ++x) {
                int i = y*width+x;
                u32 p = pp[i];
                f32 a = ((f32)((p & 0xff000000)>>24))*n;
                f32 b = ((f32)((p & 0x00ff0000)>>16))*n;
                f32 g = ((f32)((p & 0x0000ff00)>>8))*n;
                f32 r = ((f32)(p & 0x000000ff))*n;

                r*=a;
                g*=a;
                b*=a;

                p = (
                        ((u32)(a*255.0f)<<24) |
                                ((u32)(b*255.0f)<<16) |
                                ((u32)(g*255.0f)<<8) |
                                ((u32)(r*255.0f))
                );
                pp[i] = p;
            }
        }
    }

    void Bitmap::clear(u32 clearColor) {
        LEASSERTM(format == RGBA, "clear only supported for RGBA bitmaps");
        u32* pp = (u32*)data;
        for(s32 x=0; x<width; ++x) {
            for(s32 y=0; y<height; ++y) {
                int i = y*width+x;
                pp[i] = clearColor;
            }
        }
    }

    void Bitmap::setPixel(u16 x, u16 y, u32 color) {
        LEASSERTM(format == RGBA, "setpixel only supported for RGBA bitmaps");
        LEASSERT((x < width) && (y < height));
        int i = y*width+x;
        u32* pp = (u32*)data;
        pp[i] = color;
    }
}
