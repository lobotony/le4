#pragma once

#include "sokol_gfx.h"

struct TexQuadRenderer {

    sg_bindings bind;
    sg_pipeline pip;
    sg_pass_action pass_action;

    void init();

    void draw(int w, int h);

};

