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
    init_bgfx_vertex_layouts();
    mShader = new BGFXShader("color");
}

void Game::update()
{

}

void Game::render()
{
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 1.0f, 0);

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

    bgfx::setViewRect(0, 0, 0, mWidth, mHeight);
    bgfx::touch(0);

    bgfx::setState(BGFX_STATE_DEFAULT | BGFX_STATE_BLEND_ALPHA);

    int w = mWidth / 2;
    int h = mHeight / 2;
    int xoff = mWidth / 8;
    int yoff = mHeight / 8;
    renderScreenSpaceQuad(0, mShader->getProgram(), xoff, yoff, w, h, 1.0f, 0x550000FF);
    renderScreenSpaceQuad(0, mShader->getProgram(), xoff + w/ 2, yoff + h / 2, w, h, 2.0f, 0x5500FF00);

    bgfx::frame();
}
