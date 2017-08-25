#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H
#include <glm/vec3.hpp>
#include "dx11renderer.h"

union SDL_Event;
struct SDL_Window;

namespace ice
{

namespace rendering
{

class DisplayManager
{
public:
    struct Options
    {
        const char* pTitle;
        size_t screenWidth;
        size_t screenHeight;
    };

public:
    DisplayManager(const Options& options)
        : options_(options)
    {}

public:
    bool init();
    void handle_window_events(SDL_Event* pWindowEvents, unsigned numEvents);
    dx11rendering::DX11Renderer& renderer() { return renderer_; }
    Options options() const { return options_; }
    void show_window();

private:
    dx11rendering::DX11Renderer renderer_;
    Options options_;
    SDL_Window* pWindow_;
};

} // namespace rendering

} // namespace ice

#endif // DISPLAYMANAGER_H
