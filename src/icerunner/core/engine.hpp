#ifndef ENGINE_H
#define ENGINE_H
#include <chrono>
#include <thread>
#include <cstdlib>
#include <iostream>
#include "constants.hpp"

namespace ice
{
namespace core
{


template <typename DisplayManager_,
          typename EventManager_,
          typename PhysicsManager_,
          typename SoundManager_,
          typename GameManager_>
class Engine
{
public:
    Engine(const typename DisplayManager_::Options& options)
        : displayManager_(options), gameManager_(displayManager_, eventManager_, physicsManager_, soundManager_)
    {}

    bool Init()
    {
        bool success = displayManager_.Init() &&
                       eventManager_.Init() &&
                       physicsManager_.Init() &&
                       soundManager_.Init() &&
                       gameManager_.Init();
        return success;
    }

    void Run()
    {
        using namespace std::chrono;

        high_resolution_clock::time_point prev = high_resolution_clock::now();
        double lag = constants::MS_PER_UPDATE;
        while (true)
        {
            high_resolution_clock::time_point current = high_resolution_clock::now();
            milliseconds elapsed = duration_cast<milliseconds>(current - prev);

            prev = current;
            lag += elapsed.count();

            while (lag >= constants::MS_PER_UPDATE)
            {
                eventManager_.Poll();
                gameManager_.Update();

                if (eventManager_.ShouldQuit() || gameManager_.ShouldQuit()) return;

                physicsManager_.Simulate();
                soundManager_.Update();

                lag -= constants::MS_PER_UPDATE;
            }
            gameManager_.Render();
        }
    }

    DisplayManager_& GetDisplayManager() { return displayManager_; }
    EventManager_& GetEventManager() { return eventManager_; }
    PhysicsManager_& GetPhysicsManager() { return physicsManager_; }
    SoundManager_& GetSoundManager() { return soundManager_; }

private:
    DisplayManager_ displayManager_;
    EventManager_ eventManager_;
    PhysicsManager_ physicsManager_;
    SoundManager_ soundManager_;
    GameManager_ gameManager_;
};

} // namespace core
} // namespace ice

#endif // ENGINE_H
