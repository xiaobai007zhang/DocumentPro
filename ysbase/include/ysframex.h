#pragma once
#define _YSFRAMEX // 用来标示该模块被引用

// 这里包含所有需要前置的头文件
#include "ysbase.h"

// 输出接口标识
#ifdef YSFRAMEX_EXPORTS
#ifdef PF_WINDOWS
#define YSFRAMEX_CLASS __declspec(dllexport)
#define YSFRAMEX_API extern "C" __declspec(dllexport)
#else
#define YSFRAMEX_CLASS __attribute__((visibility("default")))
#define YSFRAMEX_API extern "C" __attribute__((visibility("default")))
#endif
#else
#ifdef PF_WINDOWS
#define YSFRAMEX_CLASS __declspec(dllimport)
#define YSFRAMEX_API extern "C" __declspec(dllimport)
#else
#define YSFRAMEX_CLASS
#define YSFRAMEX_API extern "C"
#endif
#endif

// 应用自动引用库
#if (defined(PF_WINDOWS) && !defined(YSFRAMEX_EXPORTS))
#if (defined(USE_DEBUG) && defined(_DEBUG))
#pragma comment (lib, "ysframexd.lib")
#else
#pragma comment (lib, "ysframex.lib")
#endif
#endif

namespace ys
{
    /*【IModule参数说明】
    {
      "id": "...",
      "soname": "ysframex",
      "cid": 0,
      "param": "cfgname",   // 配置文件名
      "intype": {
        "json": "?,?",
        "img": ?[true|false],
        "imgs": ?[true|false]
      },
      "outype": {
        "json": "?",
        "img": ?[true|false],
        "imgs": ?[true|false]
      }
    }
    // 内置图像合并功能
    {
      "id": "@Combine",
      "intype": {
        "img": true,
        "imgs": true
      },
      "outype": {
        "imgs": true
      }
    }
    // 内置数据合并功能
    {
      "id": "@Group",
      "intype": {
        "json": "?",
        "img": true,
        "imgs": true
      },
      "outype": {
        "json": "?",
        "img": true,
        "imgs": true
      }
    }
    // 内置将数组结构的顶层对象层剥离功能
    {
      "id": "@Strip",
      "intype": {
        "json": "?"
      },
      "outype": {
        "json": "?"
      }
    }
    */

    // 手动调用功能接口
    interface IProcess : public IBase
    {
        // 执行指定子过程
        virtual ERRORCODE SubProcess(pcchar mod_id, Drip& out_drip,
            pcchar in_json, IImageDataCPtr in_img, IImageArrayCPtr in_imgs,
            ErrorStack* es = nullptr) = 0;

        // 执行指定流程
        virtual ERRORCODE SubFlowProcess(pcchar flow_id, Drip& out_drip,
            pcchar in_json, IImageDataCPtr in_img, IImageArrayCPtr in_imgs,
            ErrorStack* es = nullptr) = 0;
    };
    typedef ICONST IProcess* IProcessCPtr;


    // 自定义过程
    typedef ERRORCODE(*MY_PROCESS)(Drip& out_drip, pcchar in_json,
        IImageDataCPtr in_img, IImageArrayCPtr in_imgs, IProcessCPtr proc,
        ErrorStack* es);


    // 手动设置功能接口
    interface ISetProcess : public IBase
    {
        // 设置自定义过程
        virtual ERRORCODE SetProcess(pcchar mod_id, MY_PROCESS proc,
            ErrorStack* es = nullptr) = 0;
    };


    // 创建实例
    YSFRAMEX_API IBase* ysframexCreateComponent(uint cid = 0, IBase* outer = nullptr);


    // 包装类
    class YSFRAMEX_CLASS Framex
    {
    public:
        Framex();
        ~Framex();
    public:
        // 框架下的初始化
        ERRORCODE Init(lint key, uint ppid, uint fid, int devid, pcchar res,
            pcchar cfgname, ErrorStack* es = nullptr);

        // 框架下的过程执行
        ERRORCODE Process(Drip& out_drip, pcchar in_json, IImageDataCPtr in_img,
            IImageArrayCPtr in_imgs, ErrorStack* es = nullptr);

        // 执行指定子过程
        ERRORCODE SubProcess(pcchar mod_id, Drip& out_drip,
            pcchar in_json, IImageDataCPtr in_img, IImageArrayCPtr in_imgs,
            ErrorStack* es = nullptr);

        // 执行指定流程
        ERRORCODE SubFlowProcess(pcchar flow_id, Drip& out_drip,
            pcchar in_json, IImageDataCPtr in_img, IImageArrayCPtr in_imgs,
            ErrorStack* es = nullptr);

        // 设置自定义过程
        ERRORCODE SetProcess(pcchar mod_id, MY_PROCESS proc, ErrorStack* es = nullptr);

    private:
        void* _ptr;
    };


    namespace framex_internal // 内部工具函数
    {
        // 检查配置文件
        YSFRAMEX_CLASS bool CheckConfig(pcchar cfgfile);
    }
}
