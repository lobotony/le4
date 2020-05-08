#include "TestRenderer.h"
using namespace le4;


void TestRenderer::init() {
    shader = loadShaderProgram("resources/glsl/testshader");
    glGenVertexArrays(1, &vao);GLASSERT;
    glBindVertexArray(vao);GLASSERT;

    va.begin(shader)
    .set("position", 2, GL_FLOAT)
    .end();

    LELOG("pos_loc:_ %d", va.locations[0]);

    glGenBuffers(1, &vb);GLASSERT;
    glBindBuffer(GL_ARRAY_BUFFER, vb);GLASSERT;

    numchars = 1;
    numverts = numchars * 4;
    numtris = numchars * 2;
    numindices = numtris * 3;

    indexBufferSize = numindices*sizeof(u16);
    indexBufferData = (u16*)SDL_malloc(indexBufferSize);

    vertexBufferSize = numverts*va.totalSize();
    vertexBufferData = (le4::vec2*) SDL_malloc(vertexBufferSize);

    draw_mode = GL_TRIANGLES;

    glGenBuffers(1, &ib);GLASSERT;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vb);GLASSERT;

    for (u16 i=0; i<numchars; ++i) {
        u16 index_offset = i*u16(6);
        u16 vertex_offset = i*u16(4);

        indexBufferData[index_offset+0] = vertex_offset+u16(0);
        indexBufferData[index_offset+1] = vertex_offset+u16(1);
        indexBufferData[index_offset+2] = vertex_offset+u16(2);
        indexBufferData[index_offset+3] = vertex_offset+u16(0);
        indexBufferData[index_offset+4] = vertex_offset+u16(2);
        indexBufferData[index_offset+5] = vertex_offset+u16(3);
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, indexBufferData, GL_STATIC_DRAW);GLASSERT;

    for(int i=0; i<va.count(); ++i) {
        GLuint loc = va.locations[i];
        const VertexAttribute& attrib = va.attributes[i];
        glVertexAttribPointer(loc, attrib.size, attrib.type, attrib.normalized, attrib.stride, attrib.offset);GLASSERT;
        glEnableVertexAttribArray(loc);GLASSERT;
    }

    glBindVertexArray(0);GLASSERT; // break current binding
}

void TestRenderer::draw() {
    glDisable(GL_DEPTH_TEST);GLASSERT;
    glDisable(GL_CULL_FACE);GLASSERT;
    glEnable(GL_BLEND);GLASSERT;
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);GLASSERT;
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);GLASSERT;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);GLASSERT;
    //glViewport(0, 0, 640, 480);GLASSERT;
    f32 sz = 0.5f;
    vertexBufferData[0].x = 0.0f;
    vertexBufferData[0].y = 0.0f;

    vertexBufferData[1].x = sz;
    vertexBufferData[1].y = 0.0f;

    vertexBufferData[2].x = sz;
    vertexBufferData[2].y = sz;

    vertexBufferData[3].x = 0.0f;
    vertexBufferData[3].y = sz;

    glUseProgram(shader);GLASSERT;
    glBindVertexArray(vao);GLASSERT;

    u16 _ib[] = {0,1,2, 0,2,3};
    float _vb[] = {0.0, 0.0, sz, 0, sz, sz, 0, sz};

    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertexBufferData, GL_STATIC_DRAW);GLASSERT;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, indexBufferData, GL_STATIC_DRAW);GLASSERT;
    glBufferData(GL_ARRAY_BUFFER, 8*sizeof(float), _vb, GL_STATIC_DRAW);GLASSERT;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(u16), _ib, GL_STATIC_DRAW);GLASSERT;
    glDrawElements(GL_TRIANGLES, numindices, GL_UNSIGNED_SHORT, 0);GLASSERT;
}
