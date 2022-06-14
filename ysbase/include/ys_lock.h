// LOCK头文件
// hhoking 2015
//
#pragma once
#include "cmn.h"

#ifdef PF_WINDOWS
#include <windows.h>

namespace ys
{
    class lock
    {
    private:
        CRITICAL_SECTION m_lock;
    public:
        lock() { InitializeCriticalSection(&m_lock); }
        ~lock() { DeleteCriticalSection(&m_lock); }

        void Lock() { EnterCriticalSection(&m_lock); }
        void Unlock() { LeaveCriticalSection(&m_lock); }
    };
}

#else // PF_LINUX
#include <pthread.h>

namespace ys
{
    class lock
    {
    private:
        pthread_mutex_t m_lock;
    public:
        lock() { pthread_mutex_init(&m_lock, NULL); }
        ~lock() { pthread_mutex_destroy(&m_lock); }

        void Lock() { pthread_mutex_lock(&m_lock); }
        void Unlock() { pthread_mutex_unlock(&m_lock); }
    };
}

#endif

namespace ys
{
    class autolock
    {
    private:
        lock& m_lock;
    public:
        autolock(lock& o) : m_lock(o)
        {
            m_lock.Lock();
        }
        ~autolock()
        {
            m_lock.Unlock();
        }
    };
}
