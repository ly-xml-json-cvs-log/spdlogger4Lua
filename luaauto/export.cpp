#include "luaauto_global.h"
#include "luaauto.h"
#include "luafun.h"
#include "export.h"

// ---

IMPLEMENT_EXPORT_LIB_BEGIN(libluaauto)
    EXPORT_MODULE_TO_LUA(luaAuto)
    EXPORT_MODULE_TO_LUA(luaCar)
    EXPORT_MODULE_TO_LUA(luaTesla)
    EXPORT_MODULE_TO_LUA(luaSuperCar)
    EXPORT_MODULE_TO_LUA(luaAirVehicle)
    EXPORT_MODULE_TO_LUA(luaSeafaring)

    EXPORT_FUNCTIONS_TO_LUA(some_funs)
IMPLEMENT_EXPORT_LIB_END


/*
LUAPPSHARED_C_EXPORT int luaopen_libluaauto(lua_State* L)
{
    static const luaL_Reg lualibs[] =
    {
        {"Auto",                luaAuto::open_luaAuto},
        {"Car",                 luaCar::open_luaCar},
        {"Tesla",               luaTesla::open_luaTesla},
        {"AirVehicle",          luaAirVehicle::open_luaAirVehicle},
        {"Seafaring",           luaSeafaring::open_luaSeafaring},
        {"SuperCar",            luaSuperCar::open_luaSuperCar},
        {"some_funs",           open_some_funsCar},
        {NULL, NULL}
    };

    Luapp lp(L);
    return lp.requireLibs(lualibs);
}
*/
