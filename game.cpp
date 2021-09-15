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

//    float proj[16];
//    bx::mtxProj(proj,
//                60.0f, (float)mWidth / (float)mHeight,
//                0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
//    bgfx::setViewTransform(0, view, proj);
    bgfx::setViewRect(0, 0, 0, mWidth, mHeight);
    bgfx::touch(0);

    //setDiscardColor(0.0f, 1.0f, 0.0f);
    //bgfx::setState(BGFX_STATE_DEFAULT | BGFX_STATE_BLEND_ALPHA);
    /*static uint32_t blendval = 0;
    bgfx::setState(0
                   | BGFX_STATE_DEFAULT
                   | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_ONE,BGFX_STATE_BLEND_INV_SRC_ALPHA)
        ,blendval
    );*/
    //bgfx::setState(BGFX_STATE_DEFAULT| BGFX_STATE_BLEND_FUNC_SEPARATE(BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_ZERO, BGFX_STATE_BLEND_INV_SRC_ALPHA));//BGFX_STATE_BLEND_ALPHA );
    /*bgfx::setState(BGFX_STATE_DEFAULT | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_ONE)
                                        | BGFX_STATE_BLEND_INDEPENDENT
        , 0
          | BGFX_STATE_BLEND_FUNC_RT_1(BGFX_STATE_BLEND_ZERO, BGFX_STATE_BLEND_SRC_COLOR));*/
    /*bgfx::setState(0
                   | BGFX_STATE_WRITE_RGB
                   | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_INV_SRC_ALPHA, BGFX_STATE_BLEND_SRC_ALPHA)
    );*/
    /*bgfx::setState(0
                   | BGFX_STATE_CULL_CW
                   | BGFX_STATE_WRITE_RGB
                   | BGFX_STATE_WRITE_A
                   | BGFX_STATE_DEPTH_TEST_LESS
                   | BGFX_STATE_MSAA
                   | BGFX_STATE_BLEND_ALPHA);*/


    bgfx::setState(BGFX_STATE_DEFAULT | BGFX_STATE_BLEND_ALPHA);//, 0x00000055);

    int w = mWidth / 2;
    int h = mHeight / 2;
    int xoff = mWidth / 8;
    int yoff = mHeight / 8;
    renderScreenSpaceQuad(0, mShader->getProgram(), xoff, yoff, w, h, 1.0f, 0x550000FF);
    renderScreenSpaceQuad(0, mShader->getProgram(), xoff + w/ 2, yoff + h / 2, w, h, 2.0f, 0x5500FF00);

    //bgfx::setState(BGFX_STATE_DEFAULT | BGFX_STATE_BLEND_ALPHA);
    //renderScreenSpaceQuad(0, mShader->getProgram(), 0, 0, mWidth, mHeight, 1.0f, 0xFF000000);

    bgfx::frame();
}
