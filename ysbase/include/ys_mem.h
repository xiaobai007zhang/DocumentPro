#pragma once
#include "cmn.h"
#include <utility>

#ifndef YSBASE_CLASS
#ifdef PF_WINDOWS
#define YSBASE_CLASS __declspec(dllimport)
#else
#define YSBASE_CLASS
#endif
#endif

namespace ys
{
	/* @hhoking@ 统一内存分配管理类
	*/
	class YSBASE_CLASS Memory
	{
	private:
		void* _ptr = nullptr;

	public:
		Memory() {}
		Memory(const Memory& o) = delete;
		Memory& operator=(const Memory& o) = delete;
		Memory(Memory&& o) noexcept;
		Memory& operator=(Memory&& o) noexcept;

		//构造函数，  元素字节数，   元素个数      多出的元素个数   是否初始化为零
		Memory(uint elem_size, uint elem_count, uint elem_extend, bool zero)
		{
			Alloc(elem_size, elem_count, elem_extend, zero);
		}
		explicit Memory(uint size, bool zero = true)
		{
			Alloc(1, size, 0, zero);
		}
		~Memory() { Free(); }

	public:
#if 1
		operator void* () { return _ptr; }
		operator const void* () const { return _ptr; }
#else
		// 不规范的实现（废弃的！！！），但有些旧模块要兼容时需要
		operator void* () const { return _ptr; }
#endif
		operator bool() const { return _ptr != nullptr; }

		// 内存元素大小（字节单位）
		uint ElemSize() const;

		// 内存元素个数
		uint ElemCount() const;

		// 内存容量（字节单位）
		size_t Capacity() const;

		// 内存大小（字节单位）
		size_t Size() const { return ElemSize() * ElemCount(); }

	public:
		/* 通用参数说明
			elem_size   - 内存元素大小（字节单位）
			elem_count  - 内存元素个数
			elem_extend - 多分配的内存元素个数
			zero        - 是否初始化内存为0
			grow        - 容量自动增长策略
			成功返回 内存首地址指针
			失败返回 nullptr
		*/
		// 内存分配        元素大小		元素个数		元素扩展个数	初始化为零
		void* Alloc(uint elem_size, uint elem_count, uint elem_extend, bool zero);

		// 改变内存大小
		void* Resize(uint elem_size, uint elem_count, uint elem_extend, bool grow);

		// 预留内存容量
		void Reserve(size_t capacity);

		// 内存释放
		void Free();

	public:
		// 只在了解原理的特殊场合下使用，一般不要使用！！！
		void Attach(void* ptr);
		void* Detach();
	};

	/* @hhoking@ 内存数据类型萃取
	*/
	struct BasType {};
	struct ObjType {};
	template<typename T>
	struct traits_mem { typedef ObjType value_type; };
	template<typename T>
	struct traits_mem<T*> { typedef BasType value_type; };
	template<>
	struct traits_mem<bool> { typedef BasType value_type; };
	template<>
	struct traits_mem<char> { typedef BasType value_type; };
	template<>
	struct traits_mem<unsigned char> { typedef BasType value_type; };
	template<>
	struct traits_mem<wchar_t> { typedef BasType value_type; };
	template<>
	struct traits_mem<short> { typedef BasType value_type; };
	template<>
	struct traits_mem<unsigned short> { typedef BasType value_type; };
	template<>
	struct traits_mem<int> { typedef BasType value_type; };
	template<>
	struct traits_mem<unsigned int> { typedef BasType value_type; };
	template<>
	struct traits_mem<long> { typedef BasType value_type; };
	template<>
	struct traits_mem<unsigned long> { typedef BasType value_type; };
	template<>
	struct traits_mem<long long> { typedef BasType value_type; };
	template<>
	struct traits_mem<unsigned long long> { typedef BasType value_type; };
	template<>
	struct traits_mem<float> { typedef BasType value_type; };
	template<>
	struct traits_mem<double> { typedef BasType value_type; };

	/* @hhoking@ 数据内存分配模板
		基础数据：char、uchar、wchar_t、int、uint 等
		对象数据：class、struct
	*/
	template<typename T>
	class MemoryT
	{
	private:

		Memory _mem;

#if (defined(USE_DEBUG) && defined(_DEBUG))
		T* _ptr = nullptr;
#endif
	public:
		MemoryT() {}

		/*
			禁用了拷贝构造和重载赋值运算符
		*/
		MemoryT(const MemoryT& o) = delete;
		MemoryT& operator=(const MemoryT& o) = delete;


		//移动构造函数
		MemoryT(MemoryT&& o) noexcept
		{
			_mem = std::move(o._mem);
#if (defined(USE_DEBUG) && defined(_DEBUG))
			o._ptr = _ptr;
			_ptr = (T*)(void*)_mem;
#endif
		}
		MemoryT& operator=(MemoryT&& o) noexcept
		{
			_mem = std::move(o._mem);
#if (defined(USE_DEBUG) && defined(_DEBUG))
			o._ptr = _ptr;
			_ptr = (T*)(void*)_mem;
#endif
			return *this;
		}
		explicit MemoryT(int count) { New(count); }

