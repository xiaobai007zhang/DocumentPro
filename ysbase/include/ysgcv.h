// YSGCV库头文件
// hhoking 2019.10.24
//
#pragma once
#define _YSGCV // 用来标示该模块被引用

// 这里包含所有需要前置的头文件
#include "ysbase.h"

// 输出接口标识
#ifdef YSGCV_EXPORTS
#ifdef PF_WINDOWS
#define YSGCV_CLASS __declspec(dllexport)
#define YSGCV_API extern "C" __declspec(dllexport)
#else
#define YSGCV_CLASS __attribute__((visibility("default")))
#define YSGCV_API extern "C" __attribute__((visibility("default")))
#endif
#else
#ifdef PF_WINDOWS
#define YSGCV_CLASS __declspec(dllimport)
#define YSGCV_API extern "C" __declspec(dllimport)
#else
#define YSGCV_CLASS
#define YSGCV_API extern "C"
#endif
#endif

// 应用自动引用库
#if (defined(PF_WINDOWS) && !defined(YSGCV_EXPORTS))
#if (defined(USE_DEBUG) && defined(_DEBUG))
#pragma comment (lib, "ysgcvd.lib")
#else
#pragma comment (lib, "ysgcv.lib")
#endif
#endif

#define CLS_IMAGE           0   // 图像组件
#define CLS_IMAGEARRAY      1   // 图像数组组件
#define CLS_IMAGEPROCESS    2   // 图像处理组件

namespace ys
{
    /*【IModule参数说明】
    {
      "id": "...",
      "soname": "ysgcv",
      "cid": 2,
      "param": "[RotateAngle|RotateDirection|Perspective|Resize]",  // 功能类型
      "intype": {
        "json": "[RotateAngleIn|RotateDirectionIn|PerspectiveIn|ResizeIn]",
        "img": true
      },
      "outype": {
        "img": true
      }
    }
    */
    R_STRUCT_BEGIN(RotateAngleIn)
        R_DOUBLE(degree)
        R_BOOL(fix)
        R_STRUCT_END;

    R_STRUCT_BEGIN(RotateDirectionIn)
        R_INT(flag)
        R_STRUCT_END;

    R_STRUCT_BEGIN(PerspectiveIn)
        R_FIXINTARRAY(coords, 8)
        R_INT(dw)
        R_INT(dh)
        R_STRUCT_END;

    R_STRUCT_BEGIN(ResizeIn)
        R_INT(dw)
        R_INT(dh)
        R_DOUBLE(scale)
        R_STRUCT_END;


    // 测试
    YSGCV_API void ysgcvTest();

    // 创建实例
    // cid - 内部用于区分不同实例的索引值
    YSGCV_API IBase* ysgcvCreateComponent(uint cid = 0, IBase* outer = nullptr);

    // 提供图像解析接口
    YSGCV_API IImageData* ysImageParse(pcuchar buf, int size);

    // 创建组件对象
    YSGCV_CLASS IImageData* CreateImage();
    YSGCV_CLASS IImageArray* CreateImageArray();

    // 加载和保存图像
    YSGCV_CLASS IImageData* LoadBGR(pcchar pathname);
    YSGCV_CLASS IImageData* LoadGrey(pcchar pathname);
    YSGCV_CLASS bool SaveImage(pcchar pathname, IImageDataCPtr src);

    // 加载和保存图像（利用缓冲区）
    YSGCV_CLASS IImageData* LoadBGR(pcuchar buf, int size);
    YSGCV_CLASS IImageData* LoadGrey(pcuchar buf, int size);
    YSGCV_CLASS bool SaveImage(Buffer& buf, IImageDataCPtr src, pcchar ext = ".png");

    // 任意角度旋转图像
    YSGCV_CLASS IImageData* RotateAngle(IImageDataCPtr src, double degree, bool fix);

    // 按方向旋转图像
    // flag - =0顺时针90，=1旋转180，=2逆时针90（和cv的RotateFlags一致）
    YSGCV_CLASS IImageData* RotateDirection(IImageDataCPtr src, int flag);

    // 透视矫正
    // pts - 原图上的4点坐标
    // dw、dh - 变换后的图像宽高，给0时内部默认计算一个宽高
    YSGCV_CLASS IImageData* Perspective(IImageDataCPtr src, const POINT pts[], int dw, int dh);

    // 缩放
    YSGCV_CLASS IImageData* Resize(IImageDataCPtr src, int dw, int dh, double scale);

    // 图像数据转换为平台数据
    YSGCV_CLASS bool ToCoreData(CoreData& cd, IImageDataCPtr img, const double* mean = nullptr, const double* scale = nullptr);
    YSGCV_CLASS bool ToCoreData(CoreData& cd, IImageArrayCPtr imgs, const double* mean = nullptr, const double* scale = nullptr);
}