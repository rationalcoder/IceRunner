#include <cassert>
#include <cstdio>
#include <SDL2/SDL_mixer.h>
#include "soundmanager.h"

namespace ice
{
namespace sound
{

Music::~Music()
{
    if (pMusic_)
    {
        Mix_FreeMusic(pMusic_);
        pMusic_ = nullptr;
    }
}

void Music::FadeIn(uint16_t millis)
{
    if (!pMusic_) return;
    soundManager_.FadeInMusic(pMusic_, millis);
}

void Music::FadeOut(uint16_t millis)
{
    if (!pMusic_) return;
    soundManager_.FadeOutMusic(millis);
}

void Music::SetFinishedCallback(FinishedCallback callback)
{
    Mix_HookMusicFinished(callback);
}

} // namespace sound
} // namespace ice
