#ifndef MAPTOOLS_HPP
#define MAPTOOLS_HPP
#include <cstdint>

namespace ice
{

namespace game
{

namespace map_tools
{


namespace detail
{

class DirectionalExtent
{
public:
    using Rep = int32_t;

public:
    explicit DirectionalExtent(Rep extent)
        : extent_(extent)
    {}

    operator Rep() const { return extent_; }

private:
    Rep extent_;
};

} // namespace detail

struct UpExtent : detail::DirectionalExtent { using detail::DirectionalExtent::DirectionalExtent; };
struct DownExtent : detail::DirectionalExtent { using detail::DirectionalExtent::DirectionalExtent; };
struct LeftExtent : detail::DirectionalExtent { using detail::DirectionalExtent::DirectionalExtent; };
struct RightExtent : detail::DirectionalExtent { using detail::DirectionalExtent::DirectionalExtent; };

} // namespace map_tools

} // namespace game

} // namespace ice

#endif // MAPTOOLS_HPP
