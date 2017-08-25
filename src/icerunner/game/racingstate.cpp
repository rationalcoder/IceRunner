#include "racingsubstates.hpp"

#include "core/constants.hpp"
#include "rendering/displaymanager.h"
#include "events/eventmanager.h"

namespace ice
{
namespace game
{

GameState& RacingState::GetInstance(types::GameSystems& gameSystems)
{
    static RacingState racingState(gameSystems);
    return racingState;
}

RacingState::RacingState(types::GameSystems& gameSystems)
    : GameState(gameSystems), resourceManager_(ResourceManager::GetInstance(gameSystems)),
      resources_(resourceManager_.GetRacingResources()), pNextState_(nullptr),
      hudResources_(resources_.GetHUDResources()), player1Resources_(resources_.GetPlayer1Resources()),
      player2Resources_(resources_.GetPlayer2Resources()), resultsResources_(resources_.GetResultsResources()),
      pCycleEffect_(resourceManager_.GetSoundEffect(ResourceManager::SoundEffect::eMenuCycle)),
      pSelectEffect_(resourceManager_.GetSoundEffect(ResourceManager::SoundEffect::eMenuSelect)),
      background_(resourceManager_.GetBackground()), renderer_(gameSystems.displayManager.GetRenderer())
{}

void RacingState::Enter()
{
    pNextState_ = nullptr;
    pSubState_ = &DeckChoosingSubState::GetInstance(*this);
    pSubState_->Enter();
}

void RacingState::Exit()
{
    pSubState_->Exit();
}

GameState* RacingState::Update()
{
    StatePair statePair = pSubState_->Update();
    if (statePair.pNextGameState)
    {
        pSubState_->Exit();
        return statePair.pNextGameState;
    }
    if (statePair.pNextSubState)
    {
        pSubState_->Exit();
        pSubState_ = statePair.pNextSubState;
        pSubState_->Enter();
    }

    return nullptr;
}

void RacingState::Render()
{
    pSubState_->Render();
}

} // namespace game
} // namespace ice
