## 使用Luapp 开发Lua 的扩展模块

在这个例子中，使用Luapp 将c++类包装后，输出到Lua 中，可以在Lua 中操作这些c++类。

### c++中的类

在c++中定义了如下的几个类，注意他们的继承关系，Luapp 可以支持单继承类输出到Lua 。
```cpp
class Auto
{
    std::string m_brand;
    int m_mileage;
    double m_hours;
    int m_fuel;

public:
    Auto();
    virtual ~Auto();

    virtual void drive(const std::string& driver, int mileage, double hours);
    void fuel(int quantity);
    virtual int maintain(const std::string& something);
    std::tuple<std::string, int, float, int> check() const;
};

class Car: public Auto
{
public:
    Car();
    virtual ~Car();

    double music(const std::string& song);
    bool navi(const std::string& address);
};

class Tesla : public Car
{
public:
    Tesla();
    virtual ~Tesla();

    int charge(double d);
    void bluetooth(const std::string& name);
};
```

### 在c++的头文件中的lua包装类

定义如下：
```cpp
DECLARE_LUA_OBJECT_BEGIN(luaAuto)
    EXPORT_METHOD_TO_LUA(l_drive)
    EXPORT_METHOD_TO_LUA(l_fuel)
    EXPORT_METHOD_TO_LUA(l_maintain)
    EXPORT_METHOD_TO_LUA(l_check)
DECLARE_LUA_OBJECT_END(luaAuto)

DECLARE_LUA_OBJECT_FROM_FATHER_BEGIN(luaCar, luaAuto)
    EXPORT_METHOD_TO_LUA(l_music)
    EXPORT_METHOD_TO_LUA(l_navi)
DECLARE_LUA_OBJECT_END(luaCar)

DECLARE_LUA_OBJECT_FROM_FATHER_BEGIN(luaTesla, luaCar)
    EXPORT_METHOD_TO_LUA(l_charge)
    EXPORT_METHOD_TO_LUA(l_bluetooth)
DECLARE_LUA_OBJECT_END(luaTesla)
```

### 在c++的.cpp 文件中，实现lua类对象的方法
```cpp
DEFINE_MODULE_NAME(luaAuto, "Auto")
DEFINE_META_TABLE_NAME(luaAuto, "E141BB75-DE03-4004-A700-936B68242766")

IMPLEMENT_OPENLIB_METHOD_BEGIN(luaAuto)
    META_TABLE_WITH_DELETE_BEGIN(luaAuto, Auto)
        ITEM_IN_TABLE("drive",         luaAuto::l_drive)
        ITEM_IN_TABLE("fuel",          luaAuto::l_fuel)
        ITEM_IN_TABLE("maintain",      luaAuto::l_maintain)
        ITEM_IN_TABLE("check",         luaAuto::l_check)
    META_TABLE_END

    FUNC_TABLE_WITH_NEW_BEGIN(luaAuto, Auto)
    FUNC_TABLE_END

    REGISTER_LUA_OBJECT_METHODS(luaAuto)

IMPLEMENT_OPENLIB_METHOD_END

int luaAuto::l_drive(lua_State * ls)
{
    Luapp l(ls);
    l.dispatchToObjectMethod<Auto>(&Auto::drive, l.toString(2), l.toInteger(3), l.toNumber(4));
    l.pop(4);

    return 0;
}

int luaAuto::l_fuel(lua_State * ls)
{
    Luapp l(ls);
    l.dispatchToObjectMethod<Auto>(&Auto::fuel, l.toInteger(2));
    l.pop(1);

    return 0;
}

int luaAuto::l_maintain(lua_State * ls)
{
    Luapp l(ls);
    auto p = l.dispatchToObjectMethod<Auto>(&Auto::maintain, l.toString(2));

    // 将传入的参数出栈
    l.pop(1);

    // 返回到值如栈
    l.pushInteger(p);

    return 1;
}

int luaAuto::l_check(lua_State * ls)
{
    Luapp l(ls);
    auto p = l.dispatchToObjectMethod<Auto>(&Auto::check);

    l.pushString(std::get<0>(p).c_str());
    l.pushInteger(std::get<1>(p));
    l.pushNumber(std::get<2>(p));
    l.pushInteger(std::get<3>(p));

    return 4;
}

// ---

DEFINE_MODULE_NAME(luaCar, "Car")
DEFINE_META_TABLE_NAME(luaCar, "77E5E7E8-367F-4A6A-ADE7-3C17A26A0C8D")

IMPLEMENT_OPENLIB_METHOD_BEGIN(luaCar)
    META_TABLE_WITH_DELETE_BEGIN(luaCar, Car)
        ITEM_IN_TABLE("music",         luaCar::l_music)
        ITEM_IN_TABLE("navi",          luaCar::l_navi)
    META_TABLE_END

    FUNC_TABLE_WITH_NEW_BEGIN(luaCar, Car)
    FUNC_TABLE_END

    REGISTER_LUA_OBJECT_METHODS(luaCar)
    INHERIT_METHOD_FROM_FATHER(luaAuto)

IMPLEMENT_OPENLIB_METHOD_END

int luaCar::l_music(lua_State * ls)
{
    Luapp l(ls);
    double r = l.dispatchToObjectMethod<Car>(&Car::music, l.toString(2));
    l.pop(1);
    l.pushNumber(r);

    return 1;
}

int luaCar::l_navi(lua_State * ls)
{
    Luapp l(ls);
    bool r = l.dispatchToObjectMethod<Car>(&Car::navi, l.toString(2));
    l.pop(1);
    l.pushBoolean(r);

    return 1;
}

// ---

DEFINE_MODULE_NAME(luaTesla, "Tesla")
DEFINE_META_TABLE_NAME(luaTesla, "F66A760E-3F7F-4F8B-8824-E16C07E02E7C")

IMPLEMENT_OPENLIB_METHOD_BEGIN(luaTesla)
    META_TABLE_WITH_DELETE_BEGIN(luaTesla, Tesla)
        ITEM_IN_TABLE("charge",         luaTesla::l_charge)
        ITEM_IN_TABLE("bluetooth",      luaTesla::l_bluetooth)
    META_TABLE_END

    FUNC_TABLE_WITH_NEW_BEGIN(luaTesla, Tesla)
    FUNC_TABLE_END

    REGISTER_LUA_OBJECT_METHODS(luaTesla)
    INHERIT_METHOD_FROM_FATHER(luaCar)

IMPLEMENT_OPENLIB_METHOD_END

int luaTesla::l_charge(lua_State * ls)
{
    Luapp l(ls);
    int r = l.dispatchToObjectMethod<Tesla>(&Tesla::charge, l.toNumber(2));
    l.pop(1);
    l.pushInteger(r);

    return 1;
}

int luaTesla::l_bluetooth(lua_State * ls)
{
    Luapp l(ls);
    l.dispatchToObjectMethod<Tesla>(&Tesla::bluetooth, l.toString(2));
    l.pop(1);

    return 0;
}
```

## 用于测试的lua代码

```lua
require ("libluaauto")

t=Tesla.create()

t:music("don't break my heart.")

t:navi("beijing")

t:fuel(3332)

t:drive("moto", 321, 2.4)

t:charge(3.3, 2)

t:bluetooth("apple")

t=nil

collectgarbage()
```




