#include "luapp_global.h"
#include "luapp.h"

// ---

template<>
void f_push(lua_State * ls, int const& i)
{ Luapp(ls).pushInteger(i); }

template<>
void f_push(lua_State *ls, lua_Number const& d)
{ Luapp(ls).pushNumber(d); }

template<>
void f_push(lua_State *ls, std::string const& s)
{ Luapp(ls).pushString(s.c_str()); }

template<>
void f_push(lua_State * ls, char const* const& s)
{ Luapp(ls).pushString(s); }

template<>
void f_push(lua_State * ls, lua_State* const& i)
{ UNUSED(ls); Luapp((lua_State*)i).pushThread(); }

template<>
void f_push(lua_State * ls, lua_CFunction const& i)
{ Luapp(ls).pushCClosure(i, 0); }

template<>
void f_push(lua_State * ls, void* const& i)
{ Luapp(ls).pushLightUserdata((void*)i); }

template<>
void f_push(lua_State * ls, bool const& i)
{ Luapp(ls).pushBoolean(i); }

template<>
void f_push(lua_State * ls, lua_Integer const& i)
{ Luapp(ls).pushInteger(i); }

//---

template<>
void f_pop(lua_State * ls, int& i)
{ Luapp l(ls); i = l.toInteger(-1); l.pop(1); }

template<>
void f_pop(lua_State * ls, lua_Number& d)
{ Luapp l(ls); d = l.toNumber(-1); l.pop(1); }

template<>
void f_pop(lua_State * ls, std::string& s)
{ Luapp l(ls); s = l.toString(-1); l.pop(1); }

template<>
void f_pop(lua_State * ls, lua_State*& s)
{ Luapp l(ls); s = l.toThread(-1); l.pop(1); }

template<>
void f_pop(lua_State * ls, lua_CFunction& s)
{ Luapp l(ls); s = l.toCFunction(-1); l.pop(1); }

template<>
void f_pop(lua_State * ls, void*& s)
{ Luapp l(ls); s = l.toUserdata(-1); l.pop(1); }

template<>
void f_pop(lua_State * ls, bool& s)
{ Luapp l(ls); s = l.toBoolean(-1); l.pop(1); }

template<>
void f_pop(lua_State * ls, lua_Integer& s)
{ Luapp l(ls); s = l.toInteger(-1); l.pop(1); }

//---

void stackindex_stdcout(lua_State *ls, int idx)
{
    Luapp l(ls);
    std::cout << "[" << idx << "] = " << l.valueToString(idx) << std::endl;

}

void keyvalue_stdcout(lua_State *ls)
{
    Luapp l(ls);
    std::cout << "[" << l.toString(-1) << "] = " << l.valueToString(-2) << std::endl;
}

// ---

std::string LuaStack::valueToString(int idx)
{
    std::stringstream r;
    r << typeNameFromIndex(idx) << ": ";

    switch (type(idx)) {
    case LUA_TNONE:
        r << "none";
        break;

    case LUA_TNIL:
        r << "nil";
        break;

    case LUA_TBOOLEAN:
        r << std::boolalpha << toBoolean(idx);
        break;

    case LUA_TNUMBER:
        r << toNumber(idx);
        break;

    case LUA_TSTRING:
        r << toString(idx);
        break;

    case LUA_TFUNCTION:
        r << toPointer(idx);
        break;

    case LUA_TUSERDATA:
        r << toUserdata(idx);
        break;

    case LUA_TLIGHTUSERDATA:
        r << toPointer(idx);
        break;

    case LUA_TTABLE:
        r << toPointer(idx);
        break;

    case LUA_TTHREAD:
        r << toPointer(idx);
        break;

    default:
        assert(false);
    }

    return r.str();
}

std::string LuaStack::typeNameFromIndex(int idx)
{
    return luaL_typename(m_ls, idx);
}

LuaStack::LuaStack(lua_State *ls)
    : m_ls(ls)
{

}

LuaStack::~LuaStack()
{

}

int LuaStack::absIndex(int idx) const
{
    return lua_absindex(m_ls, idx);
}

int LuaStack::getTop() const
{
    return lua_gettop(m_ls);
}

