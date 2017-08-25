#ifndef GAMERENDERSTRUCTS_HPP
#define GAMERENDERSTRUCTS_HPP
#include <vector>
#include <string>
#include <unordered_map>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "rendering/dx11structs.h"
#include "types.hpp"
#include "core/utility.hpp"

namespace ice
{

namespace game
{


// Vertex Types
struct PositionVertex
{
    glm::vec2 position;
};

struct ColoredVertex
{
    glm::vec2 position;
    glm::vec4 color;
};

struct TexturedVertex
{
    glm::vec2 position;
    glm::vec2 uv;
};

// Misc

struct NormalizedRect
{
    glm::vec2 topLeftPosition;
    glm::vec2 extent;
};

// Constant Buffers

struct FocusableConstantBuffer
{
    glm::vec4 isSelected; // only x used as boolean value;
};

struct BackgroundConstantBuffer
{
    glm::vec4 mixColor;
    glm::vec4 phaseShift;
};

class Menu
{
public:
    using ButtonListener = void(void* pUserData);

public:
    struct ElementDescription
    {
        const char* pText;
        NormalizedRect geometry;
    };

    struct ButtonDescription
    {
        ElementDescription elementDescription;
        ButtonListener* pListener;
        void* pUserData;
    };

    struct LabelDescription
    {
        ElementDescription elementDescription;
    };

    struct Description
    {
        // FIXME: hack.
        glm::vec4 colors[4] = {glm::vec4{0, 0, 0, 1}, glm::vec4{0, .45, 1, 1},
                                glm::vec4{0, 0, 0, 1}, glm::vec4{0, .45, 1, 1}}; // top left, top right, bottom right, bottom left.

        ButtonDescription* pButtonDescriptions;
        LabelDescription* pLabelDescriptions;
        uint32_t numButtons;
        uint32_t numLabels;
    };

    struct ElementRenderDescription
    {
        ElementRenderDescription()
            :  pGeometry(nullptr), pTexture(nullptr)
        {}

        dx11rendering::Geometry* pGeometry;
        dx11rendering::Texture* pTexture;

        bool IsValid() const { return pGeometry && pTexture; }
        void DeleteAndNull()
        {
            core::utility::DeleteAndNull(pGeometry);
            core::utility::DeleteAndNull(pTexture);
        }
    };

    class Field
    {
        friend class Menu;
    public:
        Field(int id, ElementRenderDescription desc)
            : desc_(desc), id_(id)
        {}

        int GetID() const { return id_; }
        void SetID(int id) { id_ = id; }
        void SetRenderDescription(ElementRenderDescription desc)
        {
            desc_ = desc;
        }

        void ClearRenderDescription()
        {
            desc_ = Menu::ElementRenderDescription{};
        }

        ElementRenderDescription GetRenderDescription() const { return desc_; }

    private:
        ElementRenderDescription desc_;
        int id_;
    };

private:
    struct Button
    {
        Button()
            : pGeometry(nullptr), pTextTexture(nullptr), pListener(nullptr)
        {}

        dx11rendering::Geometry* pGeometry;
        dx11rendering::Texture* pTextTexture;
        ButtonListener* pListener;
        void* pUserData;
    };

    struct Label
    {
        Label()
            : pGeometry(nullptr), pTextTexture(nullptr)
        {}

        dx11rendering::Geometry* pGeometry;
        dx11rendering::Texture* pTextTexture;
    };

public:
    Menu(types::GameSystems& gameSystems)
        : pMenuGeometry_(nullptr), pMenuShader_(nullptr), pElementShader_(nullptr),
          pElementConstantBuffer_(nullptr), displayManager_(gameSystems.displayManager), gameSystems_(gameSystems), focusedButtonPos_(0)
    {}

    ~Menu();

    void Free();

    bool Describe(dx11rendering::DX11Renderer& renderer, const Description& description);
    void Render();

    // FIXME: hack
    ElementRenderDescription MakeRenderDescription(const char* pText,
                                                   const NormalizedRect& geometry,
                                                   const D3D11_VIEWPORT& view = D3D11_VIEWPORT{0, 0, 1280, 720});
    Field& AddField(int id, ElementRenderDescription renderDesc = ElementRenderDescription{});
    void FocusNext();
    void FocusPrevious();
    void Press();

private:
    std::vector<Button> buttons_;
    std::vector<Label> labels_;
    std::vector<Field> fields_;
    dx11rendering::Geometry* pMenuGeometry_;
    dx11rendering::ShaderProgram* pMenuShader_;
    dx11rendering::ShaderProgram* pElementShader_;
    dx11rendering::ShaderConstantBuffer* pElementConstantBuffer_;
    rendering::DisplayManager& displayManager_;
    types::GameSystems& gameSystems_;
    uint32_t focusedButtonPos_;
};

class Background
{
    friend class ResourceManager;
public:
    Background();

    void SetColor(glm::vec3 color)
    {
        constantBufferData_.mixColor = glm::vec4(color, 1.0f);
    }

    void SetAnimationSpeed(float speed)
    {
        animationSpeed_ = speed;
    }

    void Animate();
    void Render(bool animate = true); // FIXME: hack.

private:
    void SetRenderInfo(dx11rendering::DX11Renderer& renderer, dx11rendering::RenderPass& pass);

private:
    BackgroundConstantBuffer constantBufferData_;
    dx11rendering::ShaderConstantBuffer* pConstantBuffer_;
    dx11rendering::Geometry* pGeometry_;
    dx11rendering::DX11Renderer* pRenderer_;
    dx11rendering::RenderPass* pRenderPass_;
    float animationSpeed_;
};

class Map;
struct MapSceneAssociation
{
    MapSceneAssociation()
        : pMap(nullptr)
    {}

    ~MapSceneAssociation()
    {
        Leak();
    }

    void DeleteAndNull()
    {
        core::utility::DeleteAndNull(renderDescription.pGeometry);
        core::utility::DeleteAndNull(renderDescription.pTexture);
        pMap = nullptr; // pMap is just a handle.
    }

    void Leak()
    {
        renderDescription.pGeometry = nullptr;
        renderDescription.pTexture = nullptr;
        pMap = nullptr;
    }

    Menu::ElementRenderDescription renderDescription;
    Map* pMap;
};

class Deck;
struct DeckSceneAssociation
{
    DeckSceneAssociation()
        : pDeck(nullptr)
    {}

    ~DeckSceneAssociation()
    {
        Leak();
    }

    void DeleteAndNull()
    {
        core::utility::DeleteAndNull(renderDescription.pGeometry);
        core::utility::DeleteAndNull(renderDescription.pTexture);
        pDeck = nullptr; // pDeck is just a handle.
    }

    void Leak()
    {
        renderDescription.pGeometry = nullptr;
        renderDescription.pTexture = nullptr;
        pDeck = nullptr;
    }

    Menu::ElementRenderDescription renderDescription;
    Deck* pDeck;
};

} // namespace game

} // namespace ice

#endif // GAMERENDERSTRUCTS_HPP
