#include <lua.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <cstdlib>
#include "map.hpp"
#include "mapfactory.hpp"
#include "luainterface.h"
#include "deckloader.h"

static luaL_Reg nullReg[] = {{nullptr, nullptr}};

namespace ice
{

namespace game
{

bool LuaInterface::Init()
{
    pLuaState_ = luaL_newstate();
    if (!pLuaState_) return false;
    luaL_openlibs(pLuaState_);
    return true;
}

LuaInterface::~LuaInterface()
{
    lua_close(pLuaState_);
}

bool LuaInterface::RunFile(const std::string& name)
{
    if (luaL_dofile(pLuaState_, name.c_str()) != LUA_OK)
    {
        size_t len = 0;
        const char* pError = lua_tolstring(pLuaState_, -1, &len);
        error_.clear();
        error_.insert(0, pError, len);
        error_ += "\n";
        lua_pop(pLuaState_, 1);
        return false;
    }
    return true;
}

// lua interface setup

//! Unique IDs for all of the objects in the Lua interface to be used for type-checking
typedef enum : lua_Integer
{
    ID_DECK,
    ID_MAP,
    ID_PLAYER,
    ID_GOAL,
    ID_WALL,
    ID_RIGHT_EXT,
    ID_LEFT_EXT,
    ID_UP_EXT,
    ID_DOWN_EXT,
    ID_MAP_KIT,
    ID_MAP_FACTORY,
    ID_RGB,
    ID_DECK_SETTINGS,
    ID_FACTORY_MAP
} LuaObjectID;

typedef enum : lua_Integer
{
    IDX_ICE_RUNNER = 1,
    IDX_OFFSET
} SpecialIndex;

//! Extract an object from a table on top of the stack.
//!
//! \tparam T_ The type to extract.
//! \param idx The absolute index of the table from which to extract the object.
//! \param targetID The ID of the object you want to extract.
//! \returns A pointer to the object on success, nullptr on failure.
//!
template <typename T_>
static inline T_* ExtractObject(lua_State* L, unsigned idx, LuaObjectID targetID)
{
    // [idx]: possible table
    if(!lua_istable(L, (int)idx)) return nullptr;
    lua_getfield(L, (int)idx, "_id");
    // [idx]: table
    // [-1]: possible id
    int isNum = 0;
    lua_Integer id = lua_tointegerx(L, -1, &isNum);
    if (!isNum || id != targetID)
    {
        lua_pop(L, 1);
        printf("Failed to extract object: invalid id: got %d, expected: %d\n", (int)id, (int)targetID);
        return nullptr;
    }

    // [idx]: table
    // [-1]: id
    lua_getfield(L, idx, "_instance");
    // [idx]: table
    // [-2]: id
    // [-1]: possible userdata
    T_* pUserData = (T_*)lua_touserdata(L, -1);
    if (!pUserData) printf("Failed to extract object: invalid userdata: %p\n", pUserData);
    // if the userdata is invalid or it is NULL, we will end up returning NULL, which is what we want.
    lua_pop(L, 2); // leave the stack the way it was when we got it.
    return pUserData;
}

template <typename T_>
static inline int ObjectGC(lua_State* L)
{
    T_* userdata = (T_*)lua_touserdata(L, -1);
    assert(userdata && "object not garbage collected correctly");
    userdata->~T_();
    return 0;
}

//! Create a new object an push it on the top of the stack.
//!
//! \tparam T_ The type of object to push.
//! \tparam Args_ arguments to pass to the objects constructor.
//! \param reg The array of luaL_Reg to register to the new table.
//! \param id The id of object to create.
//! \note The c++ object must be default constructible for now.
//!
template <typename T_, typename ...Args_>
static inline T_* PushObject(lua_State* L,luaL_Reg reg[], LuaObjectID id, Args_&&... args)
{
    lua_newtable(L); // Lua instance table
    lua_newuserdata(L, sizeof(T_));
    // [-2]: instance table
    // [-1]: uninitialized instance userdata
    T_* pNewObj = (T_*)lua_touserdata(L, -1);
    new (pNewObj) T_(std::forward<Args_>(args)...); // construct the object in place.
    // [-2]: instance table
    // [-1]: initialized instance userdata
    lua_newtable(L); // create the userdata's metatable
    lua_pushcfunction(L, &ObjectGC<T_>); // push the __gc metamethod
    // [-4]: instance table
    // [-3]: initialized instance userdata
    // [-2]: empty metatable
    // [-1]: __gc metamethod
    lua_setfield(L, -2, "__gc");
    // [-3]: instance table
    // [-2]: initialized instance userdata
    // [-1]: metatable with __gc metamethod
    lua_setmetatable(L, -2);
    // [-2]: instance table
    // [-1]: initialized instance userdata with metatable
    lua_setfield(L, -2, "_instance");
    // [-1]: instance table
    lua_pushinteger(L, id);
    // [-2]: instance table
    // [-1]: object id
    lua_setfield(L, -2, "_id");
    // [-1]: instance table
    luaL_setfuncs(L, reg, 0); // register all functions into the instance table
    // [-1]: instance table
    return pNewObj;
}

static inline bool ExportLeafTable(lua_State* L, lua_CFunction callOperator, const char* pName)
{
    // [-1]: parent table
    lua_newtable(L); // table
    lua_newtable(L); // metatable
    lua_pushcfunction(L, callOperator); // __call metamethod
    // [-4]: parent table
    // [-3]: table
    // [-2]: metatable
    // [-1]: __call metafunction
    lua_setfield(L, -2, "__call");
    // [-3]: parent table
    // [-2]: table
    // [-1]: metatable
    lua_setmetatable(L, -2);
    // [-3]: parent table
    // [-2]: table
    lua_setfield(L, -2, pName);
    // [-3]: parent table
    // [-2]: table
    return true;
}

class GeneralInterface
{
public:
    class RGBInterface
    {
    public:
        static int CallOperator(lua_State* L)
        {
            // [1]: RGB table
            lua_remove(L, 1);
            // [1..3]: possible R,G,B Numbers
            lua_Number r = luaL_checknumber(L, 1);
            lua_Number g = luaL_checknumber(L, 2);
            lua_Number b = luaL_checknumber(L, 3);
            luaL_Reg colorFunctions[] = {{nullptr, nullptr}};
            glm::vec3* pColor = PushObject<glm::vec3>(L, colorFunctions, ID_RGB);
            pColor->r = (float)r;
            pColor->g = (float)g;
            pColor->b = (float)b;
            // [1..3]: R,G,B Numbers (done with them)
            // RGB instance table
            return 1; // return the RGB instance table.
        }

