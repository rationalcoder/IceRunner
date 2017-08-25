#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP
#include "types.hpp"

namespace ice
{

namespace game
{

class GameState
{
protected:
    GameState(types::GameSystems& gameSystems)
        : gameSystems_(gameSystems)
    {}

public:
    virtual GameState* Update() = 0;
    virtual void Enter() = 0;
    virtual void Exit() = 0;
    virtual void Render() = 0;

protected:
    template <typename T_>
    void ReleasePointers(T_& container)
    {
        for(auto p : container)
        {
            delete p;
            p = nullptr;
        }
    }

protected:
    types::GameSystems gameSystems_;
};

} // namespace game

} // namespace ice

#endif // GAMESTATE_HPP
