#include "legl.h"
#include "le4.h"

namespace le4 {

    const char* leglErrorToString(GLenum err) {
        const char* result = NULL;
        #define ERR(e) case e: result = #e; break;
        switch(err) {
            ERR(GL_NO_ERROR);
            ERR(GL_INVALID_ENUM);
            ERR(GL_INVALID_VALUE);
            ERR(GL_INVALID_OPERATION);
            ERR(GL_OUT_OF_MEMORY);
            ERR(GL_INVALID_FRAMEBUFFER_OPERATION);
            default: {
                char* code = NULL;
                asprintf(&code, "%04x", err); // leaks but probably doesn't matter anymore in error case
                result = code;
                break;
            }
        }
        return result;
    }

    size_t leglTypeToSize(GLenum type) {
        size_t result = 0;
        switch(type) {
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

    void leglShaderSetSource(GLuint shader, const Data& source)
    {
        const GLchar* src = (GLchar*)source.bytes;
        GLint length = (GLint)source.size;
        glShaderSource(shader, 1, &src, &length);GLASSERT;
    }

    GLint leglShaderGetParam(GLuint shader, GLenum paramName)
    {
        GLint result;
        glGetShaderiv(shader, paramName, &result);GLASSERT;
        return result;
    }

    bool leglShaderCompiled(GLuint shader)
    {
        return leglShaderGetParam(shader, GL_COMPILE_STATUS) != 0;
    }

    const char* leglShaderGetLog(GLuint shader)
    {
        GLint size = leglShaderGetParam(shader, GL_INFO_LOG_LENGTH);
        char* result = NULL;
        if(size > 0)
        {
            result = (char*)SDL_malloc((size_t)size);
            GLsizei length = 0;
            glGetShaderInfoLog(shader, size, &length, result);GLASSERT;
        }
        return result;
    }

    GLint leglShaderProgramGetParam(GLuint shaderProgram, GLenum paramName)
    {
        GLint result;
        glGetProgramiv(shaderProgram, paramName, &result);GLASSERT;
        return result;
    }

    bool leglShaderProgramLinked(GLuint shaderProgram)
    {
        return leglShaderProgramGetParam(shaderProgram, GL_LINK_STATUS) != 0;
    }

    const char* leglShaderProgramGetLog(GLuint shaderProgram)
    {
        GLint size = leglShaderProgramGetParam(shaderProgram, GL_INFO_LOG_LENGTH);
        char* result = NULL;
        if(size > 0)
        {
            result = (char*)SDL_malloc((size_t)size);
            GLsizei length = 0;
            glGetProgramInfoLog(shaderProgram, size, &length, result);GLASSERT;
        }
        return result;
    }

    GLuint loadShaderProgram(const char* path) {
        char* fspath = concat(path, ".fs");
        char* vspath = concat(path, ".vs");

        Data fsdata = fileLoadResource(fspath);
        Data vsdata = fileLoadResource(vspath);

        SDL_free(fspath);
        SDL_free(vspath);

        GLuint shaderProgram = glCreateProgram();GLASSERT;

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);GLASSERT;
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);GLASSERT;

        leglShaderSetSource(vs, vsdata);
        glCompileShader(vs);GLASSERT;

        if(!leglShaderCompiled(vs))
        {
            // LELOG("================ VERTEX SHADER ==============");
            // vsdata.bytes[vsdata.size-1] = 0; // workaround for non-0-termianted string, doesn't really matter in case of compile error
            // LELOG("%s", vsdata.bytes);
            // LELOG("================ LOG ==============");
            LELOG("%s", leglShaderGetLog(vs));
            LEASSERTM(false, "ERROR: vertex shader compilation failed");
        }

        leglShaderSetSource(fs, fsdata);
        glCompileShader(fs);GLASSERT;
        if(!leglShaderCompiled(fs))
        {
            //LELOG("================ FRAGMENT SHADER ==============");
            //fsdata.bytes[vsdata.size-1] = 0; // workaround for non-0-termianted string, doesn't really matter in case of compile error
            //LELOG("%s", fsdata.bytes);
            //LELOG("================ LOG ==============");
            LELOG("%s", leglShaderGetLog(fs));
            LEASSERTM(false, "ERROR: fragment shader compilation failed");
        }

        glAttachShader(shaderProgram, vs);GLASSERT;
        glAttachShader(shaderProgram, fs);GLASSERT;
        glLinkProgram(shaderProgram);GLASSERT;

        if(!leglShaderProgramLinked(shaderProgram))
        {
            LELOG("================= SHADER PROGRAM LINK FAILED: %s", path);
            LELOG("%s", leglShaderProgramGetLog(shaderProgram));
            LEASSERTM(false, "ERROR: shader program link failed");
        }

        fsdata.deinit();
        vsdata.deinit();

        return shaderProgram;
    }

}
