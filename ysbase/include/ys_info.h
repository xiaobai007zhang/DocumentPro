#pragma once
#include <stdio.h>
#include <wchar.h>
/*
    打印调式信息（DEBUG模式下有效） 
   【注意事项】
    1. Linux下，INFO 和 WINFO 不能混用！！！
    2. INFO如果需要输出宽字符，必须先调用 setlocale，并使用“%ls”，
*/

#define __INFO(fmt, ...) \
do {\
    printf("[%s %s(ln %d)] ", __FILE__, __FUNCTION__, __LINE__); \
    printf(fmt, ##__VA_ARGS__); \
    printf("\n"); \
} while (0)

#define __WINFO(fmt, ...) \
do {\
    wprintf(L"[%hs %hs(ln %d)] ", __FILE__, __FUNCTION__, __LINE__); \
    wprintf(fmt, ##__VA_ARGS__); \
    wprintf(L"\n"); \
} while (0)

#ifdef _DEBUG

#define INFO(...)   __INFO(__VA_ARGS__)
#define WINFO(...)  __WINFO(__VA_ARGS__)

#else

#define INFO(...)
#define WINFO(...)

#endif
