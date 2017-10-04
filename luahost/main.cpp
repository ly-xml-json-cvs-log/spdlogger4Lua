#include "luahost_global.h"

void lua_test1()
{
    auto ls = Luapp::create();
    ls->openLibs();

    // 执行auto1.lua中定义的函数f
    ls->doFile("auto1.lua");
    ls->dispatchToLua2("f", 1, 2.2, "3.3");

    // 取回lua中函数f的执行结果
    std::string resutl;
    ls->popN(resutl);

    std::cout << "this is c function: " << resutl << std::endl;
}

int main(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    lua_test1();
    return 0;
}
