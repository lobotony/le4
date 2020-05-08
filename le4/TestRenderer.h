#pragma once

//#include "le4.h"
#include "legl.h"

struct TestRenderer {
    GLuint shader;
    GLuint vao;
    le4::VertexAttributes va;
    GLuint vb;
    GLuint ib;

    int numchars;
    int numverts;
    int numtris;
    int numindices;

    size_t indexBufferSize;
    le4::u16* indexBufferData;

    size_t vertexBufferSize;
    le4::vec2* vertexBufferData;

    int draw_mode;

    void init();

    void draw();
};
