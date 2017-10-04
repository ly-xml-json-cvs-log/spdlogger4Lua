#ifndef LUAPP_H
#define LUAPP_H

// ---

#ifndef DECLARE_AND_IMPLEMENT_LUA_OBJECT
#define DECLARE_AND_IMPLEMENT_LUA_OBJECT

#define DECLARE_LUA_OBJECT_BEGIN(o)                                 \
    struct o {

#define DECLARE_LUA_OBJECT_FROM_FATHER_BEGIN(o, f)                 \
    struct o: public f {

#define EXPORT_METHOD_TO_LUA(n)                                 \
    static int n(lua_State *ls);

#define DECLARE_LUA_OBJECT_END(o)                               \
    template<typename T> static int new_##o(lua_State * L) {    \
        return Luapp(L).newObject<T>(mt_##o); }                 \
    template<typename T> static int delete_##o(lua_State *L) {  \
        return Luapp(L).deleteObject<T>(mt_##o); }              \
    static const char * mt_##o;                                 \
    static const char * md_##o;                                 \
    static int open_##o(lua_State * L);                         \
    };

// used in .cpp file.
#define DEFINE_MODULE_NAME(o, n)                    \
    const char* o::md_##o = (n);

#define DEFINE_META_TABLE_NAME(o, n)                \
    const char* o::mt_##o = (n);

#define IMPLEMENT_OPENLIB_METHOD_BEGIN(o)           \
    int o::open_##o(lua_State *ls)                  \
    {

#define META_TABLE_WITH_DELETE_BEGIN(lo, co)        \
        static const struct luaL_Reg lib_m [] = {   \
        {"__gc",        lo::delete_##lo<co>},

#define META_TABLE_BEGIN                            \
        static const struct luaL_Reg lib_m [] = {

#define ITEM_IN_TABLE(luaMethod, cppMethod)         \
    {(luaMethod), (cppMethod) },

#define META_TABLE_END                              \
    {NULL,          NULL}                           \
    };

#define FUNC_TABLE_WITH_NEW_BEGIN(lo, co)           \
        static const struct luaL_Reg lib_f [] = {   \
        {"create",      lo::new_##lo<co>},

#define FUNC_TABLE_BEGIN                            \
        static const struct luaL_Reg lib_f [] = {

#define FUNC_TABLE_END                         \
    {NULL,          NULL}                           \
    };

#define REGISTER_LUA_OBJECT_METHODS(o)                 \
    Luapp l(ls);                                       \
    l.newMetatable(mt_##o);                            \
    l.pushValue(-1);                                   \
    l.setField(-2, "__index");                         \
    l.setFuncs(lib_m, 0);

#define INHERIT_METHOD_FROM_FATHER(f)                       \
    l.getMetatable(f::mt_##f);                              \
    l.tableAppend((-2), (-1), false);                \
    l.pop(1);

#define IMPLEMENT_OPENLIB_METHOD_END                 \
    luaL_newlib(ls, lib_f);                          \
    return 1;                                        \
}

// function输出到lua的宏，在.h文件中使用
#define DECLARE_OPENLIB_FUNCTION(f)                   \
    int open_##f(lua_State *ls);

// function输出到lua的宏，在.cpp文件中使用
#define IMPLEMENT_OPENLIB_FUNCTION_BEGIN(f)           \
    int open_##f(lua_State *ls)                       \
    {

#define LIST_FUNCTIONS_BEGIN               \
    static const struct luaL_Reg lib_f [] = {

#define ITEM_IN_FUNCTIONS(f)                         \
    {#f, lua_##f },

#define LIST_FUNCTIONS_END                           \
    {NULL,          NULL}                            \
    };

#define IMPLEMENT_OPENLIB_FUNCTION_END               \
    luaL_newlib(ls, lib_f);                          \
    return 1;                                        \
}

#define EXPORT_FUNCTION_TO_LUA(f)                    \
    int f(lua_State * ls);

// luapp作为so库要被链接到可执行文件时，要在可执行文件到项目中使用此宏，
// 以便导入so中到lua对象和lua函数
#define DECLARE_EXPORT_LIB_FUNCTION(n)     \
__attribute__ ((visibility("default"))) extern "C" int luaopen_##n(lua_State* L);

// 定义lua在require("name")时的函数，这个函数是动态库唯一的输出函数
// 并且输出的函数名称要遵守extern "C"约定
#define IMPLEMENT_EXPORT_LIB_BEGIN(n)   \
__attribute__ ((visibility("default"))) extern "C" int luaopen_##n(lua_State* L) \
{ static const luaL_Reg lua_modules[] = {

#define EXPORT_MODULE_TO_LUA(n)         \
{ n::md_##n,     n::open_##n },

#define EXPORT_FUNCTIONS_TO_LUA(n)      \
{ #n,          open_##n },

#define IMPLEMENT_EXPORT_LIB_END        \
{NULL, NULL}};                          \
Luapp lp(L);                            \
lp.requireLibs(lua_modules);            \
return 1;                               \
}

#endif

// ---

template<typename T>
void f_push(lua_State * ls, T const& t);

template<typename T >
void f_pop(lua_State * ls, T& t);

// ---

// 逗号表达式展开参数包
// 不通过递归方式来展开参数包，这种方式需要借助逗号表达式和初始化列表。比如前面print的例子可以改成这样
template <typename F, typename... Args>
int expandArgs(lua_State * ls, const F& f, Args&&... args)
{
    std::initializer_list<int> il {(f(ls, std::forward< Args>(args)),0)...};
    return il.size();
}
// 调用方式如下，在printarg函数中，处理每个参数
// 此处不能直接调用printarg，因为模版展开时不能确定具体的类型
// expandArgs([](lua_State *ls, auto t){printarg(std::forward(t));}, 1, 2.2, 3.3, "abcdefg");

// 在popArgs时，不能传入闭包，因为闭包中的auto参数会导致pop不到stack中的值
template <typename... Args>
int popArgs(lua_State * ls, Args&&... args)
{
   int arr[] = {(f_pop(ls, std::forward<Args>(args)), 0)...};
   return sizeof(arr) / sizeof(arr[0]);
}

// 在popArgs时，不能传入闭包，因为闭包中的auto参数会导致pop不到stack中的值
template <typename... Args>
int countOfArgs(Args&&... args)
{
   int arr[] = {((std::forward<Args>(args)), 0)...};
   return sizeof(arr) / sizeof(arr[0]);
}

// ---

// stackindex的默认输入函数
void stackindex_stdcout(lua_State * ls, int idx);

// table的默认输出函数，栈顶就是key和value
void keyvalue_stdcout(lua_State * ls);

// ---

class LuaStack
{
protected:
    lua_State * m_ls;

public:
    LuaStack(lua_State * ls);
    virtual ~LuaStack();

    // 不能拷贝构造，也不能赋值
    LuaStack(const LuaStack& ls) = delete;
    LuaStack(LuaStack&& ls) = delete;
    LuaStack operator=(const LuaStack& ls) = delete;
    LuaStack operator=(LuaStack&& ls) = delete;

    // basic stack manipulation
    int absIndex(int idx) const;
    int getTop() const;
    void setTop(int idx);
    void pushValue(int idx);
    void rotate(int idx, int n);
    void copy(int fromidx, int toidx);
    int checkStack(int n);
    void xmove(lua_State *to, int n);
    void pop(int n);

    // access functions (stack -> C)
    bool isNumber(int idx) const;
    bool isString(int idx) const;
    bool isCFunction(int idx) const;
    bool isInteger(int idx) const;
    bool isUserdata(int idx) const;
    int type(int idx) const;
    std::string typeName(int tp) const;

    lua_Number toNumberx(int idx, int *isnum) const;
    lua_Number toNumber(int idx) const;
    lua_Integer toIntegerx(int idx, int *isnum) const;
    lua_Integer toInteger(int idx) const;
    bool toBoolean(int idx) const;
    std::string toString(int idx) const;
    size_t rawLen(int idx) const;
    lua_CFunction toCFunction(int idx) const;
    void * toUserdata(int idx) const;
    lua_State * toThread(int idx) const;
    const void * toPointer(int idx) const;

    // Comparison and arithmetic functions
    int rawEqual(int idx1, int idx2) const;
    int compare(int idx1, int idx2, int op) const;
    void arith (int op);

    // push functions (C -> stack)
    void pushNil();
    void pushNumber(lua_Number n);
    void pushInteger(lua_Integer n);
    std::string pushLString(const char *s, size_t len);
    std::string pushString(const char *s);
    std::string pushVFString(const char *fmt, va_list argp);
    std::string pushFString(const char *fmt, ...);
    void pushCClosure(lua_CFunction fn, int n);
    void pushBoolean(bool b);
    void pushLightUserdata(void *p);

    // 多个参数入栈
    template <typename... Args> int pushN(Args&&... args)
    {
        int countOfArgs = expandArgs(m_ls,
                [](lua_State *ls, auto t){f_push(ls, t);},
                std::forward<Args>(args)...);

        return countOfArgs;
    }

    // 多个参数出栈
    template <typename... Args> int popN(Args&&... args)
    {
        int countOfArgs = popArgs(m_ls,
                std::forward<Args>(args)...);

        return countOfArgs;
    }

    std::string typeNameFromIndex(int idx);
    std::string valueToString(int idx);

    void dumpStack(std::function<void(lua_State*, int)> f = stackindex_stdcout);
};

// ---

class LuaThread : public LuaStack
{
public:
    LuaThread(lua_State * ls);
    virtual ~LuaThread();

    // coroutine functions
    int yieldk(int nresults, lua_KContext ctx, lua_KFunction k);
    int resume(lua_State *from, int nargs);
    int status() const;
    int isYieldable() const;
    int yield(int nresults);

    int pushThread();
    template<typename... Args> int resume(LuaThread* from, const std::string& func, Args... args);
};

// ---

class LuaState : public LuaThread
{
public:
    enum class LUA_STATE_RESOUECE_TYPE {
        NONE_STATE,
        UNIQUE_LUA_STATE,
        SHARED_LUA_STATE
    };

    LUA_STATE_RESOUECE_TYPE m_lsResource;

public:
    LuaState(lua_State * ls, LUA_STATE_RESOUECE_TYPE resourceType = LUA_STATE_RESOUECE_TYPE::SHARED_LUA_STATE);
    virtual ~LuaState();

    // state manipulation
    static lua_State * newState(lua_Alloc f, void *ud);
    const lua_Number *version() const;
    lua_CFunction atPanic(lua_CFunction panicf);
    lua_State * newThread();
    void close();

    // garbage-collection function and options
    int gc(int what, int data);

    // get functions (Lua -> stack)
    int getGlobal(const char *name);
    int getTable(int idx);
    int getField(int idx, const char *k);
    int getI(int idx, lua_Integer n);
    int rawGet(int idx);
    int rawGetI(int idx, lua_Integer n);
    int rawGetP(int idx, const void *p);

    void createTable(int narr, int nrec);
    void *newUserdata(size_t sz);
    int getMetatable(int index);
    int getUserValue(int idx);

    // set functions (stack -> Lua)
    void setGlobal(const char *name);
    void setTable(int idx);
    void setField(int idx, const char *k);
    void setI(int idx, lua_Integer n);
    void rawSet(int idx);
    void rawSetI(int idx, lua_Integer n);
    void rawSetP(int idx, const char *p);
    void setMetatable(int objindex);
    void setUserValue(int idx);

    // 'load' and 'call' functions (load and run Lua code)
    void callK(int nargs, int nresults, lua_KContext ctx, lua_KFunction k);
    void call(int nargs, int nresults);
    int pcallK(int nargs ,int nresults, int errfunc, lua_KContext ctx, lua_KFunction k);
    int pcall(int nargs, int nresults, int errfunc);
    int load(lua_Reader reader, void *dt, const char *chunkname, const char *mode);
    int dump(lua_Writer writer, void *data, int strip);

    // miscellaneous functions
    int error();
    int next(int idx);
    void concat(int n);
    size_t stringToNumber(const char *s);
    lua_Alloc getAllocF(void **ud);
    void setAllocF(lua_Alloc f, void *ud);
    void *getExtraSpace();
};

// ---

struct LuaLBuffer
{
    luaL_Buffer m_b;

    void addChar(char c);
    void addLString(const char* s, size_t l);
    void addSize(size_t n);
    void addString(const char* s);
    void addValue();

    char* prepBuffer();
    char* prepBufferSize(size_t sz);

    void pushResult();
    void pushResultSize(size_t sz);
};

// ---

class LuaLState: public LuaState
{
public:
    LuaLState(lua_State * ls, LUA_STATE_RESOUECE_TYPE resourceType = LUA_STATE_RESOUECE_TYPE::SHARED_LUA_STATE);
    virtual ~LuaLState();

    // luaL_*
    static lua_State *newLState();
    LuaLBuffer newBuffer();
    LuaLBuffer newBuffer(size_t sz);

    void argCheck(int cond, int arg, const char *extramsg);
    int argError(int arg, const char *extramsg);
    int callMeta(int obj, const char* e);
    void checkAny(int arg);
    lua_Integer checkInteger(int arg);
    const char *checkLString(int arg, size_t *l);
    lua_Number checkNumber(int arg);
//    int checkOption(int arg, const char *def, const char *const lst[]);
    void checkStack(int sz, const char *msg);
    const char *checkString(int arg);
    void checkType(int arg, int t);
    void * checkUData(int arg, const char *tname);
    void checkVersion() const;
    int doFile(const char *filename);
    int doString(const char *str);
    int error(const char *fmt, ...);
    int execResult(int stat);
    int fileResult(int stat, const char *fname);
    int getMetaField(int obj, const char *e);
    int getMetatable(const char* tname);
    int getSubtable(int idx, const char *e);
    const char *gsub(const char *s, const char *p, const char *r);
    int len(int idx) const;
    int loadBuffer(const char *buff, size_t sz, const char *name);
    int loadBufferX(const char *buff, size_t sz, const char *name, const char *mode);
    int loadFile(const char *filename);
    int loadFileX(const char *filename, const char *mode);
    int loadString(const char *s);
//    void newLib(const luaL_Reg l[]);
//    void newLibTable(const luaL_Reg l[]);
    int newMetatable(const char *tname);
    void openLibs();
    lua_Integer optInteger(int arg, lua_Integer d);
    const char *optLString(int arg, const char *d, size_t *l);
    lua_Number optNumber(int arg, lua_Number d);
    const char *optString(int arg, const char *d);
    int ref(int t);
    void requireF(const char *modname, lua_CFunction openf, int glb);
    void setFuncs(const luaL_Reg *l, int nup);
    void setMetatable(const char *tname);
    void testUData(int arg, const char* tname);
    const char *toLString(int idx, size_t *len);
    void traceBack(lua_State* l1, const char *msg, int level);
    void unRef(int t, int ref);
    void where(int lvl);

};

//---

class Luapp: public LuaLState
{
public:
    Luapp(lua_State * ls, LUA_STATE_RESOUECE_TYPE resourceType = LUA_STATE_RESOUECE_TYPE::SHARED_LUA_STATE);
    virtual ~Luapp();

    // 使用自定义的内存分配方法，创建一个Lua状态机
    static std::shared_ptr<Luapp> create(lua_Alloc f, void *ud);
    // 使用Lua本身的内存分配方法，创建一个Lua状态机，这是一般最常用的创建Lua状态机的方法
    static std::shared_ptr<Luapp> create();
    // 获取Luapp中的lua_State*变量
    lua_State* getLuaState() const;

    int tableAppend(int idxTo, int idxFrom, bool replaceable);
    int tableForeach(int index, std::function<void(lua_State*)> f);
    void tableStdcout(int index);

    int requireLibs(const luaL_Reg *libs);
    bool doFile(const std::string& fname);

    template <typename T>
    void setGlobalValue(const char * name, T value)
    {
        pushN(std::forward<T>(value));
        setGlobal(name);
    }

    template <typename T>
    bool getGlobalValue(const char * name, T& value)
    {
        bool bRet = false;
        if (getGlobal(name) != LUA_TNIL) {
            popN(std::forward<T>(value));
            bRet = true;
        }
        return bRet;
    }

    template<typename T> int newObject(const char * mtname)
    {
        T **s = (T**)newUserdata(sizeof(T*));  // lua will manage Student** pointer
        *s = new T;
        setMetatable(mtname);

        // 返回给Lua的只有T*一个变量
        return 1;
    }

    template<typename T> int deleteObject(const char * mtname)
    {
        T **s = (T**)checkUData(1, mtname);
        if( *s ) { delete *s; }

        // 不需要返回lua任何变量
        return 0;
    }

    // 把从lua来的调用，转发到相应到c++对象的方法，并返回执行结果
    // 注意：输入参数和返回值到处理，是调用者到责任，调用者对此也很清楚
    template<typename T, typename CM, typename... Args>
    auto dispatchToObjectMethod(CM f, Args... args)
    {
        // 为了能够被子类调用，此处不能使用checkUData
        // such as: T **s = (T**)checkUData(1, tname);
        T **s = (T**)toUserdata(1);
        argCheck(s != nullptr, 1, "invalid user data");

        auto fn = std::bind(f, *s, std::forward<Args>(args)...);
        return fn();
    }

    // 把从lua来的调用，转发到相应到c++函数，并返回执行结果
    // 注意：输入参数和返回值的处理，是调用者到责任，调用者对此也很清楚
    template<typename F, typename... Args>
    auto dispatchToFunction(F f, Args... args)
    {
        auto fn = std::bind(f, std::forward<Args>(args)...);
        return fn();
    }

    // 在c++中调用Lua中的方法，Lua的方法名是第一个参数，随后都是Lua方法的参数
    // 注意：输入参数和返回值的处理，是调用者到责任，调用者对此也很清楚
    template <typename... Args>
    int dispatchToLua(const std::string& name, int countOfResult, Args... args)
    {
        // lua的函数名入栈
        getGlobal(name.c_str());
        // lua函数的参数入栈
        int countOfArgs = pushN(std::forward<Args>(args)...);
        // 执行lua函数
        return pcall(countOfArgs, countOfResult, 0);
    }

    // 在c++中调用Lua中的方法，Lua的方法名是第一个参数，随后都是Lua方法的参数
    // 注意：输入参数和返回值的处理，是调用者到责任，调用者对此也很清楚
    template <typename... Args>
    int dispatchToLua2(const std::string& name, Args... args)
    {
        // lua的函数名入栈
        getGlobal(name.c_str());
        // lua函数的参数入栈
        int countOfArgs = pushN(std::forward<Args>(args)...);
        // 执行lua函数
        return pcall(countOfArgs, LUA_MULTRET, 0);
    }
};

#endif // LUAPP_H
