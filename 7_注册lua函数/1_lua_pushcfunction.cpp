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

int test1(lua_State* L)
{
	cout << "print test1" << endl;
	return 0;
}

int test2(lua_State* L)
{
	cout << "print test2" << endl;
	lua_pushstring(L, "return 1");
	lua_pushstring(L, "return 2");
	return 2;
}

int main()
{
    initLua();
    printStack("initLua");

 	// test1
    lua_pushcfunction(lua, &test1);
    printStack("lua_pushcfunction test1");
    lua_setglobal(lua, "test1");
    printStack("lua_setglobal");
    luaL_dofile(lua, "script.lua");  // 内容为 test1();
    printStack("luaL_dofile script.lua");

    cout << "----------------------------" << endl;

	// test2
    lua_pushcfunction(lua, &test2);
    printStack("lua_pushcfunction test2");
    lua_setglobal(lua, "test2");
    printStack("lua_setglobal");
    luaL_dofile(lua, "script2.lua");
    if (ret != 0)
    {
        const char * errorStr = lua_tostring(lua, -1);
        cout << "errorStr: " << errorStr << endl;
        lua_pop(lua, 1);
    }
    printStack("luaL_dofile script2.lua");  // 内容为 print("script2.lua print test2 return: ", test2())
    return 0;
}
