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
	//文件指针
	FILE* m_fp;

public:
	sfile() : m_fp(NULL) { }
	~sfile() { Close(); }
	//重载类型，方便直接返回文件指针
	operator FILE* () const { return m_fp; }

public:

	/*
		结尾带A的方法，代表用到的字符串指针是单字节的
		不带A的方法，代表用到的字符串指针是双字节的
	*/

	//szpath 是pcwchar; p是指针，c是const，wchar是wchar_t 宽字节，总称 const wchar_t *
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

	//szpath 是pcwchar; p是指针，c是const，cchar是char 单字节，总称 const char *
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

	//szpath 是pcwchar; const wchar_t *
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
	//追加方式打开
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

	//关闭文件描述符
	void Close()
	{
		if (m_fp != NULL)
		{
			fclose(m_fp);
			m_fp = NULL;
		}
	}

	//pvoid 是pvoid 是void *类型， size_t 会根据当前编译器32位还是64位来区分，unsigned int
	size_t Read(pvoid buf, size_t count)
	{
		if (m_fp != NULL)
			return fread(buf, 1, count, m_fp);
		return 0;
	}
	//pcvoid 是 const void* 
	size_t Write(pcvoid buf, size_t count)
	{
		if (m_fp != NULL)
			return fwrite(buf, 1, count, m_fp);
		return 0;
	}

	//将两个 0xFEFF 或 0xFFFE 写入到文件指针中
	size_t WriteUTF16Head(bool le = true)
	{
		if (m_fp != NULL)
		{
			ushort head = le ? 0xFEFF : 0xFFFE;
			return fwrite(&head, 1, 2, m_fp);
		}
		return 0;
	}
	//将3个0xBFBBEF写入到文件指针中
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
			//fflush 刷新文件缓冲区
			return (fflush(m_fp) == 0);
		return false;
	}
	/*
	#define SEEK_CUR    1
	#define SEEK_END    2
	#define SEEK_SET    0
	*/

	//从当前的位置开始偏移offset字节
	bool SeekFromCurrent(long offset)
	{
		if (m_fp != NULL)
			return (fseek(m_fp, offset, 1) == 0);
		return false;
	}
	//将文件的读写位置设置到开头处
	bool SeekToBegin()
	{
		if (m_fp != NULL)
			return (fseek(m_fp, 0, 0) == 0);
		return false;
	}
	//将文件的读写位置设置到末尾处
	bool SeekToEnd()
	{
		if (m_fp != NULL)
			return (fseek(m_fp, 0, 2) == 0);
		return false;
	}

	//获得文件的大小
	size_t GetLength() const
	{
		if (m_fp != NULL)
			return s_flen(m_fp);
		return 0;
	}

	//ftell 返回当前的位置
	long GetPosition() const
	{
		if (m_fp != NULL)
			return ftell(m_fp);
		return 0;
	}
};