        static bool Export(lua_State* L)
        {
            return ExportLeafTable(L, &CallOperator, "RGB");
        }
    };

public:
    static bool Export(lua_State* L)
    {
        // [1]: IceRunner
        lua_newtable(L); // General table.
        bool result = RGBInterface::Export(L);
        lua_setfield(L, IDX_ICE_RUNNER, "General");
        // [1]: IceRunner
        return result;
    }
};

class MapToolsInterface
{
public:
    class MapKitInterface
    {
    public:
        //! Takes a table containing the size and walls of the map and returns a new MapKit
        static int CallOperator(lua_State* L)
        {
            // [1]: MapKit table
            // [2]: possible argument table
            lua_remove(L, 1);
            // [1]: possible argument table
            if (lua_gettop(L) != 1) return luaL_error(L, "Expected 1 argument.");
            if (!lua_istable(L, -1)) return luaL_argerror(L, 1, "Expected argument table.");

            lua_getfield(L, -1, "size");
            int isNum = 0;
            lua_Integer size = lua_tointegerx(L, -1, &isNum);
            if (!isNum) return luaL_argerror(L, 1, "Expected field of size => Integer.");
            // [1]: argument table
            // [2]: map size
            lua_getfield(L, 1, "walls");
            lua_Integer numWalls = lua_tointegerx(L, -1, &isNum);
            if (!isNum) return luaL_argerror(L, 1, "Expected field of walls => Integer.");
            // [1]: argument table
            // [2]: map size
            // [3]: num walls
            luaL_Reg kitFunctions[] = {{nullptr, nullptr}};
            MapKit* pKit = PushObject<MapKit>(L, kitFunctions, ID_MAP_KIT);
            // [1]: argument table
            // [2]: map size
            // [3]: num walls
            // [4]: MapKit instance table
            MapKit::Description desc;
            desc.dimensions.numRows = (uint32_t)size;
            desc.dimensions.numColumns = (uint32_t)size;
            desc.wallCount = (uint32_t)numWalls;
            pKit->SetDescription(desc);
            return 1; // return the instance table.
        }

