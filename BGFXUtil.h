#pragma once

#include <bgfx/bgfx.h>

extern inline unsigned int argb_to_rgba(unsigned int argb);
extern inline unsigned int rgba_to_argb(unsigned int rgba);
extern inline unsigned int argb_to_abgr(unsigned int argb);
extern inline void argb_int_to_float(unsigned int argb, float* red, float* green, float* blue, float* alpha);
extern inline unsigned int color_to_int_argb(float red, float green, float blue, float alpha);
extern inline unsigned int encodeNormalRgba8(float _x, float _y = 0.0f, float _z = 0.0f, float _w = 0.0f);
extern const bgfx::Memory* loadMem(const char* path);
extern bgfx::ShaderHandle loadShader(const char* name);
extern void renderScreenSpaceQuad(bgfx::ViewId _view, bgfx::ProgramHandle _program, float _x, float _y, float _width, float _height, float _depth = 0.0f, uint32_t _abgr = 0xFFFFFFFF);
extern void setViewOrtho(bgfx::ViewId id, float width, float height);
extern void setTexture(bgfx::TextureHandle texture);
extern void setDiscardColor(float red, float green, float blue);
//extern void drawFrameBuffer(bgfx::ViewId id, bgfx::TextureHandle fbtex);
extern void bgfxUtilInit();
