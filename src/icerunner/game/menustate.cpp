#include "gamestates.hpp"

namespace ice
{

namespace game
{

GameState& MenuState::GetInstance(types::GameSystems& gameSystems)
{
    static MenuState menuState(gameSystems);
    return menuState;
}

void MenuState::Enter()
{
}

void MenuState::Exit()
{
}

GameState* MenuState::Update()
{
    return &MapTestingState::GetInstance(gameSystems_);
}

void MenuState::Render()
{
}

} // namespace game

} // namespace ice
