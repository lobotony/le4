#include "TexQuadRenderer.hpp"

#include "sokol_gfx.h"
#include "flextgl/flextGL.h"


//using namespace le4;

void TexQuadRenderer::init() {
    flextInit();
    /* setup sokol_gfx */
    sg_desc desc = {0};
    sg_setup(&desc);

    /* a vertex buffer */
    const float vertices[] = {
        // positions            // colors
         0.0f,  0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.5f,     0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f
    };
    sg_buffer_desc buffer_desc = {
        .size = sizeof(vertices),
        .content = vertices,
    };
    sg_buffer vbuf = sg_make_buffer(&buffer_desc);

    /* prepare a resource binding struct, only the vertex buffer is needed here */
    bind = {
        .vertex_buffers[0] = vbuf
    };

    /* a shader */
    sg_shader_desc shader_desc = {
        /* vertex attribute lookup by name is optional in GL3.x, we
           could also use "layout(location=N)" in the shader
        */
        .attrs = {
            [0].name = "position",
            [1].name = "color0"
        },
        .vs.source =
            "#version 330\n"
            "in vec4 position;\n"
            "in vec4 color0;\n"
            "out vec4 color;\n"
            "void main() {\n"
            "  gl_Position = position;\n"
            "  color = color0;\n"
            "}\n",
        .fs.source =
            "#version 330\n"
            "in vec4 color;\n"
            "out vec4 frag_color;\n"
            "void main() {\n"
            "  frag_color = color;\n"
            "}\n"
    };
    sg_shader shd = sg_make_shader(&shader_desc);

    /* a pipeline state object */
    sg_pipeline_desc pipeline_desc = {
        .shader = shd,
        /* if the vertex layout doesn't have gaps, don't need to provide strides and offsets */
        .layout = {
            .attrs = {
                [0].format=SG_VERTEXFORMAT_FLOAT3,
                [1].format=SG_VERTEXFORMAT_FLOAT4
            }
        }
    };
    pip = sg_make_pipeline(&pipeline_desc);

    /* default pass action (clear to grey) */
    pass_action = {0};
}

void TexQuadRenderer::draw(int w, int h) {
    sg_begin_default_pass(&pass_action, w, h);
    sg_apply_pipeline(pip);
    sg_apply_bindings(&bind);
    sg_draw(0, 3, 1);
    sg_end_pass();
    sg_commit();

}

