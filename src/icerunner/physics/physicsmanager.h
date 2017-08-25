#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H
#include <cstdio>
#include "core/constants.hpp"
#include "physicsstructs.hpp"

namespace ice
{

namespace physics
{

class PhysicsManager
{
public:
    bool Init() { return true; }

    void Simulate()
    {
        for (auto& scene : scenes_)
            scene.Simulate();
    }

    SceneIndex NewScene(const Scene::Description& desc = Scene::Description())
    {
        scenes_.emplace_back(desc);
        return SceneIndex((int32_t)(scenes_.size()-1));
    }

    Scene& SceneAt(SceneIndex index)
    {
        return scenes_[(int32_t)index];
    }

    void ClearScenes()
    {
        for (auto& scene : scenes_)
            scene.Clear();
        scenes_.clear();
    }

    void ClearScene(SceneIndex index)
    {
        scenes_[(int32_t)index].Clear();
    }

private:
    std::vector<Scene> scenes_;
};

} // namespace physics

} // namespace ice

#endif // PHYSICSMANAGER_H
