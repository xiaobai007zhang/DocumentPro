#pragma once
#define _YSFRAME // 用来标示该模块被引用

// 这里包含所有需要前置的头文件
#include "ysbase.h"

// 输出接口标识
#ifdef YSFRAME_EXPORTS
#ifdef PF_WINDOWS
#define YSFRAME_CLASS __declspec(dllexport)
#define YSFRAME_API extern "C" __declspec(dllexport)
#else
#define YSFRAME_CLASS __attribute__((visibility("default")))
#define YSFRAME_API extern "C" __attribute__((visibility("default")))
#endif
#else
#ifdef PF_WINDOWS
#define YSFRAME_CLASS __declspec(dllimport)
#define YSFRAME_API extern "C" __declspec(dllimport)
#else
#define YSFRAME_CLASS
#define YSFRAME_API extern "C"
#endif
#endif

// 应用自动引用库
#if (defined(PF_WINDOWS) && !defined(YSFRAME_EXPORTS))
#if (defined(USE_DEBUG) && defined(_DEBUG))
#pragma comment (lib, "ysframed.lib")
#else
#pragma comment (lib, "ysframe.lib")
#endif
#endif

namespace ys
{
    // 模型初始化
    YSFRAME_API ERRORCODE ysfInit(pcchar flag, HOCR& handle,
        uint ppid, uint fid, int devid, pcchar resdir);

    // 模型反初始化
    YSFRAME_API void ysfUninit(pcchar flag, HOCR handle);

    // 数据处理
    YSFRAME_API ERRORCODE ysfProcess(pcchar flag, pchar& jresult,
        HOCR handle, puchar data, int len, pcchar jparam);

    // 释放结果
    YSFRAME_API void ysfDelete(pcchar flag, pchar jresult);

    // 注册图像解析模块
    YSFRAME_API ERRORCODE ysfRegister(pcchar modname, pcchar funcname);

    // 处理多页数据
    YSFRAME_API ERRORCODE ysfProcessMultipage(pcchar flag, pchar& jresult,
        HOCR handle, puchar data, int len, pcchar jparam);

    // 注册多页数据解析模块
    YSFRAME_API ERRORCODE ysfRegisterMultipage(pcchar modname, pcchar funcname);
}
