#include "BGFXUtil.h"

#include <bgfx/bgfx.h>
#include <bx/pixelformat.h>
#include <bx/file.h>
#include <SDL.h>
#include <vector>
#include <fstream>

#include "BGFXShader.h"
#include "BGFXVertexLayouts.h"

inline unsigned int argb_to_rgba(unsigned int argb)
{
    return argb << 8 | argb >> 24;
}

inline unsigned int rgba_to_argb(unsigned int rgba)
{
    return rgba >> 8 | rgba << 24;
}

inline unsigned int argb_to_abgr(unsigned int argb) {
    int r = (argb >> 16) & 0xFF;
    int b = argb & 0xFF;
    return (argb & 0xFF00FF00) | (b << 16) | r;
}

inline void argb_int_to_float(unsigned int argb, float* red, float* green, float* blue, float* alpha)
{
    if (red)
        *red = ((argb & (255 << 16)) >> 16) / 255.0f;
    if (green)
        *green = ((argb & (255 << 8)) >> 8) / 255.0f;
    if (blue)
        *blue = ((argb & (255 << 0)) >> 0) / 255.0f;
    if (alpha)
        *alpha = ((argb & (255 << 24)) >> 24) / 255.0f;
}

inline unsigned int color_to_int_argb(float red, float green, float blue, float alpha)
{
    /*unsigned int r = (int)(3 * (red < 0 ? 0 : red));
	unsigned int g = (int)(7 * (green < 0 ? 0 : green));
	unsigned int b = (int)(3 * (blue < 0 ? 0 : blue));
	return (r << 6) | (g << 2) | b;*/

    //unsigned int r = (unsigned int)floor(red >= 1.0 ? 255 : red * 256.0);
    //unsigned int r = (unsigned int)(red >= 1.0 ? 255 : (red <= 0.0 ? 0 : (int)floor(red * 256.0)));
    unsigned int r = std::max(0, std::min(255, (int)floor(red * 256.0)));
    unsigned int g = std::max(0, std::min(255, (int)floor(green * 256.0)));
    unsigned int b = std::max(0, std::min(255, (int)floor(blue * 256.0)));
    unsigned int a = std::max(0, std::min(255, (int)floor(alpha * 256.0)));

    unsigned int color = 0;
    color |= (a & 255) << 24;
    color |= (r & 255) << 16;
    color |= (g & 255) << 8;
    color |= (b & 255);

    return color;
}

inline unsigned int encodeNormalRgba8(float _x, float _y, float _z, float _w)
{
    const float src[] =
        {
            _x * 0.5f + 0.5f,
            _y * 0.5f + 0.5f,
            _z * 0.5f + 0.5f,
            _w * 0.5f + 0.5f,
        };
    unsigned int dst;
    bx::packRgba8(&dst, src);
    return dst;
}

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

    if (bgfx::allocTransientBuffers(&tvb, PosColorTexcoordVertex::ms_layout, 4, &tib, 6))
    {
        PosColorTexcoordVertex* vertex = (PosColorTexcoordVertex*)tvb.data;

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
        vertex[0].u = minu;
        vertex[0].v = minv;

        vertex[1].x = maxx;
        vertex[1].y = miny;
        vertex[1].z = zz;
        vertex[1].abgr = _abgr;//0xff00ff00;
        vertex[1].u = maxu;
        vertex[1].v = minv;

        vertex[2].x = maxx;
        vertex[2].y = maxy;
        vertex[2].z = zz;
        vertex[2].abgr = _abgr;//0xffff0000;
        vertex[2].u = maxu;
        vertex[2].v = maxv;

        vertex[3].x = minx;
        vertex[3].y = maxy;
        vertex[3].z = zz;
        vertex[3].abgr = _abgr;//0xffffffff;
        vertex[3].u = minu;
        vertex[3].v = maxv;

        uint16_t* indices = (uint16_t*)tib.data;

        indices[0] = 0;
        indices[1] = 2;
        indices[2] = 1;
        indices[3] = 0;
        indices[4] = 3;
        indices[5] = 2;

        bgfx::setState(BGFX_STATE_DEFAULT);// | BGFX_STATE_BLEND_ALPHA );
        //bgfx::setState(0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_ALPHA);
        bgfx::setIndexBuffer(&tib);
        bgfx::setVertexBuffer(0, &tvb);
        bgfx::submit(_view, _program);
    }
}

void setViewOrtho(bgfx::ViewId id, float width, float height)
{
    float ortho[16];
    bx::mtxOrtho(ortho,
                 0.0f, width,
                 height, 0.0f,
                 0.0f, 100.0f,
                 0.0f, bgfx::getCaps()->homogeneousDepth);
    bgfx::setViewTransform(id, nullptr, ortho);
}

bgfx::UniformHandle mUniformTexMain;
bgfx::UniformHandle mUniformColorDiscard;

void setTexture(bgfx::TextureHandle texture)
{
    bgfx::setTexture(0, mUniformTexMain, texture);
}

void setDiscardColor(float red, float green, float blue)
{
    float vec[4];
    vec[0] = red;
    vec[1] = green;
    vec[2] = blue;
    vec[3] = 0x00;
    bgfx::setUniform(mUniformColorDiscard, vec);
}

//void drawFrameBuffer(bgfx::ViewId id, bgfx::TextureHandle fbtex)
//{
//	BGFXGraphics* graphics = (BGFXGraphics*)gx_runtime->graphics;
//	float width = graphics->getWidth();
//	float height = graphics->getHeight();
//	setViewOrtho(id, width, height);
//	setTexture(fbtex);
//	renderScreenSpaceQuad(id, graphics->getDefaultTextureShader()->getProgram(), 0, 0, width, height);
//}

void bgfxUtilInit()
{
    mUniformTexMain = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
    mUniformColorDiscard = bgfx::createUniform("s_discardColor", bgfx::UniformType::Vec4);
}