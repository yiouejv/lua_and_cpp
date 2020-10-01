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

	int ret = luaL_loadstring(lua, "print('hello')");
	cout << "ret:" << ret << endl;
	printStack("luaL_loadstring");
	
	ret = lua_pcall(lua, 0, LUA_MULTRET, 0);
	cout << "ret:" << ret << endl;
	printStack("lua_pcall");
	if (ret != 0)  // 异常情况
	{
		const char * errorStr = lua_tostring(lua, -1);  // 获取栈顶的错误信息，最后一个压入栈的负索引为-1
		cout << "errorStr: "  << errorStr << endl;
		lua_pop(lua, 1);  // 把错误信息从栈顶弹出，维持栈状态
		printStack("pop error end");
		return 0;
	}

	ret = luaL_loadstring(lua, "return 1,2,3");
	cout << "ret: " << ret << endl;
	printStack("return load");
	ret = lua_pcall(lua, 0, LUA_MULTRET, 0);
	printStack("lua_pcall return");
	if (ret != 0)
	{
		const char* errorStr = lua_tostring(lua, -1);
		cout << "errorStr: " << errorStr << endl;
		lua_pop(lua, 1);
		printStack("pop");
	}
	else
	{
		// 函数正常返回1,2,3
		// 后续可以使用返回值做后面的工作。。。
	}

	return 0;
}
