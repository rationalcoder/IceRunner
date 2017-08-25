#include <iostream>
#include <cstdlib>
#include <thread>
#include <windows.h>

#include "events/eventmanager.h"
#include "rendering/displaymanager.h"
#include "physics/physicsmanager.h"
#include "sound/soundmanager.h"
#include "game/gamemanager.h"

#include "core/engine.hpp"

int main()
{
    using namespace ice::core;
    using namespace ice::events;
    using namespace ice::rendering;
    using namespace ice::game;
    using namespace ice::physics;
    using namespace ice::sound;

    DisplayManager::Options options;
    options.screenWidth = 1280;
    options.screenHeight = 720;
    options.pTitle = "Ice Runner";

    using Engine = Engine<DisplayManager, EventManager, PhysicsManager,
                          SoundManager, GameManager>;
    Engine engine(options);
    
    if(!engine.Init())
    {
        printf("Error in game initialization\n");
        return EXIT_FAILURE;
    }
    engine.Run();

    return EXIT_SUCCESS;
}
