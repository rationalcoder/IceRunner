#include <vector>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <SDL2/SDL.h>
#include "dx11renderer.h"

using namespace ice::dx11rendering;

bool DX11Renderer::Init(HWND hWnd, size_t screenWidth, size_t screenHeight)
{
    ComPtr<IDXGIFactory> factory;
    if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory)))
    {
        printf("Couldn't Create Factory...\n");
        return false;
    }

    ComPtr<IDXGIAdapter> adapter;
    if (FAILED(factory->EnumAdapters(0, &adapter)))
    {
        printf("Couldn't find adapter...\n");
        return false;
    }

    ComPtr<IDXGIOutput> output;
    if (FAILED(adapter->EnumOutputs(0, &output)))
    {
        printf("Couldn't enum outputs...\n");
        return false;
    }

    UINT numModesSupported = 0;
    if (FAILED(output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModesSupported, NULL)))
    {
        printf("Mode not supported...\n");
        return false;
    }

    std::vector<DXGI_MODE_DESC> modes(numModesSupported);
    if (FAILED(output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModesSupported, (DXGI_MODE_DESC*)modes.data())))
    {
        printf("Couldn't get mode descriptions\n...");
        return false;
    }

    size_t targetWidth = screenWidth;
    size_t targetHeight = screenHeight;
    float targetFPS = 60.0f;

    auto isTargetMode = [=](const DXGI_MODE_DESC& mode)
    {
        if (mode.Width == targetWidth && mode.Height == targetHeight &&
            std::roundf((float)mode.RefreshRate.Numerator / mode.RefreshRate.Denominator) == targetFPS)
        {
            return true;
        }
        return false;
    };

    auto descIter = std::find_if(modes.begin(), modes.end(), isTargetMode);
    if (descIter == modes.end())
    {
        printf("Couldn't find target mode...\n");
        return false;
    }

    DXGI_ADAPTER_DESC adapterDescription{};
    if (FAILED(adapter->GetDesc(&adapterDescription)))
    {
        printf("Couldn't get adapter description...\n");
        return false;
    }

    // Apparently, all of that was just to get the adapter and mode descriptions.


    // Create the swap chain
    DXGI_SWAP_CHAIN_DESC swapChainDesc{};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc = *descIter;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = true;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    D3D_FEATURE_LEVEL featureLevelList[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    D3D_FEATURE_LEVEL maxSupportedFeatureLevel = D3D_FEATURE_LEVEL_11_1;

    HRESULT result;
    if (FAILED(result = D3D11CreateDeviceAndSwapChain(adapter, D3D_DRIVER_TYPE_UNKNOWN, 0, 0, featureLevelList, sizeof(featureLevelList)/sizeof(D3D_FEATURE_LEVEL),
                                                      D3D11_SDK_VERSION, &swapChainDesc, &swapChain_, &device_, &maxSupportedFeatureLevel, &context_)))
    {
        wchar_t* buffer;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL, result, 0, (LPWSTR)&buffer, 0, NULL);
        printf("While trying to get 11_1 support...\n");
        printf("Failed to create device and swap chain and stuff: %S...\n", buffer);
        LocalFree(buffer);
        return false;
    }

    // Setup the back buffer. It is Released at end of function because it only
    // represents the interface to the swap chain's back buffer.
    // We only need the interface to create the render target view.
    ComPtr<ID3D11Texture2D> backBuffer;
    if(FAILED(swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer)))
    {
        printf("Failed to create back buffer\n");
        return false;
    }

    // Create a render target view out of the swap chain's back buffer.
    // the back buffer is automatically swapped to point to the actual back buffer,
    // so we don't have to switch it when we draw.
    if(FAILED(device_->CreateRenderTargetView(backBuffer, NULL, &renderTargetView_)))
    {
        printf("Failed to create render target\n");
        return false;
    }

    // Setup the depth buffer
    D3D11_TEXTURE2D_DESC depthBufferDesc = {};
    depthBufferDesc.Width = (UINT)screenWidth;
    depthBufferDesc.Height = (UINT)screenWidth;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    // No multi-sampling
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    // Create the depth buffer (it is just a tex2D)
    if (FAILED(device_->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer_)))
    {
        printf("Failed to create depth stencil buffer\n");
        return false;
    }

    // We don't necessarily need to make one of these, I don't think.
    // One like this should be created for us by default, if we pass NULL.
    // This can serve as an example of what would need to be changed if we
    // need to have a custom one (not likely).
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

    // Stencil ops if a pixel is front-facing
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil ops if a pixel is back-facing
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    if (FAILED(device_->CreateDepthStencilState(&depthStencilDesc, &depthStencilState_)))
    {
        printf("Failed to create depth stencil state\n");
        return false;
    }

    // Set the depth stencil state we just created.
    // (Actually increases the reference count of the COM Interface;
    // maybe we don't need to store it as a member?)
    context_->OMSetDepthStencilState(depthStencilState_, 1);

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    if (FAILED(device_->CreateDepthStencilView(depthStencilBuffer_,
                                               &depthStencilViewDesc,
                                               &depthStencilView_)))
    {
        printf("Failed to create depth stencil view\n");
        return false;
    }

    // Bind our render target view and the depth stencil buffer (the view of it) to the pipeline.
    context_->OMSetRenderTargets(1, renderTargetView_.AddressOf(), depthStencilView_);


    // Setup the rasterizer description. This is the important part for wire frame mode.
    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.AntialiasedLineEnable = false;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.DepthBiasClamp = 0.0f;
    rasterizerDesc.DepthClipEnable = true;
    // Set wire frame here by setting D3D11_FILL_WIREFRAME
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    // Maybe set this to yes to support opengl style winding.
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.MultisampleEnable = false;
    rasterizerDesc.ScissorEnable = false;
    rasterizerDesc.SlopeScaledDepthBias = 0.0f;


    if (FAILED(device_->CreateRasterizerState(&rasterizerDesc, &solidRasterState_)))
    {
        printf("Failed to create rasterizer states\n");
        return false;
    }

    rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
    if (FAILED(device_->CreateRasterizerState(&rasterizerDesc, &wireRasterState_)))
    {
        printf("Failed to create rasterizer states\n");
        return false;
    }

    // Set the rasterizer state that we just described
    context_->RSSetState(solidRasterState_);

    // Describe the blend state for the first (our only) render target; setup default to enable alpha.
    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.IndependentBlendEnable = false;
    blendDesc.RenderTarget[0].BlendEnable = true;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    // Create the blend state.
    if (FAILED(device_->CreateBlendState(&blendDesc, &blendState_)))
    {
        printf("Failed to create blend state\n");
        return false;
    }

    // Set it
    context_->OMSetBlendState(blendState_, nullptr, UINT_MAX);

    // Set up the default view port (the entire screen at full depth)
    D3D11_VIEWPORT viewPort{};
    viewPort.Width = (float)screenWidth;
    viewPort.Height = (float)screenHeight;
    viewPort.TopLeftX = 0;
    viewPort.TopLeftY = 0;
    viewPort.MinDepth = 0.0f;
    viewPort.MaxDepth = 1.0f;

    // Set it
    context_->RSSetViewports(1, &viewPort);

    if(!resourceLoader_.Initialize())
    {
        printf("Failed to init the resource manager\n");
        return false;
    }

    return true;
}

DX11Renderer::~DX11Renderer()
{
    // Make sure we are out of full screen mode so we don't crash,
    swapChain_->SetFullscreenState(false, nullptr);
    context_->ClearState();
}

void DX11Renderer::BeginScene(D3DCOLORVALUE clearColor)
{
    context_->ClearRenderTargetView(renderTargetView_, (FLOAT*)&clearColor);
    context_->ClearDepthStencilView(depthStencilView_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DX11Renderer::EndScence()
{
    // Wait for v-sync.
    swapChain_->Present(1, 0);
}

void DX11Renderer::Draw(const RenderPass& pass)
{
    pass.Bind();
    context_->DrawIndexed(pass.GetGeometry()->GetDescription().numIndices, 0, 0);
}

void DX11Renderer::Draw(const Geometry& geo)
{
    context_->DrawIndexed(geo.GetDescription().numIndices, 0, 0);
}