void LuaStack::setTop(int idx)
{
    lua_settop(m_ls, idx);
}

void LuaStack::pushValue(int idx)
{
    lua_pushvalue(m_ls, idx);
}

void LuaStack::rotate(int idx, int n)
{
    lua_rotate(m_ls, idx, n);
}

void LuaStack::copy(int fromidx, int toidx)
{
    lua_copy(m_ls, fromidx, toidx);
}

int LuaStack::checkStack(int n)
{
    return lua_checkstack(m_ls, n);
}

void LuaStack::xmove(lua_State *to, int n)
{
    lua_xmove(m_ls, to, n);
}

void LuaStack::pop(int n)
{
    lua_pop(m_ls, n);
}

bool LuaStack::isNumber(int idx) const
{
    return lua_isnumber(m_ls, idx) == 1;
}

bool LuaStack::isString(int idx) const
{
    return lua_isstring(m_ls, idx) == 1;
}

bool LuaStack::isCFunction(int idx) const
{
    return lua_iscfunction(m_ls, idx) == 1;
}

bool LuaStack::isInteger(int idx) const
{
    return lua_isinteger(m_ls, idx) == 1;
}

bool LuaStack::isUserdata(int idx) const
{
    return lua_isuserdata(m_ls, idx) == 1;
}

int LuaStack::type(int idx) const
{
    return lua_type(m_ls, idx);
}

std::string LuaStack::typeName(int tp) const
{
    return std::string(lua_typename(m_ls, tp));
}

lua_Number LuaStack::toNumberx(int idx, int *isnum) const
{
    return lua_tonumberx(m_ls, idx, isnum);
}

lua_Number LuaStack::toNumber(int idx) const
{
    return lua_tonumber(m_ls, idx);
}

lua_Integer LuaStack::toIntegerx(int idx, int *isnum) const
{
    return lua_tointegerx(m_ls, idx, isnum);
}

lua_Integer LuaStack::toInteger(int idx) const
{
    return lua_tointeger(m_ls, idx);
}

bool LuaStack::toBoolean(int idx) const
{
    return lua_toboolean(m_ls, idx) == 1;
}

std::string LuaStack::toString(int idx) const
{
    return std::string(lua_tostring(m_ls, idx));
}

size_t LuaStack::rawLen(int idx) const
{
    return lua_rawlen(m_ls, idx);
}

lua_CFunction LuaStack::toCFunction(int idx) const
{
    return lua_tocfunction(m_ls, idx);
}

void *LuaStack::toUserdata(int idx) const
{
    return lua_touserdata(m_ls, idx);
}

lua_State *LuaStack::toThread(int idx) const
{
    return lua_tothread(m_ls, idx);
}

const void *LuaStack::toPointer(int idx) const
{
    return lua_topointer(m_ls, idx);
}

int LuaStack::rawEqual(int idx1, int idx2) const
{
    return lua_rawequal(m_ls, idx1, idx2);
}

int LuaStack::compare(int idx1, int idx2, int op) const
{
    return lua_compare(m_ls, idx1, idx2, op);
}

void LuaStack::arith(int op)
{
    return lua_arith(m_ls, op);
}

void LuaStack::pushNil()
{
    lua_pushnil(m_ls);
}

void LuaStack::pushNumber(lua_Number n)
{
    lua_pushnumber(m_ls, n);
}

void LuaStack::pushInteger(lua_Integer n)
{
    lua_pushinteger(m_ls, n);
}

std::string LuaStack::pushLString(const char *s, size_t len)
{
    return std::string(lua_pushlstring(m_ls, s, len));
}

std::string LuaStack::pushString(const char *s)
{
    return std::string(lua_pushstring(m_ls, s));
}

std::string LuaStack::pushVFString(const char *fmt, va_list argp)
{
    return std::string(lua_pushvfstring(m_ls, fmt, argp));
}

std::string LuaStack::pushFString(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    std::string r = pushVFString(fmt, args);
    va_end(args);
    return r;
}

void LuaStack::pushCClosure(lua_CFunction fn, int n)
{
    lua_pushcclosure(m_ls, fn, n);
}

void LuaStack::pushBoolean(bool b)
{
    lua_pushboolean(m_ls, b ? 1 : 0);
}

