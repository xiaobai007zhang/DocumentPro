// YSBASE库头文件
// hhoking 2019.10.10
//
#pragma once
#define _YSBASE // 用来标示该模块被引用

// 这里包含所有需要前置的头文件
#include "cmn.h"
#ifdef PF_WINDOWS
#include <windows.h>
#else
#include <stddef.h>
#endif

// 输出接口标识
#ifndef YSBASE_CLASS
#ifdef PF_WINDOWS
#define YSBASE_CLASS __declspec(dllimport)
#else
#define YSBASE_CLASS
#endif
#endif
#ifndef YSBASE_API
#ifdef PF_WINDOWS
#define YSBASE_API extern "C" __declspec(dllimport)
#else
#define YSBASE_API extern "C"
#endif
#endif

// 应用自动引用库
#if (defined(PF_WINDOWS) && !defined(YSBASE_LIBSRC))
#if (defined(USE_DEBUG) && defined(_DEBUG))
#pragma comment (lib, "ysbased.lib")
#else
#pragma comment (lib, "ysbase.lib")
#endif
#endif

#include "ys_mem.h"
#include "ys_imgtype.h"
#include "ys_doctype.h"
#include "ys_language.h"
#include "ys_coredata.h"
#include "ys_error.h"
#include "ys_util.h"
#include "ys_ocrwrap.h"
#include "ys_info.h"

#include "ys_rbase.h"
#include "ys_rbuffer.h"
#include "ys_rastring.h"
#include "ys_rutf8string.h"
#include "ys_rptrarray.h"
#include "ys_rio.h"
#include "ys_rtypes.h"
#include "ys_rlay.h"

#include "ys_iptr.h"
#include "ys_ibase.h"
#include "ys_iimage.h"
#include "ys_ievaluation.h"
#include "ys_imodule.h"

namespace ys
{
    // 测试
    YSBASE_API void ysbaseTest();


    // Summary: 进行透视变换
    // Parameters:
    // [out] dstcoords  - 目标坐标
    // [in]  srccoords  - 源坐标
    // [in]  mat        - 变换矩阵
    YSBASE_API void ysPerspectiveTransform(int dstcoords[8], const int srccoords[8], const double mat[9]);

    // Summary: 获取透视变换矩阵
    // Parameters:
    // [out] mat        - 变换矩阵
    // [in]  srccoords  - 源坐标
    // [in]  dstcoords  - 目标坐标
    YSBASE_API void ysGetPerspectiveMat(double mat[9], const int srccoords[8], const int dstcoords[8]);

    // Summary: 得到图像文件类型
    // Parameters:
    // [in] filename  - 图像文件路径名
    YSBASE_API ImgType ysGetImageType(pcchar filename);

    // Summary: 得到图像数据类型
    // Parameters:
    // [in] buf      - 图像数据缓冲区
    // [in] bufsize  - 缓冲区大小
    YSBASE_API ImgType ysGetImageTypeFromBuffer(pcuchar buf, size_t bufsize);
}