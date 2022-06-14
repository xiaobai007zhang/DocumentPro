#pragma once
#include "cmn.h"
#ifndef interface
#define interface struct
#endif

namespace ys
{
    // 基接口（所有接口都必须派生自它）
    interface IBase
    {
        // 请求接口（应用不要直接调用！！！）
        virtual bool xQuery(const char* name, void** ppv) = 0;

        // 增加引用（应用不要直接调用！！！）
        virtual void xAddRef() = 0;

        // 释放接口（应用不要直接调用！！！）
        virtual void xRelease() = 0;

        // 内部请求接口（应用不要直接调用！！！）
        virtual bool xQueryInner(const char* name, void** ppv) = 0;
    };
}

// 内部请求接口（应用不要直接调用！！！）
template<typename T, typename Q>
Q* QueryInner(T* src, const char* name)
{
    Q* ptr = nullptr;
    if (src)
        src->xQueryInner(name, (void**)&ptr);
    return ptr;
}

// 基接口实现的辅助宏
//
#define BASE_BEGIN(cls) \
private: \
    IBase* _outer = nullptr; \
    int _refc = 1; \
public: \
    static cls* Create(IBase* outer) { return new cls(outer); } \
    bool xQuery(const char* name, void** ppv) { \
        return _outer ? _outer->xQuery(name, ppv) : xQueryInner(name, ppv); \
    } \
    void xAddRef() { _refc++; } \
    void xRelease() { if (--_refc == 0) delete this; } \
    cls(IBase* outer) : _outer(outer) {

#define INNER_DEF \
        IPtr<IBase> ib;

#define INNER_COMP(func,cid) \
        ib = func(cid, static_cast<IBase*>(this));

#define INNER_ADD(itype) \
        _##itype = QueryInner<IBase, itype>(ib, #itype);

#define BASE_END \
    }

#define INNER_MEMBER(itype) \
private: \
    IPtr<itype> _##itype;

#define QUERY_BEGIN \
public: \
    bool xQueryInner(const char* name, void** ppv) { \
        *ppv = nullptr;

#define QUERY_ADD(itype) \
        if (strcmp(name, #itype) == 0) { \
            *ppv = static_cast<itype*>(this); \
            return bool(++_refc); \
        }

#define QUERY_INNER_ADD(itype) \
        if (strcmp(name, #itype) == 0) { \
            *ppv = _##itype; _##itype->xAddRef(); \
            return true; \
        }

#define QUERY_END \
        return false; \
    }

// 常量修饰符
#define ICONST

// 接口校验 
extern "C" bool sCheckSecretWord(lint key);
