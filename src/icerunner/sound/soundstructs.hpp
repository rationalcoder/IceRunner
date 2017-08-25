#ifndef SOUNDSTRUCTS_HPP
#define SOUNDSTRUCTS_HPP
#include <cassert>
#include <cstdint>
#include <vector>

struct Mix_Chunk;
typedef struct _Mix_Music Mix_Music;

namespace ice
{
namespace sound
{
class SoundManager;

class Effect
{
// FIXME: Microsoft extension / bug
    friend class SoundManager;
public:
    void Play();
    void SetVolume(int volume);

    ~Effect();

private:
    Effect(SoundManager& soundManager, Mix_Chunk* pChunk)
        : soundManager_(soundManager), pChunk_(pChunk)
    {}

private:
    SoundManager& soundManager_;
    Mix_Chunk* pChunk_;
};

class Music
{
// FIXME: Microsoft extension / bug
    friend class SoundManager;
public:
    using FinishedCallback = void(*)();

public:
    Music(SoundManager& soundManager)
        : soundManager_(soundManager), pMusic_(nullptr)
    {}

    ~Music();

    void FadeIn(uint16_t millis);
    void FadeOut(uint16_t millis);
    bool IsValid() const { return pMusic_ != nullptr; }

    static void SetFinishedCallback(FinishedCallback callback);

private:
    Music(SoundManager& soundManager, Mix_Music* pMusic)
        : soundManager_(soundManager), pMusic_(pMusic)
    {}

private:
    SoundManager& soundManager_;
    Mix_Music* pMusic_;
};

} // namespace sound
} // namespace ice

#endif // SOUNDSTRUCTS_HPP
