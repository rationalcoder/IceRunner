#include "gamestates.hpp"
#include "events/eventmanager.h"
#include "sound/soundstructs.hpp"

namespace ice
{

namespace game
{

GameState& MainMenuState::GetInstance(types::GameSystems& gameSystems)
{
    static MainMenuState menuState(gameSystems);
    return menuState;
}

MainMenuState::MainMenuState(types::GameSystems& gameSystems)
    : GameState(gameSystems), resourceManager_(ResourceManager::GetInstance(gameSystems)),
      renderer_(gameSystems.displayManager.GetRenderer()),
      menu_(resourceManager_.GetMainMenuResources().GetMenu()),
      pNextState_(nullptr),
      pCycleEffect_(resourceManager_.GetSoundEffect(ResourceManager::SoundEffect::eMenuCycle)),
      pSelectEffect_(resourceManager_.GetSoundEffect(ResourceManager::SoundEffect::eMenuSelect)),
      background_(resourceManager_.GetBackground())
{}

void MainMenuState::Enter()
{
    background_.SetColor(glm::vec3(0, .85, 1));
    pNextState_ = nullptr;
    resourceManager_.GetDJ().PlayMenu();
}

void MainMenuState::Exit()
{
}

GameState* MainMenuState::Update()
{
    const events::GameState& state = gameSystems_.eventManager.GetEventState();
    const events::PlayerState& player1State = state.GetPlayerState(events::PlayerID::e1);

    if (player1State.IsDown(events::MetaEvent::eSelect))
    {
        menu_.Press();
        pCycleEffect_->Play();
    }
    else if (player1State.IsDown(events::MovementEvent::eDown))
    {
        menu_.FocusNext();
        pCycleEffect_->Play();
    }
    else if (player1State.IsDown(events::MovementEvent::eUp))
    {
        menu_.FocusNext();
        pCycleEffect_->Play();
    }

    return pNextState_;
}

void MainMenuState::Render()
{
    D3D11_VIEWPORT menuView;
    menuView.Width = 1240;
    menuView.Height = 680;
    menuView.MaxDepth = 0;
    menuView.MinDepth = 0;
    menuView.TopLeftX = (1280-1240)/2;
    menuView.TopLeftY = (720-680)/2;

    renderer_.SetViewPort(&resourceManager_.GetFullScreenView());;
    background_.Render();

    renderer_.SetViewPort(&menuView);;
    menu_.Render();
}

void MainMenuState::RaceListener(void* pState)
{
    MainMenuState* pSelf = (MainMenuState*)pState;
    pSelf->pNextState_ = &RacingState::GetInstance(pSelf->gameSystems_);
    pSelf->pSelectEffect_->Play();
}

void MainMenuState::TestMapListener(void* pState)
{
    MainMenuState* pSelf = (MainMenuState*)pState;
    pSelf->pNextState_ = &MapTestingState::GetInstance(pSelf->gameSystems_);
    pSelf->pSelectEffect_->Play();
}

} // namespace game

} // namespace ice
