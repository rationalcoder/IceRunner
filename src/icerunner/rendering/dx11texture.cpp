#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "dx11renderer.h"

using namespace ice::dx11rendering;

bool Texture::Create(const SDL_Surface& surface)
{
    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = surface.w;
    textureDesc.Height = surface.h;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.Format = surface.format->format == SDL_PIXELFORMAT_ABGR8888 ? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_B8G8R8A8_UNORM;

    D3D11_SUBRESOURCE_DATA textureData = {};
    textureData.pSysMem = surface.pixels;
    textureData.SysMemPitch = surface.pitch;


    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.MaxLOD = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MipLODBias = 0.0f;

    if (FAILED(renderer_.device_->CreateSamplerState(&samplerDesc, &samplerState_)))
    {
        printf("Failed to create sampler state\n");
        return false;
    }

    if(FAILED(renderer_.device_->CreateTexture2D(&textureDesc, &textureData, &texture_)))
    {
        printf("Failed to create texture\n");
        return false;
    }

    HRESULT result = NO_ERROR;
    if(FAILED(result = renderer_.device_->CreateShaderResourceView(texture_, nullptr, &resourceView_)))
    {
        wchar_t* buffer;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL, result, 0, (LPWSTR)&buffer, 0, NULL);
        printf("Failed to create shader resource view for texture: %S\n", buffer);
        return false;
    }

    return true;
}

void Texture::Bind()
{
    renderer_.context_->PSSetSamplers(0, 1, samplerState_.AddressOf());
    renderer_.context_->PSSetShaderResources(0, 1, resourceView_.AddressOf());
}


