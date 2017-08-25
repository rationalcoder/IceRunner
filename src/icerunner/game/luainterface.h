#ifndef LUAINTERFACE_H
#define LUAINTERFACE_H
#include <string>

typedef struct lua_State lua_State;

namespace ice
{

namespace game
{

class LuaInterface
{
public:
    ~LuaInterface();

    bool Init();
    bool Export();
    bool RunFile(const std::string& name);
    const std::string& GetError() const { return error_; }

private:
    std::string error_;
    lua_State* pLuaState_;
};

} // namespace game

} // namespace ice


#endif // LUAINTERFACE_H
