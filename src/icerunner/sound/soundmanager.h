#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include <string>
#include "soundstructs.hpp"

namespace ice
{
namespace sound
{

class SoundManager
{
    friend class Effect;
    friend class Music;
public:
    class ResourceLoader
    {
    public:
        ResourceLoader(SoundManager& soundManager)
            : soundManager_(soundManager)
        {}

        Effect* CreateEffect(const std::string& fileName);
        Music* CreateMusic(const std::string& fileName);

    private:
        SoundManager& soundManager_;
    };

public:
     SoundManager()
        : resourceLoader_(*this)
     {}

    ~SoundManager();

     bool Init();
     void Update();
     ResourceLoader& GetResourceLoader() { return resourceLoader_; }
     const ResourceLoader& GetResourceLoader() const { return resourceLoader_; }
     void StopMusic();
     void PauseMusic();
     void ResumeMusic();
     bool MusicIsPlaying();
     bool MusicIsPaused();
     void SetMusicVolume(int volume);
     int GetMaxVolume();

private:
     void FadeOutMusic(uint16_t millis);
     void FadeInMusic(Mix_Music* pMusic, uint16_t millis);

private:
     ResourceLoader resourceLoader_;
};

} // namespace sound
} //namespace ice

#endif
