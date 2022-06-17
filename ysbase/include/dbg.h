// DBG头文件
// hhoking 2015
//
#pragma once
#include "cmn.h"

/* @hhoking@ 关于crtdbg

    1. dbg的几个宏
    _DEBUG、DEBUG、NDEBUG这几个宏在Windows下有定义，
    在Linux下需要自己定义。

    2. _CRTDBG_MAP_ALLOC
    该宏开启了更详细的内存泄漏信息。
    msdn给的定义例子是这样的：
        #define _CRTDBG_MAP_ALLOC
        #include <stdlib.h>
        #include <crtdbg.h>
    即要至少保证 _CRTDBG_MAP_ALLOC 定义在上述两个头文件之前。

    3. _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    对动态链接的库都能检测到，也即 DLL 是动态链接的库时，在最外层应用
    里调用 _CrtSetDbgFlag 就行了。
    但对静态链接的DLL库来说，只在最外层调用 _CrtSetDbgFlag 是不行的，
    必须在每一层的DLL里单独调用 _CrtSetDbgFlag
     _CrtDumpMemoryLeaks 要在退出的时候调用，以输出泄漏信息。但如果有
    多个退出时，就必须用 _CrtSetDbgFlag

    4. _CrtSetBreakAlloc(n);
    使用该函数查看调用栈来定位泄漏代码。
*/

#undef ASSERT
#ifdef PF_WINDOWS
#include <crtdbg.h>
#define ASSERT                      _ASSERT
#else
#include <assert.h> // Release必须定义NDEBUG
#define ASSERT                      assert
#endif

#undef VERIFY
#ifdef _DEBUG
#define VERIFY(f)                   ASSERT(f)
#else
#define VERIFY(f)                   ((void)(f))
#endif

#ifdef PF_WINDOWS
#undef TRACE0
#define TRACE0(str)                 _RPTW0(_CRT_WARN,str)
#undef TRACE1
#define TRACE1(str,p1)              _RPTW1(_CRT_WARN,str,p1)
#undef TRACE2
#define TRACE2(str,p1,p2)           _RPTW2(_CRT_WARN,str,p1,p2)
#undef TRACE3
#define TRACE3(str,p1,p2,p3)        _RPTW3(_CRT_WARN,str,p1,p2,p3)
#undef TRACE4
#define TRACE4(str,p1,p2,p3,p4)     _RPTW4(_CRT_WARN,str,p1,p2,p3,p4)
#undef TRACE5
#define TRACE5(str,p1,p2,p3,p4,p5)  _RPTW5(_CRT_WARN,str,p1,p2,p3,p4,p5)
#else
#define TRACE0(str)
#define TRACE1(str,p1)
#define TRACE2(str,p1,p2)
#define TRACE3(str,p1,p2,p3)
#define TRACE4(str,p1,p2,p3,p4)
#define TRACE5(str,p1,p2,p3,p4,p5)
#endif

extern "C" void dbg_open();
