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

 	struct luaL_Reg funcs[] = {
 		{"test1", test1},
 		{"test2", test2},
 		{NULL, NULL}
 	};

 	lua_newtable(lua);
 	printStack("lua_newtable");

 	for (int i = 0; ; ++i)
 	{
 		luaL_Reg v = funcs[i];
 		if (v.name == NULL || v.func == NULL)
 			break;

 		lua_pushstring(lua, v.name);
 		lua_pushcfunction(lua, v.func);
 		lua_rawset(lua, 1);
 	}
 	lua_setglobal(lua, "lib");
 	int ret = luaL_dofile(lua, "script.lua");  // lib.test1() lib.test2()
 	if (ret != 0)
 	{
 		const char * errorStr = lua_tostring(lua, -1);
 		cout << "errorStr: " << errorStr << endl;
 		lua_pop(lua, 1);
 	}
    return 0;
}
