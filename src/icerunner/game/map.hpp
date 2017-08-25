#ifndef MAP_HPP
#define MAP_HPP
#include <functional>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <glm/vec2.hpp>
#include "maptools.hpp"
#include "mapkit.hpp"

namespace ice
{
namespace game
{
class MapLoader;

class Map
{
public:
    friend class MapLoader;

private:
    class GridRect
    {
    public:
        using Rep = glm::ivec2::value_type;

    public:
        explicit GridRect(glm::ivec2& pos = glm::ivec2())
            : pos_(pos)
        {}

        GridRect(Rep row, Rep col)
            : pos_(row, col)
        {}

        Rep GetRow() const { return pos_.x; }
        Rep GetColumn() const { return pos_.y; }

    protected:
        glm::ivec2 pos_;
    };

public:

    class Difficulty
    {
    public:
        using Rep = uint32_t;

    public:
        explicit Difficulty(Rep level = 0)
            : level_(level)
        {}

        operator Rep() const { return level_; }

    private:
        Rep level_;
    };

    class Wall : public GridRect
    {
    public:
        using GridRect::GridRect;
        void ShiftCol(Rep col) { pos_.y += col; }
        void ShiftRow(Rep row) { pos_.x += row; }
    };

    struct Player : public GridRect
    {
        using GridRect::GridRect;
    };

    struct Goal : public GridRect
    {
        using GridRect::GridRect;
    };

    struct Description
    {
        Description(const std::string& name = "", Difficulty difficulty = Difficulty(), MapKit kit = MapKit())
            : name(name), kit(kit), difficulty(difficulty)
        {}

        MapKit kit;
        std::string name;
        Difficulty difficulty;
    };

public:
    Map(Description description = Description())
        : walls_(), wallExistenceMap_(description.kit.GetDimensions().numRows * description.kit.GetDimensions().numColumns), description_(description)
    {}

    void AddWall(Wall wall)
    {
        walls_.push_back(wall);
        wallExistenceMap_[wall.GetRow()*wall.GetColumn() + wall.GetColumn()] = true;
    }

    void AddWalls(Wall startWall, map_tools::UpExtent extent)
    {
        int startRow = startWall.GetRow();
        int bound = startRow - extent;
        for (int r = startWall.GetRow(); r >= bound; r--, startWall.ShiftRow(-1))
        {
            walls_.push_back(startWall);
            wallExistenceMap_[startWall.GetRow()*startWall.GetColumn() + startWall.GetColumn()] = true;
        }
    }

    void AddWalls(Wall startWall, map_tools::DownExtent extent)
    {
        int startRow = startWall.GetRow();
        int bound = startRow + extent;
        for (int r = startRow; r <= bound; r++, startWall.ShiftRow(1))
        {
            walls_.push_back(startWall);
            wallExistenceMap_[startWall.GetRow()*startWall.GetColumn() + startWall.GetColumn()] = true;
        }
    }

    void AddWalls(Wall startWall, map_tools::LeftExtent extent)
    {
        int startCol = startWall.GetColumn();
        int bound = startCol - extent;
        for (int c = startCol; c >= bound; c--, startWall.ShiftCol(-1))
        {
            walls_.push_back(startWall);
            wallExistenceMap_[startWall.GetRow()*startWall.GetColumn() + startWall.GetColumn()] = true;
        }
    }

    void AddWalls(Wall startWall, map_tools::RightExtent extent)
    {
        int startCol = startWall.GetColumn();
        int bound = startCol + extent;
        for (int c = startCol; c <= bound; c++, startWall.ShiftCol(1))
        {
            walls_.push_back(startWall);
            wallExistenceMap_[startWall.GetRow()*startWall.GetColumn() + startWall.GetColumn()] = true;
        }
    }

    bool HasWall(Wall wall)
    {
        return wallExistenceMap_[wall.GetRow()*wall.GetColumn() + wall.GetColumn()];
    }

    Player GetPlayer() const { return player_; }
    Goal GetGoal() const { return goal_; }
    void SetPlayer(const Player& player) { player_ = player; }
    void SetGoal(const Goal& goal) { goal_ = goal; }
    MapKit GetKit() const { return description_.kit; }
    void SetKit(MapKit& kit)
    {
        MapKit::Dimensions dimensions = kit.GetDimensions();
        wallExistenceMap_.resize(dimensions.numRows * dimensions.numColumns);
        description_.kit = kit;
    }

    void SetDescription(Description description)
    {
        MapKit::Dimensions dimensions = description.kit.GetDimensions();
        wallExistenceMap_.resize(dimensions.numRows * dimensions.numColumns);
        description_ = description;
    }

    const Description& GetDescription() const { return description_; }

    void Clear()
    {
        walls_.clear();
        wallExistenceMap_.clear();
    }

    std::vector<Wall>::iterator begin() { return walls_.begin(); }
    std::vector<Wall>::iterator end() { return walls_.end(); }

    std::vector<Wall>::const_iterator begin() const { return walls_.cbegin(); }
    std::vector<Wall>::const_iterator end() const { return walls_.cend(); }

private:
    std::string name_;
    std::vector<Wall> walls_;
    std::vector<bool> wallExistenceMap_;
    Description description_;
    Player player_;
    Goal goal_;
};


} // namespace game

} // namespace ice

//! Allow Map::Difficulty to be used as a key in a map.
template <>
struct std::hash<ice::game::Map::Difficulty> : std::hash<ice::game::Map::Difficulty::Rep> {};

#endif // MAP_HPP
