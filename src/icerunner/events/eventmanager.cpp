#include <cstdio>
#include <SDL2/SDL.h>
#include "hidmanager.h"
#include "eventmanager.h"

#define FRAMES_PER_EVENT 30
#define FRAMES_PER_REPEAT 10

namespace ice
{

namespace events
{

bool EventManager::Init()
{
    if (SDL_InitSubSystem(SDL_INIT_EVENTS) == 0 || xboxControllerManager_.Init()) return true;
    printf("initing: %p\n", this);

    printf("Couldn't init event manager: %s\n", SDL_GetError());
    return false;
}

EventManager::~EventManager()
{
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
    xboxControllerManager_.ShutDown();
}

void EventManager::Poll()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT:
            shouldQuit_ = true;
            return;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                shouldQuit_ = true;
                return;
            }
            break;
        default:
            break;
        }
    }

    xboxControllerManager_.Poll(rawState_);
    AdjustPlayerState(PlayerID::e1);
    AdjustPlayerState(PlayerID::e2);
}

void EventManager::AdjustPlayerState(PlayerID id)
{
    // The connection status is fine to poll for every frame.
    adjustedState_.connectionState_ = rawState_.connectionState_;
    PlayerState& adjustedPlayerState = adjustedState_.playerStates_[(size_t)id];
    PlayerState& rawPlayerState = rawState_.playerStates_[(size_t)id];
    detail::FramesHeldState& framesHeldState = framesHeldState_[(int)id];
    adjustedPlayerState.previousMetaState_ = adjustedPlayerState.currentMetaState_;
    adjustedPlayerState.previousMovementState_ = adjustedPlayerState.currentMovementState_;
    adjustedPlayerState.currentMetaState_.reset();
    adjustedPlayerState.currentMovementState_.reset();

    // If an event is down, increment the held counter for it
    // If an event is not down, reset its frame counter.
    // If an has been held for n frames, modify the adjusted state and reset its held count.
    // The equivalent of key repeat starts happening if an event is triggered and stops when it is let go.

    bool numberFramesMet = false;
    bool isNotDown = false;

    for (int event = 0; event < (int)MetaEvent::eNum_; event++)
    {
        uint8_t& framesHeld = framesHeldState.MetaFramesHeldAt((MetaEvent)event);
        numberFramesMet = framesHeld == FRAMES_PER_EVENT;
        isNotDown = !rawPlayerState.IsDown((MetaEvent)event);
        adjustedPlayerState.currentMetaState_[(size_t)event] = (framesHeld == 1);
        framesHeld += !numberFramesMet && !isNotDown;
        framesHeld *= !(numberFramesMet || isNotDown);

        auto repeatingState = framesHeldState.RepeatingStateAt((MetaEvent)event);
        if (numberFramesMet) repeatingState = true;
        if (isNotDown) repeatingState = false;

        uint8_t& repeatingFrames = framesHeldState.MetaFramesRepeatingAt((MetaEvent)event);
        if (repeatingState && repeatingFrames == FRAMES_PER_REPEAT)
        {
            adjustedPlayerState.currentMetaState_[(size_t)event] = true;
            repeatingFrames = 0;
        }
        repeatingFrames += repeatingState;
    }

    for (int event = 0; event < (int)MovementEvent::eNum_; event++)
    {
        uint8_t& framesHeld = framesHeldState.MovementFramesHeldAt((MovementEvent)event);
        numberFramesMet = framesHeld == FRAMES_PER_EVENT;
        isNotDown = !rawPlayerState.IsDown((MovementEvent)event);
        adjustedPlayerState.currentMovementState_[(size_t)event] = (framesHeld == 1);
        framesHeld += !numberFramesMet && !isNotDown;
        framesHeld *= !(numberFramesMet || isNotDown);

        auto repeatingState = framesHeldState.RepeatingStateAt((MovementEvent)event);
        if (numberFramesMet) repeatingState = true;
        if (isNotDown) repeatingState = false;

        uint8_t& repeatingFrames = framesHeldState.MovementFramesRepeatingAt((MovementEvent)event);
        if (repeatingState && repeatingFrames == FRAMES_PER_REPEAT)
        {
            adjustedPlayerState.currentMovementState_[(size_t)event] = true;
            repeatingFrames = 0;
        }
        repeatingFrames += repeatingState;
    }
}

bool EventManager::ShouldQuit()
{
    return shouldQuit_;
}

} // namespace events

} // namespace ice
