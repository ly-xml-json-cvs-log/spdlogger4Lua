# Luapp

Luapp是对Lua c API的c++封装，只有一个.h文件和一个.cpp文件。
> luapp.h
> luapp.cpp

## 说明

Luapp 中一共有5个类，分别为
```cpp
LuaStack		// 代表stack ，都是stack相关的操作函数
LuaThread		// 代表协程，就是Lua 中的线程，相关的函数都在这个类中
LuaState		// 代表State ，与状态机相关的操作函数
LuaLState		// 代表扩展的State ，提供了很多辅助类的方便的操作函数，与Lua C API中luaL_开头的函数对应
Luapp			// 扩展的类
```

## LuaStack

LuaStack是对stack的封装，其中protected的成员变量：
```cpp
protected:
    lua_State * m_ls;
```
需要关注一下，所有的操作函数都是针对这个成员变量的操作。

除了Lua 中原本的对stack的操作函数外，主要增加了两个函数：
```cpp
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
```

还有一个检查stack内容的函数，这个函数的缺省参数stackindex\_stdcout将stack的内容输出到std::cout 。
```cpp
void dumpStack(std::function<void(lua_State*, int)> f = stackindex_stdcout);
```

## LuaThread

继承自LuaStack ，这是Lua 协程相关的函数，清参考Lua C API相关的说明，就不需要额外的说明了。

## LuaState

继承自LuaThread ，Lua 状态机相关的操作，都在这个类中，具体可以参考Lua C API的相关说明。

额外需要说明的是构造函数：
```cpp
LuaState(lua_State * ls, LUA_STATE_RESOUECE_TYPE resourceType = LUA_STATE_RESOUECE_TYPE::SHARED_LUA_STATE);
```

参数resourceType代表是LuaState类中对lua\_State\* m\_ls成员变量的所有权，如果这个参数是LUA\_STATE\_RESOUECE\_TYPE::UNIQUE\_LUA\_STATE，那么就说明是完全占有，LuaState在对象析构时，会关闭m\_ls 。如果这个参数是LUA\_STATE\_RESOURCE\_TYPE::SHARED\_LUA\_STATE ，说明对m\_ls是分享式的，LuaState对象在析构时，不对m\_ls执行关闭操作。LuaState构造函数中的默认参数就是分享式的。

## LuaLState

继承自LuaState ，这是对luaL\_相关Lua C API的封装，具体清参考Lua C API的说明。需要特别关注的是构造函数，继承了LuaState类的构造特性。

## Luapp

继承自LuaLState ，构造函数也继承了LuaState的构造特性。

在需要自己创建LuaState状态机时，可以使用如下相关的方法：
```cpp
// 使用自定义的内存分配方法，创建一个Lua状态机
static std::shared_ptr<Luapp> create(lua_Alloc f, void *ud);
// 使用Lua本身的内存分配方法，创建一个Lua状态机，这是一般最常用的创建Lua状态机的方法
static std::shared_ptr<Luapp> create();
// 获取Luapp中的lua_State*变量
lua_State* getLuaState() const;
```

设置和获取Lua 中的全局变量，使用如下的方法：
```cpp
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
```

另外几个重要的方法，说明如下：
```cpp
// 把从lua来的调用，转发到相应到c++对象的方法，并返回执行结果
// 注意：输入参数和返回值的处理，是调用者到责任，调用者对此也很清楚
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
int dispatchToLua(const std::string& name, Args... args)
{
    // lua的函数名入栈
    getGlobal(name.c_str());
    // lua函数的参数入栈
    int countOfArgs = pushN(std::forward<Args>(args)...);
    // 执行lua函数
    return pcall(countOfArgs, LUA_MULTRET, 0);
}
```

## 使用方法

请参考后续的具体例子。

