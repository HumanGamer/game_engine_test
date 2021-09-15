#include "BGFXShader.h"

#include <string>

#include "BGFXUtil.h"

BGFXShader::BGFXShader(const char* name)
{
    mVertex = loadShader((std::string(name) + "V").c_str());
    mFragment = loadShader((std::string(name) + "P").c_str());

    mProgram = bgfx::createProgram(mVertex, mFragment, true);
}

BGFXShader::~BGFXShader()
{
    bgfx::destroy(mProgram);
    bgfx::destroy(mVertex);
    bgfx::destroy(mFragment);
}

bgfx::ProgramHandle BGFXShader::getProgram()
{
    return mProgram;
}