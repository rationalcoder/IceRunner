#include <chrono>
#include <ctime>
#include <random>
#include "core/utility.hpp"
#include "sound/soundmanager.h"
#include "dj.h"

static std::string menuFolder = "res\\sound\\music\\menu\\";
static std::string actionFolder = "res\\sound\\music\\action\\";

namespace ice
{
namespace game
{

bool DJ::Load()
{
    Unload();

    sound::SoundManager::ResourceLoader& loader = soundManager_.GetResourceLoader();
    core::utility::ForEachFile(menuFolder, [this, &loader](const char* fileName) {
        std::string fullPath = menuFolder + fileName;
        menuTracks_.push_back(loader.CreateMusic(fullPath));
    });

    core::utility::ForEachFile(actionFolder, [this, &loader](const char* fileName) {
        std::string fullPath = actionFolder + fileName;
        actionTracks_.push_back(loader.CreateMusic(fullPath));
    });

    std::srand((unsigned)std::time(nullptr));
    currentMenuTrack_ = std::rand() % menuTracks_.size();
    currentActionTrack_ = std::rand() % actionTracks_.size();

    return true;
}

void DJ::Start()
{
    soundManager_.SetMusicVolume(soundManager_.GetMaxVolume()/4);

    musicThreadShouldContinue_ = true;
    musicThread_.swap(std::thread([this]() {
        while (musicThreadShouldContinue_)
        {
            // If there isn't any music playing and it isn't paused,
            // fad
            if (!MusicIsPlaying() && !MusicIsPaused())
            {
                if (state_ == State::eMenu) NextMenuTrack().FadeIn(2000);
                else NextActionTrack().FadeIn(2000);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }
    }));
}

void DJ::Unload()
{
    soundManager_.StopMusic();
    musicThreadShouldContinue_ = false;
    if (musicThread_.joinable())
        musicThread_.join();

    core::utility::DeleteNullAndClear(menuTracks_);
    core::utility::DeleteNullAndClear(actionTracks_);
}

void DJ::PlayMenu()
{
    StopMusic();
    state_ = State::eMenu;
}

void DJ::PlayAction()
{
    StopMusic();
    state_ = State::eAction;
}

void DJ::Pause()
{
    std::lock_guard<std::mutex> lock(musicMutex_);
    soundManager_.PauseMusic();
}

void DJ::Resume()
{
    std::lock_guard<std::mutex> lock(musicMutex_);
    soundManager_.ResumeMusic();
}

void DJ::StopMusic()
{
    std::lock_guard<std::mutex> lock(musicMutex_);
    switch (state_)
    {
    case State::eMenu:
        if (!menuTracks_.empty())
            menuTracks_[currentMenuTrack_ -1]->FadeOut(1000);
        break;
    case State::eAction:
        if (!actionTracks_.empty())
            actionTracks_[currentActionTrack_-1]->FadeOut(1000);
        break;
    default:
        assert(false && "shouldn't get here");
    }
}

bool DJ::MusicIsPlaying()
{
    std::lock_guard<std::mutex> lock(musicMutex_);
    return soundManager_.MusicIsPlaying();
}

bool DJ::MusicIsPaused()
{
    std::lock_guard<std::mutex> lock(musicMutex_);
    return soundManager_.MusicIsPaused();
}

} // namespace game
} // namespace ice
