#ifndef GAME_H
#define GAME_H
#include "types.hpp"
#include "gamestates.hpp"

namespace ice
{

namespace game
{

class Game
{
public:
    Game(types::GameSystems gameSystems)
        : gameSystems_(gameSystems), state_(nullptr), shouldEnter_(false)
    {}

    bool Init()
    {
        state_ = &MainMenuState::GetInstance(gameSystems_);
        state_->Enter();
        return true;
    }

    void Update()
    {
        GameState* pNewState = state_->Update();
        if (pNewState)
        {
            state_->Exit();
            pNewState->Enter();
            state_ = pNewState;
        }
    }

    bool ShouldQuit()
    {
        return false;
    }

    void Render()
    {
        state_->Render();
    }

private:
    types::GameSystems gameSystems_;
    GameState* state_;
    bool shouldEnter_;
};

} // namespace game

} // namespace ice

#endif // GAME_H
