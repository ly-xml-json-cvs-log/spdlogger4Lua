#include "luaauto_global.h"
#include "fun.h"
#include "luafun.h"

// ---

IMPLEMENT_OPENLIB_FUNCTION_BEGIN(some_funs)
    LIST_FUNCTIONS_BEGIN
        ITEM_IN_FUNCTIONS(add)
        ITEM_IN_FUNCTIONS(sub)
    LIST_FUNCTIONS_END
IMPLEMENT_OPENLIB_FUNCTION_END

// ---

int lua_add(lua_State * ls)
{
    Luapp l(ls);
    auto r = l.dispatchToFunction(add, l.toNumber(1), l.toNumber(2));
    l.pop(2);
    l.pushNumber(r);

    return 1;
}

int lua_sub(lua_State * ls)
{
    Luapp l(ls);
    auto r = l.dispatchToFunction(sub, l.toNumber(1), l.toNumber(2));
    l.pop(2);
    l.pushNumber(r);

    return 1;
}
