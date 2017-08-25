#ifndef HIDMANAGER_H
#define HIDMANAGER_H
#include <cstdint>

namespace ice
{

namespace events
{

class GameState;

class HIDManager
{
public:
    class XBOXControllerManager
    {
    public:
        XBOXControllerManager()
            : player1PacketNumber_(0), player2PacketNumber_(0)
        {}

        bool Init();
        void ShutDown();
        void Poll(GameState& eventState);

    private:
        void PollPlayer1(GameState& eventState);
        void PollPlayer2(GameState& eventState);

    private:
        uint32_t player1PacketNumber_;
        uint32_t player2PacketNumber_;

    };
    XBOXControllerManager& GetXBOXControllerManager() { return xboxManager_; }

private:
    XBOXControllerManager xboxManager_;
};

} // namespace events

} // namespace ice

#endif // HIDMANAGER_H
