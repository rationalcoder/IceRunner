#ifndef PHYSICSSTRUCTS_HPP
#define PHYSICSSTRUCTS_HPP
#include <vector>
#include <glm/vec2.hpp>

namespace ice
{

namespace physics
{

// This should be in its own header, but MVC is dumb.
namespace constants
{

static constexpr float EPSILON = .0001f;

} // namespace constants

class AABB
{
    friend class Scene;
public:
    AABB(const glm::vec2& min = glm::vec2(),
         const glm::vec2& max = glm::vec2())
        : min_(min), max_(max)
    {}

    bool Intersects(const AABB& other)
    {
        if (max_.x < other.min_.x || min_.x > other.max_.x) return false;
        if (max_.y < other.min_.y || min_.y > other.max_.y) return false;
        return true;
    }

    bool Intersects(const AABB& other, glm::vec2& velocity, glm::vec2& resolutionVector)
    {
        if (max_.x < other.min_.x || min_.x > other.max_.x) return false;
        if (max_.y < other.min_.y || min_.y > other.max_.y) return false;

        // Slow but we are running out of time.
        if (max_.y > other.min_.y && velocity.y > 0)
        {
            resolutionVector.y = -(max_.y - other.min_.y) - constants::EPSILON;
            resolutionVector.x = 0;
        }
        else if (min_.y < other.max_.y && velocity.y < 0)
        {
            resolutionVector.y = other.max_.y - min_.y + constants::EPSILON;
            resolutionVector.x = 0;
        }
        else if (max_.x > other.min_.x && velocity.x > 0)
        {
            resolutionVector.x = -(max_.x - other.min_.x) - constants::EPSILON;
            resolutionVector.y = 0;
        }
        else if (min_.x < other.max_.x && velocity.x < 0)
        {
            resolutionVector.x = other.max_.x - min_.x + constants::EPSILON;
            resolutionVector.y = 0;
        }

        return true;
    }

    void SetMin(glm::vec2& min) { min_ = min; }
    void SetMax(glm::vec2& max) { max_ = max; }
    glm::vec2& GetMin() { return min_; }
    glm::vec2& GetMax() { return max_; }

private:
    glm::vec2 min_;
    glm::vec2 max_;
};

struct Player
{
    AABB boundingBox;
    glm::vec2 oldVelocity;
    glm::vec2 velocity;

    bool IsStopped() const { return velocity.x == 0 && velocity.y == 0; }
    bool WasStopped() const { return oldVelocity.x == 0 && oldVelocity.y == 0; }
};

struct Goal
{
    AABB boundingBox;
};

class Scene
{
public:
    struct Description
    {
        Player player;
        Goal goal;
    };

public:
    Scene(const Description& desc);

    void Clear();
    void SetDescription(const Description& desc);
    void SetPlayer(Player p) { player_ = p; }
    Player& GetPlayer() { return player_; }
    void Reset()
    {
        player_.velocity.x = 0;
        player_.velocity.y = 0;
        player_.boundingBox = originalPlayerAABB_;
        hitWall_ = false;
        hitGoal_ = false;
    }

    void SetGoal(Goal g) { goal_ = g; }
    void Simulate();
    bool HitGoal() const { return hitGoal_; }
    bool HitWall() const { return hitWall_; }
    void Pause() { paused_ = true; }
    void Unpause() { paused_ = false; }

    void AddWall(const AABB& wall)
    {
        if (screenQuads_[0].Intersects(wall)) wallQuads_[0].push_back(wall);
        if (screenQuads_[1].Intersects(wall)) wallQuads_[1].push_back(wall);
        if (screenQuads_[2].Intersects(wall)) wallQuads_[2].push_back(wall);
        if (screenQuads_[3].Intersects(wall)) wallQuads_[3].push_back(wall);
    }

private:
    static AABB screenQuads_[4]; // top left, top right, bottom right, bottom left
    std::vector<AABB> wallQuads_[4]; // // top left, top right, bottom right, bottom left
    Player player_;
    Goal goal_;
    AABB originalPlayerAABB_;
    bool hitGoal_;
    bool hitWall_;
    bool paused_;
};

class SceneIndex
{
public:
    explicit SceneIndex(int32_t index = -1)
        : index_(index)
    {}

    operator int32_t() const { return index_; }
    bool IsValid() const { return index_ != -1; }

private:
    int32_t index_;
};

} // namespace physics

} // namespace ice

#endif // PHYSICSSTRUCTS_HPP
