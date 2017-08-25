#include <iostream>
#include <cstdio>
#include "maprenderer.h"
#include "rendering/displaymanager.h"
#include "resourcemanager.h"
#include "core/utility.hpp"
#include "constants.hpp"

#define FAIL_FREE_AND_ERROR do { printf("(Map Renderer): Unable to create scene\n"); FreeScene(scene); return false; } while(false)


namespace ice
{

namespace game
{

// Fixme: hack
static constexpr float HALF_MAP_SIZE = constants::HALF_MAP_RATIO*720;

MapRenderer::MapRenderer(types::GameSystems& gameSystems)
    : renderer_(gameSystems.displayManager.GetRenderer()), resourceManager_(ResourceManager::GetInstance(gameSystems))
{}

bool MapRenderer::MakeSceneCommonSetup(const MapUtility::SceneInfo& sceneInfo, Scene& scene)
{
    dx11rendering::DX11Renderer::ResourceLoader& loader = renderer_.GetResourceLoader();
    scene.pPlayerGeometry_ = loader.CreateGeometry();
    scene.pWallGeometry_ = loader.CreateGeometry();
    scene.pGoalGeometry_ = loader.CreateGeometry();
    if (!(scene.pPlayerGeometry_ && scene.pWallGeometry_ && scene.pGoalGeometry_)) FAIL_FREE_AND_ERROR;

    dx11rendering::Geometry::Description geoDesc;
    geoDesc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    geoDesc.numIndices = 6;
    geoDesc.pIndices = resourceManager_.GetCommonIndices();
    geoDesc.pVertices = sceneInfo.wallVertices;
    geoDesc.numVertices = 4;
    geoDesc.vertexSize = sizeof(ColoredVertex);
    if (!scene.pWallGeometry_->Describe(&geoDesc)) FAIL_FREE_AND_ERROR;

    geoDesc.pVertices = sceneInfo.playerVertices;
    if (!scene.pPlayerGeometry_->Describe(&geoDesc)) FAIL_FREE_AND_ERROR;

    geoDesc.pVertices = sceneInfo.goalVertices;
    if (!scene.pGoalGeometry_->Describe(&geoDesc)) FAIL_FREE_AND_ERROR;
    return true;
}

//! Since our maps are square, make the scene square, leaving black borders.
bool MapRenderer::MakeFullScene(const MapUtility::SceneInfo& sceneInfo, Scene& scene)
{
    if (!MakeSceneCommonSetup(sceneInfo, scene)) return false;

    scene.fullView_.TopLeftX = 280;
    scene.fullView_.TopLeftY = 0;
    scene.fullView_.Width = 720;
    scene.fullView_.Height = 720;

    D3D11_VIEWPORT wallView = {};
    float width = (sceneInfo.wallExtent.x*720) + 1;
    float height = (sceneInfo.wallExtent.y*720) + 1;
    wallView.Width = width;
    wallView.Height = height;
    for (glm::vec2 wall : sceneInfo.wallPositions)
    {
        wallView.TopLeftX = 720*wall.x + 280;
        wallView.TopLeftY = 720*wall.y;
        scene.wallViews_.push_back(wallView);
    }

    D3D11_VIEWPORT view = wallView;
    view.TopLeftX = 720*sceneInfo.playerPos.x + 280;
    view.TopLeftY = 720*sceneInfo.playerPos.y;
    scene.playerView_ = view;

    view.TopLeftX = 720*sceneInfo.goalPos.x + 280;
    view.TopLeftY = 720*sceneInfo.goalPos.y;
    scene.goalView_ = view;

    scene.pPlayerSetterFunc_ = &FullSceneSetPlayer;
    return true;
}

void MapRenderer::FullSceneSetPlayer(Scene& scene, glm::vec2 playerPos)
{
    scene.playerView_.TopLeftX = 720*playerPos.x + 280;
    scene.playerView_.TopLeftY = 720*playerPos.y;
}

bool MapRenderer::MakePlayer1Scene(const MapUtility::SceneInfo& sceneInfo, Scene& scene)
{
    if (!MakeSceneCommonSetup(sceneInfo, scene)) return false;

    scene.fullView_.TopLeftX = 0;
    scene.fullView_.TopLeftY = 0;
    scene.fullView_.Width = HALF_MAP_SIZE;
    scene.fullView_.Height = HALF_MAP_SIZE;

    D3D11_VIEWPORT wallView = {};
    // pad by 1 pixel
    float width = (sceneInfo.wallExtent.x*HALF_MAP_SIZE) + 1;
    float height = (sceneInfo.wallExtent.y*HALF_MAP_SIZE) + 1;
    wallView.Width = width;
    wallView.Height = height;
    for (glm::vec2 wall : sceneInfo.wallPositions)
    {
        wallView.TopLeftX = HALF_MAP_SIZE*wall.x;
        wallView.TopLeftY = HALF_MAP_SIZE*wall.y;
        scene.wallViews_.push_back(wallView);
    }

    D3D11_VIEWPORT view = wallView;
    view.TopLeftX = HALF_MAP_SIZE*sceneInfo.playerPos.x;
    view.TopLeftY = HALF_MAP_SIZE*sceneInfo.playerPos.y;
    scene.playerView_ = view;

    view.TopLeftX = HALF_MAP_SIZE*sceneInfo.goalPos.x;
    view.TopLeftY = HALF_MAP_SIZE*sceneInfo.goalPos.y;
    scene.goalView_ = view;

    scene.pPlayerSetterFunc_ = &Player1SceneSetPlayer;
    return true;
}

void MapRenderer::Player1SceneSetPlayer(Scene& scene, glm::vec2 playerPos)
{
    scene.playerView_.TopLeftX = HALF_MAP_SIZE*playerPos.x;
    scene.playerView_.TopLeftY = HALF_MAP_SIZE*playerPos.y;
}

bool MapRenderer::MakePlayer2Scene(const MapUtility::SceneInfo& sceneInfo, Scene& scene)
{
    if (!MakeSceneCommonSetup(sceneInfo, scene)) return false;

    scene.fullView_.TopLeftX = HALF_MAP_SIZE;
    scene.fullView_.TopLeftY = 0;
    scene.fullView_.Width = HALF_MAP_SIZE;
    scene.fullView_.Height = HALF_MAP_SIZE;

    D3D11_VIEWPORT wallView = {};
    // add a 1 pixel padding for rendering.
    float width = (sceneInfo.wallExtent.x*HALF_MAP_SIZE) + 1;
    float height = (sceneInfo.wallExtent.y*HALF_MAP_SIZE) + 1;
    wallView.Width = width;
    wallView.Height = height;
    for (glm::vec2 wall : sceneInfo.wallPositions)
    {
        wallView.TopLeftX = HALF_MAP_SIZE*wall.x + 640;
        wallView.TopLeftY = HALF_MAP_SIZE*wall.y;
        scene.wallViews_.push_back(wallView);
    }

    D3D11_VIEWPORT view = wallView;
    view.TopLeftX = HALF_MAP_SIZE*sceneInfo.playerPos.x + 640;
    view.TopLeftY = HALF_MAP_SIZE*sceneInfo.playerPos.y;
    scene.playerView_ = view;

    view.TopLeftX = HALF_MAP_SIZE*sceneInfo.goalPos.x + 640;
    view.TopLeftY = HALF_MAP_SIZE*sceneInfo.goalPos.y;
    scene.goalView_ = view;

    scene.pPlayerSetterFunc_ = &Player2SceneSetPlayer;
    return true;
}

void MapRenderer::Player2SceneSetPlayer(Scene& scene, glm::vec2 playerPos)
{
    scene.playerView_.TopLeftX = HALF_MAP_SIZE*playerPos.x + 640;
    scene.playerView_.TopLeftY = HALF_MAP_SIZE*playerPos.y;
}

void MapRenderer::FreeScene(Scene& scene)
{
    scene.wallViews_.clear();
    core::utility::DeleteAndNull(scene.pPlayerGeometry_);
    core::utility::DeleteAndNull(scene.pWallGeometry_);
    core::utility::DeleteAndNull(scene.pGoalGeometry_);
}

void MapRenderer::RenderScene(const Scene& temp)
{
    Scene& scene = (Scene&)temp;
    dx11rendering::RenderPass::PipelineDescription pipelineDesc = {};
    pipelineDesc.pProgram = resourceManager_.GetShader(ResourceManager::Shader::eColored);

    dx11rendering::RenderPass pass(*scene.pWallGeometry_, pipelineDesc);
    pass.Bind();

    for (auto& view : scene.wallViews_)
    {
        renderer_.SetViewPort(&view);
        renderer_.Draw(*scene.pWallGeometry_);
    }

    pass.SetGeometry(*scene.pPlayerGeometry_);
    pass.Bind();
    renderer_.SetViewPort(&scene.playerView_);
    renderer_.Draw(*scene.pPlayerGeometry_);

    pass.SetGeometry(*scene.pGoalGeometry_);
    pass.Bind();
    renderer_.SetViewPort(&scene.goalView_);
    renderer_.Draw(*scene.pGoalGeometry_);
}

} // namespace game

} // namespace ice
