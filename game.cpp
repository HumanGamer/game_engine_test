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
    // Palette color is needed for blending to look correct
    bgfx::setPaletteColor(0, 0.0f, 0.0f, 0.0f, 0.0f);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 1.0f, 0, 0); // Last argument is palette index

    float ortho[16];
    bx::mtxOrtho(ortho,
                 0, mWidth,
                 mHeight,0,
                 0.0f, 100.0f,
                 0, bgfx::getCaps()->homogeneousDepth);
    bgfx::setViewTransform(0, nullptr, ortho);

    bgfx::setViewRect(0, 0, 0, mWidth, mHeight);
    bgfx::touch(0);

    const uint64_t alpha_blend_state = 0
                           | BGFX_STATE_WRITE_RGB
                           | BGFX_STATE_WRITE_A
                           //| BGFX_STATE_WRITE_Z // DO NOT WRITE Z DURING ALPHA BLENDING
                           | BGFX_STATE_DEPTH_TEST_LESS
                           | BGFX_STATE_CULL_CW
                           | BGFX_STATE_MSAA
                           | BGFX_STATE_BLEND_ALPHA;

    int w = mWidth / 2;
    int h = mHeight / 2;
    int xoff = mWidth / 8;
    int yoff = mHeight / 8;

    // Red Square with 50% opacity
    bgfx::setState(alpha_blend_state);
    renderScreenSpaceQuad(0, mShader->getProgram(), xoff, yoff, w, h, 0.0f, 0x7F0000FF);

    // Green Square with 100% opacity
    bgfx::setState(BGFX_STATE_DEFAULT);
    renderScreenSpaceQuad(0, mShader->getProgram(), xoff + w/ 2, yoff + h / 2, w, h, 1.0f, 0xFF00FF00);

    bgfx::frame();
}
