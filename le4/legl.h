#pragma once

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#include <stdlib.h>

#define LEGL_MAX_VERTEXATTRIBUTES 4
#define LEGL_MAX_TEXTURES 4
#define LEGL_MAX_UNIFORMBLOCK_ENTRIES 4


namespace le4 {

    struct leGLVertexAttribute
    {
        GLchar * name; // atribute name
        GLint size; // Number of components per generic vertex attribute. Must be 1, 2, 3, or 4.
        GLenum type; // GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, or GL_DOUBLE
        GLsizei stride;
        const GLvoid*  offset;
        GLboolean normalized;
    };


    const char* leGLErrorToString(GLenum err);
    size_t glTypeToSize(GLenum type);

    #if 1
    #define GLDEBUG {GLenum err; while((err = glGetError())) { LELOG("GL error: %s", le4::leGLErrorToString(err)); }}
    #define GLASSERT {GLenum err; int numErrors = 0; while((err = glGetError())) { LELOG("GL error: %s", le4::leGLErrorToString(err)); numErrors++;} if(numErrors>0) {LEASSERTM(false, "failed with GL error");}}
    #else
    #define GLDEBUG
    #define GLASSERT
    #endif

}