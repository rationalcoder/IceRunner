#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include "displaymanager.h"

namespace ice
{

namespace rendering
{

bool DisplayManager::Init()
{
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) goto error;

    pWindow_ = SDL_CreateWindow(options_.pTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                (int)options_.screenWidth, (int)options_.screenHeight, SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE);

    if (!pWindow_) goto close_and_error;

    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    if (SDL_GetWindowWMInfo(pWindow_, &info) != SDL_TRUE) goto close_and_error;

    if (!renderer_.Init(info.info.win.window, options_.screenWidth, options_.screenHeight)) goto close_and_error;

    return true;

close_and_error:
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
error:
    printf("Failed to init the display manager: %s\n", SDL_GetError());
    return false;
}


void DisplayManager::ShowWindow()
{
    SDL_ShowWindow(pWindow_);
}

} // namespace rendering

} // namespace ice
