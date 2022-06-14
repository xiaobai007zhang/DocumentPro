#pragma once
#define _YSHASP // 用来标示该模块被引用

// 这里包含所有需要前置的头文件
#include "cmn.h"

// 输出接口标识
#ifndef YSHASP_API
#define YSHASP_API
#endif

// 应用自动引用库
#if (defined(PF_WINDOWS) && !defined(YSHASP_LIBSRC))
#if (defined(USE_DEBUG) && defined(_DEBUG))
#pragma comment (lib, "yshaspd.lib")
#else
#pragma comment (lib, "yshasp.lib")
#endif
#endif

namespace ys
{
    // 移动端授权信息
    struct MobileAuth
    {
        char path_name[256] = { 0 };
        char package_name[256] = { 0 };
        int hash_code = 0;
        uint ppid = 0;
        uint fid = 0;

        MobileAuth() {}
        MobileAuth(const MobileAuth& o);
        MobileAuth& operator=(const MobileAuth& o);
        void SplitPointer(uint& fake_ppid, uint& fake_fid) const;
        static const MobileAuth* CombinePointer(uint fake_ppid, uint fake_fid);
    };

    // 算法模块使用
    class ModuleGuard
    {
    public:
        ModuleGuard(lint key, uint ppid, uint fid);

        bool IsOk() const;
        int ErrorCode() const;

    private:
        int m_ec = 0;
    };

    // 获取加密字
    // num1、num2、num3 - 4位数的数字，需要保密
    // timestamp - 调用者时间戳
    YSHASP_API lint GetSecretWord(int num1, int num2, int num3);
    YSHASP_API lint GetSecretWord(int num1, int num2, int num3, lint timestamp);

    // 检测加密字
    // key - GetSecretWord的返回值
    // num1、num2、num3 - 4位数的数字，和GetSecretWord的一致
    // cinterval - 检测允许的间隔时间，单位秒，最长不能超过600秒
    YSHASP_API bool CheckSecretWord(lint key, int num1, int num2, int num3, int cinterval = 600);
}

#if defined(PF_ANDROID)
#define PF_MOBILE
#endif

#if defined(PF_MOBILE)
#define CONST_CYCLE     128
namespace ys
{
    // 应用模块使用
    class ProductGuard
    {
    public:
        ProductGuard() {}
        ~ProductGuard() {}

        // 必须成对使用
        bool GetUsable(uint fake_ppid, uint fake_fid);
        void ReleaseUsable();

        // 必须在GetUsable后使用
        bool Check();

        // 错误码
        int ErrorCode() const;

        // 获取Key参数
        lint GetKey() const;
        uint GetPPID() const { return m_auth.ppid; }
        uint GetFID() const { return m_auth.fid; }

    private:
        int m_ec = 1;                       // 错误码
        MobileAuth m_auth;
        const int m_cycle = CONST_CYCLE;    // 计数周期
        int m_count = 0;                    // 当前计数
        bool m_checked = false;
    };
}
#else
#define CONST_CYCLE     512
class parallel_limit;
namespace ys
{
    // 应用模块使用
    class ProductGuard
    {
    public:
        ProductGuard();
        ~ProductGuard();

        // GetUsable调用成功后获得可用性，如果需要释放可用性必须调用ReleaseUsable
        // 如果GetUsable不成功而调用ReleaseUsable，将可能释放额外的可用性！！！
        bool GetUsable(uint ppid, uint fid);
        void ReleaseUsable();

        // 必须在GetUsable后使用
        bool Check();

        // 错误码
        int ErrorCode() const;

        // 获取参数
        lint GetKey() const;
        uint GetPPID() const { return m_ppid; }
        uint GetFID() const { return m_fid; }

        // 设置计数文件路径
        void SetCFPath(pcchar path);

    private:
        int m_ec = 1;                   // 错误码
        parallel_limit* m_pl = NULL;    // 并发限制
        uint m_ppid = 0;
        uint m_fid = 0;
        int m_cycle = 1;                // 计数周期
        int m_count = 0;                // 当前计数
        int m_num = 0;                  // 是否计数
        bool m_checked = false;
        char m_cfpath[260] = { 0 };     // 计数文件路径
    };
}
#endif
