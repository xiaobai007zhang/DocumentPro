#pragma once
#include "cmn.h"
#include <type_traits>

namespace ys
{
    /* @hhoking@ 萃取许可的基础元素类型
        只支持：
            int、uint、bool、double
        其他说明：
            char、uchar、long 可用 int 来表达
            float 可用 double 来表达
    */
    template<typename T>
    struct rtraits_elem { static const bool ok = false; };
    template<>
    struct rtraits_elem<int> { static const bool ok = true; };
    template<>
    struct rtraits_elem<uint> { static const bool ok = true; };
    template<>
    struct rtraits_elem<bool> { static const bool ok = true; };
    template<>
    struct rtraits_elem<double> { static const bool ok = true; };


    /* @hhoking@ 萃取许可的基础数组元素类型
        只支持：
            uchar       - 用于 Buffer
            char        - 用于本地字符集字符串
            wchar_t     - 用于 Unicode 字符串
            int、uint   - 用于整型数组
            bool        - 用于布尔型数组
            double      - 用于浮点型数组
    */
    template<typename T>
    struct rtraits_array_elem { static const bool ok = false; };
    template<>
    struct rtraits_array_elem<uchar> { static const bool ok = true; };
    template<>
    struct rtraits_array_elem<char> { static const bool ok = true; };
    template<>
    struct rtraits_array_elem<wchar_t> { static const bool ok = true; };
    template<>
    struct rtraits_array_elem<int> { static const bool ok = true; };
    template<>
    struct rtraits_array_elem<uint> { static const bool ok = true; };
    template<>
    struct rtraits_array_elem<bool> { static const bool ok = true; };
    template<>
    struct rtraits_array_elem<double> { static const bool ok = true; };


    /* @hhoking@ 判断一个类是否是某模板类
    */
    template<template<typename> class TM, typename T>
    struct is_template_of { static const bool ok = false; };
    template<template<typename> class TM, typename T>
    struct is_template_of<TM, TM<T>> { static const bool ok = true; };


    /* @hhoking@ 判断一个类是否派生自某类
    */
    template<typename TBase, typename TDerived>
    class is_derived
    {
        static int check(TBase* base);
        static char check(void* any);
    public:
        static const bool ok = (sizeof(int) == sizeof(check((TDerived*)0)));
    };
}