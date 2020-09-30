

### lua 函数的分类

- 可以分为改变栈，不改变栈: 是否改变虚拟栈
- 带L的函数，不带L的函数: 例如，luaL_newstate, 一般由宏定义出来

### 执行lua的多种方式

#### 方式一: luaL_dostring

参考代码: <a href="1_luaL_dostring.cpp" target="_blank">1_luaL_dostring.cpp</a>

#### 方式二: luaL_loadstring

参考代码: <a href="2_luaL_loadstring.cpp" target="_blank">2_luaL_loadstring.cpp</a>


**luaL\_dostring 和 luaL\_loadstring 的区别**

```c
#define luaL_dostring(L, s) \
	(luaL_loadstring(L, s) || lua_pcall(L, 0, LUA_MULTRET, 0))
```

```
luaL_dostring 为宏 (luaL_loadstring || lua_pcall), 当luaL_loadstring 加载没有错误时，尝试 lua_pcall 执行
luaL_loadstring 加载一个代码块, 加载成功则把编译好的代码块作为一个lua函数压到栈顶, 否则压入错误消息
```

#### 方式三: luaL_dofile

参考代码: <a href="3_luaL_dofile.cpp" target="_blank">3_luaL_dofile.cpp</a>

#### 方式四: luaL_loadfile

参考代码: <a href="4_luaL_loadfile.cpp" target="_blank">4_luaL_loadfile.cpp</a>


### 本节用到的函数

#### lua_Reader
```c
typedef const char * (*lua_Reader) (lua_State *L,
                                    void *data,
                                    size_t *size);
```
lua_load 用到的读取器函数， 每次它需要一块新的代码块的时候， lua_load 就调用读取器， 每次都会传入一个参数 data 。 读取器需要返回含有新的代码块的一块内存的指针， 并把 size 设为这块内存的大小。 内存块必须在下一次函数被调用之前一直存在。 读取器可以通过返回 NULL 或设 size 为 0 来指示代码块结束。 读取器可能返回多个块，每个块可以有任意的大于零的尺寸。

#### lua_load
```c
int lua_load (lua_State *L,
              lua_Reader reader,
              void *data,
              const char *chunkname,
              const char *mode);
```
加载一段 Lua 代码块，但不运行它。 如果没有错误， lua_load 把一个编译好的代码块作为一个 Lua 函数压到栈顶。 否则，压入错误消息。

lua_load 的返回值可以是：

- LUA_OK: 没有错误；
- LUA_ERRSYNTAX: 在预编译时碰到语法错误；
- LUA_ERRMEM: 内存分配错误；
- LUA_ERRGCMM: 在运行 \_\_gc 元方法时出错了。 （这个错误和代码块加载过程无关，它是由垃圾收集器引发的。）
- lua_load 函数使用一个用户提供的 reader 函数来读取代码块。 data 参数会被传入 reader 函数。

chunkname 这个参数可以赋予代码块一个名字， 这个名字被用于出错信息和调试信息。

lua_load 会自动检测代码块是文本的还是二进制的， 然后做对应的加载操作。 字符串 mode 的作用和函数 load 一致。 它还可以是 NULL 等价于字符串 "bt"。

lua_load 的内部会使用栈， 因此 reader 函数必须永远在每次返回时保留栈的原样。

如果返回的函数有上值， 第一个上值会被设置为 保存在注册表 LUA_RIDX_GLOBALS 索引处的全局环境。 在加载主代码块时，这个上值是 \_ENV 变量。 其它上值均被初始化为 nil。

#### luaL_loadstring

`int luaL_loadstring (lua_State *L, const char *s);`

将一个字符串加载为 Lua 代码块。 这个函数使用 lua_load 加载一个零结尾的字符串 s。

此函数的返回值和 lua_load 相同。

也和 lua_load 一样，这个函数仅加载代码块不运行。

#### luaL_dostring

`int luaL_dostring (lua_State *L, const char *str);`

加载并运行指定的字符串。 它是用下列宏定义出来：

