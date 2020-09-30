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

    // 正常状态不会改变栈
	int ret = luaL_dostring(lua, "print('hello')");
	cout << "ret:" << ret << endl;
	printStack("no error");

	// 异常情况
	ret = luaL_dostring(lua, "prin('hello')");
	cout << "ret:" << ret << endl;
	printStack("error");
	if (ret != 0)
	{
		const char * errorStr = lua_tostring(lua, -1);  // 获取栈顶的错误信息，最后一个压入栈的负索引为-1
		lua_pop(lua, 1);  // 把错误信息从栈顶弹出，维持栈状态
		cout << "errorStr: "  << errorStr << endl;
		printStack("pop error end");
	}
}
