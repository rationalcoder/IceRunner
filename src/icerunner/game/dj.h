#ifndef DJ_H
#define DJ_H
#include <cassert>
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include "sound/soundstructs.hpp"

namespace ice
{
namespace game
{

class DJ
{
private:
    enum class State
    {
        eMenu,
        eAction,
        eNum_
    };

public:
    DJ(sound::SoundManager& soundManager)
        : soundManager_(soundManager),
          defaultMusic_(soundManager),
          state_(State::eMenu),
          musicThreadShouldContinue_(false),
          currentMenuTrack_(),
          currentActionTrack_()
    {}

    bool Load();
    void Unload();

    sound::Music& NextMenuTrack()
    {
        if (menuTracks_.empty()) return defaultMusic_;

        if (currentMenuTrack_ == menuTracks_.size()) currentMenuTrack_ = 0;
        return *menuTracks_[currentMenuTrack_++];
    }

    sound::Music& NextActionTrack()
    {
        if (actionTracks_.empty()) return defaultMusic_;

        if (currentActionTrack_ == actionTracks_.size()) currentActionTrack_ = 0;
        return *actionTracks_[currentActionTrack_++];
    }

    void Start();
    void PlayMenu();
    void PlayAction();
    void Pause();
    void Resume();
    void StopMusic();

    bool MusicIsPlaying();
    bool MusicIsPaused();

private:
    std::thread musicThread_;
    std::mutex musicMutex_;
    std::vector<sound::Music*> menuTracks_;
    std::vector<sound::Music*> actionTracks_;
    sound::SoundManager& soundManager_;
    sound::Music defaultMusic_;
    State state_;
    std::atomic_bool musicThreadShouldContinue_;
    int currentMenuTrack_;
    int currentActionTrack_;
};

} // namespace game
} // namespace ice

#endif // DJ_H
