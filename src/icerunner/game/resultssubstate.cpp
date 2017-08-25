#include "racingsubstates.hpp"
#include "constants.hpp"
#include "events\eventmanager.h"

#include "glm/vec2.hpp"

namespace ice
{
namespace game
{

RacingState::SubState& RacingState::ResultsSubState::GetInstance(RacingState& racingState)
{
    static ResultsSubState instance(racingState);
    return instance;
}

RacingState::ResultsSubState::ResultsSubState(RacingState& racingState)
    : SubState(racingState), racingSubState_((RacingState::RacingSubState&)RacingState::RacingSubState::GetInstance(racingState))
{
	menuGeometry_.topLeftPosition = glm::vec2(-.75, .2);
	menuGeometry_.extent = glm::vec2(0, .14);
}

void RacingState::ResultsSubState::Enter()
{
    pResultMenu_ = racingSubState_.GetWinner() == RacingState::RacingSubState::Winner::ePlayer1 ? &racingState_.resultsResources_.GetPlayer1WinsMenu() :
                                                                                                  &racingState_.resultsResources_.GetPlayer2WinsMenu();
	racingState_.background_.SetAnimationSpeed(constants::MENU_BACKGROUND_SPEED);
}

void RacingState::ResultsSubState::Exit()
{
}

RacingState::StatePair RacingState::ResultsSubState::Update()
{
	const events::PlayerState& player1State = racingState_.gameSystems_.eventManager.GetEventState().GetPlayerState(events::PlayerID::e1);

	if (player1State.IsDown(events::MetaEvent::eBack)) 
		return StatePair(&MainMenuState::GetInstance(racingState_.gameSystems_));

	return StatePair();
}

void RacingState::ResultsSubState::Render()
{
	D3D11_VIEWPORT view = {};
	view.Width = 1240;
	view.Height = 680;
	view.TopLeftX = 20;
	view.TopLeftY = 20;

	racingState_.renderer_.SetViewPort(&racingState_.resourceManager_.GetFullScreenView());

	racingState_.background_.SetColor(glm::vec3(0.0f, .5f, 1.0f));
	racingState_.background_.Render();

	racingState_.renderer_.SetViewPort(&view);
    pResultMenu_->Render();
}

} // namespace game
} // namespace ice
