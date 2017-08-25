#include "resourcemanager.h"
#include "physics/physicsmanager.h"
#include "gamestates.hpp"

#define FAIL_AND_ERROR do { printf("Unable to allocate map testing resources\n"); return false; }while(false)

namespace ice
{

namespace game
{

bool ResourceManager::MapTestingResources::Load()
{
    NormalizedRect titleGeo;
    titleGeo.topLeftPosition = glm::vec2(-.85, .75);
    titleGeo.extent.y = .2f;

    NormalizedRect availableGeo;
    availableGeo.topLeftPosition = glm::vec2(-.75, 0);
    availableGeo.extent.y = .09f;

    //NormalizedRect underlineGeo;
    //underlineGeo.topLeftPosition = glm::vec2(0, -.06);
    //underlineGeo.extent.y = .05f;

    Menu::LabelDescription labelDesc[2];
    labelDesc[0].elementDescription.geometry = titleGeo;
    labelDesc[0].elementDescription.pText = "MAP TESTING";

    labelDesc[1].elementDescription.geometry = availableGeo;
    labelDesc[1].elementDescription.pText = "Available";

    //labelDesc[2].elementDescription.geometry = underlineGeo;
    //labelDesc[2].elementDescription.pText = "......................................";

    NormalizedRect loadMapsGeo;
    loadMapsGeo.topLeftPosition = glm::vec2(-.75, -.3);
    loadMapsGeo.extent.y = .055f;

    NormalizedRect configureGeo;
    configureGeo.topLeftPosition = glm::vec2(-.75, -.4);
    configureGeo.extent.y = .055f;

    NormalizedRect nextGeo;
    nextGeo.topLeftPosition = glm::vec2(-.75, -.5);
    nextGeo.extent.y = .055f;

    NormalizedRect prevGeo;
    prevGeo.topLeftPosition = glm::vec2(-.75, -.6);
    prevGeo.extent.y = .055f;

    NormalizedRect selectGeo;
    selectGeo.topLeftPosition = glm::vec2(-.75, -.7);
    selectGeo.extent.y = .055f;

    GameState* pUserData = &MapTestingState::GetInstance(gameSystems_);
    Menu::ButtonDescription buttonDesc[5];
    buttonDesc[0].elementDescription.geometry = loadMapsGeo;
    buttonDesc[0].elementDescription.pText = "LOAD MAPS";
    buttonDesc[0].pListener = &MapTestingState::LoadMapsListener;
    buttonDesc[0].pUserData = pUserData;

    buttonDesc[1].elementDescription.geometry = configureGeo;
    buttonDesc[1].elementDescription.pText = "RE CONFIGURE";
    buttonDesc[1].pListener = &MapTestingState::ConfigureListener;
    buttonDesc[1].pUserData = pUserData;

    buttonDesc[2].elementDescription.geometry = nextGeo;
    buttonDesc[2].elementDescription.pText = "NEXT";
    buttonDesc[2].pListener = &MapTestingState::NextMapListener;
    buttonDesc[2].pUserData = pUserData;

    buttonDesc[3].elementDescription.geometry = prevGeo;
    buttonDesc[3].elementDescription.pText = "PREV";
    buttonDesc[3].pListener = &MapTestingState::PreviousMapListener;
    buttonDesc[3].pUserData = pUserData;

    buttonDesc[4].elementDescription.geometry = selectGeo;
    buttonDesc[4].elementDescription.pText = "SELECT";
    buttonDesc[4].pListener = &MapTestingState::TestMapListener;
    buttonDesc[4].pUserData = pUserData;

    Menu::Description desc;
    desc.numButtons = 5;
    desc.numLabels = 2;
    desc.pButtonDescriptions = buttonDesc;
    desc.pLabelDescriptions = labelDesc;

    Menu& menu = menu_;

    ResourceManager& resourceManager = ResourceManager::GetInstance(gameSystems_);

    if(!menu.Describe(resourceManager.renderer_, desc)) FAIL_AND_ERROR;

    pCurrentMapField_ = &menu.AddField(0);
    pLoadingField_ = &menu.AddField(1);

    NormalizedRect loadingGeo;
    loadingGeo.topLeftPosition = glm::vec2(.6, -.8);
    loadingGeo.extent.y = .05f;
    loadingRenderDescription_ = menu_.MakeRenderDescription("Loading...", loadingGeo);
    if (!loadingRenderDescription_.IsValid()) FAIL_AND_ERROR;
    
    sceneIndex_ = resourceManager.physicsManager_.NewScene();

    return true;
}

void ResourceManager::MapTestingResources::Unload()
{
    loadingRenderDescription_.DeleteAndNull();
    gameSystems_.physicsManager.ClearScene(sceneIndex_);
    menu_.Free();
}

} // namespace game

} // namespace ice