void LuaStack::pushLightUserdata(void *p)
{
    lua_pushlightuserdata(m_ls, p);
}

void LuaStack::dumpStack(std::function<void (lua_State *, int)> f)
{
    std::cout << "----- stack dump -----" << std::endl;
    int top = lua_gettop(m_ls);
    for (int i = top; i >= 1; --i) {
        f(m_ls, i);
    }
    std::cout << "--------- end --------" << std::endl;
}

// ---

LuaThread::LuaThread(lua_State *ls)
    : LuaStack(ls)
{

}

LuaThread::~LuaThread()
{

}

int LuaThread::yieldk(int nresults, lua_KContext ctx, lua_KFunction k)
{
    return lua_yieldk(m_ls, nresults, ctx, k);
}

int LuaThread::resume(lua_State *from, int nargs)
{
    return lua_resume(m_ls, from, nargs);
}

int LuaThread::status() const
{
    return lua_status(m_ls);
}

int LuaThread::isYieldable() const
{
    return lua_isyieldable(m_ls);
}

int LuaThread::yield(int nresults)
{
    return lua_yield(m_ls, nresults);
}

int LuaThread::pushThread()
{
    return lua_pushthread(m_ls);
}

template<typename... Args>
int LuaThread::resume(LuaThread *from, const std::string &func, Args... args)
{
    // lua的函数名入栈
    lua_getglobal(from->m_ls, func.c_str());
    // lua函数的参数入栈
    int countOfArgs = from->pushN(std::forward<Args>(args)...);
    // 执行lua函数
    return resume(from->m_ls, countOfArgs);
}

// ---

LuaState::LuaState(lua_State *ls, LUA_STATE_RESOUECE_TYPE resourceType)
    : LuaThread(ls), m_lsResource(resourceType)
{

}

LuaState::~LuaState()
{
    if (m_lsResource == LuaState::LUA_STATE_RESOUECE_TYPE::UNIQUE_LUA_STATE) {
        close();
    }
}

const lua_Number *LuaState::version() const
{
    return lua_version(m_ls);
}

lua_CFunction LuaState::atPanic(lua_CFunction panicf)
{
    return lua_atpanic(m_ls, panicf);
}

lua_State *LuaState::newThread()
{
    return lua_newthread(m_ls);
}

lua_State *LuaState::newState(lua_Alloc f, void *ud)
{
    return lua_newstate(f, ud);
}

void LuaState::close()
{
    lua_close(m_ls);
    m_ls = nullptr;
    m_lsResource = LuaState::LUA_STATE_RESOUECE_TYPE::NONE_STATE;
}

int LuaState::gc(int what, int data)
{
    return lua_gc(m_ls, what, data);
}

int LuaState::getGlobal(const char *name)
{
    return lua_getglobal(m_ls, name);
}

int LuaState::getTable(int idx)
{
    return lua_gettable(m_ls, idx);
}

int LuaState::getField(int idx, const char *k)
{
    return lua_getfield(m_ls, idx, k);
}

void LuaState::createTable(int narr, int nrec)
{
    lua_createtable(m_ls, narr, nrec);
}

void *LuaState::newUserdata(size_t sz)
{
    return lua_newuserdata(m_ls, sz);
}

int LuaState::getMetatable(int index)
{
    return lua_getmetatable(m_ls, index);
}

int LuaState::getUserValue(int idx)
{
    return lua_getuservalue(m_ls, idx);
}

void LuaState::setGlobal(const char *name)
{
    lua_setglobal(m_ls, name);
}

void LuaState::setField(int idx, const char *k)
{
    lua_setfield(m_ls, idx, k);
}

void LuaState::setMetatable(int objindex)
{
    lua_setmetatable(m_ls, objindex);
}

void LuaState::callK(int nargs, int nresults, lua_KContext ctx, lua_KFunction k)
{
    return lua_callk(m_ls, nargs, nresults, ctx, k);
}

void LuaState::call(int nargs, int nresults)
{
    lua_call(m_ls, nargs, nresults);
}

int LuaState::pcallK(int nargs, int nresults, int errfunc, lua_KContext ctx, lua_KFunction k)
{
    return lua_pcallk(m_ls, nargs, nresults, errfunc, ctx, k);
}

