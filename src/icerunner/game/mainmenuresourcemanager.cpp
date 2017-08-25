#include "resourcemanager.h"
#include "gamestates.hpp"

#define FAIL_AND_ERROR do { printf("Failed to create main menu resources\n"); return false; } while(false);

namespace ice
{
namespace game
{

bool ResourceManager::MainMenuResources::Load()
{
    NormalizedRect titleGeo;
    titleGeo.topLeftPosition = glm::vec2(-.75, .75);
    titleGeo.extent.y = .2f;

    Menu::LabelDescription labelDesc;
    labelDesc.elementDescription.geometry = titleGeo;
    labelDesc.elementDescription.pText = "ICE RUNNER";

    NormalizedRect testMapGeo;
    testMapGeo.topLeftPosition = glm::vec2(-.3, 0);
    testMapGeo.extent.y = .1f;

    NormalizedRect raceGeo;
    raceGeo.topLeftPosition = glm::vec2(-.17, -.4);
    raceGeo.extent.y = .1f;

    GameState* pUserData = &MainMenuState::GetInstance(gameSystems_);
    Menu::ButtonDescription buttonDesc[2];
    buttonDesc[0].elementDescription.geometry = testMapGeo;
    buttonDesc[0].elementDescription.pText = "TEST MAP";
    buttonDesc[0].pListener = &MainMenuState::TestMapListener;
    buttonDesc[0].pUserData = pUserData;

    buttonDesc[1].elementDescription.geometry = raceGeo;
    buttonDesc[1].elementDescription.pText = "RACE";
    buttonDesc[1].pListener = &MainMenuState::RaceListener;
    buttonDesc[1].pUserData = pUserData;

    Menu::Description desc;
    desc.numButtons = 2;
    desc.numLabels = 1;
    desc.pButtonDescriptions = buttonDesc;
    desc.pLabelDescriptions = &labelDesc;

    Menu& menu = menu_;

    ResourceManager& resourceManager = ResourceManager::GetInstance(gameSystems_);
    if(!menu.Describe(resourceManager.renderer_, desc)) FAIL_AND_ERROR;

    return true;
}

void ResourceManager::MainMenuResources::Unload()
{
    menu_.Free();
}

} // namespace game
} // namespace ice
