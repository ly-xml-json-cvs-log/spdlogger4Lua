#include "luaspdlogger_global.h"
#include "luafun.h"
#include "luaspdlogger.h"

// ---

DEFINE_MODULE_NAME(lua_spdlogger_object, "logger_object")
DEFINE_META_TABLE_NAME(lua_spdlogger_object, "496DA522-71FE-48B6-A270-A4186353AEEF")

IMPLEMENT_OPENLIB_METHOD_BEGIN(lua_spdlogger_object)
    META_TABLE_BEGIN
        ITEM_IN_TABLE("info",               lua_spdlogger_object::l_info)
        ITEM_IN_TABLE("warn",               lua_spdlogger_object::l_warn)
        ITEM_IN_TABLE("error",              lua_spdlogger_object::l_error)
        ITEM_IN_TABLE("critical",           lua_spdlogger_object::l_critical)
        ITEM_IN_TABLE("should_log",         lua_spdlogger_object::l_should_log)
        ITEM_IN_TABLE("set_level",          lua_spdlogger_object::l_set_level)
        ITEM_IN_TABLE("level",              lua_spdlogger_object::l_level)
        ITEM_IN_TABLE("name",               lua_spdlogger_object::l_name)
        ITEM_IN_TABLE("set_pattern",        lua_spdlogger_object::l_set_pattern)
        ITEM_IN_TABLE("flush_on",           lua_spdlogger_object::l_flush_on)
    META_TABLE_END

    FUNC_TABLE_BEGIN
    FUNC_TABLE_END

    REGISTER_LUA_OBJECT_METHODS(lua_spdlogger_object)
IMPLEMENT_OPENLIB_METHOD_END

// ---

spdlog::logger* getLogger(lua_State* ls)
{
    Luapp l(ls);
    spdlog::logger **s = (spdlog::logger**)l.checkUData(1, lua_spdlogger_object::mt_lua_spdlogger_object);
    l.argCheck(s != nullptr, 1, "invalid user data");

    return *s;
}

int lua_spdlogger_object::l_should_log(lua_State *ls)
{
    Luapp l(ls);
    int level = l.checkInteger(2);
    getLogger(ls)->should_log(spdlog::level::level_enum(level));
    return 0;
}

int lua_spdlogger_object::l_set_level(lua_State *ls)
{
    Luapp l(ls);
    int level = l.checkInteger(2);
    getLogger(ls)->set_level(spdlog::level::level_enum(level));
    return 0;
}

int lua_spdlogger_object::l_level(lua_State *ls)
{
    Luapp l(ls);
    int level = getLogger(ls)->level();
    l.pushInteger(level);
    return 1;
}

int lua_spdlogger_object::l_name(lua_State *ls)
{
    Luapp l(ls);
    std::string name = getLogger(ls)->name();
    l.pushString(name.c_str());
    return 1;
}

int lua_spdlogger_object::l_set_pattern(lua_State *ls)
{
    Luapp l(ls);
    std::string pattern(l.checkString(2));
    getLogger(ls)->set_pattern(pattern);
    return 0;
}

int lua_spdlogger_object::l_flush_on(lua_State *ls)
{
    Luapp l(ls);
    int level = l.checkInteger(2);
    getLogger(ls)->flush_on(spdlog::level::level_enum(level));
    return 0;
}

int lua_spdlogger_object::l_info(lua_State * ls)
{
    Luapp l(ls);
    getLogger(ls)->info(l.checkString(2));

    return 0;
}

int lua_spdlogger_object::l_warn(lua_State * ls)
{
    Luapp l(ls);
    getLogger(ls)->warn(l.checkString(2));

    return 0;
}

int lua_spdlogger_object::l_error(lua_State * ls)
{
    Luapp l(ls);
    getLogger(ls)->error(l.checkString(2));

    return 0;
}

int lua_spdlogger_object::l_critical(lua_State * ls)
{
    Luapp l(ls);
    getLogger(ls)->critical(l.checkString(2));

    return 0;
}

// ---

IMPLEMENT_EXPORT_LIB_BEGIN(libluaspdlogger)
    EXPORT_MODULE_TO_LUA(lua_spdlogger_object)
    EXPORT_FUNCTIONS_TO_LUA(spdlogger)
IMPLEMENT_EXPORT_LIB_END
