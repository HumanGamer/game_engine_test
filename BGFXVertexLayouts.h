#pragma once
#include <bgfx/bgfx.h>

struct PosColorVertex
{
    float x, y, z;
    uint32_t abgr;

    static void init()
    {
        ms_layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();
    }

    static bgfx::VertexLayout ms_layout;
};

extern void init_bgfx_vertex_layouts();
