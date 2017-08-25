#ifndef DECKLOADER_H
#define DECKLOADER_H
#include <cstdlib>
#include <string>
#include <vector>
#include "deckettings.hpp"
#include "luainterface.h"

namespace ice
{

namespace game
{

class MapKit;

class Deck
{
private:
    using MapContainer = std::vector<Map*>;

    template <typename WrappedIterator_>
    class CustomMapIterator
    {
    public:
        CustomMapIterator(WrappedIterator_ iter)
            : iter_(iter)
        {}

        typename WrappedIterator_::value_type & operator->() { return **iter_; }

    private:
        WrappedIterator_ iter_;
    };

public:
    using MapIterator = typename CustomMapIterator<typename MapContainer::iterator>;
    using CMapIterator = const CustomMapIterator<typename MapContainer::const_iterator>;

public:
    Deck() = default;

    void AddMap(Map* pMap) { maps_.push_back(pMap); }

    MapIterator begin() { return maps_.begin(); }
    MapIterator end() { return maps_.end(); }

    CMapIterator cbegin() const { return maps_.cbegin(); }
    CMapIterator cend() const { return maps_.cend(); }

    Map::Difficulty GetMaxDifficulty() const { return Map::Difficulty((Map::Difficulty::Rep)(maps_.size())); }

    Map& MapAt(Map::Difficulty difficulty) { return *maps_[difficulty-1]; }
    const Map& MapAt(Map::Difficulty difficulty) const { return *maps_[difficulty-1]; }

    void SetName(const std::string& name) { name_ = name; }
    std::string GetName() const { return name_; }

    MapIterator FindMapWith(Map::Difficulty difficulty)
    {
        auto index = difficulty-1;
        return index < maps_.size() && index > 0 ? (maps_.begin() + index) : end();
    }

    CMapIterator FindMapWith(Map::Difficulty difficulty) const
    {
        auto index = difficulty-1;
        return index < maps_.size() && index > 0 ? (maps_.cbegin() + index) : cend();
    }

private:
    MapContainer maps_;
    std::string name_;
};

class DeckLoader
{
    friend class LuaInterface;
public:
    static DeckLoader& GetInstance()
    {
        static DeckLoader deckLoader;
        return deckLoader;
    }

private:
    class LoadingContext
    {
    public:
        std::vector<Map>* GetCurrentMaps() { return pMaps_; }
        std::vector<Deck>* GetCurrentDecks() { return pDecks_; }
        std::unordered_map<std::string, Map*>* GetCurrentStorageMap() { return pMapNameToStorageMap; }
        void SetCurrentMaps(std::vector<Map>& maps) { pMaps_ = &maps; }
        void SetCurrentDecks(std::vector<Deck>& decks) { pDecks_ = &decks; }
        void SetCurrentStorageMap(std::unordered_map<std::string, Map*>& storageMap) { pMapNameToStorageMap = &storageMap; }

    private:
        std::vector<Map>* pMaps_;
        std::vector<Deck>* pDecks_;
        std::unordered_map<std::string, Map*>* pMapNameToStorageMap;
    };

public:
    bool Init();
    bool Configure();
    bool LoadDecks();
    bool LoadMaps();

    const std::string& GetError() const { return luaInterface_.GetError(); }
    DeckSettings& GetDeckSettings() { return settings_; }
    void ClearAll()
    {
        ClearSettings();
        ClearDecks();
        ClearMaps();
    }

    void ClearSettings()
    {
        settings_.Clear();
    }

    void ClearDecks()
    {
        player1Decks_.clear();
        player2Decks_.clear();
    }

    void ClearMaps()
    {
        player1Maps_.clear();
        player2Maps_.clear();
        player1MapNameToStorageMap_.clear();
        player2MapNameToStorageMap_.clear();
    }

    void SetPlayer1Folder(const std::string& folder)
    {
        player1Folder_ = folder;
    }

    void SetPlayer2Folder(const std::string& folder)
    {
        player2Folder_ = folder;
    }

    //! Called by the LuaInterface. Kind of a hack to keep it public.
    void AddMap(const Map& map) { context_.GetCurrentMaps()->push_back(map); }
    void AddDeck(const Deck& deck) { context_.GetCurrentDecks()->push_back(deck); }
    Map* MapNamed(const std::string& name)
    {
        auto* pCurrentStorageMap = context_.GetCurrentStorageMap();
        auto iter = pCurrentStorageMap->find(name);
        return iter != pCurrentStorageMap->end() ? iter->second : nullptr;
    }

    template <typename DeckHandler_>
    void ForEachDeck(DeckHandler_ handler)
    {
        ForEachPlayer1Deck(handler);
        ForEachPlayer2Deck(handler);
    }

    template <typename DeckHandler_>
    void ForEachPlayer1Deck(DeckHandler_ handler)
    {
        for (auto& deck : player1Decks_)
            handler(deck);
    }

    template <typename DeckHandler_>
    void ForEachPlayer2Deck(DeckHandler_ handler)
    {
        for (auto& deck : player2Decks_)
            handler(deck);
    }


    template <typename MapHandler_>
    void ForEachMap(MapHandler_ handler)
    {
        ForEachPlayer1Map(handler);
        ForEachPlayer2Map(handler);
    }

    template <typename MapHandler_>
    void ForEachPlayer1Map(MapHandler_ handler)
    {
        for (auto& map : player1Maps_)
            handler(map);
    }

    template <typename MapHandler_>
    void ForEachPlayer2Map(MapHandler_ handler)
    {
        for (auto& map : player2Maps_)
            handler(map);
    }

private:
    DeckLoader()
        : settings_(),
          player1Folder_("res\\scripts\\player1\\"),
          player2Folder_("res\\scripts\\player2\\")
    {}

    bool ExportInterface(lua_State* L);

    //! Only for internal use.
    void NameMaps()
    {
        for (auto& map : player1Maps_)
            player1MapNameToStorageMap_.insert({map.GetDescription().name, &map});
        for (auto& map : player2Maps_)
            player2MapNameToStorageMap_.insert({map.GetDescription().name, &map});
    }

private:
    LuaInterface luaInterface_;
    DeckSettings settings_;
    LoadingContext context_;
    std::vector<Deck> player1Decks_;
    std::vector<Deck> player2Decks_;
    std::vector<Map> player1Maps_;
    std::vector<Map> player2Maps_;
    std::unordered_map<std::string, Map*> player1MapNameToStorageMap_;
    std::unordered_map<std::string, Map*> player2MapNameToStorageMap_;
    std::string player1Folder_;
    std::string player2Folder_;
};

} // namespace game

} // namespace ice

#endif // DECKLOADER_H