int LuaState::pcall(int nargs, int nresults, int errfunc)
{
    return lua_pcall(m_ls, nargs, nresults, errfunc);
}

int LuaState::load(lua_Reader reader, void *dt, const char *chunkname, const char *mode)
{
    return lua_load(m_ls, reader, dt, chunkname, mode);
}

int LuaState::dump(lua_Writer writer, void *data, int strip)
{
    return lua_dump(m_ls, writer, data, strip);
}

int LuaState::error()
{
    return lua_error(m_ls);
}

int LuaState::next(int idx)
{
    return lua_next(m_ls, idx);
}

void LuaState::concat(int n)
{
    return lua_concat(m_ls, n);
}

size_t LuaState::stringToNumber(const char *s)
{
    return lua_stringtonumber(m_ls, s);
}

lua_Alloc LuaState::getAllocF(void **ud)
{
    return lua_getallocf(m_ls, ud);
}

void LuaState::setAllocF(lua_Alloc f, void *ud)
{
    return lua_setallocf(m_ls, f, ud);
}

void *LuaState::getExtraSpace()
{
    return lua_getextraspace(m_ls);
}

LuaLState::LuaLState(lua_State *ls, LuaState::LUA_STATE_RESOUECE_TYPE resourceType)
    : LuaState(ls, resourceType)
{

}

LuaLState::~LuaLState()
{

}

lua_State *LuaLState::newLState()
{
    return luaL_newstate();
}

LuaLBuffer LuaLState::newBuffer()
{
    LuaLBuffer lb;
    luaL_buffinit(m_ls, &lb.m_b);

    return lb;
}

LuaLBuffer LuaLState::newBuffer(size_t sz)
{
    LuaLBuffer lb;
    luaL_buffinitsize(m_ls, &lb.m_b, sz);

    return lb;
}

int LuaLState::getMetatable(const char *tname)
{
    return luaL_getmetatable(m_ls, tname);
}

int LuaLState::getSubtable(int idx, const char *e)
{
    return luaL_getsubtable(m_ls, idx, e);
}

const char *LuaLState::gsub(const char *s, const char *p, const char *r)
{
    return luaL_gsub(m_ls, s, p, r);
}

int LuaLState::len(int idx) const
{
    return luaL_len(m_ls, idx);
}

int LuaLState::loadBuffer(const char *buff, size_t sz, const char *name)
{
    return luaL_loadbuffer(m_ls, buff, sz, name);
}

int LuaLState::loadBufferX(const char *buff, size_t sz, const char *name, const char *mode)
{
    return luaL_loadbufferx(m_ls, buff, sz, name, mode);
}

void *LuaLState::checkUData(int arg, const char *tname)
{
    return luaL_checkudata(m_ls, arg, tname);
}

lua_Integer LuaLState::checkInteger(int arg)
{
    return luaL_checkinteger(m_ls, arg);
}

const char *LuaLState::checkLString(int arg, size_t *l)
{
    return luaL_checklstring(m_ls, arg, l);
}

lua_Number LuaLState::checkNumber(int arg)
{
    return luaL_checknumber(m_ls, arg);
}

//int LuaLState::checkOption(int arg, const char *def, const char * const lst[])
//{
//    return luaL_checkoption(m_ls, arg, def, lst);
//}

void LuaLState::checkStack(int sz, const char *msg)
{
    luaL_checkstack(m_ls, sz, msg);
}

const char *LuaLState::checkString(int arg)
{
    return luaL_checkstring(m_ls, arg);
}

void LuaLState::checkType(int arg, int t)
{
    luaL_checktype(m_ls, arg, t);
}

int LuaLState::callMeta(int obj, const char *e)
{
    return luaL_callmeta(m_ls, obj, e);
}

void LuaLState::checkAny(int arg)
{
    luaL_checkany(m_ls, arg);
}

int LuaLState::argError(int arg, const char *extramsg)
{
    return luaL_argerror(m_ls, arg, extramsg);
}

void LuaLState::argCheck(int cond, int arg, const char *extramsg)
{
    luaL_argcheck(m_ls, cond, arg, extramsg);
}

