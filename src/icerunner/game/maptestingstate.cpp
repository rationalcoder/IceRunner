#include <iostream>
#include <future>
#include <glm/vec2.hpp>
#include "rendering/displaymanager.h"
#include "events/eventmanager.h"
#include "sound/soundmanager.h"

#include "gamestates.hpp"
#include "deckloader.h"
#include "game/constants.hpp"
#include "maputility.hpp"
#include "physics/physicsmanager.h"

#include <chrono>
#include <ctime>

namespace ice
{

namespace game
{

MapTestingState::MapTestingState(types::GameSystems& gameSystems)
    : GameState(gameSystems), inMenu_(true), renderer_(gameSystems.displayManager.GetRenderer()),
      resourceManager_(ResourceManager::GetInstance(gameSystems)), deckLoader_(DeckLoader::GetInstance()),
      mapRenderer_(MapRenderer::GetInstance(gameSystems)), currentMapIndex_(0)
{}

GameState& MapTestingState::GetInstance(types::GameSystems& gameSystems)
{
    static MapTestingState deckChoosingState(gameSystems);
    return deckChoosingState;
}

static void DebugPrintFactories()
{
    DeckSettings& settings = DeckLoader::GetInstance().GetDeckSettings();
    for (int i = 1; i <= settings.GetSize(); i++)
    {
        MapFactory* pFactory = &settings.MapFactoryAt(Map::Difficulty(i));
        std::cout << "Got factory:" << '\n';
        glm::vec3 mapColor = pFactory->GetMapColor();
        glm::vec3 playerColor = pFactory->GetPlayerColor();
        glm::vec3 wallColor = pFactory->GetWallColor();
        glm::vec3 goalColor = pFactory->GetGoalColor();
        std::cout << "\tMap Color (RGB): " << mapColor.x << ',' << mapColor.y << ',' << mapColor.z << '\n';
        std::cout << "\tPlayer Color (RGB): " << playerColor.x << ',' << playerColor.y << ',' << playerColor.z << '\n';
        std::cout << "\tWall Color (RGB): " << wallColor.x << ',' << wallColor.y << ',' << wallColor.z << '\n';
        std::cout << "\tGoal Color (RGB): " << goalColor.x << ',' << goalColor.y << ',' << goalColor.z << '\n';
        std::cout << "\tMap Difficulty: " << pFactory->GetDifficulty() << '\n';
        std::cout << "\tMap Square Size: " << pFactory->GetMapKit().GetDescription().dimensions.numRows << std::endl;
    }
}

void MapTestingState::Enter()
{
    ResourceManager::MapTestingResources& resources = resourceManager_.GetMapTestingResources();
    pMenu_ = &resources.GetMenu();
    loadingField_ = resources.GetLoadingField();
    currentMapField_ = resources.GetCurrentMapField();
    loadingFieldOnDescription_ = resources.GetLoadingRenderDescription();
    resourceManager_.GetBackground().SetColor(glm::vec3(0, .85, 1));

    ReleaseMapResources();

    DeckLoader& loader = DeckLoader::GetInstance();
    hadSuccessfulConfigure_ = loader.Configure();
    if(!hadSuccessfulConfigure_)
        std::cout << loader.GetError() << std::endl;

    hadSuccessfulMapLoading_ = loader.LoadMaps();
    if (!hadSuccessfulMapLoading_)
        std::cout << loader.GetError() << std::endl;

    if (hadSuccessfulConfigure_ && hadSuccessfulMapLoading_)
    {
        pCurrentPhysicsScene_ = &gameSystems_.physicsManager.SceneAt(resources.GetSceneIndex());
        // creates map resources and loads an initial physics scene if there is at least one map.
        CreateMapResources();
    }
}

void MapTestingState::Exit()
{
    ReleaseMapResources();
    ReleasePhysicsScene();
}

//////////////////// Game Logic ///////////////////////////

GameState* MapTestingState::Update()
{
    Background& background = resourceManager_.GetBackground();
    background.SetAnimationSpeed(inMenu_ ? constants::MENU_BACKGROUND_SPEED : constants::GAMEPLAY_BACKGROUND_SPEED);
    if (inMenu_) return UpdateInMenu();
    else UpdateOutOfMenu();

    return nullptr;
}

GameState* MapTestingState::UpdateInMenu()
{
    const events::PlayerState& player1State = gameSystems_.eventManager.GetEventState().GetPlayerState(events::PlayerID::e1);

    if (player1State.IsDown(events::MetaEvent::eBack)) return &MainMenuState::GetInstance(gameSystems_);

    sound::Effect* pEffect = resourceManager_.GetSoundEffect(ResourceManager::SoundEffect::eMenuCycle);
    if (player1State.IsDown(events::MovementEvent::eUp))
    {
        pMenu_->FocusPrevious();
        pEffect->Play();
    }
    else if (player1State.IsDown(events::MovementEvent::eDown))
    {
        pMenu_->FocusNext();
        pEffect->Play();
    }
    else if (player1State.IsDown(events::MetaEvent::eSelect))
        pMenu_->Press();

    return nullptr;
}

void MapTestingState::UpdateOutOfMenu()
{
    const events::PlayerState& player1State = gameSystems_.eventManager.GetEventState().GetPlayerState(events::PlayerID::e1);
    if (player1State.IsDown(events::MetaEvent::eBack) || pCurrentPhysicsScene_->HitGoal())
    {
        // Just stop simulating the scene. It will be cleaned up and reset in the test map listener.
        pCurrentPhysicsScene_->Pause();
        inMenu_ = true;

        if (pCurrentPhysicsScene_->HitGoal())
            resourceManager_.GetSoundEffect(ResourceManager::SoundEffect::eGoal)->Play();
        // TODO: add exit sound here.
        resourceManager_.GetDJ().PlayMenu();
    }

    physics::Player& player = pCurrentPhysicsScene_->GetPlayer();

    currentRenderScene_.SetPlayer(player.boundingBox.GetMin());
    if (!player.IsStopped()) return;

    if (!player.WasStopped())
        resourceManager_.GetSoundEffect(ResourceManager::SoundEffect::eBump)->Play();

    if (player1State.GetNumMovementEventsDown() > 1) return;
    else if (player1State.WasDown(events::MovementEvent::eRight)) player.velocity = glm::vec2(constants::PLAYER_SPEED, 0);
    else if (player1State.WasDown(events::MovementEvent::eLeft)) player.velocity = glm::vec2(-constants::PLAYER_SPEED, 0);
    else if (player1State.WasDown(events::MovementEvent::eUp)) player.velocity = glm::vec2(0, -constants::PLAYER_SPEED);
    else if (player1State.WasDown(events::MovementEvent::eDown)) player.velocity = glm::vec2(0, constants::PLAYER_SPEED);
}

//////////////////// Action Listeners ///////////////////////////

void MapTestingState::NextMapListener(void* pTestingState)
{
    MapTestingState* pState = (MapTestingState*)pTestingState;
    int currentIndex = pState->currentMapIndex_;
    if (currentIndex == -1)
    {
        pState->currentMapField_->ClearRenderDescription();
        return;
    }

    if (currentIndex == pState->mapAssociations_.size()-1) pState->currentMapIndex_ = 0;
    else pState->currentMapIndex_++;

    // uncomment to check for performance issues

    MapSceneAssociation& currentAssoc = pState->mapAssociations_[pState->currentMapIndex_];
    pState->currentMapField_->SetRenderDescription(currentAssoc.renderDescription);
    pState->resourceManager_.GetBackground().SetColor(pState->deckLoader_.GetDeckSettings().MapFactoryAt(currentAssoc.pMap->GetDescription().difficulty).GetMapColor());
    pState->resourceManager_.GetSoundEffect(ResourceManager::SoundEffect::eMenuCycle)->Play();
}

void MapTestingState::PreviousMapListener(void* pTestingState)
{
    MapTestingState* pState = (MapTestingState*)pTestingState;
    int currentIndex = pState->currentMapIndex_;
    if (currentIndex == -1)
    {
        pState->currentMapField_->ClearRenderDescription();
        return;
    }

    if (currentIndex == 0) pState->currentMapIndex_ = (int)(pState->mapAssociations_.size()-1);
    else pState->currentMapIndex_--;

    MapSceneAssociation& currentAssoc = pState->mapAssociations_[pState->currentMapIndex_];
    pState->currentMapField_->SetRenderDescription(pState->mapAssociations_[pState->currentMapIndex_].renderDescription);
    pState->resourceManager_.GetBackground().SetColor(pState->deckLoader_.GetDeckSettings().MapFactoryAt(currentAssoc.pMap->GetDescription().difficulty).GetMapColor());

    pState->resourceManager_.GetSoundEffect(ResourceManager::SoundEffect::eMenuCycle)->Play();
}

void MapTestingState::TestMapListener(void* pTestingState)
{
    // If we failed to load something, print an error and do nothing.
    MapTestingState* pState = (MapTestingState*)pTestingState;
    if (!pState->hadSuccessfulConfigure_ || (pState->configureFuture_.valid() && !pState->configureFuture_.get()))
    {
        printf("Unable to configure the deck loader: %s\n Please fix the error and reconfigure\n", DeckLoader::GetInstance().GetError().c_str());
        return;
    }
    if (!pState->hadSuccessfulMapLoading_ || (pState->mapLoadingFuture_.valid() && !pState->mapLoadingFuture_.get()))
    {
        printf("Unable to load maps: %s\n Please fix the error and reload the maps\n", DeckLoader::GetInstance().GetError().c_str());
        return;
    }

    // If there aren't any maps, do nothing.
    if (pState->currentMapIndex_ == -1) return;

    // Get the basic scene info, then create the render and physics scene using it.
    MapUtility::GetSceneInfo(*pState->mapAssociations_[pState->currentMapIndex_].pMap, pState->currentSceneInfo_);
    pState->mapRenderer_.FreeScene(pState->currentRenderScene_);
    pState->mapRenderer_.MakeFullScene(pState->currentSceneInfo_, pState->currentRenderScene_);
    pState->LoadPhysicsScene();

    pState->pCurrentPhysicsScene_->Unpause();

    pState->resourceManager_.GetDJ().PlayAction();
    pState->inMenu_ = false;
    pState->resourceManager_.GetSoundEffect(ResourceManager::SoundEffect::eMenuSelect)->Play();
}

void MapTestingState::ConfigureListener(void* pTestingState)
{
    MapTestingState* pState = (MapTestingState*)pTestingState;
    // If we are in the process of loading something, don't do anything.
    if (pState->configureFuture_.valid() && pState->configureFuture_.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) return;
    if (pState->mapLoadingFuture_.valid() && pState->mapLoadingFuture_.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) return;

    pState->ReleaseMapResources();

    DeckLoader& loader = DeckLoader::GetInstance();
    pState->loadingField_->SetRenderDescription(pState->loadingFieldOnDescription_);
    pState->configureFuture_ = std::async(std::launch::async, [&loader, pState]() {
        bool result = loader.Configure();
        if (result) pState->CreateMapResources();
        pState->loadingField_->ClearRenderDescription();
        pState->hadSuccessfulConfigure_ = result;
        return result;
    });
}

void MapTestingState::LoadMapsListener(void* pTestingState)
{
    MapTestingState* pState = (MapTestingState*)pTestingState;
    // If we are in the process of loading something, don't do anything.
    if (pState->mapLoadingFuture_.valid() && pState->mapLoadingFuture_.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) return;
    if (pState->configureFuture_.valid() && pState->configureFuture_.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) return;

    pState->ReleaseMapResources();

    DeckLoader& loader = DeckLoader::GetInstance();
    pState->loadingField_->SetRenderDescription(pState->loadingFieldOnDescription_);
    pState->mapLoadingFuture_ = std::async(std::launch::async, [&loader, pState]() {
        bool result = loader.LoadMaps();
        if (result) pState->CreateMapResources();
        pState->loadingField_->ClearRenderDescription();
        pState->hadSuccessfulMapLoading_ = result;
        return result;
    });
}

//////////////////// Rendering ///////////////////////////

void MapTestingState::Render()
{
    if (inMenu_) RenderInMenu();
    else RenderOutOfMenu();
}

void MapTestingState::RenderInMenu()
{
    D3D11_VIEWPORT menuView;
    menuView.Width = 800;
    menuView.Height = 600;
    menuView.MaxDepth = 0;
    menuView.MinDepth = 0;
    menuView.TopLeftX = (1280-800)/2;
    menuView.TopLeftY = (720-600)/2;

    RenderFullBackground();

    renderer_.SetViewPort(&menuView);
    pMenu_->Render();
}

void MapTestingState::RenderOutOfMenu()
{
    dx11rendering::RenderPass::PipelineDescription pipelineDesc;
    pipelineDesc.pProgram = pWallShader_;
    pipelineDesc.pPSConstantBuffer = nullptr;
    pipelineDesc.pVSConstantBuffer = nullptr;
    pipelineDesc.pTexture = nullptr;

    // uncomment to check for performance issues
    //std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    //start = std::chrono::high_resolution_clock::now();

    RenderMapBackground();
    mapRenderer_.RenderScene(currentRenderScene_);
    //end = std::chrono::high_resolution_clock::now();

    //std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;
}

void MapTestingState::RenderFullBackground()
{
    Background& background = resourceManager_.GetBackground();
    renderer_.SetViewPort(&resourceManager_.GetFullScreenView());
    background.Render();
}

void MapTestingState::RenderMapBackground()
{
    Background& background = resourceManager_.GetBackground();
    renderer_.SetViewPort(&currentRenderScene_.GetView());
    background.Render();
}

//////////////////// Resource Management ///////////////////////////

bool MapTestingState::CreateMapResources()
{
    NormalizedRect currentMapGeo;
    currentMapGeo.topLeftPosition = glm::vec2(.2, -.03);
    currentMapGeo.extent.y = .055f;

    MapSceneAssociation assoc;
    deckLoader_.ForEachMap([&assoc, currentMapGeo, this](Map& map)
    {
        assoc.pMap = &map;
        assoc.renderDescription = pMenu_->MakeRenderDescription(map.GetDescription().name.c_str(), currentMapGeo);
        if (!assoc.renderDescription.IsValid())
        {
            printf("Failed to allocate resources for map: %s. Skipping it.\n", map.GetDescription().name.c_str());
            return;
        }
        mapAssociations_.push_back(assoc);
    });

    assoc.Leak();

    currentMapIndex_ = -1;
    if (!mapAssociations_.empty())
    {
        MapSceneAssociation& currentAssociation = mapAssociations_[0];
        MapUtility::GetSceneInfo(*currentAssociation.pMap, currentSceneInfo_);
        currentMapField_->SetRenderDescription(currentAssociation.renderDescription);
        currentMapIndex_ = 0;

        resourceManager_.GetBackground().SetColor(deckLoader_.GetDeckSettings().MapFactoryAt(currentAssociation.pMap->GetDescription().difficulty).GetMapColor());

        LoadPhysicsScene();
    }

    return true;
}

void MapTestingState::ReleaseMapResources()
{
    currentSceneInfo_.wallPositions.clear();
    for (auto& assoc : mapAssociations_)
        assoc.DeleteAndNull();

    mapAssociations_.clear();
    mapRenderer_.FreeScene(currentRenderScene_);
    currentMapIndex_ = -1;
    currentMapField_->ClearRenderDescription();
}

void MapTestingState::LoadPhysicsScene()
{
    // Clear the physics sceen and add the player, goal, and walls of the map at the active map index.
    physics::Scene& scene = *pCurrentPhysicsScene_;
    ReleasePhysicsScene();

    glm::vec2 epsilonVector(physics::constants::EPSILON, physics::constants::EPSILON);

    glm::vec2 wallExtent = currentSceneInfo_.wallExtent;
    glm::vec2 playerPos = currentSceneInfo_.playerPos + epsilonVector;
    physics::Player player;
    player.boundingBox.SetMin(playerPos);
    player.boundingBox.SetMax(playerPos + wallExtent - epsilonVector - epsilonVector);

    glm::vec2 goalPos = currentSceneInfo_.goalPos + epsilonVector;
    physics::Goal goal;
    goal.boundingBox.SetMin(goalPos);
    goal.boundingBox.SetMax(goalPos + wallExtent - epsilonVector - epsilonVector);

    physics::AABB wall;
    for (glm::vec2& pos : currentSceneInfo_.wallPositions)
    {
        wall.SetMin(pos);
        wall.SetMax(pos + wallExtent);
        scene.AddWall(wall);
    }

    physics::Scene::Description desc { player, goal };
    scene.SetDescription(desc);
}

void MapTestingState::ReleasePhysicsScene()
{
    pCurrentPhysicsScene_->Clear();
}

} // namespace game

} // namespace ice
