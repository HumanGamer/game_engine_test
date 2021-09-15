#pragma once

#include <bgfx/bgfx.h>

class BGFXShader
{
public:
    BGFXShader(const char* name);
    virtual ~BGFXShader();

    bgfx::ProgramHandle getProgram();

private:
    bgfx::ProgramHandle mProgram;
    bgfx::ShaderHandle mVertex;
    bgfx::ShaderHandle mFragment;
};
