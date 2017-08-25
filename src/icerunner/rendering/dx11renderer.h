#ifndef DX11RENDERER_H
#define DX11RENDERER_H
#include <cstdlib>
#include <cstdint>
#include <string>
#include <cassert>
#include <array>
#include <vector>
#include <memory>
#include "dx11structs.h"

struct SDL_Color;

namespace ice
{

namespace dx11rendering
{


class DX11Renderer
{
public:
    friend class ShaderProgram;
    friend class ShaderConstantBuffer;
    friend class Geometry;
    friend class Texture;

public:
    class ResourceLoader
    {
        friend class DX11Renderer;
    public:
        bool Initialize();
        ShaderProgram* CreateShaderProgram(const std::string& vsFileName, const std::string& psFileName);
        Geometry* CreateGeometry();
        Texture* CreateTextureFromImage(const std::string& fileName);
        Texture* CreateTextureFromFont(const std::string& fileName, const std::string& text, size_t size, const SDL_Color& color);
        ShaderConstantBuffer* CreateShaderConstantBuffer();

    public:
        ~ResourceLoader();
        ResourceLoader operator=(const ResourceLoader&) = delete;
        ResourceLoader(const ResourceLoader&) = delete;

    private:
        ResourceLoader(DX11Renderer& renderer)
            : renderer_(renderer)
        {}

    private:
        DX11Renderer& renderer_;
    };

    enum class FillMode
    {
        SOLID,
        WIRE_FRAME
    };

public:
    DX11Renderer()
        : resourceLoader_(*this)
    {}

    ~DX11Renderer();

    bool Init(HWND hWnd, size_t screenWidth, size_t screenHeight);
    // void Shutdown(); just destroy all in the destructor for now.

    void BeginScene(D3DCOLORVALUE clearColor);
    void EndScence();
    void Draw(const RenderPass& pass);
    void Draw(const Geometry& geo);

    void Resize(size_t width, size_t height);

    ResourceLoader& GetResourceLoader() { return resourceLoader_; }

    void SetViewPort(D3D11_VIEWPORT* pViewPort)
    {
        context_->RSSetViewports(1, pViewPort);
    }

    void SetFillMode(FillMode mode)
    {
        if (mode == FillMode::SOLID)
            context_->RSSetState(solidRasterState_);
        else
            context_->RSSetState(wireRasterState_);
    }

private:
    ResourceLoader resourceLoader_;
    ComPtr<ID3D11Device> device_;
    ComPtr<ID3D11DeviceContext> context_;
    ComPtr<IDXGISwapChain> swapChain_;
    ComPtr<ID3D11RenderTargetView> renderTargetView_;
    ComPtr<ID3D11Texture2D> depthStencilBuffer_;
    ComPtr<ID3D11DepthStencilState> depthStencilState_;
    ComPtr<ID3D11DepthStencilView> depthStencilView_;
    ComPtr<ID3D11RasterizerState> solidRasterState_;
    ComPtr<ID3D11RasterizerState> wireRasterState_;
    ComPtr<ID3D11BlendState> blendState_;
};

} // namespace dx11rendering

} // namespace ice

#endif // DX11RENDERER_H
