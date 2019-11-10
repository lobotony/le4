#pragma once

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#include <stdlib.h>

namespace le4 {

    const char* leGLErrorToString(GLenum err);
    size_t glTypeToSize(GLenum type);

}