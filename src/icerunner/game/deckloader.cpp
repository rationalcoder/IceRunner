#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <utility>
#include <algorithm>
#include <lua.hpp>
#include "core/utility.hpp"
#include "deckloader.h"
#include "map.hpp"

namespace ice
{

namespace game
{

bool DeckLoader::Init()
{
    return luaInterface_.Init() && luaInterface_.Export();
}

bool DeckLoader::Configure()
{
    ClearSettings();
    return luaInterface_.RunFile("res/scripts/config.lua");
}

bool DeckLoader::LoadDecks()
{
    ClearDecks();
    if (!LoadMaps())
    {
        printf("Error: %s\n", luaInterface_.GetError().c_str());
        return false;
    }

    context_.SetCurrentDecks(player1Decks_);
    context_.SetCurrentStorageMap(player1MapNameToStorageMap_);
    bool success = false;
    std::string player1DecksFolder = player1Folder_ + "decks\\";
    core::utility::ForEachFile(player1DecksFolder, [this, &success, &player1DecksFolder](const std::string& fileName) {
        success = luaInterface_.RunFile(player1DecksFolder + fileName);
    });
    if (!success)
        printf("Error during deck loading: %s\n", luaInterface_.GetError().c_str());

    context_.SetCurrentDecks(player2Decks_);
    context_.SetCurrentStorageMap(player2MapNameToStorageMap_);
    std::string player2DecksFolder = player2Folder_ + "decks\\";
    core::utility::ForEachFile(player2DecksFolder, [this, &success, &player2DecksFolder](const std::string& fileName) {
        success = luaInterface_.RunFile(player2DecksFolder + fileName);
    });
    if (!success)
        printf("Error during deck loading: %s\n", luaInterface_.GetError().c_str());

    return success;
}

bool DeckLoader::LoadMaps()
{
    ClearMaps();

    context_.SetCurrentMaps(player1Maps_);
    bool success = false;
    std::string player1MapsFolder = player1Folder_ + "maps\\";
    core::utility::ForEachFile(player1MapsFolder, [this, &success, &player1MapsFolder](const std::string& fileName) {
        success = luaInterface_.RunFile(player1MapsFolder + fileName);
    });
    if (!success)
        printf("Error during map loading: %s\n", luaInterface_.GetError().c_str());

    context_.SetCurrentMaps(player2Maps_);
    std::string player2MapsFolder = player2Folder_ + "maps\\";
    core::utility::ForEachFile(player2MapsFolder, [this, &success, &player2MapsFolder](const std::string& fileName) {
        success = luaInterface_.RunFile(player2MapsFolder + fileName);
    });
    if (!success)
        printf("Error during map loading: %s\n", luaInterface_.GetError().c_str());

    NameMaps();
    return success;
}

} // namespace game

} // namespace ice
