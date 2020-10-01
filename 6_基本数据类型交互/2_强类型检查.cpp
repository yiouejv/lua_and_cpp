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

	int ret = luaL_dofile(lua, "script.lua");
	cout << ret << endl;
	if (ret != 0)
	{
		const char * errorStr = lua_tostring(lua, -1);
		cout << "errorStr: " << errorStr << endl;
		lua_pop(lua, 1);
		return 0;
	}
	printStack("luaL_dofile");

	lua_getglobal(lua, "i");
	lua_getglobal(lua, "f");
	lua_getglobal(lua, "s");
	lua_getglobal(lua, "func");
	lua_getglobal(lua, "b");
	lua_getglobal(lua, "n");
	lua_getglobal(lua, "t");
	lua_getglobal(lua, "co");

	printStack("lua_getglobal");


	int _type;
	const char* _typename = nullptr;

	int eCount = 8;  // script.lua 中自定义的全局变量的个数
	for (int i = 1; i <= eCount; ++i)
	{
		_type = lua_type(lua, i);
		_typename = lua_typename(lua, _type);
		cout << "index: " << i << " ";
		if (_typename == "function" ||
			_typename == "table" ||
			_typename == "nil"
			)
		{
			const void * pointer = lua_topointer(lua, i);
			cout << "pointer: " << pointer << " ";
		}
		else if (_typename == "boolean")
		{
			bool b = lua_toboolean(lua, i);
			cout << "boolean: " << b << " ";
		}
		else if (_typename == "thread")
		{
			lua_State* co = lua_tothread(lua, i);
			cout << "coroutine: " << co << " ";
		}
		else
		{
			const char* str = lua_tostring(lua, i);
			cout << "str: " << str << " ";
		}
		cout << "type: " << _type << " ";
		cout << "typename: " << _typename << endl;
	}
	return 0;
}
