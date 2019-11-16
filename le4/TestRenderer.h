#pragma once

#include "legl.h"

struct TestRenderer {
    int shader;
    GLuint vao;

    void init();

    void draw();
};
