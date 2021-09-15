#include <bgfx/bgfx.h>
#include "game.h"

Game::Game(SDL_Window* window) : mWindow(window)
{

}

Game::~Game()
{

}

void Game::init()
{

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
