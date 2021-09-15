#include <iostream>
#include <SDL.h>
#include <SDL_syswm.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include "game.h"

static void* sdlNativeWindowHandle(SDL_Window* _window)
{
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(_window, &wmi) )
    {
        return NULL;
    }

#	if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    #		if ENTRY_CONFIG_USE_WAYLAND
		wl_egl_window *win_impl = (wl_egl_window*)SDL_GetWindowData(_window, "wl_egl_window");
		if(!win_impl)
		{
			int width, height;
			SDL_GetWindowSize(_window, &width, &height);
			struct wl_surface* surface = wmi.info.wl.surface;
			if(!surface)
				return nullptr;
			win_impl = wl_egl_window_create(surface, width, height);
			SDL_SetWindowData(_window, "wl_egl_window", win_impl);
		}
		return (void*)(uintptr_t)win_impl;
#		else
		return (void*)wmi.info.x11.window;
#		endif
#	elif BX_PLATFORM_OSX || BX_PLATFORM_IOS
    return wmi.info.cocoa.window;
#	elif BX_PLATFORM_WINDOWS
    return wmi.info.win.window;
#   elif BX_PLATFORM_ANDROID
    return wmi.info.android.window;
#	endif // BX_PLATFORM_
}

inline bool sdlSetWindow(SDL_Window* _window)
{
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(_window, &wmi) )
    {
        return false;
    }

    bgfx::PlatformData pd;
#	if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    #		if ENTRY_CONFIG_USE_WAYLAND
		pd.ndt          = wmi.info.wl.display;
#		else
		pd.ndt          = wmi.info.x11.display;
#		endif
#	else
    pd.ndt          = NULL;
#	endif // BX_PLATFORM_
    pd.nwh          = sdlNativeWindowHandle(_window);

    pd.context      = NULL;
    pd.backBuffer   = NULL;
    pd.backBufferDS = NULL;
    bgfx::setPlatformData(pd);

    return true;
}

static void sdlDestroyWindow(SDL_Window* _window)
{
    if(!_window)
        return;
#	if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    #		if ENTRY_CONFIG_USE_WAYLAND
		wl_egl_window *win_impl = (wl_egl_window*)SDL_GetWindowData(_window, "wl_egl_window");
		if(win_impl)
		{
			SDL_SetWindowData(_window, "wl_egl_window", nullptr);
			wl_egl_window_destroy(win_impl);
		}
#		endif
#	endif
    SDL_DestroyWindow(_window);
}

static SDL_Window* window = nullptr;

bool initGraphics(uint32_t width, uint32_t height)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "Failed to initialize SDL" << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Game Engine Test",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          width, height, SDL_WindowFlags::SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cout << "Failed to create window" << std::endl;
        return false;
    }

    sdlSetWindow(window);

    bgfx::Init init = {};
    //init.type = bgfx::RendererType::OpenGL;
    init.resolution.width = width;
    init.resolution.height = height;
    init.resolution.reset = BGFX_RESET_VSYNC;
    bgfx::init(init);

    return true;
}

void gameLoop();

static Game* game;

int main(int argc, char* argv[])
{
    uint32_t width = 800, height = 600;
    if (!initGraphics(width, height))
        return 1;

    game = new Game(window, width, height);

    game->init();
    gameLoop();

    sdlDestroyWindow(window);

    return 0;
}

void gameLoop()
{
    static bool running = true;

    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }

        game->update();
        game->render();
    }
}
