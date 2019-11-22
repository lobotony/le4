#pragma once

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#include <stdlib.h>
#include "le4.h"

#define LEGL_MAX_VERTEXATTRIBUTES 4
#define LEGL_MAX_TEXTURES 4
#define LEGL_MAX_UNIFORMBLOCK_ENTRIES 4


namespace le4 {

    struct VertexAttribute
    {
        const char * name; // attribute name
        GLint size; // Number of components per generic vertex attribute. Must be 1, 2, 3, or 4.
        GLenum type; // GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, or GL_DOUBLE
        GLsizei stride;
        const GLvoid*  offset;
        GLboolean normalized;

        void init();
        void init(const char* inName, GLint inSize, GLenum inType);

        size_t sizeInBytes();
    };

    struct VertexAttributes {
        VertexAttribute attributes[LEGL_MAX_VERTEXATTRIBUTES];

        VertexAttributes& begin();
        VertexAttributes& set(const char* inName, GLint inSize, GLenum inType);
        VertexAttributes& end();

        size_t totalSize();

    private:
        int update_index;
        void updateStrideOffset();
    };


    const char* leglErrorToString(GLenum err);
    size_t leglTypeToSize(GLenum type);

    GLuint loadShaderProgram(const char* path);

    #if 1
    #define GLDEBUG {GLenum err; while((err = glGetError())) { LELOG("GL error: %s", le4::leglErrorToString(err)); }}
    #define GLASSERT {GLenum err; int numErrors = 0; while((err = glGetError())) { LELOG("GL error: %s", le4::leglErrorToString(err)); numErrors++;} if(numErrors>0) {LEASSERTM(false, "failed with GL error");}}
    #else
    #define GLDEBUG
    #define GLASSERT
    #endif

}