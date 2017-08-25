#ifndef MAPRENDERER_H
#define MAPRENDERER_H
#include <d3d11.h>
#include <vector>
#include "types.hpp"
#include <glm/vec2.hpp>
#include "rendering/dx11fwd.hpp"
#include "maputility.hpp"

namespace ice
{

namespace game
{

class Map;
class ResourceManager;

class MapRenderer
{
public:
    class Scene;
    friend class Scene;

private:
    using PlayerSetterFunc = void(MapRenderer::Scene& scene, glm::vec2 playerPos);

public:
    class Scene
    {
        friend class MapRenderer;
    public:
        Scene()
            : pPlayerGeometry_(nullptr), pWallGeometry_(nullptr),
              pGoalGeometry_(nullptr), pPlayerSetterFunc_(nullptr)
        {}

        void SetPlayer(const glm::vec2& pos) { pPlayerSetterFunc_(*this, pos); }
        const D3D11_VIEWPORT& GetView() const { return fullView_; }
        D3D11_VIEWPORT& GetView() { return fullView_; }

    private:
        std::vector<D3D11_VIEWPORT> wallViews_;
        D3D11_VIEWPORT playerView_;
        D3D11_VIEWPORT goalView_;
        D3D11_VIEWPORT fullView_;
        dx11rendering::Geometry* pPlayerGeometry_;
        dx11rendering::Geometry* pWallGeometry_;
        dx11rendering::Geometry* pGoalGeometry_;
        PlayerSetterFunc* pPlayerSetterFunc_;
    };

public:
    static MapRenderer& GetInstance(types::GameSystems& gameSystems)
    {
        static MapRenderer mapRenderer(gameSystems);
        return mapRenderer;
    }

    bool MakeFullScene(const MapUtility::SceneInfo& sceneInfo, Scene& scene);
    bool MakePlayer1Scene(const MapUtility::SceneInfo& sceneInfo, Scene& scene);
    bool MakePlayer2Scene(const MapUtility::SceneInfo& sceneInfo, Scene& scene);

    static void FullSceneSetPlayer(Scene& scene, glm::vec2 playerPos);
    static void Player1SceneSetPlayer(Scene& scene, glm::vec2 playerPos);
    static void Player2SceneSetPlayer(Scene& scene, glm::vec2 playerPos);

    void FreeScene(Scene& scene);
    void RenderScene(const Scene& scene);

private:
    MapRenderer(types::GameSystems& gameSystems);
    bool MakeSceneCommonSetup(const MapUtility::SceneInfo& sceneInfo, Scene& scene);

private:
    dx11rendering::DX11Renderer& renderer_;
    ResourceManager& resourceManager_;
};

} // namespace game

} // namespace ice

#endif // MAPRENDERER_H