        static bool Export(lua_State* L)
        {
            // [1]: IceRunner
            // [2]: MapTools
            return ExportLeafTable(L, &CallOperator, "MapKit");
            // [1]: IceRunner
            // [2]: MapTools
        }
    };

    class ExtentInterface
    {
    public:
        template <typename Extent_, LuaObjectID ID_>
        static int CommonCallOperator(lua_State* L)
        {
            lua_remove(L, 1);
            // [-1]: possible number
            if (!lua_isinteger(L, -1)) return luaL_argerror(L, 1, "Expected integer");
            lua_Integer ext = lua_tointeger(L, -1);
            PushObject<Extent_>(L, nullReg, ID_, (typename Extent_::Rep)ext);
            return 1;
        }
    };


public:
    static bool Export(lua_State* L)
    {
        // [1]: IceRunner
        lua_newtable(L); // MapTools
        bool result = MapKitInterface::Export(L);
        // [1]: IceRunner
        // [2]: MapTools
        result &= ExportLeafTable(L, &ExtentInterface::CommonCallOperator<map_tools::UpExtent, ID_UP_EXT>, "UpExtent");
        result &= ExportLeafTable(L, &ExtentInterface::CommonCallOperator<map_tools::DownExtent, ID_DOWN_EXT>, "DownExtent");
        result &= ExportLeafTable(L, &ExtentInterface::CommonCallOperator<map_tools::LeftExtent, ID_LEFT_EXT>, "LeftExtent");
        result &= ExportLeafTable(L, &ExtentInterface::CommonCallOperator<map_tools::RightExtent, ID_RIGHT_EXT>, "RightExtent");

        lua_setfield(L, IDX_ICE_RUNNER, "MapTools");
        // [1]: IceRunner
        return result;
    }
};

struct MapInterface
{
    struct WallInterface
    {
        static int CallOperator(lua_State* L)
        {
            // [1]: Wall table
            // [2]: possible integer
            // [3]: possible integer
            lua_remove(L, 1);
            if (lua_gettop(L) != 2) return luaL_error(L, "Expected two arguments");
            if (!lua_isinteger(L, -2)) return luaL_argerror(L, 1, "Expected integer as first argument");
            if (!lua_isinteger(L, -1)) return luaL_argerror(L, 1, "Expected integer as second argument");
            lua_Integer first = lua_tointeger(L, -2);
            lua_Integer second = lua_tointeger(L, -1);
            PushObject<Map::Wall>(L, nullReg, ID_WALL, (Map::Wall::Rep)first, (Map::Wall::Rep)second);
            return 1;
        }

        static bool Export(lua_State* L)
        {
            // [1]: IceRunner
            // [2]: Map
            return ExportLeafTable(L, &CallOperator, "Wall");
        }
    };

