#include "TestRenderer.h"
using namespace le4;


void TestRenderer::init() {
    glGenVertexArrays(1, &vao);GLASSERT;
    shader = loadShaderProgram("resources/glsl/testshader");
    va.begin()
    .set("position", 2, GL_FLOAT)
    .end();
}

void TestRenderer::draw() {

}