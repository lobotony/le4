#include "legl.h"
#include "le4.h"

namespace le4 {

const char* glErrorToString(GLenum err)
{
    const char* result = NULL;

#define ERR(e) case e: result = #e; break;

    switch(err)
    {
        ERR(GL_NO_ERROR);
        ERR(GL_INVALID_ENUM);
        ERR(GL_INVALID_VALUE);
        ERR(GL_INVALID_OPERATION);
        ERR(GL_OUT_OF_MEMORY);
        ERR(GL_INVALID_FRAMEBUFFER_OPERATION);
        default:
        {
            char* code = NULL;
            asprintf(&code, "%04x", err); // leaks but probably doesn't matter anymore in error case
            result = code;
            break;
        }
    }

    return result;
}

size_t glTypeToSize(GLenum type)
{
    size_t result = 0;
    switch(type)
    {
        case GL_UNSIGNED_BYTE:result = 1;break;
        case GL_UNSIGNED_SHORT:result = 2;break;
        case GL_UNSIGNED_INT:result = 4;break;
        default:
            LEASSERTM(false, "unknown type: %d", type);
            result = 0;
            break;
    }
    return result;
}

}
