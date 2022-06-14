#pragma once
#include "ys_error.h"
#include "ys_ibase.h"
#include "ys_iimage.h"
#include "ys_iptr.h"

namespace ys
{
    // 结果数据结构
    struct Drip
    {
        StringA json;           // JSON数据
        IPtr<IImageData> img;   // 图像数据
        IPtr<IImageArray> imgs; // 图像数组数据

        Drip() {}
        Drip(const Drip& o) = delete;
        Drip& operator=(const Drip& o) = delete;
        Drip(Drip&& o) noexcept
        {
            json = std::move(o.json);
            img = std::move(o.img);
            imgs = std::move(o.imgs);
        }
        Drip& operator=(Drip&& o) noexcept
        {
            json = std::move(o.json);
            img = std::move(o.img);
            imgs = std::move(o.imgs);
            return *this;
        }
    };

    // 模块接口
    interface IModule : public IBase
    {
        // 初始化
        // key    - 接口调用校验值
        // ppid   - 加密锁产品编号
        // fid    - 加密锁特征编号
        // devid  - 设备编号，-1表示CPU，GPU编号为0,1,...
        // res    - 资源（可以是模型文件路径名，也可以是资源目录名）
        // jparam - 控制参数（具体实现如果没有额外参数，可以为nullptr）
        // es     - 错误堆栈
        virtual ERRORCODE Init(
            lint key,
            uint ppid,
            uint fid,
            int devid,
            pcchar res,
            pcchar jparam,
            ErrorStack* es = nullptr
        ) = 0;

        // 执行
        // out_drip - 输出数据
        // in_json  - 输入JSON数据（可以为nullptr）
        // in_img   - 输入图像数据（可以为nullptr）
        // in_imgs  - 输入图像数组数据（可以为nullptr）
        // es       - 错误堆栈
        virtual ERRORCODE Process(
            Drip& out_drip,
            pcchar in_json,
            IImageDataCPtr in_img,
            IImageArrayCPtr in_imgs,
            ErrorStack* es = nullptr
        ) = 0;
    };
}