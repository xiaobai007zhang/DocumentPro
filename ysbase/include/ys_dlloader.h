// DLLOADER头文件
// hhoking 2019.6.11
//
#pragma once
#include "cmn.h"

#ifdef PF_WINDOWS
#include <windows.h>

namespace ys
{
    class dlloader
    {
    private:
        HMODULE m_handle;

    public:
        dlloader() :m_handle(NULL) {}
        ~dlloader() { Free(); }

    public:
        bool Load(pcchar name)
        {
            if (m_handle == NULL)
            {
                // 如果name不带路径，LoadLibrary会按照：
                // “本地路径、父进程路径、系统目录、环境变量PATH”
                // 的顺序进行搜索。
                m_handle = ::LoadLibraryA(name);
                return (m_handle != NULL);
            }
            return false;
        }

        void Free()
        {
            if (m_handle != NULL)
            {
                ::FreeLibrary(m_handle);
                m_handle = NULL;
            }
        }

        FARPROC GetProc(pcchar name)
        {
            return (m_handle != NULL) ? ::GetProcAddress(m_handle, name) : NULL;
        }

        bool IsEmpty()
        {
            return (m_handle == NULL);
        }

        pcchar Error()
        {
            return "dlloader error!";
        }
    };
}

#else
#include <dlfcn.h>

namespace ys
{
    class dlloader
    {
    private:
        void* m_handle;

    public:
        dlloader() :m_handle(NULL) {}
        ~dlloader() { Free(); }

    public:
        bool Load(pcchar name)
        {
            if (m_handle == NULL)
            {
                m_handle = dlopen(name, RTLD_NOW);
                return (m_handle != NULL);
            }
            return false;
        }

        void Free()
        {
            if (m_handle != NULL)
            {
                dlclose(m_handle);
                m_handle = NULL;
            }
        }

        void* GetProc(pcchar name)
        {
            return (m_handle != NULL) ? dlsym(m_handle, name) : NULL;
        }

        bool IsEmpty()
        {
            return (m_handle == NULL);
        }

        pcchar Error()
        {
            return dlerror();
        }
    };
}

#endif
