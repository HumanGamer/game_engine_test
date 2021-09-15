#include "BGFXUtil.h"

#include <bgfx/bgfx.h>
#include <bx/pixelformat.h>
#include <bx/file.h>
#include <SDL.h>
#include <vector>
#include <fstream>

#include "BGFXVertexLayouts.h"

const bgfx::Memory* loadMem(const char* path)
{
    std::ifstream file(path, std::ios::binary);
    if (file.fail())
    {

        SDL_Log("Failed to load %s.", path);
        return nullptr;
    }

    std::vector<char> buffer = std::vector<char>(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>());
    const bgfx::Memory* mem = bgfx::alloc(buffer.size() + 1);
    memcpy_s(mem->data, buffer.size() + 1, buffer.data(), buffer.size());
    mem->data[mem->size - 1] = '\0';
    return mem;
}

bgfx::ShaderHandle loadShader(const char* name)
{
    char filePath[512];

    const char* shaderPath = "???";

    switch (bgfx::getRendererType())
    {
        case bgfx::RendererType::Noop:
        case bgfx::RendererType::Direct3D9:  shaderPath = "data/shaders/dx9/";   break;
        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12: shaderPath = "data/shaders/dx11/";  break;
        case bgfx::RendererType::Agc:
        case bgfx::RendererType::Gnm:        shaderPath = "data/shaders/pssl/";  break;
        case bgfx::RendererType::Metal:      shaderPath = "data/shaders/metal/"; break;
        case bgfx::RendererType::Nvn:        shaderPath = "data/shaders/nvn/";   break;
        case bgfx::RendererType::OpenGL:     shaderPath = "data/shaders/glsl/";  break;
        case bgfx::RendererType::OpenGLES:   shaderPath = "data/shaders/essl/";  break;
        case bgfx::RendererType::Vulkan:     shaderPath = "data/shaders/spirv/"; break;
        case bgfx::RendererType::WebGPU:     shaderPath = "data/shaders/spirv/"; break;

        case bgfx::RendererType::Count:
            BX_ASSERT(false, "You should not be here!");
            break;
    }

    bx::strCopy(filePath, BX_COUNTOF(filePath), shaderPath);
    bx::strCat(filePath, BX_COUNTOF(filePath), name);
    bx::strCat(filePath, BX_COUNTOF(filePath), ".bin");

    bgfx::ShaderHandle handle = bgfx::createShader(loadMem(filePath));
    bgfx::setName(handle, name);

    return handle;
}

void renderScreenSpaceQuad(bgfx::ViewId _view, bgfx::ProgramHandle _program, float _x, float _y, float _width, float _height, float _depth, uint32_t _abgr)
{
    bgfx::TransientVertexBuffer tvb;
    bgfx::TransientIndexBuffer tib;

    if (bgfx::allocTransientBuffers(&tvb, PosColorVertex::ms_layout, 4, &tib, 6))
    {
        PosColorVertex* vertex = (PosColorVertex*)tvb.data;

        float zz = _depth;

        const float minx = _x;
        const float maxx = _x + _width;
        const float miny = _y;
        const float maxy = _y + _height;

        float minu = 0.0f;
        float minv = 0.0f;
        float maxu = 1.0f;
        float maxv = 1.0f;

        vertex[0].x = minx;
        vertex[0].y = miny;
        vertex[0].z = zz;
        vertex[0].abgr = _abgr;//0xff0000ff;

        vertex[1].x = maxx;
        vertex[1].y = miny;
        vertex[1].z = zz;
        vertex[1].abgr = _abgr;//0xff00ff00;

        vertex[2].x = maxx;
        vertex[2].y = maxy;
        vertex[2].z = zz;
        vertex[2].abgr = _abgr;//0xffff0000;

        vertex[3].x = minx;
        vertex[3].y = maxy;
        vertex[3].z = zz;
        vertex[3].abgr = _abgr;//0xffffffff;

        uint16_t* indices = (uint16_t*)tib.data;

        indices[0] = 0;
        indices[1] = 2;
        indices[2] = 1;
        indices[3] = 0;
        indices[4] = 3;
        indices[5] = 2;

        //bgfx::setState(BGFX_STATE_DEFAULT | BGFX_STATE_BLEND_ALPHA );
        //bgfx::setState(0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_ALPHA);
        bgfx::setIndexBuffer(&tib);
        bgfx::setVertexBuffer(0, &tvb);
        bgfx::submit(_view, _program);
    }
}