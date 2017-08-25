#ifndef RACINGSUBSTATES_HPP
#define RACINGSUBSTATES_HPP
#include "gamestates.hpp"
#include "events/eventfwd.hpp"

namespace ice
{
namespace game
{

class RacingState::SubState
{
public:
    SubState(RacingState& racingState)
        : racingState_(racingState)
    {}

    virtual void Enter() = 0;
    virtual void Exit() = 0;
    virtual RacingState::StatePair Update() = 0;
    virtual void Render() = 0;

protected:
    RacingState& racingState_;
};

struct RacingState::StatePair
{
    StatePair(GameState* pNextGameState = nullptr, RacingState::SubState* pNextSubState = nullptr)
        : pNextGameState(pNextGameState), pNextSubState(pNextSubState)
    {}

    GameState* pNextGameState;
    RacingState::SubState* pNextSubState;
};

class RacingState::DeckChoosingSubState final : public RacingState::SubState
{
public:
    static RacingState::SubState& GetInstance(RacingState& racingState);
    virtual void Enter() override;
    virtual void Exit() override;
    virtual RacingState::StatePair Update() override;
    virtual void Render() override;

    Deck* GetPlayer1SelectedDeck() { return pPlayer1SelectedDeck_; }
    Deck* GetPlayer2SelectedDeck() { return pPlayer2SelectedDeck_; }

public:
    static void ConfigureListener(void* pState);
    static void LoadDecksListener(void* pState);
    static void Player1NextDeckListener(void* pState);
    static void Player1PreviousDeckListener(void* pState);
    static void Player1SelectDeckListener(void* pState);
    static void Player2NextDeckListener(void* pState);
    static void Player2PreviousDeckListener(void* pState);
    static void Player2SelectDeckListener(void* pState);

private:
    DeckChoosingSubState(RacingState& racingState);
    void HandlePlayerMenuInput(const events::PlayerState& playerState, Menu& menu);
    bool CreateDeckResources();
    void ReleaseDeckResources();

private:
    DeckLoader& deckLoader_;
    GameState* pNextGameState_;
    SubState* pNextSubState_;
    Deck* pPlayer1SelectedDeck_;
    Deck* pPlayer2SelectedDeck_;
    std::vector<DeckSceneAssociation> player1DeckSceneAssociations_;
    std::vector<DeckSceneAssociation> player2DeckSceneAssociations_;
    int player1CurrentAssociation_;
    int player2CurrentAssociation_;

    bool player1Ready_;
    bool player2Ready_;

    std::future<bool> configureFuture_;
    bool hadSuccessfulConfigure_;
    std::future<bool> deckLoadingFuture_;
    bool hadSuccessfulDeckLoading_;

    Menu::Field* pLoadingField_;
    Menu::ElementRenderDescription loadingFieldOnDescription_;
};

class RacingState::RacingSubState final : public RacingState::SubState
{
public:
    enum class Winner
    {
        ePlayer1,
        ePlayer2,
        eNum_
    };

public:
    static RacingState::SubState& GetInstance(RacingState& racingState);
    virtual void Enter() override;
    virtual void Exit() override;
    virtual RacingState::StatePair Update() override;
    virtual void Render() override;

    Winner GetWinner() const { return winner_; }

private:
    RacingSubState(RacingState& racingState);
    ~RacingSubState() = default;

    void UpdatePlayer1();
    void UpdatePlayer2();
    void LoadCurrentPlayer1Map();
    void LoadCurrentPlayer2Map();

private:
    std::array<Menu::ElementRenderDescription, 7> countDownRenderDescriptions_;
    MapRenderer::Scene player1RenderScene_;
    MapRenderer::Scene player2RenderScene_;
    MapUtility::SceneInfo player1SceneInfo_;
    MapUtility::SceneInfo player2SceneInfo_;
    physics::Scene* pPlayer1PhysicsScene_;
    physics::Scene* pPlayer2PhysicsScene_;
    Deck* pPlayer1Deck_;
    Deck* pPlayer2Deck_;
    Map* pPlayer1CurrentMap_;
    Map* pPlayer2CurrentMap_;
    int player1CurrentMap_;
    int player2CurrentMap_;
    int player1DeckSize_;
    int player2DeckSize_;
    Winner winner_;

    int currentCount_;
    int frameCount_;
    bool countingDown_;

    RacingState::GameState* pNextGameState_;
    RacingState::SubState* pNextSubState_;
    physics::PhysicsManager& physicsManager_;
    MapRenderer& mapRenderer_;

    Menu::Field* pPlayer1CurrentMapField_;
    Menu::ElementRenderDescription player1CurrentMapDescription_;
    Menu::Field* pPlayer2CurrentMapField_;
    Menu::ElementRenderDescription player2CurrentMapDescription_;

	sound::Effect* tone_;
	sound::Effect* go_;
};

class RacingState::ResultsSubState final : public RacingState::SubState
{
public:
    static RacingState::SubState& GetInstance(RacingState& racingState);
    virtual void Enter() override;
    virtual void Exit() override;
    virtual RacingState::StatePair Update() override;
    virtual void Render() override;

private:
    ResultsSubState(RacingState& racingState);

private:
    Menu* pResultMenu_;
	NormalizedRect menuGeometry_;

	RacingState::RacingSubState& racingSubState_;
};

} // namespace game
} // namespace ice

#endif // RACINGSUBSTATES_HPP
