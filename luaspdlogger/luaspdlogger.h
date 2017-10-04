#ifndef LUASPDLOGGER_H
#define LUASPDLOGGER_H

// ---

spdlog::logger* getLogger(lua_State* l);

// ---

DECLARE_LUA_OBJECT_BEGIN(lua_spdlogger_object)
    EXPORT_METHOD_TO_LUA(l_info)
    EXPORT_METHOD_TO_LUA(l_warn)
    EXPORT_METHOD_TO_LUA(l_error)
    EXPORT_METHOD_TO_LUA(l_critical)

    EXPORT_METHOD_TO_LUA(l_should_log)
    EXPORT_METHOD_TO_LUA(l_set_level)
    EXPORT_METHOD_TO_LUA(l_level)
    EXPORT_METHOD_TO_LUA(l_name)
    EXPORT_METHOD_TO_LUA(l_set_pattern)
    EXPORT_METHOD_TO_LUA(l_flush_on)
DECLARE_LUA_OBJECT_END(lua_spdlogger_object)

// ---

DECLARE_EXPORT_LIB_FUNCTION(libluaspdlogger)

// ---

#endif // LUASPDLOGGER_H
