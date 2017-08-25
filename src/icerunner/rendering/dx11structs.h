#ifndef DX11STRUCTS_H
#define DX11STRUCTS_H
#include <string>
#include <d3d11.h>
#include <cassert>
#include "comptrhack.hpp"

struct SDL_Surface;

namespace ice
{

namespace dx11rendering
{

class DX11Renderer;

class ShaderProgram
{
    friend class DX11Renderer;
    friend class RenderPass;
public:
    struct InputDescription
    {
        D3D11_INPUT_ELEMENT_DESC* pInputElementDescriptions;
        uint32_t numInputElementDescriptions;
    };

    struct ShaderDescription
    {
        const char* pShaderModel;
        const char* pEntryPoint;
    };

    struct CompileDescription
    {
        InputDescription inputDescription;
        ShaderDescription vsDescription;
        ShaderDescription psDescription;
    };

public:
    bool Compile(const CompileDescription& compileDesc, void** ppErrorBuffer, size_t* pErrorSize);

private:
    void Bind();

public:
    ~ShaderProgram() = default;

private:
    ShaderProgram(DX11Renderer& renderer, const std::string& vsFileName, const std::string& psFileName)
        : vsFileName_(vsFileName), psFileName_(psFileName), renderer_(renderer)
    {}

private:
    std::string vsFileName_;
    std::string psFileName_;
    ComPtr<ID3D11VertexShader> vertexShader_;
    ComPtr<ID3D11PixelShader> pixelShader_;
    ComPtr<ID3D11InputLayout> inputLayout_;
    ComPtr<ID3DBlob> errorBuffer_;
    DX11Renderer& renderer_;
};

class ShaderConstantBuffer
{
public:
    friend class DX11Renderer;
    friend class RenderPass;

public:
    struct Description
    {
        const void* pInitialData;
        size_t size;
    };

public:
    bool Describe(const Description& description);
    bool Update(const void* pData);

private:
    void VSBind();
    void PSBind();

public:
    ~ShaderConstantBuffer() = default;

private:
    ShaderConstantBuffer(DX11Renderer& renderer)
        : renderer_(renderer), size_()
    {}

private:
    ComPtr<ID3D11Buffer> buffer_;
    DX11Renderer& renderer_;
    size_t size_;
};

class Geometry
{
public:
    friend class DX11Renderer;
    friend class RenderPass;

public:
    struct Description
    {
        const void* pVertices;
        const uint32_t* pIndices;
        uint32_t vertexSize;
        uint32_t numVertices;
        uint32_t numIndices;
        D3D_PRIMITIVE_TOPOLOGY topology;
    };

public:
    bool Describe(Geometry::Description* desc);
    Description GetDescription() const { return description_; }

private:
    void Bind();

private:
    Geometry(DX11Renderer& renderer)
        : renderer_(renderer)
    {}

private:
    Description description_;
    ComPtr<ID3D11Buffer> vertexBuffer_;
    ComPtr<ID3D11Buffer> indexBuffer_;
    DX11Renderer& renderer_;
    D3D_PRIMITIVE_TOPOLOGY topology_;
};

class Texture
{
public:
    friend class DX11Renderer;
    friend class RenderPass;

public:
    ~Texture() = default;

    void Bind();
    float GetAspectRatio() { return aspectRatio_; }
    void SetAspectRatio(float ratio) { aspectRatio_ = ratio; }

private:
    Texture(DX11Renderer& renderer)
        : renderer_(renderer)
    {}

    bool Create(const SDL_Surface& surface);

private:
    ComPtr<ID3D11Texture2D> texture_;
    ComPtr<ID3D11SamplerState> samplerState_;
    ComPtr<ID3D11ShaderResourceView> resourceView_;
    DX11Renderer& renderer_;
    float aspectRatio_;
};


class RenderPass
{
public:
    struct PipelineDescription
    {
        PipelineDescription()
            : pProgram(nullptr), pTexture(nullptr), \
              pVSConstantBuffer(nullptr), pPSConstantBuffer(nullptr)
        {}

        ShaderProgram* pProgram;
        Texture* pTexture;
        ShaderConstantBuffer* pVSConstantBuffer;
        ShaderConstantBuffer* pPSConstantBuffer;
    };

public:
    RenderPass()
        : pGeometry_(nullptr), pipelineDescription_()
    {}

    RenderPass(Geometry& geometry, const PipelineDescription& description)
        : pGeometry_(&geometry), pipelineDescription_(description)
    {}

    void Bind() const
    {
        assert(pipelineDescription_.pProgram);

        pGeometry_->Bind();
        pipelineDescription_.pProgram->Bind();

        Texture* pTexture = pipelineDescription_.pTexture;
        if (pTexture)
            pipelineDescription_.pTexture->Bind();

        ShaderConstantBuffer* pVSConstantShaderBuffer = pipelineDescription_.pVSConstantBuffer;
        if (pVSConstantShaderBuffer)
            pVSConstantShaderBuffer->VSBind();

        ShaderConstantBuffer* pPSConstantShaderBuffer = pipelineDescription_.pPSConstantBuffer;
        if (pPSConstantShaderBuffer)
            pPSConstantShaderBuffer->PSBind();
    }

    PipelineDescription& GetPipelineDescription() { return pipelineDescription_; }
    const PipelineDescription& GetPipelineDescription() const { return pipelineDescription_; }
    void SetPipelineDescription(const PipelineDescription& description) { pipelineDescription_ = description; }
    void SetGeometry(dx11rendering::Geometry& geometry) { pGeometry_ = &geometry; }
    dx11rendering::Geometry* GetGeometry() { return pGeometry_; }
    const dx11rendering::Geometry* GetGeometry() const { return pGeometry_; }

private:
    Geometry* pGeometry_;
    PipelineDescription pipelineDescription_;
};

} // namespace dx11rendering

} // namespace ice

#endif // DX11STRUCTS_H
