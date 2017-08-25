#include "events/eventmanager.h"
#include "deckloader.h"
#include "physics/physicsmanager.h"
#include "racingsubstates.hpp"

#include "core/constants.hpp"

// FIXME: hack because MSVC is borderline retarded.
namespace ice
{
namespace game
{
namespace constants
{

constexpr float MENU_BACKGROUND_SPEED = 16.0f;
constexpr float GAMEPLAY_BACKGROUND_SPEED = 24.0f;
constexpr float PLAYER_SPEED = .01f;
constexpr float HALF_MAP_RATIO = 640/720.0f;

} // namespace constants
} // namespace game
} // namespace ice

namespace ice
{
namespace game
{

constexpr int FRAMES_PER_COUNT = (int)(1000/ice::core::constants::MS_PER_UPDATE);

RacingState::SubState& RacingState::RacingSubState::GetInstance(RacingState& racingState)
{
    static RacingSubState instance(racingState);
    return instance;
}

RacingState::RacingSubState::RacingSubState(RacingState& racingState)
    : SubState(racingState), currentCount_(0), frameCount_(0), physicsManager_(racingState.gameSystems_.physicsManager),
      mapRenderer_(MapRenderer::GetInstance(racingState_.gameSystems_))
{
    ResourceManager::RacingResources::HUDResources& hudResources = racingState_.hudResources_;

    countDownRenderDescriptions_[3] = hudResources.GetCountDown3RenderDescription();
    countDownRenderDescriptions_[4] = hudResources.GetCountDown2RenderDescription();
    countDownRenderDescriptions_[5] = hudResources.GetCountDown1RenderDescription();
    countDownRenderDescriptions_[6] = hudResources.GetCountDownGoRenderDescription();
}

void RacingState::RacingSubState::Enter()
{
    countingDown_ = true;
    currentCount_ = 0;
    frameCount_ = 0;

    RacingState::DeckChoosingSubState& deckChoosingState = (RacingState::DeckChoosingSubState&)RacingState::DeckChoosingSubState::GetInstance(racingState_);
    pPlayer1Deck_ = deckChoosingState.GetPlayer1SelectedDeck();
    pPlayer2Deck_ = deckChoosingState.GetPlayer2SelectedDeck();
    player1DeckSize_ = (int)pPlayer1Deck_->GetMaxDifficulty();
    player2DeckSize_ = (int)pPlayer2Deck_->GetMaxDifficulty();
    player1CurrentMap_ = 1;
    player2CurrentMap_ = 1;
    winner_ = Winner::eNum_;
    pNextGameState_ = nullptr;
    pNextSubState_ = nullptr;
    pPlayer1PhysicsScene_ = &physicsManager_.SceneAt(racingState_.player1Resources_.GetSceneIndex());
    pPlayer2PhysicsScene_ = &physicsManager_.SceneAt(racingState_.player2Resources_.GetSceneIndex());

    pPlayer1CurrentMapField_ = racingState_.hudResources_.GetPlayer1CurrentMapField();
    pPlayer1CurrentMapField_->ClearRenderDescription();
    pPlayer2CurrentMapField_ = racingState_.hudResources_.GetPlayer2CurrentMapField();
    pPlayer2CurrentMapField_->ClearRenderDescription();

    LoadCurrentPlayer1Map();
    LoadCurrentPlayer2Map();

    racingState_.background_.SetAnimationSpeed(constants::GAMEPLAY_BACKGROUND_SPEED);

	tone_ = racingState_.resourceManager_.GetSoundEffect(ResourceManager::SoundEffect::eCountDownTone);
	go_ = racingState_.resourceManager_.GetSoundEffect(ResourceManager::SoundEffect::eGoTone);

    racingState_.resourceManager_.GetDJ().PlayAction();
}

void RacingState::RacingSubState::Exit()
{
    mapRenderer_.FreeScene(player1RenderScene_);
    mapRenderer_.FreeScene(player2RenderScene_);
    player1SceneInfo_.wallPositions.clear();
    player2SceneInfo_.wallPositions.clear();
    player1CurrentMapDescription_.DeleteAndNull();
    player2CurrentMapDescription_.DeleteAndNull();
}

RacingState::StatePair RacingState::RacingSubState::Update()
{
    if (countingDown_)
    {
        if (currentCount_ == countDownRenderDescriptions_.size())
        {
            currentCount_ = 0;
            countingDown_ = false;

        }
        else
        {
            if (frameCount_++ == FRAMES_PER_COUNT)
            {
                currentCount_++;
                frameCount_ = 0;

				switch (currentCount_)
				{
				case 3:
				case 4:
				case 5:
					tone_->Play();
					break;
				case 6:
					go_->Play();
					break;
				default:
					break;
				}
            }
        }
    }

    const events::GameState& gameState = racingState_.gameSystems_.eventManager.GetEventState();
    const events::PlayerState& player1State = gameState.GetPlayerState(events::PlayerID::e1);

    if (player1State.IsDown(events::MetaEvent::eBack))
        return StatePair(nullptr, &RacingState::DeckChoosingSubState::GetInstance(racingState_));

    UpdatePlayer1();
    // Only update player 2 if player1 didn't win this frame.
    if (winner_ == Winner::eNum_) UpdatePlayer2();

    return StatePair(pNextGameState_, pNextSubState_);
}

void RacingState::RacingSubState::UpdatePlayer1()
{
    if (countingDown_ && currentCount_ != countDownRenderDescriptions_.size()-1) return;

    if (pPlayer1PhysicsScene_->HitGoal())
    {
        racingState_.resourceManager_.GetSoundEffect(ResourceManager::SoundEffect::eGoal)->Play();
        if (player1CurrentMap_ == player1DeckSize_)
        {
            winner_ = Winner::ePlayer1;
            pNextSubState_ = &RacingState::ResultsSubState::GetInstance(racingState_);
            return;
        }
        player1CurrentMap_++;
        LoadCurrentPlayer1Map();
    }

    physics::Player& player = pPlayer1PhysicsScene_->GetPlayer();

    player1RenderScene_.SetPlayer(player.boundingBox.GetMin());
    if (!player.IsStopped()) return;

    if (!player.WasStopped())
        racingState_.resourceManager_.GetSoundEffect(ResourceManager::SoundEffect::eBump)->Play();

    const events::PlayerState& playerState = racingState_.gameSystems_.eventManager.GetEventState().GetPlayerState(events::PlayerID::e1);

    if (playerState.IsDown(events::MetaEvent::eReset)) pPlayer1PhysicsScene_->Reset();

    if (playerState.GetNumMovementEventsDown() > 1) return;
    else if (playerState.WasDown(events::MovementEvent::eRight)) player.velocity = glm::vec2(constants::PLAYER_SPEED, 0);
    else if (playerState.WasDown(events::MovementEvent::eLeft)) player.velocity = glm::vec2(-constants::PLAYER_SPEED, 0);
    else if (playerState.WasDown(events::MovementEvent::eUp)) player.velocity = glm::vec2(0, -constants::PLAYER_SPEED);
    else if (playerState.WasDown(events::MovementEvent::eDown)) player.velocity = glm::vec2(0, constants::PLAYER_SPEED);
}

void RacingState::RacingSubState::UpdatePlayer2()
{
    if (countingDown_ && currentCount_ != countDownRenderDescriptions_.size()-1) return;
    if (pPlayer2PhysicsScene_->HitGoal())
    {
        racingState_.resourceManager_.GetSoundEffect(ResourceManager::SoundEffect::eGoal)->Play();
        if (player2CurrentMap_ == player2DeckSize_)
        {
            winner_ = Winner::ePlayer2;
            pNextSubState_ = &RacingState::ResultsSubState::GetInstance(racingState_);
            return;
        }
        player2CurrentMap_++;
        LoadCurrentPlayer2Map();
    }

    physics::Player& player = pPlayer2PhysicsScene_->GetPlayer();

    player2RenderScene_.SetPlayer(player.boundingBox.GetMin());
    if (!player.IsStopped()) return;

    if (!player.WasStopped())
        racingState_.resourceManager_.GetSoundEffect(ResourceManager::SoundEffect::eBump)->Play();

    const events::PlayerState& playerState = racingState_.gameSystems_.eventManager.GetEventState().GetPlayerState(events::PlayerID::e2);

    if (playerState.IsDown(events::MetaEvent::eReset)) pPlayer2PhysicsScene_->Reset();

    if (playerState.GetNumMovementEventsDown() > 1) return;
    else if (playerState.WasDown(events::MovementEvent::eRight)) player.velocity = glm::vec2(constants::PLAYER_SPEED, 0);
    else if (playerState.WasDown(events::MovementEvent::eLeft)) player.velocity = glm::vec2(-constants::PLAYER_SPEED, 0);
    else if (playerState.WasDown(events::MovementEvent::eUp)) player.velocity = glm::vec2(0, -constants::PLAYER_SPEED);
    else if (playerState.WasDown(events::MovementEvent::eDown)) player.velocity = glm::vec2(0, constants::PLAYER_SPEED);
}

void RacingState::RacingSubState::LoadCurrentPlayer1Map()
{
    mapRenderer_.FreeScene(player1RenderScene_);
    player1CurrentMapDescription_.DeleteAndNull();
    pPlayer1CurrentMap_ = &pPlayer1Deck_->MapAt(Map::Difficulty(player1CurrentMap_));
    NormalizedRect geo;
    geo.topLeftPosition = glm::vec2(.25, -.6);
    geo.extent = glm::vec2(0, .3);
    player1CurrentMapDescription_ = racingState_.hudResources_.GetPlayer1HUD().MakeRenderDescription(pPlayer1CurrentMap_->GetDescription().name.c_str(), geo,
                                                                                                     racingState_.hudResources_.GetPlayer1HUDView());
    pPlayer1CurrentMapField_->SetRenderDescription(player1CurrentMapDescription_);

    MapUtility::GetSceneInfo(*pPlayer1CurrentMap_, player1SceneInfo_);
    MapUtility::MakePhysicsScene(player1SceneInfo_, *pPlayer1PhysicsScene_);
    mapRenderer_.MakePlayer1Scene(player1SceneInfo_, player1RenderScene_);
    pPlayer1PhysicsScene_->Unpause();
}

void RacingState::RacingSubState::LoadCurrentPlayer2Map()
{
    mapRenderer_.FreeScene(player2RenderScene_);
    player2CurrentMapDescription_.DeleteAndNull();
    pPlayer2CurrentMap_ = &pPlayer2Deck_->MapAt(Map::Difficulty(player2CurrentMap_));

    NormalizedRect geo;
    geo.topLeftPosition = glm::vec2(.25, -.6);
    geo.extent = glm::vec2(0, .3);
    player2CurrentMapDescription_ = racingState_.hudResources_.GetPlayer2HUD().MakeRenderDescription(pPlayer2CurrentMap_->GetDescription().name.c_str(), geo,
                                                                                                     racingState_.hudResources_.GetPlayer2HUDView());
    pPlayer2CurrentMapField_->SetRenderDescription(player2CurrentMapDescription_);

    MapUtility::GetSceneInfo(*pPlayer2CurrentMap_, player2SceneInfo_);
    MapUtility::MakePhysicsScene(player2SceneInfo_, *pPlayer2PhysicsScene_);
    mapRenderer_.MakePlayer2Scene(player2SceneInfo_, player2RenderScene_);
    pPlayer2PhysicsScene_->Unpause();
}

void RacingState::RacingSubState::Render()
{
    ResourceManager::RacingResources::HUDResources& hudResources = racingState_.hudResources_;

    D3D11_VIEWPORT player1HUDView = hudResources.GetPlayer1HUDView();
    racingState_.renderer_.SetViewPort(&player1HUDView);
    hudResources.GetPlayer1HUD().Render();

    D3D11_VIEWPORT player2HUDView = hudResources.GetPlayer2HUDView();
    racingState_.renderer_.SetViewPort(&player2HUDView);
    hudResources.GetPlayer2HUD().Render();

    D3D11_VIEWPORT countDownHUDView = hudResources.GetCountDownHUDView();
    racingState_.renderer_.SetViewPort(&countDownHUDView);

    if (countingDown_ && currentCount_ < countDownRenderDescriptions_.size()) // FIXME: hack
        hudResources.GetCountDownField()->SetRenderDescription(countDownRenderDescriptions_[currentCount_]);
    else
        hudResources.GetCountDownField()->SetRenderDescription(countDownRenderDescriptions_.back());
    hudResources.GetCountDownHUD().Render();

    MapRenderer& mapRenderer = MapRenderer::GetInstance(racingState_.gameSystems_);

    racingState_.renderer_.SetViewPort(&player1RenderScene_.GetView());
    racingState_.background_.SetColor(player1SceneInfo_.mapColor);
    racingState_.background_.Render();
    mapRenderer.RenderScene(player1RenderScene_);

    racingState_.renderer_.SetViewPort(&player2RenderScene_.GetView());
    racingState_.background_.SetColor(player2SceneInfo_.mapColor);
    racingState_.background_.Render(false);
    mapRenderer.RenderScene(player2RenderScene_);
}

} // namespace game
} // namespace ice
