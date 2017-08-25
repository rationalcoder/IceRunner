#include <windows.h>
#include <Xinput.h>
#include <SDL2/SDL.h>
#include "hidmanager.h"
#include "eventstructs.hpp"

#define LEFT_THUMB_DEADZONE (XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE + 10000)

namespace ice
{

namespace events
{

bool HIDManager::XBOXControllerManager::Init()
{
    XInputEnable(true);
    return true;
}

void HIDManager::XBOXControllerManager::ShutDown()
{
    XInputEnable(false);
}

void HIDManager::XBOXControllerManager::PollPlayer1(GameState& eventState)
{
    XINPUT_STATE player1Controller;
    DWORD player1Connected = XInputGetState(0, &player1Controller);
    PlayerState& player1State = (PlayerState&)eventState.GetPlayerState(PlayerID::e1);

    player1State.previousMetaState_ = player1State.currentMetaState_;
    player1State.previousMovementState_ = player1State.currentMovementState_;

    player1State.currentMetaState_.reset();
    player1State.currentMovementState_.reset();

    player1State.latestMetaEvent_ = MetaEvent::eNum_;
    player1State.latestMovementEvent_ = MovementEvent::eNum_;

    if (player1Connected == ERROR_SUCCESS)
    {
        eventState.connectionState_.set((size_t)PlayerID::e1);

        if (player1Controller.dwPacketNumber == player1PacketNumber_)
        {
            player1State.currentMetaState_ = player1State.previousMetaState_;
            player1State.currentMovementState_ = player1State.previousMovementState_;
        }
        else
        {
            XINPUT_GAMEPAD& pad = player1Controller.Gamepad;
            player1PacketNumber_ = player1Controller.dwPacketNumber;

            player1State.currentMovementState_.set((size_t)MovementEvent::eUp, pad.sThumbLY > LEFT_THUMB_DEADZONE);
            player1State.currentMovementState_.set((size_t)MovementEvent::eDown, pad.sThumbLY < -LEFT_THUMB_DEADZONE);
            player1State.currentMovementState_.set((size_t)MovementEvent::eLeft, pad.sThumbLX < -LEFT_THUMB_DEADZONE);
            player1State.currentMovementState_.set((size_t)MovementEvent::eRight, pad.sThumbLX > LEFT_THUMB_DEADZONE);

            player1State.currentMetaState_.set((size_t)MetaEvent::eBack, !!(pad.wButtons & (XINPUT_GAMEPAD_BACK | XINPUT_GAMEPAD_B)));
            player1State.currentMetaState_.set((size_t)MetaEvent::ePause, !!(pad.wButtons & XINPUT_GAMEPAD_START));
            player1State.currentMetaState_.set((size_t)MetaEvent::eSelect, !!(pad.wButtons & XINPUT_GAMEPAD_A));
            player1State.currentMetaState_.set((size_t)MetaEvent::eReset, !!(pad.wButtons & XINPUT_GAMEPAD_Y));
            player1State.latestMetaEvent_ = MetaEvent::eNum_;
            player1State.latestMovementEvent_ = MovementEvent::eNum_;
        }
    }
    else
    {
        eventState.connectionState_.set((size_t)PlayerID::e1, false);
    }
}

void HIDManager::XBOXControllerManager::PollPlayer2(GameState& eventState)
{
    XINPUT_STATE player2Controller;
    DWORD player2Connected = XInputGetState(1, &player2Controller);
    PlayerState& player2State = (PlayerState&)eventState.GetPlayerState(PlayerID::e2);

    player2State.previousMetaState_ = player2State.currentMetaState_;
    player2State.previousMovementState_ = player2State.currentMovementState_;

    player2State.currentMetaState_.reset();
    player2State.currentMovementState_.reset();

    player2State.latestMetaEvent_ = MetaEvent::eNum_;
    player2State.latestMovementEvent_ = MovementEvent::eNum_;

    if (player2Connected == ERROR_SUCCESS)
    {
        eventState.connectionState_.set((size_t)PlayerID::e2);

        if (player2Controller.dwPacketNumber == player1PacketNumber_)
        {
            player2State.currentMetaState_ = player2State.previousMetaState_;
            player2State.currentMovementState_ = player2State.previousMovementState_;
        }
        else
        {
            XINPUT_GAMEPAD& pad = player2Controller.Gamepad;
            player1PacketNumber_ = player2Controller.dwPacketNumber;

            player2State.currentMovementState_.set((size_t)MovementEvent::eUp, pad.sThumbLY > LEFT_THUMB_DEADZONE);
            player2State.currentMovementState_.set((size_t)MovementEvent::eDown, pad.sThumbLY < -LEFT_THUMB_DEADZONE);
            player2State.currentMovementState_.set((size_t)MovementEvent::eLeft, pad.sThumbLX < -LEFT_THUMB_DEADZONE);
            player2State.currentMovementState_.set((size_t)MovementEvent::eRight, pad.sThumbLX > LEFT_THUMB_DEADZONE);

            player2State.currentMetaState_.set((size_t)MetaEvent::eBack, !!(pad.wButtons & (XINPUT_GAMEPAD_BACK | XINPUT_GAMEPAD_B)));
            player2State.currentMetaState_.set((size_t)MetaEvent::ePause, !!(pad.wButtons & XINPUT_GAMEPAD_START));
            player2State.currentMetaState_.set((size_t)MetaEvent::eSelect, !!(pad.wButtons & XINPUT_GAMEPAD_A));
            player2State.currentMetaState_.set((size_t)MetaEvent::eReset, !!(pad.wButtons & XINPUT_GAMEPAD_Y));
            player2State.latestMetaEvent_ = MetaEvent::eNum_;
            player2State.latestMovementEvent_ = MovementEvent::eNum_;
        }
    }
    else
    {
        eventState.connectionState_.set((size_t)PlayerID::e2, false);
    }
}

void HIDManager::XBOXControllerManager::Poll(GameState& eventState)
{
    PollPlayer1(eventState);
    PollPlayer2(eventState);
}

} // namespace events

} // namespace ice
