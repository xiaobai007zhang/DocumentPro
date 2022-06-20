#pragma once
#include "ys_rbase.h"

namespace ys
{
	template<typename T>
	class RPtrArray : public ReflexBase
	{
	protected:
		MemoryT<T*> _mem;

	public:
		RPtrArray(const wchar_t* name = nullptr, RObject* parent = nullptr)
			: ReflexBase(name)
		{
			if (parent) parent->Register(this);
		}
		RPtrArray(const RPtrArray& o) = delete;
		RPtrArray& operator=(const RPtrArray& o) = delete;
		RPtrArray& operator=(RPtrArray&& o) noexcept
		{
			TRACE2(L"[RPtrArray m=] l = %s, r = %s\n", _name, o._name);
			_mem = std::move(o._mem);
			return *this;
		}
		~RPtrArray()
		{
			TRACE0(L"[RPtrArray ~]\n");
			for (int i = 0; i < _mem.Count(); i++)
			{
				T* t = _mem[i];
				if (t)
					delete t;
			}
		}

		operator T** () { return _mem; }
		operator const T** () const { return _mem; }
		T*& operator[](int idx) { return _mem[idx]; }
		const T*& operator[](int idx) const { return _mem[idx]; }
		int Count() const { return _mem.Count(); }

		int Add(T*& val) // 添加元素指针（外部不能再操作该指针！！！）
		{
			int idx = _mem.Add(val);
			val = nullptr;
			return idx;
		}
		void Erase(int idx) // 删除指定元素指针（同时释放指针内存）
		{
			T* val = _mem[idx];
			if (val) delete val;
			_mem.Erase(idx, 1);
		}
		void Insert(int idx, T*& val) // 指定位置插入元素指针（外部不能再操作该指针！！！）
		{
			_mem.Insert(idx, val);
			val = nullptr;
		}
		T* Detach(int idx) // 分离元素指针
		{
			T* val = _mem[idx];
			_mem.Erase(idx);
			return val;
		}

		void Sort(MY_COMPARE compare)
		{
			qsort(_mem, Count(), sizeof(T*), compare);
		}

	private:
		void _serialize(ValueW& val, DocumentW::AllocatorType& allocator) const
		{
			ASSERT(val.IsArray());
			for (auto it : _mem)
			{
				ValueW valObj(kObjectType);
				it->Serialize(valObj, allocator);
				val.PushBack(valObj, allocator);
			}
		}

		bool _deserialize(const ValueW& val)
		{
			ASSERT(val.IsArray());
			if (val.Size() > 0)
			{
				for (uint i = 0; i < val.Size(); i++)
				{
					const ValueW& valObj = val[i];
					json_node_<int> valt;
					if (valt.Find(valObj, L"type"))
					{
						T* t = T::New(valt);
						if (t)
						{
							t->Deserialize(valObj);
							_mem.Add(t);
						}
					}
				}
				return true;
			}
			return false;
		}

	public:
		void Serialize(ValueW& val, DocumentW::AllocatorType& allocator) const
		{
			if (_name)
			{
				ValueW valt(kArrayType);
				_serialize(valt, allocator);
				val.AddMember(StringRef(_name), valt, allocator);
			}
			else
			{
				_serialize(val, allocator);
			}
		}

		bool Deserialize(const ValueW& val)
		{
			if (_name)
			{
				json_node_array valt;
				if (valt.Find(val, _name))
				{
					_deserialized = true;
					return _deserialize(valt);
				}
				_deserialized = false;
				return false;
			}
			else
			{
				_deserialized = true;
				return _deserialize(val);
			}
		}

		TraversalStatus Traversal(MY_FUNC func, const void* param)
		{
			TraversalStatus ts = func(this, param);
			if (ts == TraversalStatus::Continue)
			{
				for (auto it : _mem)
				{
					ts = it->Traversal(func, param);
					if (ts == TraversalStatus::Stop)
						break;
				}
			}
			return ts;
		}
	};
}

//跟R_STRUCT_BEGIN宏是一样的，可以看作一个带New的基类，就是多了个静态New函数，需要重新实现静态函数，缺少定义
#define R_STRUCT_BASE(cls) \
struct cls : public ys::RObject \
{ \
    cls(const wchar_t* name = nullptr, ys::RObject* parent = nullptr) \
        : RObject(name, parent){} \
    static cls* New(int type);

//指针数组类型
#define R_PTRARRAY(n,o)          ys::RPtrArray<o>         n{ L ### n, this };
