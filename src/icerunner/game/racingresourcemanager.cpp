#include <array>
#include "racingsubstates.hpp"
#include "resourcemanager.h"
#include "physics/physicsmanager.h"

namespace ice
{
namespace game
{

//////////////////// HUD ///////////////////////////

bool ResourceManager::RacingResources::HUDResources::Load()
{
    //////////////////// COMMON ///////////////////////////

    currentDeckGeo_.topLeftPosition = glm::vec2(.2, -.03);
    currentDeckGeo_.extent.y = .055f;

    NormalizedRect commonCurrentMapGeo;
    commonCurrentMapGeo.topLeftPosition = glm::vec2(-.95, -.5);
    commonCurrentMapGeo.extent.y = .4f;
    commonCurrentMapGeo.extent.x = .5f;

    //////////////////// PLAYER 1 ///////////////////////////

    NormalizedRect commonTitleGeo;
    commonTitleGeo.topLeftPosition = glm::vec2(-.25, .90);
    commonTitleGeo.extent.y = .6f;
    commonTitleGeo.extent.x = .5f;

    Menu::LabelDescription player1LabelsDesc[2];
    player1LabelsDesc[0].elementDescription.geometry = commonCurrentMapGeo;
    player1LabelsDesc[0].elementDescription.pText = "PLAYING";
    player1LabelsDesc[1].elementDescription.geometry = commonTitleGeo;
    player1LabelsDesc[1].elementDescription.pText = "PLAYER 1";

    Menu::Description player1HUDDesc;
    player1HUDDesc.numButtons = 0;
    player1HUDDesc.numLabels = 2;
    player1HUDDesc.pButtonDescriptions = nullptr;
    player1HUDDesc.pLabelDescriptions = player1LabelsDesc;

    player1HUDDesc.colors[0] = glm::vec4(0, 0, 0, 1);
    player1HUDDesc.colors[1] = glm::vec4(0, 0, 0, 1);
    player1HUDDesc.colors[2] = glm::vec4(0, .25, .8, 1);
    player1HUDDesc.colors[3] = glm::vec4(0, .25, .8, 1);

    ResourceManager& resourceManager = ResourceManager::GetInstance(gameSystems_);
    if(!player1HUD_.Describe(resourceManager.renderer_, player1HUDDesc))
    {
        printf("Failed to load player 1's HUD\n");
        return false;
    }

    pPlayer1CurrentMapField_ = &player1HUD_.AddField(0);

    player1HUDView_.TopLeftX = 0;
    player1HUDView_.TopLeftY = 640;
    player1HUDView_.Width = 600;
    player1HUDView_.Height = 80;

    //////////////////// PLAYER 2 ///////////////////////////

    Menu::LabelDescription player2LabelsDesc[2];
    player2LabelsDesc[0].elementDescription.geometry = commonCurrentMapGeo;
    player2LabelsDesc[0].elementDescription.pText = "PLAYING";
    player2LabelsDesc[1].elementDescription.geometry = commonTitleGeo;
    player2LabelsDesc[1].elementDescription.pText = "PLAYER 2";

    Menu::Description player2HUDDesc;
    player2HUDDesc.numButtons = 0;
    player2HUDDesc.numLabels = 2;
    player2HUDDesc.pButtonDescriptions = nullptr;
    player2HUDDesc.pLabelDescriptions = player2LabelsDesc;

    player2HUDDesc.colors[0] = glm::vec4(0, 0, 0, 1);
    player2HUDDesc.colors[1] = glm::vec4(0, 0, 0, 1);
    player2HUDDesc.colors[2] = glm::vec4(.8, .25, 0, 1);
    player2HUDDesc.colors[3] = glm::vec4(.8, .25, 0, 1);

    if(!player2HUD_.Describe(resourceManager.renderer_, player2HUDDesc))
    {
        printf("Failed to load player 2's HUD\n");
        return false;
    }

    pPlayer2CurrentMapField_ = &player2HUD_.AddField(0);

    player2HUDView_.TopLeftX = 680;
    player2HUDView_.TopLeftY = 640;
    player2HUDView_.Width = 600;
    player2HUDView_.Height = 80;

    //////////////////// COUNTDOWN ///////////////////////////

    Menu::Description countDownDesc;
    countDownDesc.numButtons = 0;
    countDownDesc.numLabels = 0;
    countDownDesc.pLabelDescriptions = nullptr;
    countDownDesc.pButtonDescriptions = nullptr;

    countDownDesc.colors[0] = glm::vec4(.1, .1, .1, 1);
    countDownDesc.colors[1] = glm::vec4(.1, .1, .1, 1);
    countDownDesc.colors[2] = glm::vec4(.5, .5, .5, 1);
    countDownDesc.colors[3] = glm::vec4(.5, .5, .5, 1);

    if(!countDownHUD_.Describe(resourceManager.renderer_, countDownDesc))
    {
        printf("Failed to load the count down HUD\n");
        return false;
    }

    pCountDownField_ = &countDownHUD_.AddField(0);

    countDownView_.TopLeftX = 600;
    countDownView_.TopLeftY = 640;
    countDownView_.Width = 80;
    countDownView_.Height = 80;

    NormalizedRect countDown1Geo;
    countDown1Geo.topLeftPosition = glm::vec2(-.40, .6);
    countDown1Geo.extent = glm::vec2(0, 1.5);

    NormalizedRect countDown2Geo;
    countDown2Geo.topLeftPosition = glm::vec2(-.65, .6);
    countDown2Geo.extent = glm::vec2(0, 1.5);

    NormalizedRect countDown3Geo;
    countDown3Geo.topLeftPosition = glm::vec2(-.75, .6);
    countDown3Geo.extent = glm::vec2(1.5, 1.5);

    NormalizedRect goGeo;
    goGeo.topLeftPosition = glm::vec2(-.75, .45);
    goGeo.extent = glm::vec2(1.5, 1.2);

    countDown1RenderDesc_ = countDownHUD_.MakeRenderDescription("1", countDown1Geo);
    countDown2RenderDesc_ = countDownHUD_.MakeRenderDescription("2", countDown2Geo);
    countDown3RenderDesc_ = countDownHUD_.MakeRenderDescription("3", countDown3Geo, countDownView_);
    countDownGoRenderDesc_ = countDownHUD_.MakeRenderDescription("GO!", goGeo, countDownView_);

    if (!countDown1RenderDesc_.IsValid() ||
        !countDown2RenderDesc_.IsValid() ||
        !countDown3RenderDesc_.IsValid())
    {
        printf("Failed to load the count down numbers\n");
        return false;
    }

    //////////////////// DECK CHOOSING ///////////////////////////
    RacingState::DeckChoosingSubState* pDeckChoosingSubState = (RacingState::DeckChoosingSubState*)&RacingState::DeckChoosingSubState::GetInstance((RacingState&)RacingState::GetInstance(gameSystems_));

    std::array<Menu::ButtonDescription, 5> deckChoosingMenu1ButtonDescs;
    deckChoosingMenu1ButtonDescs[0].elementDescription.geometry  = NormalizedRect{glm::vec2(-.75, -.3), glm::vec2(0, .055)};
    deckChoosingMenu1ButtonDescs[0].elementDescription.pText     = "LOAD DECKS";
    deckChoosingMenu1ButtonDescs[0].pListener = &RacingState::DeckChoosingSubState::LoadDecksListener;
    deckChoosingMenu1ButtonDescs[0].pUserData = pDeckChoosingSubState;

    deckChoosingMenu1ButtonDescs[1].elementDescription.geometry  = NormalizedRect{glm::vec2(-.75, -.4), glm::vec2(0, .055)};
    deckChoosingMenu1ButtonDescs[1].elementDescription.pText     = "RE CONFIGURE";
    deckChoosingMenu1ButtonDescs[1].pListener = &RacingState::DeckChoosingSubState::ConfigureListener;
    deckChoosingMenu1ButtonDescs[1].pUserData = pDeckChoosingSubState;

    deckChoosingMenu1ButtonDescs[2].elementDescription.geometry  = NormalizedRect{glm::vec2(-.75, -.5), glm::vec2(0, .055)};
    deckChoosingMenu1ButtonDescs[2].elementDescription.pText     = "NEXT";
    deckChoosingMenu1ButtonDescs[2].pListener = &RacingState::DeckChoosingSubState::Player1NextDeckListener;
    deckChoosingMenu1ButtonDescs[2].pUserData = pDeckChoosingSubState;

    deckChoosingMenu1ButtonDescs[3].elementDescription.geometry  = NormalizedRect{glm::vec2(-.75, -.6), glm::vec2(0, .055)};
    deckChoosingMenu1ButtonDescs[3].elementDescription.pText     = "PREV";
    deckChoosingMenu1ButtonDescs[3].pListener = &RacingState::DeckChoosingSubState::Player1PreviousDeckListener;
    deckChoosingMenu1ButtonDescs[3].pUserData = pDeckChoosingSubState;

    deckChoosingMenu1ButtonDescs[4].elementDescription.geometry  = NormalizedRect{glm::vec2(-.75, -.7), glm::vec2(0, .055)};
    deckChoosingMenu1ButtonDescs[4].elementDescription.pText     = "SELECT";
    deckChoosingMenu1ButtonDescs[4].pListener = &RacingState::DeckChoosingSubState::Player1SelectDeckListener;
    deckChoosingMenu1ButtonDescs[4].pUserData = pDeckChoosingSubState;

    std::array<Menu::LabelDescription, 2> deckChoosingMenu1LabelDescs;
    deckChoosingMenu1LabelDescs[0].elementDescription.geometry = NormalizedRect{glm::vec2(-.72, .75), glm::vec2(0, .23)};
    deckChoosingMenu1LabelDescs[0].elementDescription.pText    = "PLAYER 1";

    deckChoosingMenu1LabelDescs[1].elementDescription.geometry = NormalizedRect{glm::vec2(-.75, 0), glm::vec2(0, .09)};
    deckChoosingMenu1LabelDescs[1].elementDescription.pText    = "Available";

    Menu::Description deckChoosingMenu1Desc;
    deckChoosingMenu1Desc.numLabels = (uint32_t)deckChoosingMenu1LabelDescs.size();
    deckChoosingMenu1Desc.pLabelDescriptions = deckChoosingMenu1LabelDescs.data();
    deckChoosingMenu1Desc.numButtons = (uint32_t)deckChoosingMenu1ButtonDescs.size();
    deckChoosingMenu1Desc.pButtonDescriptions = deckChoosingMenu1ButtonDescs.data();
    deckChoosingMenu1Desc.colors[0] = glm::vec4(0, 0, 0, 1);
    deckChoosingMenu1Desc.colors[1] = glm::vec4(0, 0, 0, 1);
    deckChoosingMenu1Desc.colors[2] = glm::vec4(0, .25, .8, 1);
    deckChoosingMenu1Desc.colors[3] = glm::vec4(0, .25, .8, 1);

    if (!player1DeckChoosingMenu_.Describe(resourceManager.renderer_, deckChoosingMenu1Desc))
    {
        printf("Failed to create player 1's deck choosing menu\n");
        return false;
    }

    pPlayer1CurrentDeckField_ = &player1DeckChoosingMenu_.AddField(0);

    player1DeckChoosingMenuView_.Width = 620;
    player1DeckChoosingMenuView_.Height = 680;
    player1DeckChoosingMenuView_.TopLeftX = 20;
    player1DeckChoosingMenuView_.TopLeftY = 20;

    std::array<Menu::ButtonDescription, 3> deckChoosingMenu2ButtonDescs;
    deckChoosingMenu2ButtonDescs[0].elementDescription.geometry  = NormalizedRect{glm::vec2(-.75, -.3), glm::vec2(0, .055)};
    deckChoosingMenu2ButtonDescs[0].elementDescription.pText     = "NEXT";
    deckChoosingMenu2ButtonDescs[0].pListener = &RacingState::DeckChoosingSubState::Player2NextDeckListener;
    deckChoosingMenu2ButtonDescs[0].pUserData = pDeckChoosingSubState;

    deckChoosingMenu2ButtonDescs[1].elementDescription.geometry  = NormalizedRect{glm::vec2(-.75, -.4), glm::vec2(0, .055)};
    deckChoosingMenu2ButtonDescs[1].elementDescription.pText     = "PREV";
    deckChoosingMenu2ButtonDescs[1].pListener = &RacingState::DeckChoosingSubState::Player2PreviousDeckListener;
    deckChoosingMenu2ButtonDescs[1].pUserData = pDeckChoosingSubState;

    deckChoosingMenu2ButtonDescs[2].elementDescription.geometry  = NormalizedRect{glm::vec2(-.75, -.5), glm::vec2(0, .055)};
    deckChoosingMenu2ButtonDescs[2].elementDescription.pText     = "SELECT";
    deckChoosingMenu2ButtonDescs[2].pListener = &RacingState::DeckChoosingSubState::Player2SelectDeckListener;
    deckChoosingMenu2ButtonDescs[2].pUserData = pDeckChoosingSubState;

    std::array<Menu::LabelDescription, 2> deckChoosingMenu2LabelDescs;
    deckChoosingMenu2LabelDescs[0].elementDescription.geometry = NormalizedRect{glm::vec2(-.72, .75), glm::vec2(0, .23)};
    deckChoosingMenu2LabelDescs[0].elementDescription.pText    = "PLAYER 2";

    deckChoosingMenu2LabelDescs[1].elementDescription.geometry = NormalizedRect{glm::vec2(-.75, 0), glm::vec2(0, .09)};
    deckChoosingMenu2LabelDescs[1].elementDescription.pText    = "Available";

    Menu::Description deckChoosingMenu2Desc;
    deckChoosingMenu2Desc.numLabels = (uint32_t)deckChoosingMenu2LabelDescs.size();
    deckChoosingMenu2Desc.pLabelDescriptions = deckChoosingMenu2LabelDescs.data();
    deckChoosingMenu2Desc.numButtons = (uint32_t)deckChoosingMenu2ButtonDescs.size();
    deckChoosingMenu2Desc.pButtonDescriptions = deckChoosingMenu2ButtonDescs.data();
    deckChoosingMenu2Desc.colors[0] = glm::vec4(0, 0, 0, 1);
    deckChoosingMenu2Desc.colors[1] = glm::vec4(0, 0, 0, 1);
    deckChoosingMenu2Desc.colors[2] = glm::vec4(.8, .25, 0, 1);
    deckChoosingMenu2Desc.colors[3] = glm::vec4(.8, .25, 0, 1);

    if (!player2DeckChoosingMenu_.Describe(resourceManager.renderer_, deckChoosingMenu2Desc))
    {
        printf("Failed to create player 2's deck choosing menu\n");
        return false;
    }

    pPlayer2CurrentDeckField_ = &player2DeckChoosingMenu_.AddField(0);

    player2DeckChoosingMenuView_.Width = 620;
    player2DeckChoosingMenuView_.Height = 680;
    player2DeckChoosingMenuView_.TopLeftX = 640;
    player2DeckChoosingMenuView_.TopLeftY = 20;

    pLoadingField_ = &player1DeckChoosingMenu_.AddField(1);
    NormalizedRect loadingGeo;
    loadingGeo.topLeftPosition = glm::vec2(.6, -.8);
    loadingGeo.extent.y = .05f;
    loadingRenderDescription_ = player1DeckChoosingMenu_.MakeRenderDescription("Loading...", loadingGeo);
    if (!loadingRenderDescription_.IsValid())
    {
        printf("Failed to create loading render description\n");
        return false;
    }


    return true;
}

void ResourceManager::RacingResources::HUDResources::Unload()
{
    countDown1RenderDesc_.DeleteAndNull();
    countDown2RenderDesc_.DeleteAndNull();
    countDown3RenderDesc_.DeleteAndNull();

    player1HUD_.Free();
    player2HUD_.Free();
    pauseMenu_.Free();
    countDownHUD_.Free();
}

//////////////////// Player 1 ///////////////////////////

bool ResourceManager::RacingResources::Player1Resources::Load()
{
    sceneIndex_ = gameSystems_.physicsManager.NewScene();
    return true;
}

void ResourceManager::RacingResources::Player1Resources::Unload()
{
    gameSystems_.physicsManager.ClearScene(sceneIndex_);
}

//////////////////// Player 2 ///////////////////////////

bool ResourceManager::RacingResources::Player2Resources::Load()
{
    sceneIndex_ = gameSystems_.physicsManager.NewScene();
    return true;
}

void ResourceManager::RacingResources::Player2Resources::Unload()
{
    gameSystems_.physicsManager.ClearScene(sceneIndex_);
}

bool ResourceManager::RacingResources::ResultsResources::Load()
{
    NormalizedRect resultsMenuGeo;
    resultsMenuGeo.topLeftPosition = glm::vec2(-.75, .2);
    resultsMenuGeo.extent = glm::vec2(0, .14);

    Menu::LabelDescription winnerLabelDesc;
    winnerLabelDesc.elementDescription.geometry = resultsMenuGeo;
    winnerLabelDesc.elementDescription.pText = "PLAYER 1 WINS!!";

    Menu::Description menuDesc;
    menuDesc.pLabelDescriptions = &winnerLabelDesc;
    menuDesc.pButtonDescriptions = nullptr;
    menuDesc.numButtons = 0;
    menuDesc.numLabels = 1;
    menuDesc.colors[0] = glm::vec4(0, 0, 0, 1);
    menuDesc.colors[1] = glm::vec4(0, 0, 0, 1);
    menuDesc.colors[2] = glm::vec4(0, .25, .8, 1);
    menuDesc.colors[3] = glm::vec4(0, .25, .8, 1);

    if (!player1WinsMenu_.Describe(renderer_, menuDesc))
    {
        printf("Failed to allocate player 2 wins menu\n");
        return false;
    }

    winnerLabelDesc.elementDescription.pText = "PLAYER 2 WINS!!";
    menuDesc.colors[0] = glm::vec4(0, 0, 0, 1);
    menuDesc.colors[1] = glm::vec4(0, 0, 0, 1);
    menuDesc.colors[2] = glm::vec4(.8, .25, 0, 1);
    menuDesc.colors[3] = glm::vec4(.8, .25, 0, 1);

    if (!player2WinsMenu_.Describe(renderer_, menuDesc))
    {
        printf("Failed to allocate player 1 wins menu\n");
        return false;
    }

    return true;
}



} // namespace game
} // namespace ice
