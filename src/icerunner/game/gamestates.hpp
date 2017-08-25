#ifndef GAMESTATES_HPP
#define GAMESTATES_HPP
#include <cstdio>
#include <vector>
#include <array>
#include <future>
#include "gamestate.hpp"
#include "gamerenderstructs.hpp"
#include "maprenderer.h"
#include "resourcemanager.h"

class DeckLoader;

namespace ice
{

namespace game
{

class MainMenuState final : public GameState
{
    friend class ResourceManager;
protected:
    using GameState::GameState;

public:
    static GameState& GetInstance(types::GameSystems& gameSystems);

    virtual void Enter() override;
    virtual void Exit() override;
    virtual GameState* Update() override;
    virtual void Render() override;

private:
    MainMenuState(types::GameSystems& gameSystems_);

private:
    static void RaceListener(void* pState);
    static void TestMapListener(void* pState);

private:
    ResourceManager& resourceManager_;
    dx11rendering::DX11Renderer& renderer_;
    Menu& menu_;
    GameState* pNextState_;
    sound::Effect* pCycleEffect_;
    sound::Effect* pSelectEffect_;
    Background& background_;
};


class MapTestingState final : public GameState
{
    friend class ResourceManager;
public:
    static GameState& GetInstance(types::GameSystems& gameSystems);
    virtual void Enter() override;
    virtual void Exit() override;
    virtual GameState* Update() override;
    virtual void Render() override;
    
private:
    MapTestingState(types::GameSystems& gameSystems);
    
private:
    GameState* UpdateInMenu();
    void UpdateOutOfMenu();
    void RenderInMenu();
    void RenderOutOfMenu();
    void RenderFullBackground();
    void RenderMapBackground();

    bool CreateMapResources();
    void ReleaseMapResources();
    void LoadPhysicsScene();
    void ReleasePhysicsScene();

    // Action listeners
    static void NextMapListener(void* pTestingState);
    static void PreviousMapListener(void* pTestingState);
    static void TestMapListener(void* pTestingState);
    static void ConfigureListener(void* pTestingState);
    static void LoadMapsListener(void* pTestingState);

private:
    bool inMenu_;
    Menu* pMenu_;
    std::future<bool> configureFuture_;
    bool hadSuccessfulConfigure_;
    std::future<bool> mapLoadingFuture_;
    bool hadSuccessfulMapLoading_;

    BackgroundConstantBuffer backgroundBuffer_;
    dx11rendering::DX11Renderer& renderer_;
    dx11rendering::Geometry* pBackgroundGeometry_;
    dx11rendering::ShaderProgram* pBackgroundShader_;
    dx11rendering::ShaderConstantBuffer* pBackgroundConstantBuffer_;

    dx11rendering::ShaderProgram* pWallShader_;
    dx11rendering::Geometry* pWall1Geometry_;
    dx11rendering::Geometry* pWall2Geometry_;
    ResourceManager& resourceManager_;
    DeckLoader& deckLoader_;
    MapRenderer& mapRenderer_;

    Menu::Field* loadingField_;
    Menu::Field* currentMapField_;
    Menu::ElementRenderDescription loadingFieldOnDescription_;

    std::vector<MapSceneAssociation> mapAssociations_;
    int currentMapIndex_;
    MapUtility::SceneInfo currentSceneInfo_;
    MapRenderer::Scene currentRenderScene_;
    physics::Scene* pCurrentPhysicsScene_;
};

class RacingState final : public GameState
{
    friend class DeckChoosingSubState;
    friend class RacingSubState;
    friend class ResultsSubState;

    friend class ResourceManager::RacingResources::HUDResources;
    friend class ResourceManager::RacingResources::Player1Resources;
    friend class ResourceManager::RacingResources::Player2Resources;
private:
    struct StatePair;
    class SubState;

    class DeckChoosingSubState;
    class RacingSubState;
    class ResultsSubState;

public:
    static GameState& GetInstance(types::GameSystems& gameSystems);
    virtual void Enter() override;
    virtual void Exit() override;
    virtual GameState* Update() override;
    virtual void Render() override;

private:
    RacingState(types::GameSystems& gameSystems);

private:
    ResourceManager& resourceManager_;
    ResourceManager::RacingResources& resources_;
    ResourceManager::RacingResources::HUDResources& hudResources_;
    ResourceManager::RacingResources::Player1Resources& player1Resources_;
    ResourceManager::RacingResources::Player2Resources& player2Resources_;
    ResourceManager::RacingResources::ResultsResources& resultsResources_;

    GameState* pNextState_;
    sound::Effect* pCycleEffect_;
    sound::Effect* pSelectEffect_;
    Background& background_;
    dx11rendering::DX11Renderer& renderer_;
    SubState* pSubState_;
};


} // namespace game

} // namespace ice

#endif // GAMESTATES_HPP
