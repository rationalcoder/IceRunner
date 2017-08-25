#include <algorithm>
#include "core/utility.hpp"
#include "rendering/dx11renderer.h"
#include "resourcemanager.h"
#include "gamerenderstructs.hpp"
#include "sound/soundmanager.h"

namespace ice
{

namespace game
{

bool ResourceManager::Load()
{
    bool result = LoadBasicResources() &&
                  mainMenuResources_.Load() &&
                  mapTestingResources_.Load() &&
                  racingResources_.Load();

    LoadViews();
    LoadPasses();
    LoadAggregates();
    return result;
}

void ResourceManager::Unload()
{
    mainMenuResources_.Unload();
    mapTestingResources_.Unload();
    racingResources_.Unload();

    dj_.Unload();
    FreeResources();
}

void ResourceManager::FreeResources()
{
    core::utility::DeleteAndNull(shaders_);
    core::utility::DeleteAndNull(constantBuffers_);
    core::utility::DeleteAndNull(soundEffects_);
    core::utility::DeleteAndNull(pFullScreenQuad_);
}

ResourceManager::~ResourceManager()
{
    // TODO: check for leaks... (singleton destructor is otherwise useless).
}

bool ResourceManager::LoadBasicResources()
{
    dx11rendering::DX11Renderer::ResourceLoader& resourceLoader = renderer_.GetResourceLoader();
    shaders_[(size_t)Shader::eBackground] = resourceLoader.CreateShaderProgram("shaders/background.vs", "shaders/background.ps");
    shaders_[(size_t)Shader::eColored] = resourceLoader.CreateShaderProgram("shaders/colored_quad.vs", "shaders/colored_quad.ps");
    shaders_[(size_t)Shader::eTextured] = resourceLoader.CreateShaderProgram("shaders/textured_quad.vs", "shaders/textured_quad.ps");
    shaders_[(size_t)Shader::eFocusableTextured] = resourceLoader.CreateShaderProgram("shaders/textured_quad.vs", "shaders/focusable_textured_quad.ps");

    if (core::utility::AnyAreNull(shaders_)) goto failure;

    pFullScreenQuad_ = resourceLoader.CreateGeometry();

    if (!pFullScreenQuad_ || !LoadGeometry() || !LoadShaders() || !LoadSound() || !LoadConstantBuffers()) goto failure;

    return true;

failure:
    printf("Unable to load basic resources\n");
    return false;
}

bool ResourceManager::LoadGeometry()
{
    PositionVertex vertices[] =
    {
        {glm::vec2(-1, 1)},  // top left
        {glm::vec2(1, 1)},  // top right
        {glm::vec2(1, -1)},  // bottom right
        {glm::vec2(-1, -1)},  // bottom left
    };

    uint32_t indices[] = { 0, 1, 3, 1, 2, 3 };

    dx11rendering::Geometry::Description geometryDescription;
    geometryDescription.numIndices = sizeof(indices)/sizeof(uint32_t);
    geometryDescription.numVertices = sizeof(vertices)/sizeof(PositionVertex);
    geometryDescription.pIndices = indices;
    geometryDescription.pVertices = vertices;
    geometryDescription.vertexSize = sizeof(PositionVertex);
    geometryDescription.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    if (!pFullScreenQuad_->Describe(&geometryDescription)) goto failure;

    return true;

failure:
    printf("Failed to load geometry\n");
    return false;
}

bool ResourceManager::LoadShaders()
{
    D3D11_INPUT_ELEMENT_DESC inputDesc;
    inputDesc.SemanticName = "POSITION";
    inputDesc.SemanticIndex = 0;
    inputDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
    inputDesc.InputSlot = 0;
    inputDesc.AlignedByteOffset = 0;
    inputDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputDesc.InstanceDataStepRate = 0;

    dx11rendering::ShaderProgram::CompileDescription compileDescription;
    compileDescription.vsDescription.pEntryPoint = "main";
    compileDescription.vsDescription.pShaderModel = "vs_5_0";
    compileDescription.psDescription.pEntryPoint = "main";
    compileDescription.psDescription.pShaderModel = "ps_5_0";
    compileDescription.inputDescription.numInputElementDescriptions = 1;
    compileDescription.inputDescription.pInputElementDescriptions = &inputDesc;

    void* pError = nullptr;
    size_t errorSize = 0;
    if (!shaders_[(int)Shader::eBackground]->Compile(compileDescription, (void**)&pError, &errorSize)) goto failure;

    D3D11_INPUT_ELEMENT_DESC coloredShaderInputDesc[2];
    // position
    coloredShaderInputDesc[0].AlignedByteOffset = 0;
    coloredShaderInputDesc[0].Format = DXGI_FORMAT_R32G32_FLOAT;
    coloredShaderInputDesc[0].InputSlot = 0;
    coloredShaderInputDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    coloredShaderInputDesc[0].InstanceDataStepRate = 0;
    coloredShaderInputDesc[0].SemanticIndex = 0;
    coloredShaderInputDesc[0].SemanticName = "POSITION";

    // color
    coloredShaderInputDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    coloredShaderInputDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    coloredShaderInputDesc[1].InputSlot = 0;
    coloredShaderInputDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    coloredShaderInputDesc[1].InstanceDataStepRate = 0;
    coloredShaderInputDesc[1].SemanticIndex = 0;
    coloredShaderInputDesc[1].SemanticName = "COLOR";

    dx11rendering::ShaderProgram::CompileDescription coloredShaderCompileDesc;
    coloredShaderCompileDesc.inputDescription.numInputElementDescriptions = sizeof(coloredShaderInputDesc)/sizeof(D3D11_INPUT_ELEMENT_DESC);
    coloredShaderCompileDesc.inputDescription.pInputElementDescriptions = coloredShaderInputDesc;
    coloredShaderCompileDesc.psDescription.pEntryPoint = "main";
    coloredShaderCompileDesc.vsDescription.pEntryPoint = "main";
    coloredShaderCompileDesc.psDescription.pShaderModel = "ps_5_0";
    coloredShaderCompileDesc.vsDescription.pShaderModel = "vs_5_0";

    if (!shaders_[(int)Shader::eColored]->Compile(coloredShaderCompileDesc, &pError, &errorSize)) goto failure;

    D3D11_INPUT_ELEMENT_DESC texturedShader[2];
    // position
    texturedShader[0].AlignedByteOffset = 0;
    texturedShader[0].Format = DXGI_FORMAT_R32G32_FLOAT;
    texturedShader[0].InputSlot = 0;
    texturedShader[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    texturedShader[0].InstanceDataStepRate = 0;
    texturedShader[0].SemanticIndex = 0;
    texturedShader[0].SemanticName = "POSITION";

    // uv
    texturedShader[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    texturedShader[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    texturedShader[1].InputSlot = 0;
    texturedShader[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    texturedShader[1].InstanceDataStepRate = 0;
    texturedShader[1].SemanticIndex = 0;
    texturedShader[1].SemanticName = "TEXCOORD";

    dx11rendering::ShaderProgram::CompileDescription texturedShadercompileDesc;
    texturedShadercompileDesc.inputDescription.numInputElementDescriptions = sizeof(texturedShader)/sizeof(D3D11_INPUT_ELEMENT_DESC);
    texturedShadercompileDesc.inputDescription.pInputElementDescriptions = texturedShader;
    texturedShadercompileDesc.psDescription.pEntryPoint = "main";
    texturedShadercompileDesc.vsDescription.pEntryPoint = "main";
    texturedShadercompileDesc.psDescription.pShaderModel = "ps_5_0";
    texturedShadercompileDesc.vsDescription.pShaderModel = "vs_5_0";

    // Slight waste because these two shaders share a vertex shader, but whatever.
    if (!shaders_[(int)Shader::eTextured]->Compile(texturedShadercompileDesc, &pError, &errorSize)) goto failure;
    if (!shaders_[(int)Shader::eFocusableTextured]->Compile(texturedShadercompileDesc, &pError, &errorSize)) goto failure;

    return true;
failure:
    printf("Failed to load shaders: %s\n", (char*)pError);
    return false;
}

bool ResourceManager::LoadSound()
{
    sound::SoundManager::ResourceLoader& loader = gameSystems_.soundManager.GetResourceLoader();

    soundEffects_[(size_t)SoundEffect::eBump] = loader.CreateEffect("res/sound/effects/bump.ogg");
    soundEffects_[(size_t)SoundEffect::eGoal] = loader.CreateEffect("res/sound/effects/goal.ogg");
    soundEffects_[(size_t)SoundEffect::eMenuCycle] = loader.CreateEffect("res/sound/effects/menu_cycle.ogg");
    soundEffects_[(size_t)SoundEffect::eMenuSelect] = loader.CreateEffect("res/sound/effects/menu_select.ogg");
	soundEffects_[(size_t)SoundEffect::eCountDownTone] = loader.CreateEffect("res/sound/effects/count_down_tone.ogg");
	soundEffects_[(size_t)SoundEffect::eGoTone] = loader.CreateEffect("res/sound/effects/go_tone.ogg");
    if (core::utility::AnyAreNull(soundEffects_)) goto failure;

	for (sound::Effect* pEffect : soundEffects_)
		pEffect->SetVolume(gameSystems_.soundManager.GetMaxVolume() / 4);

    if (!dj_.Load()) goto failure;
    dj_.Start();

    return true;
failure:
    printf("Failed to load sound\n");
    return false;
}

bool ResourceManager::LoadConstantBuffers()
{
    dx11rendering::DX11Renderer::ResourceLoader& loader = renderer_.GetResourceLoader();

    constantBuffers_[(int)ConstantBuffer::eBackground] = loader.CreateShaderConstantBuffer();
    constantBuffers_[(int)ConstantBuffer::eFocusableTextured] = loader.CreateShaderConstantBuffer();

    if (core::utility::AnyAreNull(constantBuffers_)) goto failure;

    BackgroundConstantBuffer backgroundBuffer;
    backgroundBuffer.mixColor = glm::vec4(0, .85, 1, 1);
    backgroundBuffer.phaseShift = glm::vec4();

    dx11rendering::ShaderConstantBuffer::Description constantBufferDesc;
    constantBufferDesc.pInitialData = &backgroundBuffer;
    constantBufferDesc.size = sizeof(BackgroundConstantBuffer);

    if(!constantBuffers_[(int)ConstantBuffer::eBackground]->Describe(constantBufferDesc)) goto failure;

    FocusableConstantBuffer focusableBuffer;
    focusableBuffer.isSelected = glm::vec4(); // false

    constantBufferDesc.pInitialData = &focusableBuffer;
    constantBufferDesc.size = sizeof(FocusableConstantBuffer);

    if (!constantBuffers_[(int)ConstantBuffer::eFocusableTextured]->Describe(constantBufferDesc)) goto failure;

    return true;
failure:
    printf("Failed to create constant buffers\n");
    return false;
}

void ResourceManager::LoadViews()
{
    rendering::DisplayManager& displayManager = gameSystems_.displayManager;

    size_t width = displayManager.GetOptions().screenWidth;
    size_t height = displayManager.GetOptions().screenHeight;

    D3D11_VIEWPORT view = {};
    view.Width = (float)width;
    view.Height = (float)height;
    fullScreenView_ = view;

    view.Width = width/2.0f;
    view.TopLeftX = 0;
    view.TopLeftY = 0;
    player1Vew_ = view;

    view.TopLeftX = width/2.0f;
    player2Vew_ = view;
}

void ResourceManager::LoadPasses()
{
    dx11rendering::RenderPass::PipelineDescription desc;
    desc.pProgram = GetShader(Shader::eBackground);
    desc.pPSConstantBuffer = GetConstantBuffer(ConstantBuffer::eBackground);
    desc.pVSConstantBuffer = nullptr;
    desc.pTexture = nullptr;

    backgroundPass_.SetGeometry(*pFullScreenQuad_);
    backgroundPass_.SetPipelineDescription(desc);
}

void ResourceManager::LoadAggregates()
{
    background_.SetRenderInfo(renderer_, backgroundPass_);
}


} // namespace game

} // namespace ice
