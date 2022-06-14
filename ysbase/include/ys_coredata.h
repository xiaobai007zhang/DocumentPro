#pragma once
#include "ys_mem.h"
#include "ys_basetype.h"
#include <utility>

namespace ys
{
    // 核心数据类
    class CoreData
    {
        Memory _p;                      // 数据
        MemoryT<int> _d;                // 维度值（按高维到低维的顺序）
        BaseType _t = BaseType::Float;  // 数据类型

    public:
        CoreData() {}
        CoreData(int d0, BaseType t) { New(d0, t); }
        CoreData(int d0, int d1, BaseType t) { New(d0, d1, t); }
        CoreData(int d0, int d1, int d2, BaseType t) { New(d0, d1, d2, t); }
        CoreData(int n, int c, int h, int w, BaseType t = BaseType::Float) { New(n, c, h, w, t); }
        CoreData(const CoreData& o) = delete;
        CoreData& operator=(const CoreData& o) = delete;
        CoreData(CoreData&& o) noexcept
        {
            _p = std::move(o._p);
            _d = std::move(o._d);
            std::swap(_t, o._t);
        }
        CoreData& operator=(CoreData&& o) noexcept
        {
            _p = std::move(o._p);
            _d = std::move(o._d);
            std::swap(_t, o._t);
            return *this;
        }

        // 获取数据指针
        void* p() { return _p; }
        const void* p() const { return _p; }

        // 获取维度值个数
        int dc() const { return _d.Count(); }

        // 获取指定维度值（调用者需要确保idx有效，否则返回0这有可能导致崩溃）
        int d(int idx) const { return idx < dc() ? _d[idx] : 0; }

        // 兼容nchw（样本数、通道数、高、宽）的定义
        int n() const { return d(0); }
        int c() const { return d(1); }
        int h() const { return d(2); }
        int w() const { return d(3); }

        // 获取数据类型
        BaseType t() const { return _t; }

        // 创建1维数据缓冲区
        void* New(int d0, BaseType t)
        {
            _d.Resize(1);
            _d[0] = d0;
            _t = t;
            return _p.Alloc(BTSIZE(t), d0, 0, false);
        }

        // 创建2维数据缓冲区
        void* New(int d0, int d1, BaseType t)
        {
            _d.Resize(2);
            _d[0] = d0;
            _d[1] = d1;
            _t = t;
            return _p.Alloc(BTSIZE(t), d0 * d1, 0, false);
        }

        // 创建3维数据缓冲区
        void* New(int d0, int d1, int d2, BaseType t)
        {
            _d.Resize(3);
            _d[0] = d0;
            _d[1] = d1;
            _d[2] = d2;
            _t = t;
            return _p.Alloc(BTSIZE(t), d0 * d1 * d2, 0, false);
        }

        // 创建4维数据缓冲区
        void* New(int n, int c, int h, int w, BaseType t = BaseType::Float)
        {
            _d.Resize(4);
            _d[0] = n;
            _d[1] = c;
            _d[2] = h;
            _d[3] = w;
            _t = t;
            return _p.Alloc(BTSIZE(t), n * c * h * w, 0, false);
        }

        // 创建n维数据缓冲区
        void* New(int d[], int c, BaseType t)
        {
            uint elem_count = 1;
            _d.Resize(c);
            for (int i = 0; i < c; i++)
            {
                _d[i] = d[i];
                elem_count *= d[i];
            }
            _t = t;
            return _p.Alloc(BTSIZE(t), elem_count, 0, false);
        }

        // 重新改变4维缓冲区大小
        void* Resize(int n, int c, int h, int w, BaseType t = BaseType::Float)
        {
            _d.Resize(4);
            _d[0] = n;
            _d[1] = c;
            _d[2] = h;
            _d[3] = w;
            _t = t;
            return _p.Resize(BTSIZE(t), n * c * h * w, 0, false);
        }

        // 重新改变n维缓冲区大小
        void* Resize(int d[], int c, BaseType t)
        {
            uint elem_count = 1;
            _d.Resize(c);
            for (int i = 0; i < c; i++)
            {
                _d[i] = d[i];
                elem_count *= d[i];
            }
            _t = t;
            return _p.Resize(BTSIZE(t), elem_count, 0, false);
        }
    };
}