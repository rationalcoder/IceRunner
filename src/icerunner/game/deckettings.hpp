#ifndef DECKSETTINGS_HPP
#define DECKSETTINGS_HPP
#include <unordered_map>
#include "mapfactory.hpp"

namespace ice
{

namespace game
{

class DeckSettings
{
private:
    using FactoryMap = std::unordered_map<Map::Difficulty, MapFactory>;

public:
    DeckSettings()
        : mapFactories_()
    {}

    using MapFactoryIterator = typename FactoryMap::iterator;
    using CMapFactoryIterator = typename FactoryMap::const_iterator;

public:
    MapFactory& MapFactoryAt(Map::Difficulty difficulty) { return mapFactories_.at(difficulty); }
    const MapFactory& MapFactoryAt(Map::Difficulty difficulty) const { return mapFactories_.at(difficulty); }
    size_t GetSize() const { return mapFactories_.size(); }

    void SetFactoryAt(Map::Difficulty difficulty, const MapFactory& factory)
    {
        mapFactories_.insert({difficulty, factory});
    }

    void Clear() { mapFactories_.clear(); }

private:
    FactoryMap mapFactories_;
};

} // namespace game

} // namespace ice

#endif // DECKSETTINGS_HPP