    struct PlayerInterface
    {
        static int CallOperator(lua_State* L)
        {
            // [1]: Player table
            // [2]: possible integer
            // [3]: possible integer
            lua_remove(L, 1);
            if (lua_gettop(L) != 2) return luaL_error(L, "Expected two arguments");
            if (!lua_isinteger(L, -2)) return luaL_argerror(L, 1, "Expected integer as first argument");
            if (!lua_isinteger(L, -1)) return luaL_argerror(L, 1, "Expected integer as second argument");
            lua_Integer first = lua_tointeger(L, -2);
            lua_Integer second = lua_tointeger(L, -1);
            PushObject<Map::Player>(L, nullReg, ID_PLAYER, (Map::Player::Rep)first, (Map::Player::Rep)second);
            return 1;
        }

        static bool Export(lua_State* L)
        {
            // [1]: IceRunner
            // [2]: Map
            return ExportLeafTable(L, &CallOperator, "Player");
        }
    };

    struct GoalInterface
    {
        static int CallOperator(lua_State* L)
        {
            // [1]: Goal table
            // [2]: possible integer
            // [3]: possible integer
            lua_remove(L, 1);
            if (lua_gettop(L) != 2) return luaL_error(L, "Expected two arguments");
            if (!lua_isinteger(L, -2)) return luaL_argerror(L, 1, "Expected integer as first argument");
            if (!lua_isinteger(L, -1)) return luaL_argerror(L, 1, "Expected integer as second argument");
            lua_Integer first = lua_tointeger(L, -2);
            lua_Integer second = lua_tointeger(L, -1);
            PushObject<Map::Goal>(L, nullReg, ID_GOAL, (Map::Goal::Rep)first, (Map::Goal::Rep)second);
            return 1;
        }

        static bool Export(lua_State* L)
        {
            // [1]: IceRunner
            // [2]: Map
            return ExportLeafTable(L, &CallOperator, "Goal");
        }
    };

    static int AddWalls(lua_State* L)
    {
        // [1]: Map
        // [2]: possible Wall
        // [3]: possible extent
        if (lua_gettop(L) != 3) return luaL_error(L, "add_walls Expects three arguments. Did you forget the ':'?");
        if (!lua_istable(L, 1)) return luaL_argerror(L, 1, "Expected a table. Did you forget the ':'?");
        Map* pMap = ExtractObject<Map>(L, 1, ID_MAP);
        if (!pMap) return luaL_argerror(L, 1, "Expected Map");
        lua_rotate(L, 2, 1);
        // [1]: Map
        // [2]: possible extent
        // [3]: possible Wall
        Map::Wall* pWall = ExtractObject<Map::Wall>(L, 3, ID_WALL);
        if (!pWall) return luaL_argerror(L, 2, "Expected a Wall");
        Map::Wall wall = *pWall;
        lua_pop(L, 1);
        // [1]: Map
        // [2]: possible extent
        if(!lua_istable(L, -1)) return luaL_argerror(L, 2, "Expected Extent");
        lua_getfield(L, -1, "_id");
        int isNum = 0;
        lua_Integer id = lua_tointegerx(L, -1, &isNum);
        if (!isNum) return luaL_argerror(L, 2, "Expected Extent");
        lua_pop(L, 1);
        // [1]: Map
        // [2]: possible extent
        lua_getfield(L, -1, "_instance");
        map_tools::detail::DirectionalExtent* pExt = (map_tools::detail::DirectionalExtent*)lua_touserdata(L, -1);
        if (!pExt) return luaL_argerror(L, 2, "Expected Extent");
        // [1]: Map
        // [2]: possible extent
        // [3]: id
        // [4]: instance
        MapKit kit = pMap->GetKit();
        MapKit::Dimensions dimensions = kit.GetDimensions();
        MapKit::WallCount wallCount = kit.GetWallCount();
        auto ext = (map_tools::detail::DirectionalExtent::Rep)*pExt;
        const char* outOfBoundsMessage = "Wall position plus extent runs out of bounds.";
        const char* tooManyWallsMessage = "Maximum wall count exceeded.";
        switch (id)
        {
        case ID_UP_EXT:
            if (wall.GetRow() - ext <= 0) return luaL_argerror(L, 2, outOfBoundsMessage);
            if (ext + 1 >= (int)wallCount) return luaL_argerror(L, 2, tooManyWallsMessage);
            pMap->AddWalls(wall, *((map_tools::UpExtent*)pExt));
            break;
        case ID_DOWN_EXT:
            if (wall.GetRow() + ext >= (int)dimensions.numRows) return luaL_argerror(L, 3, outOfBoundsMessage);
            if (ext + 1 >= (int)wallCount) return luaL_argerror(L, 3, tooManyWallsMessage);
            pMap->AddWalls(wall, *((map_tools::DownExtent*)pExt));
            break;
        case ID_LEFT_EXT:
            if (wall.GetColumn() - ext < 0) return luaL_argerror(L, 3, outOfBoundsMessage);
            if (ext + 1 >= (int)wallCount) return luaL_argerror(L, 3, tooManyWallsMessage);
            pMap->AddWalls(wall, *((map_tools::LeftExtent*)pExt));
            break;
        case ID_RIGHT_EXT:
            if (wall.GetColumn() + ext >= (int)dimensions.numColumns) return luaL_argerror(L, 3, outOfBoundsMessage);
            if (ext + 1 >= (int)wallCount) return luaL_argerror(L, 3, tooManyWallsMessage);
            pMap->AddWalls(wall, *((map_tools::RightExtent*)pExt));
            break;
        default: return luaL_argerror(L, 2, "Expected Extent");
        }
        return 0;
    }

