#pragma once

#include <SDL.h>
#include "BGFXShader.h"

class Game
{
public:
    Game(SDL_Window* window, uint32_t width, uint32_t height);
    ~Game();

    void init();
    void update();
    void render();

private:
    SDL_Window* mWindow;

    BGFXShader* mShader;
    uint32_t mWidth, mHeight;
};