		//析构的时候会根据当前的类型来进行区分析构
		~MemoryT() { Delete(); }

	public:
		operator T* () { return (T*)(void*)_mem; }
		operator const T* () const { return (const T*)(const void*)_mem; }
		T& operator[](int idx) { return ((T*)(void*)_mem)[idx]; }
		const T& operator[](int idx) const { return ((const T*)(const void*)_mem)[idx]; }
		operator bool() const { return _mem; }

		int Count() const { return (int)_mem.ElemCount(); }
		int Capacity() const { return (int)_mem.Capacity(); }

	public:
		//分配内存， 个数        是否初始化
		T* New(int count, bool zero = false)
		{
			typename traits_mem<T>::value_type type;
#if (defined(USE_DEBUG) && defined(_DEBUG))
			return _ptr = _new(count, zero, type);
#else
			return _new(count, zero, type);
#endif
		}

		template<typename... A>
		T* New(int count, A... args)
		{
			typename traits_mem<T>::value_type type;
#if (defined(USE_DEBUG) && defined(_DEBUG))
			return _ptr = _new(count, false, type, args...);
#else
			return _new(count, false, type, args...);
#endif
		}

		void Delete()
		{
			typename traits_mem<T>::value_type type;
			_delete(type);
		}

		// 将数据截断到count指定的大小
		//（如果count超过当前数据大小则不做任何事情，反之被截断数据被析构）
		void Truncate(int count)
		{
			typename traits_mem<T>::value_type type;
			_truncate(count, type);
		}

		// 改变数据大小
		//（如果是缩小则引起析构，如果是扩大则引起构造）
		//（反复调用该函数的场景，grow 可以设置为 true）
		T* Resize(int count, bool grow = false)
		{
			typename traits_mem<T>::value_type type;
#if (defined(USE_DEBUG) && defined(_DEBUG))
			return _ptr = _resize(count, grow, type);
#else
			return _resize(count, grow, type);
#endif
		}

		template<typename... A>
		T* Resize(int count, bool grow, A... args)
		{
			typename traits_mem<T>::value_type type;
#if (defined(USE_DEBUG) && defined(_DEBUG))
			return _ptr = _resize(count, grow, type, args...);
#else
			return _resize(count, grow, type, args...);
#endif
		}

		// 预留内存容量
		//（注意：不要对已有数据的对象调用该函数，否则可能造成内存泄漏！！！）
		void Reserve(int count)
		{
			//MemT调用Reserve，其实就是调用了Mem的Reserve函数
			_mem.Reserve(count * sizeof(T));
		}

		//右值类型
		int Add(T&& val)
		{
			int idx = Count();

			//添加一个对象，分配内存，并初始化为零
			T* t = Resize(idx + 1, true);

			//val就是要添加的对象内容，在分配内存后，把val添加到最后一个位置
			t[idx] = std::move(val);
			return idx;
		}

		//左值类型同右值类型的操作
		int Add(const T& val)
		{
			int idx = Count();
			T* t = Resize(idx + 1, true);
			t[idx] = val;
			return idx;
		}

		void Erase(int index, int count = 1)
		{
			typename traits_mem<T>::value_type type;

			_erase(index, count, type);
		}

		void Insert(int index, T&& val)
		{
			int oldcount = Count();
			T* t = Resize(oldcount + 1, true);
			for (int i = oldcount; i > index; i--)
			{
				t[i] = std::move(t[i - 1]);
			}
			t[index] = std::move(val);
		}

		void Insert(int index, const T& val)
		{
			int oldcount = Count();
			T* t = Resize(oldcount + 1, true);
			for (int i = oldcount; i > index; i--)
			{
				t[i] = std::move(t[i - 1]);
			}
			t[index] = val;
		}

	private:
		T* _new(int count, bool zero, BasType)
		{
			T* t = (T*)_mem.Alloc(sizeof(T), count, 1, zero); // 基础数据类型多分配一个单位
			if (t) t[count] = 0; // 将该单位设为0以满足C风格字符串需求
			return t;
		}

		//多参数模板new分配内存
		template<typename... A>
		T* _new(int count, bool, ObjType, A... args)
		{
			int oldcount = Count();
			if (oldcount > 0)
			{
				//把原有的全部析构，然后重新new出来连续的空间
				T* t = (T*)(void*)_mem;
				for (int i = 0; i < oldcount; i++)
					(t + i)->~T();
			}
			T* t = (T*)_mem.Alloc(sizeof(T), count, 0, false);
			if (t)
			{
				for (int i = 0; i < count; i++)
					new ((void*)(t + i)) T{ args... };
			}
			return t;
		}
		//基本类型可以直接删除
		void _delete(BasType)
		{
			if (_mem)
				_mem.Free();
		}
		//对象类型需要析构然后在删除
		void _delete(ObjType)
		{
			if (_mem)
			{
				int oldcount = Count();
				if (oldcount > 0)
				{
					T* t = (T*)(void*)_mem;
					for (int i = 0; i < oldcount; i++)
						(t + i)->~T();
				}
				_mem.Free();
			}
		}

