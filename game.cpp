#include "game.h"
#include <bgfx/bgfx.h>
#include <bx/math.h>

#include "BGFXUtil.h"
#include "BGFXVertexLayouts.h"

Game::Game(SDL_Window* window, uint32_t width, uint32_t height) : mWindow(window), mWidth(width), mHeight(height)
{

}

Game::~Game()
{

}

void Game::init()
{
    bgfxUtilInit();
    init_bgfx_vertex_layouts();
    mShader = new BGFXShader("color");
}

void Game::update()
{

}

void Game::render()
{
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);

    const bx::Vec3 at = {0.0f, 0.0f, 0.0f};
    const bx::Vec3 eye = {0.0f, 0.0f, -35.0f};

    float view[16];
    bx::mtxLookAt(view, eye, at);

    float ortho[16];
    bx::mtxOrtho(ortho,
                 0, mWidth,
                 mHeight,0,
                 0.0f, 100.0f,
                 0, bgfx::getCaps()->homogeneousDepth);
    bgfx::setViewTransform(0, nullptr, ortho);

//    float proj[16];
//    bx::mtxProj(proj,
//                60.0f, (float)mWidth / (float)mHeight,
//                0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
//    bgfx::setViewTransform(0, view, proj);
    bgfx::setViewRect(0, 0, 0, mWidth, mHeight);
    bgfx::touch(0);

    //setDiscardColor(0.0f, 1.0f, 0.0f);
    renderScreenSpaceQuad(0, mShader->getProgram(), 10, 10, 100, 100, 0.0f, 0xFF0000FF);
    renderScreenSpaceQuad(0, mShader->getProgram(), 60, 60, 100, 100, 0.0f, 0xFF00FF00);

    bgfx::frame();
}
