#include "BGFXVertexLayouts.h"

bgfx::VertexLayout PosColorTexcoordVertex::ms_layout;
bgfx::VertexLayout PosNormalTangentTexcoordVertex::ms_layout;

void init_bgfx_vertex_layouts()
{
    static bool initialized = false;
    if (initialized)
        return;
    initialized = true;

    PosColorTexcoordVertex::init();
    PosNormalTangentTexcoordVertex::init();
}
