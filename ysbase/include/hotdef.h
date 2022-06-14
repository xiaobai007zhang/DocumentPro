// HOT库通用定义
// hhoking 2015
//
#pragma once

// 这里包含所有需要前置的头文件
#include "cmn.h"
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#ifdef PF_WINDOWS
#include <windows.h>
#endif

// 输出接口标识
#ifndef HOT_API
#define HOT_API extern "C"
#endif

// 应用自动引用库
#if (defined(PF_WINDOWS) && !defined(HOT_LIBSRC))
#if (defined(USE_DEBUG) && defined(_DEBUG))
#pragma comment (lib, "hotd.lib")
#else
#pragma comment (lib, "hot.lib")
#endif
#endif
