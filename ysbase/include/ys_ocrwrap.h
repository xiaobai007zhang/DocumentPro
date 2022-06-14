#pragma once
#include "cmn.h"
#include "ys_errorcode.h"
#include "ys_iimage.h"

namespace ys
{
    /* @hhoking@ OCR接口包装协议

        这里定义一种OCR接口包装方式，所有符合这种包装方式的模块需要满足下列协议：
        1. 模块命名：
            linux下   - libysocr[flag].so
            windows下 - ysocr[flag].dll
        2. 模块需要实现三个C风格的接口函数：
            ys[flag]Init
            ys[flag]Uninit
            ys[flag]Process
    */

    // 识别核心句柄
    MY_HANDLE(HOCR);

    // 初始化
    // handle - 返回的识别核心句柄
    // ppid   - 加密锁产品编号
    // fid    - 加密锁特征编号
    // devid  - 设备编号，-1表示CPU，GPU编号为0,1,...
    // resdir - 资源目录名
    typedef ERRORCODE(*OCR_INIT)(HOCR& handle, uint ppid, uint fid, int devid, pcchar resdir);

    // 反初始化
    // handle   - 识别核心句柄
    typedef void(*OCR_UNINIT)(HOCR handle);

    // 处理
    // jresult  - 返回结果JSON字符串，失败返回NULL
    // handle   - 识别核心句柄
    // img      - 图像指针
    // jparam   - 控制参数（没有可以为NULL）
    typedef ERRORCODE(*OCR_PROCESS)(StringA& jresult, HOCR handle, IImageDataCPtr img, pcchar jparam);
}