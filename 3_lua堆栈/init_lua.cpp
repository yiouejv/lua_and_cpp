#include <lua.hpp>
#include <iostream>

lua_State* L = nullptr;

void initLua()
{
	L = luaL_newstate();
	if (L == nullptr)
	{
		printf("lua 初始化失败!");
		return;
	}
	luaL_openlibs(L);
}

void printStack(const char* tag)
{
	if (L == nullptr)
	{
		printf("lua_State * is nullptr <%s>\n", tag);
		return;
	}

	int sz = lua_gettop(L);
	printf("lua Stack Size:%d; <%s>\n", sz, tag);
	for (int i = 1; i <= sz; ++i)
	{
		const char *str = lua_tostring(L, i);
		printf("index: %d, value: %s <%s>\n", i, str, tag);
	}
	printf("\n");
}

int main()
{
	initLua();
	printStack("initLua");
	lua_pushstring(L, "a");
	printStack("push a");

	lua_pushstring(L, "b");
	printStack("push b");

	lua_pushstring(L, "c");
	printStack("push c");

	lua_pushstring(L, nullptr);
	printStack("push nullptr");

	lua_settop(L, 1);
	printStack("settop 1");
	return 0;
}