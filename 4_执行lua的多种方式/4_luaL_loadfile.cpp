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

	int ret = luaL_loadfile(lua, "./test1.lua");
	cout << "ret: " << ret << endl;
	printStack("luaL_loadfile");

	ret = lua_pcall(lua, 0, LUA_MULTRET, 0);
	cout << "ret: " << ret << endl;
	printStack("lua_pcall");
	if (ret != 0)
	{
		const char* errorStr = lua_tostring(lua, -1);
		cout << "errorStr: " << errorStr << endl;
		lua_pop(lua, 1);
		printStack("error");
	}
	return 0;
}
