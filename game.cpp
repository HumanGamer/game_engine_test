#include "game.h"
#include <bgfx/bgfx.h>

#include "BGFXUtil.h"

Game::Game(SDL_Window* window) : mWindow(window)
{

}

Game::~Game()
{

}

void Game::init()
{
    bgfxUtilInit();
    mShader = new BGFXShader("textureDiscardColor");
}

void Game::update()
{

}

void Game::render()
{
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);
    bgfx::touch(0);



    bgfx::frame();
}
