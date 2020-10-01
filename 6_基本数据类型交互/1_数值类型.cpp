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

	lua_pushstring(lua, "123");
	cout << "123 lua_tointegerx: " << lua_tointegerx(lua, -1, NULL) << endl;
	cout << "123 lua_tointeger: " << lua_tointeger(lua, -1) << endl;
	cout << "123 lua_tonumberx: " << lua_tonumberx(lua, -1, NULL) << endl;
	cout << "123 lua_tonumber: " << lua_tonumber(lua, -1) << endl;

	lua_pushstring(lua, "123.0");
	cout << "123.0 lua_tointeger: " << lua_tointeger(lua, -1) << endl;
	cout << "123.0 lua_tonumber: " << lua_tonumber(lua, -1) << endl;

	lua_pushstring(lua, "123.1");
	cout << "123.1 lua_tointeger: " << lua_tointeger(lua, -1) << endl;
	cout << "123.1 lua_tonumber: " << lua_tonumber(lua, -1) << endl;
	return 0;
}