		void _truncate(int count, BasType)
		{
			if (count < Count())
			{
				T* t = (T*)_mem.Resize(sizeof(T), count, 1, false); // 这里的1是为了保持一致的写法，并不必要
				t[count] = 0;
			}
		}

		void _truncate(int count, ObjType)
		{
			int oldcount = Count();
			if (count < oldcount)
			{
				T* t = (T*)(void*)_mem;
				for (int i = count; i < oldcount; i++)
					(t + i)->~T();
				_mem.Resize(sizeof(T), count, 0, false);
			}
		}

		T* _resize(int count, bool grow, BasType)
		{
			if (count != Count())
			{
				T* t = (T*)_mem.Resize(sizeof(T), count, 1, grow);
				if (t) t[count] = 0; // 将该单位设为0以满足C风格字符串需求
			}
			return (T*)(void*)_mem;
		}

		template<typename... A>
		T* _resize(int count, bool grow, ObjType, A... args)
		{
			int oldcount = Count();
			if (count != oldcount)
			{
				if (count < oldcount)
				{
					T* t = (T*)(void*)_mem;
					for (int i = count; i < oldcount; i++)
						(t + i)->~T();
				}
				_mem.Resize(sizeof(T), count, 0, grow);
				if (count > oldcount)
				{
					T* t = (T*)(void*)_mem;
					for (int i = oldcount; i < count; i++)
						new ((void*)(t + i)) T{ args... };
				}
			}
			return (T*)(void*)_mem;
		}

		//基本类型直接将内存缩小就可以了
		void _erase(int index, int count, BasType)
		{
			int oldcount = Count();
			//判断传进来index 的合法性
			if (index >= 0 && index < oldcount && count > 0)
			{
				if (count > oldcount - index)
					count = oldcount - index;
				int newcount = oldcount - count;
				T* t = (T*)(void*)_mem;
				memmove(t + index, t + index + count, (newcount - index + 1) * sizeof(T)); // 注意：这里多移动一个单位(一般是结尾0)
				_mem.Resize(sizeof(T), newcount, 1, false); // 这里的1是为了保持一致的写法，并不必要
			}
		}
		//如果是对象的话需要手动调用析构函数
		void _erase(int index, int count, ObjType)
		{
			int oldcount = Count();
			if (index >= 0 && index < oldcount && count > 0)
			{
				if (count > oldcount - index)
					count = oldcount - index;
				int newcount = oldcount - count;
				T* t = (T*)(void*)_mem;
				for (int i = index; i < index + count; i++)
				{
					(t + i)->~T();
				}
				memmove(t + index, t + index + count, (newcount - index) * sizeof(T));
				_mem.Resize(sizeof(T), newcount, 0, false);
			}
		}

	public:
		T* begin() noexcept
		{
			return (T*)(void*)_mem;
		}

		const T* begin() const noexcept
		{
			return (const T*)(const void*)_mem;
		}

		T* end() noexcept
		{
			return (T*)(void*)_mem + Count();
		}

		const T* end() const noexcept
		{
			return (const T*)(const void*)_mem + Count();
		}

	public:
		// 只在了解原理的特殊场合下使用，一般不要使用！！！
		void Attach(T* ptr)
		{
			_mem.Attach(ptr);
		}
		T* Detach()
		{
			return (T*)_mem.Detach();
		}
	};

	// 字符串
	//（windows平台为本地字符集，linux平台为utf8字符集）
	typedef MemoryT<char>           StringA;

	// 宽字符串
	//（windows平台为2字节宽，linux平台为4字节宽）
	typedef MemoryT<wchar_t>        StringW;

	// 缓冲空间
	typedef MemoryT<uchar>          Buffer;
}


/* @hhoking@ 性能测试

	主要对比测试了使用频率最高的函数：
	MemoryT::Add
	vector::push_back

	首先构造一个结构：

	struct A
	{
		int _a[128] = { 0 };
		A(int a1, int a2) { _a[0] = a1; _a[1] = a2; }
	};

	分别循环一百万次：

	const int count = 1000000;
	MemoryT<A> a1;
	for (int i = 0; i < count; i++)
	{
		a1.Add(A(1, 2));
	}
	std::vector<A> a2;
	for (int i = 0; i < count; i++)
	{
		a2.push_back(A(1, 2));
	}

	耗时：
	a1 : 687 ms
	a2 : 1250 ms

	结论：
	MemoryT 性能高一些，但实际差异可以忽略不计。
*/