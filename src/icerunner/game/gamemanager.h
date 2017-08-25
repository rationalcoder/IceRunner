#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include <glm/vec3.hpp>
#include "types.hpp"
#include "game.h"

namespace ice 
{

namespace game
{

class GameManager
{
public:
    GameManager(rendering::DisplayManager& displayManager,
                events::EventManager& eventManager,
                physics::PhysicsManager& physicsManager,
                sound::SoundManager& soundManager)
        : gameSystems_(displayManager, eventManager, physicsManager, soundManager),
          game_(gameSystems_),
          targetColorIndex_()
    {}

    ~GameManager();

    bool Init();
    void Update();
    bool ShouldQuit();
    void Render();

private:
    types::GameSystems gameSystems_;
    Game game_;
    glm::vec3 currentColor_;
    int targetColorIndex_;
};

} // namespace game

} // namespace ice

#endif // GAMEMANAGER_H
