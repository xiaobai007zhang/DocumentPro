#pragma once

namespace ys
{
    // 基础数据类型
    enum class BaseType
    {
        Int8 = 1,
        Uint8 = 1 + (1 << 4),
        Int16 = 2,
        Uint16 = 2 + (1 << 4),
        Int32 = 4,
        Uint32 = 4 + (1 << 4),
        Float = 4 + (2 << 4),
        Int64 = 8,
        Uint64 = 8 + (1 << 4),
        Double = 8 + (2 << 4)
    };
}

#define BTSIZE(n)   (int(n) & 0xF)