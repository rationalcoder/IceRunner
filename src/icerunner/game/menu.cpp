#include <SDL2/SDL_pixels.h>
#include "game/gamerenderstructs.hpp"
#include "rendering/dx11renderer.h"
#include "resourcemanager.h"
#include "core/utility.hpp"

namespace ice
{

namespace game
{

Menu::~Menu()
{
    Free();
}

void Menu::Free()
{
    core::utility::DeleteAndNull(pMenuGeometry_);

    for(Button& button : buttons_)
    {
        core::utility::DeleteAndNull(button.pGeometry);
        core::utility::DeleteAndNull(button.pTextTexture);
    }

    for(Label& label : labels_)
    {
        core::utility::DeleteAndNull(label.pGeometry);
        core::utility::DeleteAndNull(label.pTextTexture);
    }
}

bool Menu::Describe(dx11rendering::DX11Renderer& renderer, const Description& description)
{
    buttons_.reserve(description.numButtons);
    labels_.reserve(description.numLabels);
    fields_.reserve(10); // FIXME: hack

    dx11rendering::DX11Renderer::ResourceLoader& resourceLoader = renderer.GetResourceLoader();
    pMenuGeometry_ = resourceLoader.CreateGeometry();

    if (!pMenuGeometry_)
    {
        printf("Failed to allocate high level menu resources\n");
        this->Free();
        return false;
    }

    // 2 triangles: (top left, top right, bottom left), (top right, bottom right, bottom left)
    // The fact that they are common would suggest reuse, but we don't really care about performance right now.
    uint32_t commonIndices[] = { 0, 1, 3, 1, 2, 3 };
    uint32_t numIndices = sizeof(commonIndices)/sizeof(uint32_t);
    uint32_t numVertices = 4; // always drawing rectangles.

    ColoredVertex menuVertices[] =
    {
        {glm::vec2(-1, 1), description.colors[0]}, // top left
        {glm::vec2(1, 1), description.colors[1]}, // top right
        {glm::vec2(1, -1), description.colors[2]}, // bottom right
        {glm::vec2(-1, -1), description.colors[3]} // bottom left
    };

    // Create the menu geometry.
    dx11rendering::Geometry::Description menuGeometryDesc;
    menuGeometryDesc.numIndices = numIndices;
    menuGeometryDesc.numVertices = numVertices;
    menuGeometryDesc.pVertices = menuVertices;
    menuGeometryDesc.pIndices = commonIndices;
    menuGeometryDesc.vertexSize = sizeof(ColoredVertex);
    menuGeometryDesc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    if (!pMenuGeometry_->Describe(&menuGeometryDesc))
    {
        printf("Failed to allocate menu geometry\n");
        this->Free();
        return false;
    }

    // For each button description, create the buttons geometry and texture, set them in a button,
    // and add the new button to the vector.
    ButtonDescription* pButtonDescs = description.pButtonDescriptions;
    for (uint32_t i = 0; i < description.numButtons; i++)
    {
        ButtonDescription description = pButtonDescs[i];

        dx11rendering::Texture* pTextTexture = resourceLoader.CreateTextureFromFont("fonts/underwhelmed.ttf", description.elementDescription.pText, 70, SDL_Color{0, 220, 255, 255});
        if (!pTextTexture)
        {
            printf("Failed to allocate button texture; index: %u\n", i);
            this->Free();
            return false;
        }

        NormalizedRect& buttonGeometry = description.elementDescription.geometry;
        glm::vec2 extent = buttonGeometry.extent;
        if (extent.x == 0)
            extent.x = extent.y*pTextTexture->GetAspectRatio();

        TexturedVertex vertices[] =
        {
            {buttonGeometry.topLeftPosition, glm::vec2(0, 0)}, // top left
            {buttonGeometry.topLeftPosition + glm::vec2(extent.x, 0), glm::vec2(1, 0)}, // top right
            {buttonGeometry.topLeftPosition + glm::vec2(extent.x, -extent.y), glm::vec2(1, 1)}, // bottom right
            {buttonGeometry.topLeftPosition + glm::vec2(0, -extent.y), glm::vec2(0, 1)} // bottom left
        };

        dx11rendering::Geometry::Description newButtonDesc;
        newButtonDesc.numIndices = numIndices;
        newButtonDesc.numVertices = numVertices;
        newButtonDesc.pIndices = commonIndices;
        newButtonDesc.pVertices = vertices;
        newButtonDesc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        newButtonDesc.vertexSize = sizeof(TexturedVertex);

        Button newButton;
        newButton.pListener = pButtonDescs[i].pListener;
        newButton.pGeometry = resourceLoader.CreateGeometry();
        newButton.pUserData = pButtonDescs[i].pUserData;
        newButton.pTextTexture = pTextTexture;
        if (!newButton.pGeometry)
        {
            printf("Failed to allocate button geometry; index: %u\n", i);
            this->Free();
            return false;
        }

        if(!newButton.pGeometry->Describe(&newButtonDesc))
        {
            printf("Failed to allocate button geometry; index: %u\n", i);
            this->Free();
            return false;
        }


        buttons_.push_back(newButton);
    }

    // For each label description, create the labels geometry and texture, set them in a label,
    // and add the new label to the vector.
    LabelDescription* pLabelDescription = description.pLabelDescriptions;
    for (uint32_t i = 0; i < description.numLabels; i++)
    {
        LabelDescription description = pLabelDescription[i];

        dx11rendering::Texture* pTextTexture = resourceLoader.CreateTextureFromFont("fonts/underwhe.ttf", description.elementDescription.pText, 70, SDL_Color{0, 220, 255, 255});
        if(!pTextTexture)
        {
            printf("Failed to allocate label geometry; index: %u\n", i);
            this->Free();
            return false;
        }

        NormalizedRect& labelGeometry = description.elementDescription.geometry;
        glm::vec2 extent = labelGeometry.extent;
        if (extent.x == 0)
            extent.x = extent.y*pTextTexture->GetAspectRatio();

        TexturedVertex vertices[] =
        {
            {labelGeometry.topLeftPosition, glm::vec2(0, 0)}, // top left
            {labelGeometry.topLeftPosition + glm::vec2(extent.x, 0), glm::vec2(1, 0)}, // top right
            {labelGeometry.topLeftPosition + glm::vec2(extent.x, -extent.y), glm::vec2(1, 1)}, // bottom right
            {labelGeometry.topLeftPosition + glm::vec2(0, -extent.y), glm::vec2(0, 1)} // bottom left
        };

        dx11rendering::Geometry::Description newLabelDesc;
        newLabelDesc.numIndices = numIndices;
        newLabelDesc.numVertices = numVertices;
        newLabelDesc.pIndices = commonIndices;
        newLabelDesc.pVertices = vertices;
        newLabelDesc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        newLabelDesc.vertexSize = sizeof(TexturedVertex);

        Label newLabel;
        newLabel.pTextTexture = pTextTexture;
        newLabel.pGeometry = resourceLoader.CreateGeometry();
        if (!newLabel.pGeometry)
        {
            printf("Failed to allocate label geometry; index: %u\n", i);
            this->Free();
            return false;
        }

        if(!newLabel.pGeometry->Describe(&newLabelDesc))
        {
            printf("Failed to allocate label geometry; index: %u\n", i);
            this->Free();
            return false;
        }


        labels_.push_back(newLabel);
    }

    if (!buttons_.empty()) focusedButtonPos_ = 1;

    ResourceManager& resourceManager = ResourceManager::GetInstance(gameSystems_);
    pElementConstantBuffer_ = resourceManager.GetConstantBuffer(ResourceManager::ConstantBuffer::eFocusableTextured);
    pElementShader_ = resourceManager.GetShader(ResourceManager::Shader::eFocusableTextured);

    return true;
}

void Menu::Render()
{
    ResourceManager& resourceManager = ResourceManager::GetInstance(gameSystems_);
    dx11rendering::DX11Renderer& renderer = gameSystems_.displayManager.GetRenderer();

    dx11rendering::RenderPass::PipelineDescription pipelineDesc;
    pipelineDesc.pProgram = resourceManager.GetShader(ResourceManager::Shader::eColored);
    pipelineDesc.pPSConstantBuffer = nullptr;
    pipelineDesc.pVSConstantBuffer = nullptr;
    pipelineDesc.pTexture = nullptr;

    dx11rendering::RenderPass menuPass(*pMenuGeometry_, pipelineDesc);
    renderer.Draw(menuPass);

    dx11rendering::RenderPass::PipelineDescription labelPipelineDesc = {};
    labelPipelineDesc.pProgram = pElementShader_;
    labelPipelineDesc.pPSConstantBuffer = pElementConstantBuffer_;

    for (auto& label : labels_)
    {
        labelPipelineDesc.pTexture = label.pTextTexture;
        dx11rendering::RenderPass labelPass(*label.pGeometry, labelPipelineDesc);
        renderer.Draw(labelPass);
    }

    dx11rendering::RenderPass::PipelineDescription fieldPipelineDesc = {};
    fieldPipelineDesc.pProgram = pElementShader_;
    fieldPipelineDesc.pPSConstantBuffer = pElementConstantBuffer_;

    for (auto& field : fields_)
    {
        if (!field.desc_.IsValid()) continue;
        fieldPipelineDesc.pTexture = field.desc_.pTexture;
        dx11rendering::RenderPass fieldPass(*field.desc_.pGeometry, fieldPipelineDesc);
        renderer.Draw(fieldPass);
    }

    FocusableConstantBuffer selectedBuffer;
    selectedBuffer.isSelected.x = 1.0f;

    FocusableConstantBuffer unselectedBuffer;
    unselectedBuffer.isSelected.x = 0;

    dx11rendering::RenderPass::PipelineDescription buttonPipelineDesc;
    buttonPipelineDesc.pProgram = pElementShader_;
    buttonPipelineDesc.pPSConstantBuffer = pElementConstantBuffer_;
    buttonPipelineDesc.pVSConstantBuffer = nullptr;

    int focusIndex = focusedButtonPos_-1;
    for (int i = 0; i < (int)buttons_.size(); i++)
    {
        buttonPipelineDesc.pTexture = buttons_[i].pTextTexture;

        dx11rendering::RenderPass buttonPass(*buttons_[i].pGeometry, buttonPipelineDesc);
        buttonPass.Bind();

        if (i == focusIndex)
        {
            pElementConstantBuffer_->Update(&selectedBuffer);
            renderer.Draw(*buttons_[i].pGeometry);
            pElementConstantBuffer_->Update(&unselectedBuffer);
        }
        else
        {
            renderer.Draw(*buttons_[i].pGeometry);
        }
    }
}

Menu::ElementRenderDescription
Menu::MakeRenderDescription(const char *pText, const NormalizedRect& geometry, const D3D11_VIEWPORT& view)
{
    dx11rendering::DX11Renderer::ResourceLoader& loader = displayManager_.GetRenderer().GetResourceLoader();
    dx11rendering::Texture* pTextTexture = loader.CreateTextureFromFont("fonts/underwhe.ttf", pText, 60, SDL_Color{0, 220, 255, 255});

    Menu::ElementRenderDescription desc;
    if (!pTextTexture)
    {
        printf("Unable to create texture for field: %s\n", pText);
        return desc;
    }

    float width = geometry.extent.x;
    float height = geometry.extent.y;
    // if width is zero, figure out what its width should be given that this menu is
    // going to be rendered given this view port.
    if (width == 0)
    {
        // width = (inverse of fraction of full width that we are rendering at) *
        //         (height * (fraction of full height that we are rendering at) * aspect ratio)
        width = (((height*1280)/view.Width)*(view.Height/720))*pTextTexture->GetAspectRatio();
    }

    glm::vec2 topLeft = geometry.topLeftPosition;

    TexturedVertex vertices[] =
    {
        {topLeft, glm::vec2(0, 0)}, // top left
        {topLeft + glm::vec2(width, 0), glm::vec2(1, 0)}, // top right
        {topLeft + glm::vec2(width, -height), glm::vec2(1, 1)}, // bottom right
        {topLeft + glm::vec2(0, -height), glm::vec2(0, 1)} // bottom left
    };

    // FIXME: Code duplication.
    uint32_t commonIndices[] = { 0, 1, 3, 1, 2, 3 };
    uint32_t numIndices = sizeof(commonIndices)/sizeof(uint32_t);
    uint32_t numVertices = 4; // always drawing rectangles.

    dx11rendering::Geometry::Description newFieldDesc;
    newFieldDesc.numIndices = numIndices;
    newFieldDesc.numVertices = numVertices;
    newFieldDesc.pIndices = commonIndices;
    newFieldDesc.pVertices = vertices;
    newFieldDesc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    newFieldDesc.vertexSize = sizeof(TexturedVertex);

    dx11rendering::Geometry* pFieldGeometry = loader.CreateGeometry();
    if (!pFieldGeometry || !pFieldGeometry->Describe(&newFieldDesc))
    {
        printf("Unable to create geometry for field: %s\n", pText);
        delete pTextTexture;
    }

    desc.pTexture = pTextTexture;
    desc.pGeometry = pFieldGeometry;
    return desc;
}

Menu::Field& Menu::AddField(int id, ElementRenderDescription renderDesc)
{
    fields_.emplace_back(id, renderDesc);
    return fields_.back();
}

void Menu::FocusNext()
{
    if (buttons_.empty()) return;

    if (focusedButtonPos_ == buttons_.size()) focusedButtonPos_ = 1;
    else focusedButtonPos_++;
}

void Menu::FocusPrevious()
{
    if (buttons_.empty()) return;

    if (focusedButtonPos_ == 1) focusedButtonPos_ = (uint32_t)buttons_.size();
    else focusedButtonPos_--;
}

void Menu::Press()
{
    if (buttons_.empty()) return;
    Button& focusedButton = buttons_[focusedButtonPos_-1];
    focusedButton.pListener(focusedButton.pUserData);
}

} // namespace game

} // namespace ice
