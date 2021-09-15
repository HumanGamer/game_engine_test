#pragma once

#include <SDL.h>
#include "BGFXShader.h"

class Game
{
public:
    Game(SDL_Window* window);
    ~Game();

    void init();
    void update();
    void render();

private:
    SDL_Window* mWindow;

    BGFXShader* mShader;
};