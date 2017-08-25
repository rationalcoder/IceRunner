#ifndef MAPFACTORY_HPP
#define MAPFACTORY_HPP
#include <glm/vec3.hpp>
#include "map.hpp"

namespace ice
{
namespace game
{

class MapFactory
{
public:
    MapFactory()
        : prototype_(), mapKit_(), mapColor_(), playerColor_(), difficuly_()
    {}

    MapKit& GetMapKit() { return mapKit_; }
    const MapKit& GetMapKit() const { return mapKit_; }
    void SetMapKit(MapKit kit) { mapKit_ = kit; }

    glm::vec3 GetPlayerColor() const { return playerColor_; }
    void SetPlayerColor(glm::vec3 color) { playerColor_ = color; }
    glm::vec3 GetGoalColor() const { return goalColor_; }
    void SetGoalColor(glm::vec3 color) { goalColor_ = color; }
    glm::vec3 GetMapColor() const { return mapColor_; }
    void SetMapColor(glm::vec3 color) { mapColor_ = color; }
    glm::vec3 GetWallColor() const { return wallColor_; }
    void SetWallColor(glm::vec3 color) { wallColor_ = color; }

    Map::Difficulty GetDifficulty() const { return difficuly_; }
    void SetDifficulty(Map::Difficulty difficulty) { difficuly_ = difficulty; }

    Map& GetPrototype() { return prototype_; }
    const Map& GetPrototype() const { return prototype_; }
    void SetPrototype(Map prototype) { prototype_ = prototype; }

    Map Make() { return prototype_; }

private:
    Map prototype_;
    MapKit mapKit_;
    glm::vec3 mapColor_;
    glm::vec3 wallColor_;
    glm::vec3 playerColor_;
    glm::vec3 goalColor_;
    Map::Difficulty difficuly_;
};

} // namespace game
} // namespace ice

#endif // MAPFACTORY_HPP
