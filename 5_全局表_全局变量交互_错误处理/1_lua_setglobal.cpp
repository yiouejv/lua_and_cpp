#include <lua.hpp>
#include <iostream>
using namespace std;

lua_State* lua = nullptr;

void initLua()
{
    lua = luaL_newstate();
    if (lua == nullptr)
    {
        printf("lua 初始化失败!");
        return;
    }
    luaL_openlibs(lua);
}

void printStack(const char* tag)
{
    if (lua == nullptr)
    {
        printf("lua_State * is nullptr <%s>\n", tag);
        return;
    }

    int sz = lua_gettop(lua);
    printf("lua Stack Size:%d; <%s>\n", sz, tag);
    for (int i = 1; i <= sz; ++i)
    {
        const char *str = lua_tostring(lua, i);
        printf("index: %d, value: %s <%s>\n", i, str, tag);
    }
    printf("\n");
}

int main()
{
    initLua();
    printStack("initLua");

    // 栈为空时, 设置一个全局变量导致错误
    lua_setglobal(lua, "var1");
    printStack("setglobal error");
    lua_settop(lua, 0);  // 恢复栈状态
    printStack("settop 0");

    lua_pushstring(lua, "hello");
    printStack("push hello");
    lua_setglobal(lua, "var2");  // 从堆栈上弹出一个值，并将其设为全局变量 var2 的新值。
    printStack("lua_setglobal");

    int ret = luaL_dofile(lua, "./set_global.lua");
    if (ret != 0)
    {
        const char* errorStr = lua_tostring(lua, -1);
        cout << "errorStr " << errorStr << endl;
        lua_pop(lua, 1);
    }

    return 0;
}
