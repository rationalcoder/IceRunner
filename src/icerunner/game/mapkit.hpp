#ifndef MAPKIT_HPP
#define MAPKIT_HPP
#include <cstdint>

namespace ice
{

namespace game
{

class MapKit
{
public:
    class WallCount
    {
    public:
        explicit WallCount(uint32_t count = 0)
            : count_(count)
        {}

        operator uint32_t() const { return count_; }
        WallCount& operator = (uint32_t rhs)
        {
            count_ = rhs;
            return *this;
        }

    private:
        uint32_t count_;
    };

    struct Dimensions
    {
        Dimensions()
            : numRows(), numColumns()
        {}

        Dimensions(uint32_t numRows, uint32_t numColumns)
            : numRows(numRows), numColumns(numColumns)
        {}

        uint32_t numRows;
        uint32_t numColumns;
    };

public:
    struct Description
    {
        Description(Dimensions dimensions = Dimensions(), WallCount wallCount = WallCount())
            : dimensions(dimensions), wallCount(wallCount)
        {}

        Dimensions dimensions;
        WallCount wallCount;
    };

public:
    explicit MapKit(Description description = Description())
        : description_(description)
    {}

    Description GetDescription() const { return description_; }
    void SetDescription(Description description) { description_ = description; }
    Dimensions GetDimensions() const { return description_.dimensions; }
    void SetDimensions(Dimensions dimensions) { description_.dimensions = dimensions; }
    WallCount GetWallCount() const { return description_.wallCount; }

private:
    Description description_;
};

} // namespace game

} // namespace ice

#endif // MAPKIT_HPP
