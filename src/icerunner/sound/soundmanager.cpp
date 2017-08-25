#include <cstdio>
#include <thread>
#include <mutex>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "soundmanager.h"

namespace ice
{
namespace sound
{

bool SoundManager::Init()
{
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
    {
        printf("failed to init audio\n");
        goto fail1;
    }

    if (Mix_Init(MIX_INIT_OGG) == 0)
    {
        printf("failed to init OGG: %s\n", Mix_GetError());
        goto fail2;
    }

    if (Mix_OpenAudio(44100, AUDIO_F32, 2, 1024))
    {
        printf("failed to open audio: %s\n", Mix_GetError());
        goto fail3;
    }

    return true;
fail3:
    Mix_Quit();
fail2:
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
fail1:
    printf("failed to init the sound manager\n");
    return false;
}

SoundManager::~SoundManager()
{
    StopMusic();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void SoundManager::Update()
{
    // not doing anything in here after all...
}

void SoundManager::FadeOutMusic(uint16_t millis)
{
    std::thread t([this, millis](){
        Mix_FadeOutMusic(millis);
    });
    t.detach();
}

void SoundManager::FadeInMusic(Mix_Music* pMusic, uint16_t millis)
{
    std::thread t([this, pMusic, millis](){
        Mix_FadeInMusic(pMusic, 0, millis);
    });
    t.detach();
}

void SoundManager::StopMusic()
{
    Mix_HaltMusic();
}

void SoundManager::PauseMusic()
{
    Mix_PauseMusic();
}

void SoundManager::ResumeMusic()
{
    Mix_ResumeMusic();
}

bool SoundManager::MusicIsPlaying()
{
    return !!Mix_PlayingMusic();
}

bool SoundManager::MusicIsPaused()
{
    return !!Mix_PausedMusic();
}

void SoundManager::SetMusicVolume(int volume)
{
    Mix_VolumeMusic(volume);
}

int SoundManager::GetMaxVolume()
{
    return MIX_MAX_VOLUME;
}

} // namespace sound
} // namespace ice
