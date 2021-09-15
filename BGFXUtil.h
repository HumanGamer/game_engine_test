#pragma once

#include <bgfx/bgfx.h>

extern const bgfx::Memory* loadMem(const char* path);
extern bgfx::ShaderHandle loadShader(const char* name);
extern void renderScreenSpaceQuad(bgfx::ViewId _view, bgfx::ProgramHandle _program, float _x, float _y, float _width, float _height, float _depth = 0.0f, uint32_t _abgr = 0xFFFFFFFF);
