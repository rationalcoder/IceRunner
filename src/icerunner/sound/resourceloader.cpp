#include <cstdio>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "soundmanager.h"

namespace ice
{
namespace sound
{

// FIXME: hack. If anything fails to load, just return an empty wrapper that
// does notheing when used.

Effect* SoundManager::ResourceLoader::CreateEffect(const std::string& fileName)
{
    Mix_Chunk* pChunk = Mix_LoadWAV(fileName.c_str());
    if (!pChunk)
        printf("Failed to load sound effect %s: %s\n", fileName.c_str(), Mix_GetError());
    Effect* res = new Effect(soundManager_, pChunk);
    return res;
}

Music* SoundManager::ResourceLoader::CreateMusic(const std::string& fileName)
{
    Mix_Music* pMusic = Mix_LoadMUS(fileName.c_str());
    if (!pMusic)
        printf("Failed to load music %s: %s\n", fileName.c_str(), Mix_GetError());

    Music* res = new Music(soundManager_, pMusic);
    return res;
}

} // namespace sound
} // namespace ice