    static int SetPlayer(lua_State* L)
    {
        // [1]: possible Map
        // [2]: possible Player
        if (lua_gettop(L) != 2) return luaL_error(L, "%sExpected 2 arguments. Did you forget the ':'?", (luaL_where(L, 0), lua_tostring(L, -1)));

        Map* pMap = ExtractObject<Map>(L, 1, ID_MAP);
        if (!pMap) return luaL_argerror(L, 1, "Expected Map");
        Map::Player* pPlayer = ExtractObject<Map::Player>(L, 2, ID_PLAYER);
        if (!pPlayer) return luaL_argerror(L, 2, "Expected Player");
        pMap->SetPlayer(*pPlayer);
        return 0;
    }

    static int SetGoal(lua_State* L)
    {
        // [1]: possible Map
        // [2]: possible Goal
        if (lua_gettop(L) != 2) return luaL_error(L, "%sExpected 2 arguments. Did you forget the ':'?", (luaL_where(L, 0), lua_tostring(L, -1)));

        Map* pMap = ExtractObject<Map>(L, 1, ID_MAP);
        if (!pMap) return luaL_argerror(L, 1, "Expected Map");
        Map::Goal* pGoal = ExtractObject<Map::Goal>(L, 2, ID_GOAL);
        if (!pGoal) return luaL_argerror(L, 2, "Expected Goal");
        pMap->SetGoal(*pGoal);
        return 0;
    }

    //! Takes a table containing the size, difficuly level, and number of walls in the map and returns the new Map
    static int CallOperator(lua_State* L)
    {
        // [1]: Map
        // [2]: possible input table
        lua_remove(L, 1);
        // [1]: possible argument table
        if (lua_gettop(L) != 1) return luaL_error(L, "Expected 1 argument.");
        if (!lua_istable(L, -1)) return luaL_argerror(L, 1, "Expected argument table.");
        lua_getfield(L, -1, "name");
        // [1]: input table
        // [2]: possible name string
        if (!lua_isstring(L, -1)) return luaL_argerror(L, 1, "Expected field of name => string");
        std::string mapName(lua_tostring(L, -1));
        lua_pop(L, 1);
        lua_getfield(L, -1, "level");
        // [1]: input table
        // [2]: possible level number
        if (!lua_isinteger(L, -1)) return luaL_argerror(L, 1, "Expected field of level => integer");
        lua_Integer level = lua_tointeger(L, -1);
        lua_pop(L, 1);
        lua_getfield(L, -1, "kit");
        // [1]: input table
        // [2]: possible map kit
        MapKit* pKit = ExtractObject<MapKit>(L, 2, ID_MAP_KIT);
        if (!pKit) return luaL_argerror(L, 1, "Expected field of kit => MapKit");
        Map::Description desc(mapName, Map::Difficulty((Map::Difficulty::Rep)level), *pKit);
        luaL_Reg reg[] =
        {
            {"add_walls", &AddWalls},
            {"set_player", &SetPlayer},
            {"set_goal", &SetGoal},
            {NULL, NULL},
        };
        PushObject<Map>(L, reg, ID_MAP, desc);
        // [1]: input table
        // [2]: MapKit
        // [3]: Map
        return 1;
    }

