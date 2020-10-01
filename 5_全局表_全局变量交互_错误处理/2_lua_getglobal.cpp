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

	luaL_dofile(lua, "global.lua");
	printStack("run lua");

	int type = lua_getglobal(lua, "var2");
	cout << "type: " << type << endl;
	printStack("lua_getglobal var2");

	type = lua_getglobal(lua, "print");
	cout << "type: " << type << endl;
	printStack("lua_getglobal print");

	type = lua_getglobal(lua, "_G");
	cout << "type: " << type << endl;
	printStack("lua_getglobal _G");

	type = lua_getglobal(lua, "aaaaaaa");  // 不存在的
	cout << "type: " << type << endl;  // nil
	printStack("lua_getglobal aaaaaaa");
	return 0;
}
