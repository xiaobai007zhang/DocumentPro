// HOT库头文件
// hhoking 2015
//
#pragma once
#include "cmn.h"
#include <stdio.h>


// 获得文件内容大小
inline size_t s_flen(FILE* f)
{
    long pos = ftell(f);
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, pos, SEEK_SET);
    return size;
}

// 获得文件内容剩余大小
inline size_t s_fleft(FILE* f)
{
    long pos = ftell(f);
    fseek(f, 0, SEEK_END);
    size_t left = ftell(f) - pos;
    fseek(f, pos, SEEK_SET);
    return left;
}


class sfile
{
private:
    FILE* m_fp;

public:
    sfile() : m_fp(NULL) { }
    ~sfile() { Close(); }

    operator FILE* () const { return m_fp; }

public:
    bool OpenRead(szpath filename)
    {
        if (m_fp == NULL)
        {
#ifdef _USE_WSZPATH
            m_fp = _wfopen(filename, L"rb");
#else
            m_fp = fopen(filename, "rb");
#endif
            return (m_fp != NULL);
        }
        printf("sfile::OpenRead false!\n");
        return false;
    }

    bool OpenReadA(pcchar filename)
    {
        if (m_fp == NULL)
        {
            m_fp = fopen(filename, "rb");
            return (m_fp != NULL);
        }
        printf("sfile::OpenReadA false!\n");
        return false;
    }

    bool OpenWrite(szpath filename)
    {
        if (m_fp == NULL)
        {
#ifdef _USE_WSZPATH
            m_fp = _wfopen(filename, L"wb");
#else
            m_fp = fopen(filename, "wb");
#endif
            return (m_fp != NULL);
        }
        printf("sfile::OpenWrite false!\n");
        return false;
    }

    bool OpenWriteA(pcchar filename)
    {
        if (m_fp == NULL)
        {
            m_fp = fopen(filename, "wb");
            return (m_fp != NULL);
        }
        printf("sfile::OpenWriteA false!\n");
        return false;
    }

    bool OpenAppend(szpath filename)
    {
        if (m_fp == NULL)
        {
#ifdef _USE_WSZPATH
            m_fp = _wfopen(filename, L"ab");
#else
            m_fp = fopen(filename, "ab");
#endif
            return (m_fp != NULL);
        }
        printf("sfile::OpenAppend false!\n");
        return false;
    }

    bool OpenAppendA(pcchar filename)
    {
        if (m_fp == NULL)
        {
            m_fp = fopen(filename, "ab");
            return (m_fp != NULL);
        }
        printf("sfile::OpenAppend false!\n");
        return false;
    }

    void Close()
    {
        if (m_fp != NULL)
        {
            fclose(m_fp);
            m_fp = NULL;
        }
    }

    size_t Read(pvoid buf, size_t count)
    {
        if (m_fp != NULL)
            return fread(buf, 1, count, m_fp);
        return 0;
    }

    size_t Write(pcvoid buf, size_t count)
    {
        if (m_fp != NULL)
            return fwrite(buf, 1, count, m_fp);
        return 0;
    }

    size_t WriteUTF16Head(bool le = true)
    {
        if (m_fp != NULL)
        {
            ushort head = le ? 0xFEFF : 0xFFFE;
            return fwrite(&head, 1, 2, m_fp);
        }
        return 0;
    }

    size_t WriteUTF8Head()
    {
        if (m_fp != NULL)
        {
            uint head = 0xBFBBEF;
            return fwrite(&head, 1, 3, m_fp);
        }
        return 0;
    }

    bool Flush()
    {
        if (m_fp != NULL)
            return (fflush(m_fp) == 0);
        return false;
    }

    bool SeekFromCurrent(long offset)
    {
        if (m_fp != NULL)
            return (fseek(m_fp, offset, 1) == 0);
        return false;
    }

    bool SeekToBegin()
    {
        if (m_fp != NULL)
            return (fseek(m_fp, 0, 0) == 0);
        return false;
    }

    bool SeekToEnd()
    {
        if (m_fp != NULL)
            return (fseek(m_fp, 0, 2) == 0);
        return false;
    }

    size_t GetLength() const
    {
        if (m_fp != NULL)
            return s_flen(m_fp);
        return 0;
    }

    long GetPosition() const
    {
        if (m_fp != NULL)
            return ftell(m_fp);
        return 0;
    }
};