void LuaLState::openLibs()
{
    luaL_openlibs(m_ls);
}

lua_Integer LuaLState::optInteger(int arg, lua_Integer d)
{
    return luaL_optinteger(m_ls, arg, d);
}

const char *LuaLState::optLString(int arg, const char *d, size_t *l)
{
    return luaL_optlstring(m_ls, arg, d, l);
}

lua_Number LuaLState::optNumber(int arg, lua_Number d)
{
    return luaL_optnumber(m_ls, arg, d);
}

const char *LuaLState::optString(int arg, const char *d)
{
    return luaL_optstring(m_ls, arg, d);
}

int LuaLState::ref(int t)
{
    return luaL_ref(m_ls, t);
}

void LuaLState::requireF(const char *modname, lua_CFunction openf, int glb)
{
    luaL_requiref(m_ls, modname, openf, glb);
}

void LuaLState::checkVersion() const
{
    return luaL_checkversion(m_ls);
}

int LuaLState::loadFile(const char *filename)
{
    return luaL_loadfile(m_ls, filename);
}

int LuaLState::loadFileX(const char *filename, const char *mode)
{
    return luaL_loadfilex(m_ls, filename, mode);
}

int LuaLState::loadString(const char *s)
{
    return luaL_loadstring(m_ls, s);
}

//void LuaLState::newLib(const luaL_Reg l[])
//{
//    luaL_newlib(m_ls, l);
//}

//void LuaLState::newLibTable(const luaL_Reg l[])
//{
//    luaL_newlibtable(m_ls, l);
//}

int LuaLState::doFile(const char *filename)
{
    return luaL_dofile(m_ls, filename);
}

int LuaLState::doString(const char *str)
{
    return luaL_dostring(m_ls, str);
}

int LuaLState::execResult(int stat)
{
    return luaL_execresult(m_ls, stat);
}

int LuaLState::fileResult(int stat, const char *fname)
{
    return luaL_fileresult(m_ls, stat, fname);
}

int LuaLState::getMetaField(int obj, const char *e)
{
    return luaL_getmetafield(m_ls, obj, e);
}

//int LuaLState::error(const char *fmt)
//{
//    return luaL_error(m_ls, fmt, )
//}

int LuaLState::newMetatable(const char *tname)
{
    return luaL_newmetatable(m_ls, tname);
}

void LuaLState::setFuncs(const luaL_Reg *l, int nup)
{
    luaL_setfuncs(m_ls, l, nup);
}

void LuaLState::setMetatable(const char *tname)
{
    luaL_setmetatable(m_ls, tname);
}

void LuaLState::testUData(int arg, const char *tname)
{
    luaL_testudata(m_ls, arg, tname);
}

const char *LuaLState::toLString(int idx, size_t *len)
{
    return luaL_tolstring(m_ls, idx, len);
}

void LuaLState::traceBack(lua_State *l1, const char *msg, int level)
{
    luaL_traceback(m_ls, l1, msg, level);
}

void LuaLState::unRef(int t, int ref)
{
    luaL_unref(m_ls, t, ref);
}

void LuaLState::where(int lvl)
{
    luaL_where(m_ls, lvl);
}

Luapp::Luapp(lua_State *ls, LuaState::LUA_STATE_RESOUECE_TYPE resourceType)
    : LuaLState(ls, resourceType)
{

}

Luapp::~Luapp()
{

}

std::shared_ptr<Luapp> Luapp::create(lua_Alloc f, void *ud)
{
    lua_State * ls = lua_newstate(f, ud);
    if (!ls) {
        return nullptr;
    }

    return std::make_shared<Luapp>(ls, LuaState::LUA_STATE_RESOUECE_TYPE::UNIQUE_LUA_STATE);
}

std::shared_ptr<Luapp> Luapp::create()
{
    lua_State* ls = luaL_newstate();
    if (!ls) {
        return nullptr;
    }

    return std::make_shared<Luapp>(ls, LuaState::LUA_STATE_RESOUECE_TYPE::UNIQUE_LUA_STATE);
}

lua_State *Luapp::getLuaState() const
{
    return m_ls;
}

