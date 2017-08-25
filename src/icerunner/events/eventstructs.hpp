#ifndef EVENTSTRUCTS_HPP
#define EVENTSTRUCTS_HPP
#include <bitset>
#include "hidmanager.h"

namespace ice
{

namespace events
{

enum class MovementEvent
{
    eUp,
    eDown,
    eLeft,
    eRight,
    eNum_ //!< Used for array bounds AND an invalid value
};

enum class MetaEvent
{
    ePause,
    eReset,
    eSelect,
    eBack,
    eNum_ //!< Used for array bounds AND an invalid value
};

enum class EventType
{
    eMovement,
    eMeta
};

enum class PlayerID
{
    e1,
    e2,
    eNum_
};

class EventManager;
class PlayerState
{
    friend class EventManager;
    friend class HIDManager;
    friend class HIDManager::XBOXControllerManager;
public:
    PlayerState()
        : currentMovementState_(), previousMovementState_(), currentMetaState_(), previousMetaState_(),
          latestMovementEvent_(MovementEvent::eNum_), latestMetaEvent_(MetaEvent::eNum_)
    {}

    bool IsDown(MovementEvent event) const { return currentMovementState_[(int)event]; }
    bool IsDown(MetaEvent event) const { return currentMetaState_[(int)event]; }

    bool WasDown(MovementEvent event) const { return previousMovementState_[(int)event]; }
    bool WasDown(MetaEvent event) const { return previousMovementState_[(int)event]; }

    bool IsHeld(MovementEvent event) const { return IsDown(event) && WasDown(event); }
    bool IsHeld(MetaEvent event) const { return IsDown(event) && WasDown(event); }

    size_t GetNumMovementEventsDown() const { return currentMovementState_.count(); }
    size_t GetNumMetaEventsDown() const { return currentMetaState_.count(); }

    MovementEvent GetLatestMovementEvent() const { return latestMovementEvent_; }
    MetaEvent GetLatestMetaEvent() const { return latestMetaEvent_; }

private:
    std::bitset<(size_t)MovementEvent::eNum_> currentMovementState_;
    std::bitset<(size_t)MovementEvent::eNum_> previousMovementState_;
    std::bitset<(size_t)MetaEvent::eNum_> currentMetaState_;
    std::bitset<(size_t)MetaEvent::eNum_> previousMetaState_;
    MovementEvent latestMovementEvent_;
    MetaEvent latestMetaEvent_;
};

class GameState
{
    friend class EventManager;
    friend class HIDManager;
    friend class HIDManager::XBOXControllerManager;
public:
    const PlayerState& GetPlayerState(PlayerID id) const { return playerStates_[(int)id]; }
    bool IsPlayerConnected(PlayerID id) const { return connectionState_[(int)id]; }

private:
    PlayerState playerStates_[(size_t)PlayerID::eNum_];
    std::bitset<(size_t)PlayerID::eNum_> connectionState_;
};

namespace detail
{

class FramesHeldState
{
public:
    FramesHeldState()
        : movementFramesHeld_(), metaFramesHeld_(), metaFramesRepeating_(),
          movementFramesRepeating_(), metaRepeatingState_(), movementRepeatingState_()
    {}

    uint8_t& MovementFramesHeldAt(MovementEvent event)
    {
        return movementFramesHeld_[(int)event];
    }

    uint8_t& MetaFramesHeldAt(MetaEvent event)
    {
        return metaFramesHeld_[(int)event];
    }

    uint8_t& MovementFramesRepeatingAt(MovementEvent event)
    {
        return movementFramesRepeating_[(int)event];
    }

    uint8_t& MetaFramesRepeatingAt(MetaEvent event)
    {
        return metaFramesRepeating_[(int)event];
    }

    std::bitset<(size_t)MetaEvent::eNum_>::reference RepeatingStateAt(MetaEvent event)
    {
        return metaRepeatingState_[(size_t)event];
    }

    std::bitset<(size_t)MovementEvent::eNum_>::reference RepeatingStateAt(MovementEvent event)
    {
        return movementRepeatingState_[(size_t)event];
    }

private:
    uint8_t movementFramesHeld_[(size_t)MovementEvent::eNum_];
    uint8_t metaFramesHeld_[(size_t)MetaEvent::eNum_];
    uint8_t movementFramesRepeating_[(size_t)MovementEvent::eNum_];
    uint8_t metaFramesRepeating_[(size_t)MetaEvent::eNum_];
    std::bitset<(size_t)MetaEvent::eNum_> metaRepeatingState_;
    std::bitset<(size_t)MovementEvent::eNum_> movementRepeatingState_;
};

} // namespace detail

} // namespace events

} // namespace ice

#endif // EVENTSTRUCTS_HPP
