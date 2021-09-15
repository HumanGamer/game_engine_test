#include "BGFXVertexLayouts.h"

bgfx::VertexLayout PosColorVertex::ms_layout;

void init_bgfx_vertex_layouts()
{
    static bool initialized = false;
    if (initialized)
        return;
    initialized = true;

    PosColorVertex::init();
}
