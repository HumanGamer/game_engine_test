#pragma once
#include <bgfx/bgfx.h>

struct PosColorTexcoordVertex
{
    float x, y, z;
    uint32_t abgr;
    //uint32_t abgr_discard;
    float u, v;

    static void init()
    {
        ms_layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                //.add(bgfx::Attrib::Color1, 4, bgfx::AttribType::Uint8, true)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();
    }

    static bgfx::VertexLayout ms_layout;
};

struct PosNormalTangentTexcoordVertex
{
    float x, y, z;
    uint32_t normal, tangent;
    int16_t u, v;

    static void init()
    {
        ms_layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Normal, 4, bgfx::AttribType::Uint8, true, true)
            .add(bgfx::Attrib::Tangent, 4, bgfx::AttribType::Uint8, true, true)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Int16, true, true)
            .end();
    }

    static bgfx::VertexLayout ms_layout;
};

extern void init_bgfx_vertex_layouts();
