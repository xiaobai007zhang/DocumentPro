#pragma once
#include <utility>

namespace ys
{
    template<typename T>
    class IPtr
    {
    private:
        T*  _ptr = nullptr;

    public:
        IPtr() {}
        IPtr(const IPtr& o)
        {
            if (o)
            {
                _ptr = o._ptr;
                _ptr->xAddRef();
            }
        }
        IPtr& operator=(const IPtr& o)
        {
            Release();
            if (o)
            {
                _ptr = o._ptr;
                _ptr->xAddRef();
            }
            return *this;
        }
        IPtr(IPtr&& o) noexcept
        {
            std::swap(_ptr, o._ptr);
        }
        IPtr& operator=(IPtr&& o) noexcept
        {
            std::swap(_ptr, o._ptr);
            return *this;
        }
        IPtr(T*&& ptr) noexcept
        {
            _ptr = ptr; // 右值默认是初始指针，直接包装
        }
        IPtr& operator=(T*&& ptr) noexcept
        {
            Release();
            _ptr = ptr; // 右值默认是初始指针，直接包装
            return *this;
        }
        ~IPtr() { Release(); }

    public:
        operator bool() const { return _ptr != nullptr; }
        operator T*() const { return _ptr; }
        T* operator ->() const { return _ptr; }

    public:
        template<typename Q>
        Q* Query(const char* name)
        {
            Q* ptr = nullptr;
            if (_ptr)
                _ptr->xQuery(name, (void**)&ptr);
            return ptr;
        }

        void Release()
        {
            if (_ptr)
            {
                _ptr->xRelease();
                _ptr = nullptr;
            }
        }
    };

    template<typename T, typename Q>
    Q* Query(T* src, const char* name)
    {
        Q* ptr = nullptr;
        if (src)
            src->xQuery(name, (void**)&ptr);
        return ptr;
    }

    template<typename T>
    T* Ref(T* src)
    {
        if (src)
        {
            src->xAddRef();
            return src;
        }
        return nullptr;
    }
}