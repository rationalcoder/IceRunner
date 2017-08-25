#include "gamemanager.h"
#include <glm/gtc/epsilon.hpp>
#include <array>
#include "events/eventmanager.h"
#include "rendering/displaymanager.h"
#include "sound/soundmanager.h"
#include "deckloader.h"
#include "resourcemanager.h"

namespace ice
{
namespace game
{

static std::array<glm::vec3, 5> targetColors = {
    glm::vec3{0/255.0f, 31/255.0f, 26/255.0f},
    glm::vec3{0/255.0f, 6/255.0f, 21/255.0f},
    glm::vec3{14/255.0f, 0/255.0f, 31/255.0f},
    glm::vec3{14/255.0f, 0/255.0f, 31/255.0f},
    glm::vec3{31/255.0f, 0/255.0f, 28/255.0f},
};

bool GameManager::Init()
{
    bool res = ResourceManager::GetInstance(gameSystems_).Load() &&
               DeckLoader::GetInstance().Init() &&
               game_.Init();
    if (res) gameSystems_.displayManager.ShowWindow();

    //currentColor_ = targetColors[targetColorIndex_++];
    return res;
}

GameManager::~GameManager()
{
    // FIXME: hack. The game resource manager should probably be accessible through
    // the game manager and not as a singleton. As it stands, the resource manager
    // has a static destructor, making it run well after the subsystems have been destroyed,
    // leading to crashes.
    ResourceManager::GetInstance(gameSystems_).Unload();
}

void GameManager::Update()
{
    game_.Update();
}

bool GameManager::ShouldQuit()
{
    return game_.ShouldQuit();
}

static void Lerp(glm::vec3& from, glm::vec3& to, float f, glm::vec3* pResult)
{
    *pResult = (from * (1.0f - f)) + (to * f);
}

void GameManager::Render()
{
    dx11rendering::DX11Renderer& renderer = gameSystems_.displayManager.GetRenderer();
    //if (targetColorIndex_ == targetColors.size()) targetColorIndex_ = 0;

    //glm::vec3 t = targetColors[targetColorIndex_];
    //if (glm::all(glm::epsilonEqual(currentColor_, t , .004f))) targetColorIndex_++;
    //else Lerp(currentColor_, targetColors[targetColorIndex_], .01f, &currentColor_);

    //renderer.BeginScene(D3DCOLORVALUE{currentColor_.r, currentColor_.g, currentColor_.b, 1});
    renderer.BeginScene(D3DCOLORVALUE{0, .05f, .1f, 1});

    game_.Render();

    renderer.EndScence();
}

} // namespace game

} // namespace ice
