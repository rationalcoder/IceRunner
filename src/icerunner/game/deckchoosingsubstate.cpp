#include "racingsubstates.hpp"

#include "events/eventmanager.h"

namespace ice
{
namespace game
{

RacingState::SubState& RacingState::DeckChoosingSubState::GetInstance(RacingState& racingState)
{
    static DeckChoosingSubState instance(racingState);
    return instance;
}

RacingState::DeckChoosingSubState::DeckChoosingSubState(RacingState& racingState)
    : SubState(racingState), deckLoader_(DeckLoader::GetInstance())
{}

void RacingState::DeckChoosingSubState::Enter()
{
    pNextGameState_ = nullptr;
    pNextSubState_ = nullptr;
    pPlayer1SelectedDeck_ = nullptr;
    pPlayer2SelectedDeck_ = nullptr;
    player1CurrentAssociation_ = 0;
    player2CurrentAssociation_ = 0;
    player1Ready_ = false;
    player2Ready_ = false;

    loadingFieldOnDescription_ = racingState_.hudResources_.GetLoadingRenderDescription();
    pLoadingField_  = racingState_.hudResources_.GetLoadingField();
    pLoadingField_->ClearRenderDescription();

    hadSuccessfulConfigure_ = deckLoader_.Configure();
    hadSuccessfulDeckLoading_ = deckLoader_.LoadDecks();
    if (!hadSuccessfulConfigure_)
        printf("Error in configuration: %s Fix the problem and reconfigure.\n", deckLoader_.GetError().c_str());
    if (!hadSuccessfulDeckLoading_)
        printf("Error in deck loading: %s Fix the problem and reload the decks.\n", deckLoader_.GetError().c_str());
    if (hadSuccessfulConfigure_ && hadSuccessfulDeckLoading_) CreateDeckResources();
    racingState_.resourceManager_.GetDJ().PlayMenu();
}

void RacingState::DeckChoosingSubState::Exit()
{
    ReleaseDeckResources();
}

RacingState::StatePair RacingState::DeckChoosingSubState::Update()
{
    const events::GameState& gameState = racingState_.gameSystems_.eventManager.GetEventState();
    const events::PlayerState& player1State = gameState.GetPlayerState(events::PlayerID::e1);
    const events::PlayerState& player2State = gameState.GetPlayerState(events::PlayerID::e2);

    // only player1 can back out.
    if (player1State.IsDown(events::MetaEvent::eBack))
        return StatePair(&MainMenuState::GetInstance(racingState_.gameSystems_));

    Menu& player1Menu = racingState_.hudResources_.GetPlayer1DeckChoosingMenu();
    Menu& player2Menu = racingState_.hudResources_.GetPlayer2DeckChoosingMenu();
    if (!player1Ready_) HandlePlayerMenuInput(player1State, player1Menu);
    if (!player2Ready_) HandlePlayerMenuInput(player2State, player2Menu);
    if (player1Ready_ && player2Ready_) pNextSubState_ = &RacingState::RacingSubState::GetInstance(racingState_);
    return StatePair(pNextGameState_, pNextSubState_);
}

void RacingState::DeckChoosingSubState::HandlePlayerMenuInput(const events::PlayerState& state, Menu& menu)
{
    sound::Effect& cycleEffect = *racingState_.resourceManager_.GetSoundEffect(ResourceManager::SoundEffect::eMenuCycle);
    if (state.IsDown(events::MovementEvent::eDown))
    {
        menu.FocusNext();
        cycleEffect.Play();
    }
    else if (state.IsDown(events::MovementEvent::eUp))
    {
        menu.FocusPrevious();
        cycleEffect.Play();
    }

    if (state.IsDown(events::MetaEvent::eSelect))
    {
        menu.Press();
        cycleEffect.Play();
    }
}

void RacingState::DeckChoosingSubState::Render()
{
    dx11rendering::DX11Renderer& renderer = racingState_.renderer_;

    renderer.SetViewPort(&racingState_.resourceManager_.GetFullScreenView());
    Background& background = racingState_.resourceManager_.GetBackground();
    background.Render();

    renderer.SetViewPort(&racingState_.hudResources_.GetPlayer1DeckChoosingMenuView());
    racingState_.hudResources_.GetPlayer1DeckChoosingMenu().Render();

    renderer.SetViewPort(&racingState_.hudResources_.GetPlayer2DeckChoosingMenuView());
    racingState_.hudResources_.GetPlayer2DeckChoosingMenu().Render();
}

//////////////////// PLAYER 1 ///////////////////////////

void RacingState::DeckChoosingSubState::ConfigureListener(void* pTestingState)
{
    DeckChoosingSubState* pState = (DeckChoosingSubState*)pTestingState;
    // If we are in the process of loading something, don't do anything.
    if (pState->configureFuture_.valid() && pState->configureFuture_.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) return;
    if (pState->deckLoadingFuture_.valid() && pState->deckLoadingFuture_.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) return;

    pState->ReleaseDeckResources();

    DeckLoader& loader = DeckLoader::GetInstance();
    pState->pLoadingField_->SetRenderDescription(pState->loadingFieldOnDescription_);
    pState->deckLoadingFuture_ = std::async(std::launch::async, [&loader, pState]() {
        bool result = loader.Configure();
        if (result) pState->CreateDeckResources();
        pState->pLoadingField_->ClearRenderDescription();
        pState->hadSuccessfulConfigure_ = result;
        return result;
    });
}

void RacingState::DeckChoosingSubState::LoadDecksListener(void* pTestingState)
{
    DeckChoosingSubState* pState = (DeckChoosingSubState*)pTestingState;
    // If we are in the process of loading something, don't do anything.
    if (pState->deckLoadingFuture_.valid() && pState->deckLoadingFuture_.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) return;
    if (pState->configureFuture_.valid() && pState->configureFuture_.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) return;

    pState->ReleaseDeckResources();

    DeckLoader& loader = DeckLoader::GetInstance();
    pState->pLoadingField_->SetRenderDescription(pState->loadingFieldOnDescription_);
    pState->deckLoadingFuture_ = std::async(std::launch::async, [&loader, pState]() {
        bool result = loader.LoadDecks();
        if (result) pState->CreateDeckResources();
        pState->pLoadingField_->ClearRenderDescription();
        pState->hadSuccessfulDeckLoading_ = result;
        return result;
    });
}

void RacingState::DeckChoosingSubState::Player1NextDeckListener(void* pTestingState)
{
    DeckChoosingSubState* pState = (DeckChoosingSubState*)pTestingState;
    if (pState->player1CurrentAssociation_ == -1)
        pState->racingState_.hudResources_.GetPlayer1CurrentDeckField()->ClearRenderDescription();

    if (pState->player1CurrentAssociation_ == pState->player1DeckSceneAssociations_.size()-1) pState->player1CurrentAssociation_ = 0;
    else pState->player1CurrentAssociation_++;

    DeckSceneAssociation& currentAssoc = pState->player1DeckSceneAssociations_[pState->player1CurrentAssociation_];
    pState->racingState_.hudResources_.GetPlayer1CurrentDeckField()->SetRenderDescription(currentAssoc.renderDescription);
    pState->racingState_.resourceManager_.GetSoundEffect(ResourceManager::SoundEffect::eMenuCycle)->Play();
}

void RacingState::DeckChoosingSubState::Player1PreviousDeckListener(void* pTestingState)
{
    DeckChoosingSubState* pState = (DeckChoosingSubState*)pTestingState;
    if (pState->player1CurrentAssociation_ == -1)
        pState->racingState_.hudResources_.GetPlayer1CurrentDeckField()->ClearRenderDescription();

    if (pState->player1CurrentAssociation_ == 0) pState->player1CurrentAssociation_ = (int)(pState->player1DeckSceneAssociations_.size()-1);
    else pState->player1CurrentAssociation_--;

    DeckSceneAssociation& currentAssoc = pState->player1DeckSceneAssociations_[pState->player1CurrentAssociation_];
    pState->racingState_.hudResources_.GetPlayer1CurrentDeckField()->SetRenderDescription(currentAssoc.renderDescription);
    pState->racingState_.resourceManager_.GetSoundEffect(ResourceManager::SoundEffect::eMenuCycle)->Play();
}

void RacingState::DeckChoosingSubState::Player1SelectDeckListener(void* pTestingState)
{
    DeckChoosingSubState* pState = (DeckChoosingSubState*)pTestingState;
    if (!pState->hadSuccessfulConfigure_ || (pState->configureFuture_.valid() && !pState->configureFuture_.get()))
    {
        printf("Unable to configure the deck loader: %s\n Please fix the error and reconfigure\n", DeckLoader::GetInstance().GetError().c_str());
        return;
    }

    if (!pState->hadSuccessfulDeckLoading_ || (pState->deckLoadingFuture_.valid() && !pState->deckLoadingFuture_.get()))
    {
        printf("Unable to load decks: %s\n Please fix the error and reload the decks\n", DeckLoader::GetInstance().GetError().c_str());
        return;
    }

    pState->pPlayer1SelectedDeck_ = pState->player1DeckSceneAssociations_[pState->player1CurrentAssociation_].pDeck;
    pState->player1Ready_ = true;
    pState->racingState_.resourceManager_.GetSoundEffect(ResourceManager::SoundEffect::eMenuSelect)->Play();
}

//////////////////// PLAYER 2 ///////////////////////////

void RacingState::DeckChoosingSubState::Player2NextDeckListener(void* pTestingState)
{
    DeckChoosingSubState* pState = (DeckChoosingSubState*)pTestingState;
    if (pState->player2CurrentAssociation_ == -1)
        pState->racingState_.hudResources_.GetPlayer2CurrentDeckField()->ClearRenderDescription();

    if (pState->player2CurrentAssociation_ == pState->player2DeckSceneAssociations_.size()-1) pState->player2CurrentAssociation_ = 0;
    else pState->player2CurrentAssociation_++;

    DeckSceneAssociation& currentAssoc = pState->player2DeckSceneAssociations_[pState->player2CurrentAssociation_];
    pState->racingState_.hudResources_.GetPlayer2CurrentDeckField()->SetRenderDescription(currentAssoc.renderDescription);
    pState->racingState_.resourceManager_.GetSoundEffect(ResourceManager::SoundEffect::eMenuCycle)->Play();
}

void RacingState::DeckChoosingSubState::Player2PreviousDeckListener(void* pTestingState)
{
    DeckChoosingSubState* pState = (DeckChoosingSubState*)pTestingState;
    if (pState->player2CurrentAssociation_ == -1)
        pState->racingState_.hudResources_.GetPlayer2CurrentDeckField()->ClearRenderDescription();

    if (pState->player2CurrentAssociation_ == 0) pState->player2CurrentAssociation_ = (int)(pState->player2DeckSceneAssociations_.size()-1);
    else pState->player2CurrentAssociation_--;

    DeckSceneAssociation& currentAssoc = pState->player2DeckSceneAssociations_[pState->player2CurrentAssociation_];
    pState->racingState_.hudResources_.GetPlayer2CurrentDeckField()->SetRenderDescription(currentAssoc.renderDescription);
    pState->racingState_.resourceManager_.GetSoundEffect(ResourceManager::SoundEffect::eMenuCycle)->Play();
}

void RacingState::DeckChoosingSubState::Player2SelectDeckListener(void* pTestingState)
{
    DeckChoosingSubState* pState = (DeckChoosingSubState*)pTestingState;
    if (!pState->hadSuccessfulConfigure_ || (pState->configureFuture_.valid() && !pState->configureFuture_.get()))
    {
        printf("Unable to configure the deck loader: %s\n Please fix the error and reconfigure\n", DeckLoader::GetInstance().GetError().c_str());
        return;
    }

    if (!pState->hadSuccessfulDeckLoading_ || (pState->deckLoadingFuture_.valid() && !pState->deckLoadingFuture_.get()))
    {
        printf("Unable to load decks: %s\n Please fix the error and reload the decks\n", DeckLoader::GetInstance().GetError().c_str());
        return;
    }

    pState->pPlayer2SelectedDeck_ = pState->player2DeckSceneAssociations_[pState->player2CurrentAssociation_].pDeck;
    pState->player2Ready_ = true;
    pState->racingState_.resourceManager_.GetSoundEffect(ResourceManager::SoundEffect::eMenuSelect)->Play();
}

bool RacingState::DeckChoosingSubState::CreateDeckResources()
{
    NormalizedRect currentDeckGeo;
    currentDeckGeo.topLeftPosition = glm::vec2(.2, -.03);
    currentDeckGeo.extent.y = .055f;

    DeckSceneAssociation assoc;
    deckLoader_.ForEachPlayer1Deck([&assoc, currentDeckGeo, this](Deck& deck)
    {
        assoc.pDeck = &deck;
        assoc.renderDescription = racingState_.hudResources_.GetPlayer1DeckChoosingMenu().MakeRenderDescription(deck.GetName().c_str(), currentDeckGeo);
        if (!assoc.renderDescription.IsValid())
        {
            printf("Failed to allocate resources for deck: %s. Skipping it.\n", deck.GetName().c_str());
            return;
        }
        player1DeckSceneAssociations_.push_back(assoc);
    });

    player1CurrentAssociation_ = -1;

    if (!player1DeckSceneAssociations_.empty())
    {
        DeckSceneAssociation& currentAssociation = player1DeckSceneAssociations_[0];
        racingState_.hudResources_.GetPlayer1CurrentDeckField()->SetRenderDescription(currentAssociation.renderDescription);
        player1CurrentAssociation_ = 0;
    }

    deckLoader_.ForEachPlayer2Deck([&assoc, currentDeckGeo, this](Deck& deck)
    {
        assoc.pDeck = &deck;
        assoc.renderDescription = racingState_.hudResources_.GetPlayer2DeckChoosingMenu().MakeRenderDescription(deck.GetName().c_str(), currentDeckGeo);
        if (!assoc.renderDescription.IsValid())
        {
            printf("Failed to allocate resources for deck: %s. Skipping it.\n", deck.GetName().c_str());
            return;
        }
        player2DeckSceneAssociations_.push_back(assoc);
    });

    player2CurrentAssociation_ = -1;

    if (!player2DeckSceneAssociations_.empty())
    {
        DeckSceneAssociation& currentAssociation = player2DeckSceneAssociations_[0];
        racingState_.hudResources_.GetPlayer2CurrentDeckField()->SetRenderDescription(currentAssociation.renderDescription);
        player2CurrentAssociation_ = 0;
    }

    return true;
}

void RacingState::DeckChoosingSubState::ReleaseDeckResources()
{
    racingState_.hudResources_.GetPlayer1CurrentDeckField()->ClearRenderDescription();
    racingState_.hudResources_.GetPlayer2CurrentDeckField()->ClearRenderDescription();
    std::for_each(player1DeckSceneAssociations_.begin(), player1DeckSceneAssociations_.end(), [](DeckSceneAssociation& assoc) {
        assoc.DeleteAndNull();
    });
    player1DeckSceneAssociations_.clear();

    std::for_each(player2DeckSceneAssociations_.begin(), player2DeckSceneAssociations_.end(), [](DeckSceneAssociation& assoc) {
        assoc.DeleteAndNull();
    });
    player2DeckSceneAssociations_.clear();
}

} // namespace game
} // namespace ice
