#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H
#include "eventstructs.hpp"
#include "hidmanager.h"

namespace ice
{

namespace events
{

class EventManager
{
public:
    EventManager()
        : xboxControllerManager_(hidManager_.GetXBOXControllerManager()), shouldQuit_(false)
    {}

    ~EventManager();

    bool Init();
    void Poll();
    bool ShouldQuit();

    const GameState& GetEventState() const { return adjustedState_; }

private:
    void AdjustPlayerState(PlayerID id);

private:
    HIDManager hidManager_;
    GameState rawState_;
    GameState adjustedState_;
    detail::FramesHeldState framesHeldState_[2];
    HIDManager::XBOXControllerManager& xboxControllerManager_;
    bool shouldQuit_;
};

} // namespace events

} // namespace ice

#endif // EVENTMANAGER_H
