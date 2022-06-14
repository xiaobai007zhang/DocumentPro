#pragma once
#include "ys_error.h"
#include "ys_coredata.h"
#include "ys_ibase.h"

namespace ys
{
    // 部署平台接口
    interface IEvaluation : public IBase
    {
        // 初始化
        // key    - 接口调用校验值
        // fname  - 模型文件路径名
        // devid  - 设备ID（-1表示使用CPU）
        // jparam - 控制参数（具体实现如果没有额外参数，可以为nullptr）
        // es     - 错误堆栈
        virtual ERRORCODE Init(lint key, pcchar fname, int devid, pcchar jparam,
            ErrorStack* es = nullptr) = 0;

        // 执行
        // data_out - 输出数据
        // data_in  - 输入数据
        // es       - 错误堆栈
        virtual ERRORCODE Classify(CoreData& data_out, const CoreData& data_in,
            ErrorStack* es = nullptr) = 0;
    };
}