    static bool Export(lua_State* L)
    {
        // [1]: IceRunner
        lua_newtable(L); // Map
        lua_newtable(L); // Map metatable
        lua_pushcfunction(L, &CallOperator);
        // [1]: IceRunner
        // [2]: Map
        // [3]: Map metatable
        // [4]: __call metamethod
        lua_setfield(L, -2, "__call");
        // [1]: IceRunner
        // [2]: Map
        // [3]: Map metatable
        lua_setmetatable(L, -2);
        // [1]: IceRunner
        // [2]: Map
        bool result = PlayerInterface::Export(L);
        result &= WallInterface::Export(L);
        result &= GoalInterface::Export(L);
        // [1]: IceRunner
        // [2]: Map
        lua_setfield(L, -2, "Map");
        // [1]: IceRunner
        return true;
    }
};

class DeckSettingsInterface
{
public:
    class MapFactoryInterface
    {
    public:
        //! Return a new instance of MapFactory
        static int CallOperator(lua_State* L)
        {
            // [1]: MapFactory table
            // [2]: possible input table
            lua_remove(L, 1);
            // [1]: possible input table
            if (lua_gettop(L) != 1) return luaL_error(L, "Expected 1 argument");
            if (!lua_istable(L, -1)) return luaL_argerror(L, 1, "Table expected");
            // [1]: input table
            lua_getfield(L, 1, "map_color");
            // [1]: input table
            // [2]: possible RGB table
            glm::vec3* pMapColor = ExtractObject<glm::vec3>(L, 2, ID_RGB);
            if (!pMapColor) return luaL_argerror(L, 1, "Expected field of map_color => RGB");
            lua_pop(L, 1);
            lua_getfield(L, 1, "player_color");
            // [1]: input table
            // [2]: possible RGB table
            glm::vec3* pPlayerColor = ExtractObject<glm::vec3>(L, 2, ID_RGB);
            if (!pPlayerColor) return luaL_argerror(L, 1, "Expected field of player_color => RGB");
            lua_pop(L, 1);
            lua_getfield(L, 1, "wall_color");
            // [1]: input table
            // [2]: possible RGB table
            glm::vec3* pWallColor = ExtractObject<glm::vec3>(L, 2, ID_RGB);
            if (!pWallColor) return luaL_argerror(L, 1, "Expected field of wall_color => RGB");
            lua_pop(L, 1);
            lua_getfield(L, 1, "goal_color");
            // [1]: input table
            // [2]: possible RGB table
            glm::vec3* pGoalColor = ExtractObject<glm::vec3>(L, 2, ID_RGB);
            if (!pGoalColor) return luaL_argerror(L, 1, "Expected field of goal_color => RGB");
            lua_pop(L, 1);
            lua_getfield(L, 1, "kit");
            // [1]: input table
            // [2]: possible MapKit table
            MapKit* pKit = ExtractObject<MapKit>(L, 2, ID_MAP_KIT);
            if (!pKit) return luaL_error(L, "Expected field of kit => MapKit");
            lua_pop(L, 1);
            // [1]: input table
            luaL_Reg factoryFunctions[] = { {nullptr, nullptr} };
            MapFactory* pNewFactory = PushObject<MapFactory>(L, factoryFunctions, ID_MAP_FACTORY);
            pNewFactory->SetMapColor(*pMapColor);
            pNewFactory->SetPlayerColor(*pPlayerColor);
            pNewFactory->SetMapKit(*pKit);
            pNewFactory->SetWallColor(*pWallColor);
            pNewFactory->SetGoalColor(*pGoalColor);
            // [1]: input table
            // [2]: MapFactory instance table
            return 1;
        }