int Luapp::tableAppend(int idxTo, int idxFrom, bool replaceable)
{
    int iRet = 0;
    idxTo = absIndex(idxTo);
    idxFrom = absIndex(idxFrom);

    pushNil();
    // 现在的栈：-1 => nil; idxTo => tableTo, idxFrom => tableFrom
    while (next(idxFrom))
    {
        // 现在的栈：-1 => value; -2 => key; idxTo => tableTo, idxFrom => tableFrom
        // 拷贝一份 key 到栈顶，然后对它做 lua_tostring 就不会改变原始的 key 值了
        pushValue(-2);
        // 现在的栈：-1 => key; -2 => value; -3 => key; idxTo => tableTo, idxFrom => tableFrom
        std::string key = toString(-1);

        // 判断tableTo中是否已经有了这个key
        if (LUA_TNIL == getField(idxTo, key.c_str()) || replaceable) {
            // key不存在或者要替换，将从tableTo中的取出的值和key出栈
            pop(2);

            // 将value加入tableTo，value会自动出栈
            setField(idxTo, key.c_str());

            ++iRet;
        } else {
            // key存在，并且不要替换，直接pop(3)就可以了
            pop(3);
        }
        // 现在的栈：-1 => key; idxTo => tableTo, idxFrom => tableFrom
    }

    return iRet;
}

int Luapp::tableForeach(int index, std::function<void(lua_State*)> f)
{
    int iRet = 0;
    index = absIndex(index);

    pushNil();
    // 现在的栈：-1 => nil; index => table
    while (next(index))
    {
        // 现在的栈：-1 => value; -2 => key; index => table
        // 拷贝一份 key 到栈顶，然后对它做 lua_tostring 就不会改变原始的 key 值了
        pushValue(-2);
        // 现在的栈：-1 => key; -2 => value; -3 => key; index => table
        f(m_ls);

        // 弹出 value 和拷贝的 key，留下原始的 key 作为下一次 lua_next 的参数
        pop(2);
        ++iRet;
        // 现在的栈：-1 => key; index => table
    }
    // 现在的栈：index => table （最后 lua_next 返回 0 的时候它已经把上一次留下的 key 给弹出了）
    // 所以栈已经恢复到进入这个函数时的状态

    return iRet;
}

void Luapp::tableStdcout(int index)
{
    dumpStack();

    std::cout << typeNameFromIndex(index) << "(" << toPointer(index) << ") = {" << std::endl;
    tableForeach(index, keyvalue_stdcout);
    std::cout << "};" << std::endl;

    dumpStack();
}

int Luapp::requireLibs(const luaL_Reg *libs)
{
    int iRet = 0;
    for(const luaL_Reg *lib = libs; lib->func != NULL; lib++)
    {
        requireF(lib->name, lib->func, 1);
        pop(1);
        ++iRet;
    }

    return iRet;
}

bool Luapp::doFile(const std::string &fname)
{
    bool r = false;

    int status = loadFile(fname.c_str());
    if(status == LUA_OK)
    {
        pcall(0, LUA_MULTRET, 0);
    }
    else
    {
        // TODO:
        std::cerr << " Could not load the script. error no.: " << status << std::endl;
        r = false;
    }

    return r;
}

void LuaLBuffer::addChar(char c)
{
    luaL_addchar(&m_b, c);
}

void LuaLBuffer::addLString(const char *s, size_t l)
{
    luaL_addlstring(&m_b, s, l);
}

void LuaLBuffer::addSize(size_t n)
{
    luaL_addsize(&m_b, n);
}

void LuaLBuffer::addString(const char *s)
{
    luaL_addstring(&m_b, s);
}

void LuaLBuffer::addValue()
{
    luaL_addvalue(&m_b);
}

char *LuaLBuffer::prepBuffer()
{
    return luaL_prepbuffer(&m_b);
}

char *LuaLBuffer::prepBufferSize(size_t sz)
{
    return luaL_prepbuffsize(&m_b, sz);
}

void LuaLBuffer::pushResult()
{
    luaL_pushresult(&m_b);
}

void LuaLBuffer::pushResultSize(size_t sz)
{
    luaL_pushresultsize(&m_b, sz);
}
