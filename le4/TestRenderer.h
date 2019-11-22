#pragma once

#include "legl.h"

struct TestRenderer {
    int shader;
    GLuint vao;
    le4::VertexAttributes va;

    void init();

    void draw();
};