        static bool Export(lua_State* L)
        {
            // [1]: IceRunner
            // [2]: DeckSettings
            lua_newtable(L); // MapFactory table
            lua_newtable(L); // MapFactory metatable
            lua_pushcfunction(L, &CallOperator);
            // [1]: IceRunner
            // [2]: DeckSettings
            // [3]: MapFactory table
            // [4]: MapFactory metatable
            // [5]: __call metamethod
            lua_setfield(L, -2, "__call");
            lua_setmetatable(L, -2);
            // [1]: IceRunner
            // [2]: DeckSettings
            // [3]: MapFactory table
            lua_setfield(L, -2, "MapFactory");
            // [1]: IceRunner
            // [2]: DeckSettings
            return true;
        }
    };

public:
    //! Returns a new instance of DeckSettings
    static int CallOperator(lua_State* L)
    {
        luaL_Reg reg[] =
        {
            {"set_factories", SetFactories},
            {nullptr, nullptr}
        };

        PushObject<DeckSettings>(L, reg, ID_DECK_SETTINGS);
        return 1; // return the instance
    }

    static int SetFactories(lua_State* L)
    {
        unsigned numArgs = lua_gettop(L);
        if (numArgs != 2) return luaL_error(L, "set_factories expected 2 arguments, got %d.", numArgs);

        // [1]: DeckSettings instance
        // [2]: factory map
        DeckSettings* pDeckSettings = ExtractObject<DeckSettings>(L, 1, ID_DECK_SETTINGS);
        if (!pDeckSettings) return luaL_error(L, "invalid first parameter to set_factories.");
        assert(lua_gettop(L) == 2 && "Bug in Extract");

        DeckLoader& loader = DeckLoader::GetInstance();
        // [1]: DeckSettings instance
        // [2]: factory map
        for(lua_Integer level = 1; true; level++, lua_pop(L, 1))
        {
            lua_geti(L, 2, level);
            MapFactory* pFactory = ExtractObject<MapFactory>(L, 3, ID_MAP_FACTORY);
            if (!pFactory) break;
            Map::Difficulty difficulty((Map::Difficulty::Rep)level);
            pFactory->SetDifficulty(difficulty);
            loader.GetDeckSettings().SetFactoryAt(difficulty, *pFactory);
        }

        return 0;
    }

