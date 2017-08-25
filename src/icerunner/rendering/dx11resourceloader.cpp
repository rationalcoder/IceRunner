#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "dx11renderer.h"

using namespace ice::dx11rendering;

bool DX11Renderer::ResourceLoader::Initialize()
{
    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        printf("Couldn't initialize IMG: %s\n", IMG_GetError());
        return false;
    }

    if(TTF_Init() == -1)
    {
        printf("Couldn't initialize TTF: %s\n", TTF_GetError());
        return false;
    }

    return true;
}
DX11Renderer::ResourceLoader::~ResourceLoader()
{
    TTF_Quit();
}

ShaderProgram* DX11Renderer::ResourceLoader::CreateShaderProgram(const std::string& vsFileName, const std::string& psFileName)
{
    // if allocation fails, we crash...
    return new ShaderProgram(renderer_, vsFileName, psFileName);
}

Geometry* DX11Renderer::ResourceLoader::CreateGeometry()
{
    // if allocation fails, we crash...
    return new Geometry(renderer_);
}

Texture* DX11Renderer::ResourceLoader::CreateTextureFromImage(const std::string& fileName)
{
    SDL_Surface* pSurface = IMG_Load(fileName.c_str());
    if (!pSurface)
    {
        printf("Couldn't IMG_load(%s)\n", fileName.c_str());
        return nullptr;
    }

    if (pSurface->format->format != SDL_PIXELFORMAT_ABGR8888)
    {
        printf("Can't load texture with format: %s\n", SDL_GetPixelFormatName(pSurface->format->format));
        SDL_FreeSurface(pSurface);
        return nullptr;
    }

    Texture* pResult = new Texture(renderer_);
    if (!pResult->Create(*pSurface))
    {
        SDL_FreeSurface(pSurface);
        delete pResult;
        return nullptr;
    }

    pResult->SetAspectRatio(pSurface->w/(float)pSurface->h);

    SDL_FreeSurface(pSurface);

    return pResult;
}


Texture* DX11Renderer::ResourceLoader::CreateTextureFromFont(const std::string& fileName, const std::string& text, size_t size, const SDL_Color& color)
{
    TTF_Font* pFont = TTF_OpenFont(fileName.c_str(), (int)size);
    if (!pFont)
    {
        printf("Couldn't open font: %s: %s\n", fileName.c_str(), TTF_GetError());
        return nullptr;
    }

    SDL_Surface* pSurface = TTF_RenderText_Blended(pFont, text.c_str(), color);
    if (!pSurface)
    {
        printf("Couldn't Render font to an SDL_Surface: %s\n", TTF_GetError());
        TTF_CloseFont(pFont);
        return nullptr;
    }

    if (pSurface->format->format != SDL_PIXELFORMAT_ABGR8888 && pSurface->format->format != SDL_PIXELFORMAT_ARGB8888)
    {
        printf("Can't load texture from font with format: %s\n", SDL_GetPixelFormatName(pSurface->format->format));
        SDL_FreeSurface(pSurface);
        TTF_CloseFont(pFont);
        return nullptr;
    }

    Texture* pResult = new Texture(renderer_);
    if (!pResult->Create(*pSurface))
    {
        printf("Closing font\n");
        SDL_FreeSurface(pSurface);
        TTF_CloseFont(pFont);
        delete pResult;
        return nullptr;
    }

    pResult->SetAspectRatio(pSurface->w/(float)pSurface->h);

    SDL_FreeSurface(pSurface);
    TTF_CloseFont(pFont);

    return pResult;
}

ShaderConstantBuffer* DX11Renderer::ResourceLoader::CreateShaderConstantBuffer()
{
    return new ShaderConstantBuffer(renderer_);
}
