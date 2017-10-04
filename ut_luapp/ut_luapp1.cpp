#include "ut_luapp_global.h"
#include "../luapp/luapp.h"
#include "doctest/doctest.h"

int cfunction(lua_State* ls) {
    UNUSED(ls);
    return 0;
}

TEST_SUITE("luapp.cpp test suite.");

TEST_CASE("stack test case") {

    auto l = Luapp::create();
    l->pushNil();
    l->pushBoolean(false);
    l->pushBoolean(true);
    l->pushInteger(222);
    l->pushNumber(3.3333);
    l->pushString("sss222");
    l->pushFString("format %s, %d, %f", "abc", 8, 9.9);
    l->pushLString("string of len", 3);
    l->pushCClosure(cfunction, 0);
    l->pushLightUserdata((void*)111);
    l->pushThread();
    int iCount = l->getTop();

    REQUIRE(l->getTop() == iCount);

    SUBCASE("ut1") {
        stackindex_stdcout(l->getLuaState(), 1);
        stackindex_stdcout(l->getLuaState(), 2);
        stackindex_stdcout(l->getLuaState(), 3);
        stackindex_stdcout(l->getLuaState(), 4);
        stackindex_stdcout(l->getLuaState(), 5);
        stackindex_stdcout(l->getLuaState(), 6);
        stackindex_stdcout(l->getLuaState(), 7);
        stackindex_stdcout(l->getLuaState(), 8);
        stackindex_stdcout(l->getLuaState(), 9);
        stackindex_stdcout(l->getLuaState(), 10);
        stackindex_stdcout(l->getLuaState(), 11);

        CHECK(l->getTop() == iCount);
    }

    SUBCASE("ut2") {
        CHECK(l->getTop() == iCount);
        l->dumpStack();
    }

    SUBCASE("ut3") {
        bool b1, b2;
        lua_Integer i;
        lua_Number n;
        std::string s1, s2, s3;
        lua_CFunction cfun;
        void* user;
        lua_State* t1;

        l->popN(t1, user, cfun, s3, s2, s1, n, i, b2, b1);
        l->pop(1);      // pop out Nil

        std::cout << std::boolalpha;
        std::cout << b1 << b2 << i << n << s1 << s2 << s3 << cfun << user << t1 << std::endl;

        CHECK(l->getTop() == 0);
        l->dumpStack();

        l->pushNil();
        l->pushN(b1, b2, i, n, s1, s2, s3, cfun, user, t1);

        CHECK(l->getTop() == iCount);
        l->dumpStack();
    }
}


TEST_SUITE_END();
