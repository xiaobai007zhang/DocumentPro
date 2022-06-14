// CMN头文件
// hhoking 2015
//
#pragma once


// 是否是 WindowsCE 平台
#ifndef PF_WINCE
#   if (defined(WINCE) || defined(_WIN32_WCE) || defined(UNDER_CE))
#       define PF_WINCE
#   endif
#endif

// 是否是 WindowsPC 平台
#ifndef PF_WINPC
#   if (!defined(PF_WINCE) && (defined(WIN32) || defined(_WIN32)))
#       define PF_WINPC
#   endif
#endif

// 是否是 Windows 平台
#ifndef PF_WINDOWS
#   if (defined(PF_WINPC) || defined(PF_WINCE))
#       define PF_WINDOWS
#   endif
#endif

// 是否是 Linux 平台
#if (!defined(PF_LINUX) && !defined(PF_ANDROID))
#   if (!defined(PF_WINDOWS) && defined(__linux__))
#       if defined(__ANDROID__)
#           define PF_ANDROID
#       else
#           define PF_LINUX
#       endif
#   endif
#endif

// 基本类型
typedef unsigned char               uchar;  //  8bits
typedef unsigned short              ushort; // 16bits
typedef unsigned int                uint;   // 32bits
typedef unsigned long               ulong;  // windows:32bits,linux:64bits
#ifndef PF_WINDOWS
#define __int64                     long long
#endif
typedef __int64                     lint;   // 64bits
typedef unsigned __int64            ulint;  // 64bits
typedef wchar_t                     wch;    // windows:16bits,linux:32bits

// 扩展基本类型
typedef void*                       pvoid;
typedef const void*                 pcvoid;
typedef char*                       pchar;
typedef const char*                 pcchar;
typedef uchar*                      puchar;
typedef const uchar*                pcuchar;
typedef wch*                        pwchar;
typedef const wch*                  pcwchar;
typedef short*                      pshort;
typedef const short*                pcshort;
typedef ushort*                     pushort;
typedef const ushort*               pcushort;
typedef int*                        pint;
typedef const int*                  pcint;
typedef uint*                       puint;
typedef const uint*                 pcuint;
typedef long*                       plong;
typedef const long*                 pclong;
typedef ulong*                      pulong;
typedef const ulong*                pculong;

// 布尔返回值定义
typedef int                         BOOL;
#ifndef FALSE
#define FALSE                       0
#endif
#ifndef TRUE
#define TRUE                        1
#endif

// 默认参数值
#ifdef __cplusplus
#define DFT(x)                      = x
#else
#define DFT(x)
#endif

// 空值
#ifndef NULL
#ifdef __cplusplus
#define NULL                        0
#else
#define NULL                        ((void *)0)
#endif
#endif

// 取大小宏
#ifndef _max
#define _max(a, b)                  (((a) > (b)) ? (a) : (b))
#endif
#ifndef _min
#define _min(a, b)                  (((a) < (b)) ? (a) : (b))
#endif

// 交换数
#ifndef _swap
#define _swap(a, b)                 ((a) ^= (b), (b) ^= (a), (a) ^= (b))
#endif

// 是否使用宽字节字符串宏
#ifdef PF_WINDOWS
#define _USE_WSZPATH
#endif

// 路径字符串类型定义
#ifdef _USE_WSZPATH
typedef pcwchar                     szpath;
typedef pcwchar                     szcmn;
typedef pwchar                      szbuf;
#define ___sz(x)                    L ## x
#define _sz(x)                      ___sz(x)
#else
typedef pcchar                      szpath;
typedef pcchar                      szcmn;
typedef pchar                       szbuf;
#define ___sz(x)                    x       // __sz 居然和Android的libc++里的定义冲突了！修改再加一个下划线！ 
#define _sz(x)                      ___sz(x) 
#endif

// 非Widows环境下未定义的结构
#ifndef PF_WINDOWS
typedef struct tagRECT
{
    int left;
    int top;
    int right;
    int bottom;
}
RECT;

typedef struct tagPOINT
{
    int x;
    int y;
}
POINT;

typedef struct tagSIZE
{
    int cx;
    int cy;
}
SIZE;
#endif

// 函数调用约定(Calling Convention)
#ifdef PF_WINDOWS
#ifdef __GNUC__
#define CC_C                        __attribute__((__cdecl__))
#define CC_STD                      __attribute__((__stdcall__))
#define CC_FAST                     __attribute__((__fastcall__))
#else
#define CC_C                        __cdecl
#define CC_STD                      __stdcall
#define CC_FAST                     __fastcall
#endif
#else
#define CC_C
#define CC_STD
#define CC_FAST
#endif

// 比较函数
typedef int (CC_C * FUNC_COMPARE)(pcvoid elem1, pcvoid elem2);

// 句柄类型
#define MY_HANDLE(name)    typedef struct tag##name{int unused;} *name

// 预定义宏值
#define BINA_BLACK  1  // 8位二值数据前景(黑色)值预定义
#define BINA_WHITE  0  // 8位二值数据背景(白色)值预定义

// 位深转换
#if 0
#define S8S16       << 8
#define S16S8       >> 8
#else
#define S8S16       * 257
#define S16S8       / 257
#endif

// 路径分隔符
#ifdef PF_WINDOWS
#define SEP     _sz('\\')
#define SEPS    _sz("\\/")
#else
#define SEP     _sz('/')
#define SEPS    _sz("/")
#endif