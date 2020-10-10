
### 注册lua函数

<a href="1_lua_pushcfunction.cpp">1_lua_pushcfunction.cpp</a>
<a href="2_luaL_newlib.cpp">2_luaL_newlib.cpp</a>
<a href="3_lua_newtable_AND_lua_rawset.cpp">3_lua_newtable_AND_lua_rawset.cpp</a>
<a href="4_luaL_requiref.cpp">4_luaL_requiref.cpp</a>


### 用到的函数

#### lua_CFunction

`typedef int (*lua_CFunction) (lua_State *L);`

C 函数的类型。

为了正确的和 Lua 通讯， C 函数必须使用下列协议。 这个协议定义了参数以及返回值传递方法： C 函数通过 Lua 中的栈来接受参数， 参数以正序入栈（第一个参数首先入栈）。 因此，当函数开始的时候， lua_gettop(L) 可以返回函数收到的参数个数。 第一个参数（如果有的话）在索引 1 的地方， 而最后一个参数在索引 lua_gettop(L) 处。 当需要向 Lua 返回值的时候， C 函数只需要把它们以正序压到堆栈上（第一个返回值最先压入）， 然后返回这些返回值的个数。 在这些返回值之下的，堆栈上的东西都会被 Lua 丢掉。 和 Lua 函数一样，从 Lua 中调用 C 函数也可以有很多返回值。

下面这个例子中的函数将接收若干数字参数，并返回它们的平均数与和：

     static int foo (lua_State *L) {
       int n = lua_gettop(L);    /* 参数的个数 */
       lua_Number sum = 0.0;
       int i;
       for (i = 1; i <= n; i++) {
         if (!lua_isnumber(L, i)) {
           lua_pushliteral(L, "incorrect argument");
           lua_error(L);
         }
         sum += lua_tonumber(L, i);
       }
       lua_pushnumber(L, sum/n);        /* 第一个返回值 */
       lua_pushnumber(L, sum);         /* 第二个返回值 */
       return 2;                   /* 返回值的个数 */
     }

#### lua_pushcfunction

`void lua_pushcfunction (lua_State *L, lua_CFunction f);`

将一个 C 函数压栈。 这个函数接收一个 C 函数指针， 并将一个类型为 function 的 Lua 值压栈。 当这个栈顶的值被调用时，将触发对应的 C 函数。

注册到 Lua 中的任何函数都必须遵循正确的协议来接收参数和返回值。

lua_pushcfunction 是作为一个宏定义出现的：
```c
#define lua_pushcfunction(L,f)  lua_pushcclosure(L,f,0)
```

#### lua_pushcclosure

`void lua_pushcclosure (lua_State *L, lua_CFunction fn, int n);`

把一个新的 C 闭包压栈。

当创建了一个 C 函数后， 你可以给它关联一些值， 这就是在创建一个 C 闭包； 接下来无论函数何时被调用，这些值都可以被这个函数访问到。 为了将一些值关联到一个 C 函数上， 首先这些值需要先被压入堆栈（如果有多个值，第一个先压）。 接下来调用 lua_pushcclosure 来创建出闭包并把这个 C 函数压到栈上。 参数 n 告之函数有多少个值需要关联到函数上。 lua_pushcclosure 也会把这些值从栈上弹出。

n 的最大值是 255 。

当 n 为零时， 这个函数将创建出一个 轻量 C 函数， 它就是一个指向 C 函数的指针。 这种情况下，不可能抛出内存错误。




#### luaL_newlib

`void luaL_newlib (lua_State *L, const luaL_Reg l[]);`

创建一张新的表，并把列表 l 中的函数注册进去。

它是用下列宏实现的：
```
(luaL_newlibtable(L,l), luaL_setfuncs(L,l,0))
```
数组 l 必须是一个数组，而不能是一个指针。

#### luaL_newlibtable

`void luaL_newlibtable (lua_State *L, const luaL_Reg l[]);`

创建一张新的表，并预分配足够保存下数组 l 内容的空间（但不填充）。 这是给 luaL_setfuncs 一起用的。

它以宏形式实现， 数组 l 必须是一个数组，而不能是一个指针。

#### luaL_setfuncs

`void luaL_setfuncs (lua_State *L, const luaL_Reg *l, int nup);`

把数组 l 中的所有函数注册到栈顶的表中（该表在可选的上值之下，见下面的解说）。

若 nup 不为零， 所有的函数都共享 nup 个上值。 这些值必须在调用之前，压在表之上。 这些值在注册完毕后都会从栈弹出。

#### luaL_Reg

```c
typedef struct luaL_Reg {
  const char *name;
  lua_CFunction func;
} luaL_Reg;
```
用于 luaL_setfuncs 注册函数的数组类型。 name 指函数名，func 是函数指针。 

**任何 luaL_Reg 数组必须以一对 name 与 func 皆为 NULL 结束。**



#### lua_settable

`void lua_settable (lua_State *L, int index);`

做一个等价于 t[k] = v 的操作， 这里 t 是给出的索引处的值， v 是栈顶的那个值， k 是栈顶之下的值。

这个函数会将键和值都弹出栈。 跟在 Lua 中一样，这个函数可能触发一个 "newindex" 事件的元方法

#### lua_rawset

`void lua_rawset (lua_State *L, int index);`

类似于 lua_settable ， 但是是做一次直接赋值（不触发元方法）。

#### lua_createtable

`void lua_createtable (lua_State *L, int narr, int nrec);`

创建一张新的空表压栈。 参数 narr 建议了这张表作为序列使用时会有多少个元素； 参数 nrec 建议了这张表可能拥有多少序列之外的元素。 Lua 会使用这些建议来预分配这张新表。 如果你知道这张表用途的更多信息，预分配可以提高性能。 否则，你可以使用函数 lua_newtable 。

#### lua_newtable

`void lua_newtable (lua_State *L);`

创建一张空表，并将其压栈。 它等价于 lua_createtable(L, 0, 0) 。

#### luaL_requiref

`void luaL_requiref (lua_State *L, const char *modname,
                    lua_CFunction openf, int glb);`

如果 modname 不在 package.loaded 中， 则调用函数 openf ，并传入字符串 modname。 将其返回值置入 package.loaded[modname]。 这个行为好似该函数通过 require 调用过一样。

如果 glb 为真， 同时也将模块设到全局变量 modname 里。

在栈上留下该模块的副本。