    static bool Export(lua_State* L)
    {
        // [1]: IceRunner
        lua_newtable(L); // DeckSettings
        // [1]: IceRunner
        // [2]: DeckSettings
        lua_newtable(L); // DeckSettings metatable
        lua_pushcfunction(L, &CallOperator); // push the call metafunction
        // [1]: IceRunner
        // [2]: DeckSettings
        // [3]: DeckSettings metatable
        // [4]: __call metafunction
        lua_setfield(L, -2, "__call"); // add it to the DeckSettings metatable
        lua_setmetatable(L, -2); // set the DeckSettings metatable
        // [1]: IceRunner
        // [2]: DeckSettings
        if (!MapFactoryInterface::Export(L)) return false;
        lua_setfield(L, IDX_ICE_RUNNER, "DeckSettings");
        // [1]: IceRunner
        return true;
    }
};

class DeckInterface
{
public:
    static int CallOperator(lua_State* L)
    {
        // [1]: Deck table
        // [2..n] args
        lua_remove(L, 1);
        // [1..n] args
        lua_Integer numArgs = lua_gettop(L);
        if (numArgs != 1) return luaL_error(L, "Deck() takes 1 argument, got %d\n", (int)numArgs);
        if (!lua_istable(L, -1)) return luaL_argerror(L, 1, "expected argument table");
        if (lua_getfield(L, -1, "name") != LUA_TSTRING) return luaL_argerror(L, 1, "expected field of `name => string`");
        // [1]: input table
        // [2]: name
        std::string name(lua_tostring(L, -1));
        lua_pop(L, 1);
        Deck* pDeck = PushObject<Deck>(L, nullReg, ID_DECK);
        pDeck->SetName(name);
        // [1]: input table
        // [2]: Deck
        if (lua_getfield(L, 1, "map_list") != LUA_TTABLE) return luaL_argerror(L, 1, "expected field of `map_list => table`");
        // [1]: input table
        // [2]: Deck
        // [3]: map name table

        DeckLoader& loader = DeckLoader::GetInstance();
        // while we keep finding map names in the table..
        for (int i = 1; lua_rawgeti(L, -1, i) == LUA_TSTRING; lua_pop(L, 1), i++)
        {
            const char* pName = lua_tostring(L, -1);
            Map* pMap = loader.MapNamed(pName);
            if (!pMap) return luaL_error(L, "Map %s not found in your maps directory.", pName);
            if (pMap->GetDescription().difficulty != i) return luaL_error(L, "Map %s (level %d) is not a level %d map.", pName, pMap->GetDescription().difficulty, i);
            pDeck->AddMap(pMap);
        }
        // [1]: input table
        // [2]: Deck
        // [3]: map name table
        // [4]: the nil value that stopped the above loop.
        lua_pop(L, 2);

        // [1]: input table
        // [2]: Deck
        return 1;
    }

    static bool Export(lua_State* L)
    {
        // [1]: IceRunner
        lua_newtable(L); // Deck table
        lua_newtable(L); // Deck metatable
        lua_pushcfunction(L, &CallOperator); // _call metamethod
        // [1]: IceRunner
        // [2]: Deck
        // [3]: Deck metatable
        // [4]: _call metamethod
        lua_setfield(L, -2, "__call");
        lua_setmetatable(L, -2);
        // [1]: IceRunner
        // [2]: Deck
        lua_setfield(L, -2, "Deck");
        // [1]: IceRunner
        return true;
    }
};

class IceRunnerInterface
{
public:
    static int Configure(lua_State* L)
    {
        (void)L;
        return 0;
    }

    static int RegisterMap(lua_State* L)
    {
        // [1]: possible Map
        if (lua_gettop(L) != 1) return luaL_error(L, "register_map expects 1 argument.");
        Map* pMap = ExtractObject<Map>(L, 1, ID_MAP);
        luaL_argcheck(L, pMap, 1, "Expected Map");
        DeckLoader::GetInstance().AddMap(*pMap);
        return 0;
    }

    static int RegisterDeck(lua_State* L)
    {
        if (lua_gettop(L) != 1) return luaL_error(L, "register_deck expects 1 argument");
        Deck* pDeck = ExtractObject<Deck>(L, 1, ID_DECK);
        if (!pDeck) printf("didn't get a deck\n");
        luaL_argcheck(L, pDeck, 1, "Expected Deck");
        DeckLoader::GetInstance().AddDeck(*pDeck);
        return 0;
    }

    static bool Export(lua_State* L)
    {
        lua_newtable(L); // IceRunner
        bool result = DeckSettingsInterface::Export(L);
        result &= MapToolsInterface::Export(L);
        result &= GeneralInterface::Export(L);
        result &= MapInterface::Export(L);
        result &= DeckInterface::Export(L);

        luaL_Reg funcs[] =
        {
            {"configure", &Configure},
            {"register_map", &RegisterMap},
            {"register_deck", &RegisterDeck},
            {nullptr, nullptr}
        };

        luaL_setfuncs(L, funcs, 0);
        lua_setglobal(L, "IceRunner");
        assert(lua_gettop(L) == 0 && "Stack should be empty");
        return result;
    }
};

bool LuaInterface::Export()
{
    return IceRunnerInterface::Export(pLuaState_);
}

} // namespace game

} // namespace ice
