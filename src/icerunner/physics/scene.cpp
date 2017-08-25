#include "physicsstructs.hpp"

namespace ice
{

namespace physics
{

AABB Scene::screenQuads_[4] =
{
    {glm::vec2(0, 0), glm::vec2(.5, .5)}, // top left
    {glm::vec2(.5, 0), glm::vec2(1, .5)}, // top right
    {glm::vec2(.5, .5), glm::vec2(1, 1)}, // bottom right
    {glm::vec2(0, .5), glm::vec2(.5, 1)}, // bottom left
};

Scene::Scene(const Description& desc)
    : hitGoal_(false), hitWall_(false), paused_(true)
{
    SetDescription(desc);
}

void Scene::SetDescription(const Description& desc)
{
    goal_ = desc.goal;
    player_ = desc.player;
    originalPlayerAABB_ = player_.boundingBox;
}

//!
void Scene::Simulate()
{
    if (paused_) return;

    player_.oldVelocity = player_.velocity;

    if (player_.IsStopped()) return;
    else if (hitWall_ || hitGoal_)
    {
        // If the player is moving this frame and we were
        // stopped last frame, set our old velocity to zero
        // instead of the current velocity.
        player_.oldVelocity = glm::vec2();
    }

    if (hitGoal_) hitGoal_ = false;
    if (hitWall_) hitWall_ = false;

    //printf("%d, %d, %d, %d\n", (int)wallQuads_[0].size(), (int)wallQuads_[1].size(), (int)wallQuads_[2].size(), (int)wallQuads_[3].size());


    glm::vec2 resolutionVector;

    player_.boundingBox.min_ += player_.velocity;
    player_.boundingBox.max_ += player_.velocity;

    AABB newBounding = player_.boundingBox;
    for (int i = 0; i < 4; i++)
    {
        if (newBounding.Intersects(screenQuads_[i]))
        {
            for (const AABB& wall : wallQuads_[i])
            {
                if (newBounding.Intersects(wall, player_.velocity, resolutionVector))
                {
                    player_.boundingBox.min_ += resolutionVector;
                    player_.boundingBox.max_ += resolutionVector;
                    player_.velocity.x = 0;
                    player_.velocity.y = 0;
                    hitWall_ = true;
                    return;
                }
            }
        }
    }

    if (newBounding.Intersects(goal_.boundingBox))
    {
        hitGoal_ = true;
        player_.velocity.x = 0;
        player_.velocity.y = 0;
    }
}

void Scene::Clear()
{
    wallQuads_[0].clear();
    wallQuads_[1].clear();
    wallQuads_[2].clear();
    wallQuads_[3].clear();

    Reset();
    Pause();
}

} // namespace physics

} // namespace ice
