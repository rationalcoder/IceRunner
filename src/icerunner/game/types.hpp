#ifndef TYPES_HPP
#define TYPES_HPP

// forward declarations
namespace ice
{

namespace rendering
{
    class DisplayManager;
} // rendering

namespace physics
{
    class PhysicsManager;
} // pysics

namespace events
{
    class EventManager;
} // pysics

namespace sound
{
    class SoundManager;
} // pysics

} // namespace ice


namespace ice
{

namespace game
{

namespace types
{

struct GameSystems
{
    GameSystems(rendering::DisplayManager& displayManager,
               events::EventManager& eventManager,
               physics::PhysicsManager& physicsManager,
               sound::SoundManager& soundManager)
        : displayManager(displayManager),
          physicsManager(physicsManager),
          eventManager(eventManager),
          soundManager(soundManager)
    {}

    rendering::DisplayManager& displayManager;
    events::EventManager& eventManager;
    physics::PhysicsManager& physicsManager;
    sound::SoundManager& soundManager;
};

} // namespace types

} // namespace core

} // namespace ice

#endif // TYPES_HPP