`(luaL_loadstring(L, str) || lua_pcall(L, 0, LUA_MULTRET, 0))`

如果没有错误，函数返回 假； 有错则返回 真。

#### lua_call

`void lua_call (lua_State *L, int nargs, int nresults);`
调用一个函数。

要调用一个函数请遵循以下协议： 首先，要调用的函数应该被压入栈； 接着，把需要传递给这个函数的参数按正序压栈； 这是指第一个参数首先压栈。 最后调用一下 lua_call； nargs 是你压入栈的参数个数。 当函数调用完毕后，所有的参数以及函数本身都会出栈。 而函数的返回值这时则被压栈。 返回值的个数将被调整为 nresults 个， 除非 nresults 被设置成 LUA_MULTRET。 在这种情况下，所有的返回值都被压入堆栈中。 Lua 会保证返回值都放入栈空间中。 函数返回值将按正序压栈（第一个返回值首先压栈）， 因此在调用结束后，最后一个返回值将被放在栈顶。

被调用函数内发生的错误将（通过 longjmp ）一直上抛。

#### lua_pcall

`int lua_pcall (lua_State *L, int nargs, int nresults, int msgh);`

以保护模式调用一个函数。

nargs 和 nresults 的含义与 lua_call 中的相同。 如果在调用过程中没有发生错误， lua_pcall 的行为和 lua_call 完全一致。 但是，如果有错误发生的话， lua_pcall 会捕获它， 然后把唯一的值（错误消息）压栈，然后返回错误码。 同 lua_call 一样， lua_pcall 总是把函数本身和它的参数从栈上移除。

如果 msgh 是 0 ， 返回在栈顶的错误消息就和原始错误消息完全一致。 否则， msgh 就被当成是 错误处理函数 在栈上的索引位置。 （在当前的实现里，这个索引不能是伪索引。） 在发生运行时错误时， 这个函数会被调用而参数就是错误消息。 错误处理函数的返回值将被 lua_pcall 作为错误消息返回在堆栈上。

典型的用法中，错误处理函数被用来给错误消息加上更多的调试信息， 比如栈跟踪信息。 这些信息在 lua_pcall 返回后， 由于栈已经展开，所以收集不到了。

lua_pcall 函数会返回下列常数 （定义在 lua.h 内）中的一个：

LUA_OK (0): 成功。
LUA_ERRRUN (2): 运行时错误。
LUA_ERRMEM (4): 内存分配错误。对于这种错，Lua 不会调用错误处理函数。
LUA_ERRGCMM (5): 在运行 \_\_gc 元方法时发生的错误。 （这个错误和被调用的函数无关。）
LUA_ERRERR (6): 在运行错误处理函数时发生的错误。


#### lua_pop

`void lua_pop (lua_State *L, int n);`

从栈中弹出 n 个元素。

#### luaL_loadfilex

```c
int luaL_loadfilex (lua_State *L, const char *filename,
                                            const char *mode);
```
把一个文件加载为 Lua 代码块。 这个函数使用 lua_load 加载文件中的数据。 代码块的名字被命名为 filename。 如果 filename 为 NULL， 它从标准输入加载。 如果文件的第一行以 # 打头，则忽略这一行。

mode 字符串的作用同函数 lua_load。

此函数的返回值和 lua_load 相同， 不过它还可能产生一个叫做 LUA_ERRFILE 的出错码。这种错误发生于无法打开或读入文件时，或是文件的模式错误。

和 lua_load 一样，这个函数仅加载代码块不运行。

#### luaL_loadfile

`int luaL_loadfile (lua_State *L, const char *filename);`

等价于 luaL_loadfilex， 其 mode 参数等于 NULL。

#### luaL_dofile

`int luaL_dofile (lua_State *L, const char *filename);`

加载并运行指定的文件。 它是用下列宏定义出来：

`(luaL_loadfile(L, filename) || lua_pcall(L, 0, LUA_MULTRET, 0))`

如果没有错误，函数返回假； 有错则返回真。

