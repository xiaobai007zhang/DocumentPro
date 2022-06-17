#pragma once
#include "dbg.h"
#include "ys_rutil.h"
#include "ys_rtraits.h"
#include "ys_util.h"
#include <stdlib.h>
#define YSBASE_USE_STL
#ifdef YSBASE_USE_STL
#include <sstream>
#include <iostream>
#endif

/* @hhoking@ 通用数据交换框架（General Data Exchange Framework）
	设计要点：
	因为所有类对象的定义都必须自携带json字段名，所以不支持拷贝构造
*/
namespace ys
{
	enum class TraversalStatus
	{
		Continue = 0,   // 继续遍历
		Stop = 1,       // 终止遍历
		StopCurNode = 2 // 终止当前节点遍历
	};

	class ReflexBase;

	//函数指针，返回枚举类型状态（三种状态）
	typedef TraversalStatus(*MY_FUNC)(ReflexBase* rb, const void* param);

	//函数指针，比较两个数据，此刻使用万能指针，代表可以比较不同的数据
	typedef int (*MY_COMPARE)(const void* elem1, const void* elem2);

	// 反射/复杂 基类
	// 理解：所有类的基类，提供了字段，是否序列化，反序列化功能
	class ReflexBase
	{
	protected:
		const wchar_t* _name; // 字段名
		bool _disabled;       // 是否屏蔽当前字段的序列化
		bool _deserialized;   // 是否通过反序列化得到的值
		ReflexBase(const wchar_t* name) : _name(name), _disabled(false), _deserialized(false) {}
		ReflexBase(const ReflexBase& o) = delete;

		//重载赋值运算符，把字段名、屏蔽序列化的标志位
		ReflexBase& operator=(const ReflexBase& o)
		{
			//TRACE 0-5 是不同的处理，涉及到很多宏定义，比较繁琐，暂时不看
			TRACE2(L"[ReflexBase c=] l = %s, r = %s\n", _name, o._name);
			_disabled = o._disabled;
			return *this;
		}
		virtual ~ReflexBase()
		{
			TRACE0(L"[ReflexBase ~]\n");
		}

	public:
		const wchar_t* Name() const { return _name; }
		void SetName(const wchar_t* name) { _name = name; } // 必须传常量字符串！！！

		//判断传入的字段名是否和已有的字段名相等
		bool IsName(const wchar_t* name) const { return (_name && name) ? wcscmp(_name, name) == 0 : false; }

		//设置是否允许序列化
		void SetDisabled(bool disabled) { _disabled = disabled; }

		//返回 允许序列化标志
		bool IsDisabled() const { return _disabled; }

		//返回反序列化的状态，是否允许
		bool IsDeserialized() const { return _deserialized; }

		//序列化处理操作
		virtual void Serialize(ValueW& val, DocumentW::AllocatorType& allocator) const = 0;

		//反序列化操作
		virtual bool Deserialize(const ValueW& val) = 0;

		//类似代理操作，某一个参数为函数指针，然后在该函数体内部使用传进来的函数
		virtual TraversalStatus Traversal(MY_FUNC func, const void* param) { return func(this, param); }
	};

	// 遍历函数示例：将非反序列化得到的字段屏蔽掉
	// 用意：只接收反序列化的数据，如果遇到了非反序列化的数据，那么直接停止遍历
	inline TraversalStatus set_disable(ReflexBase* rb, const void* param)
	{
		if (!rb->IsDeserialized())
		{
			rb->SetDisabled(true);
			return TraversalStatus::StopCurNode;
		}
		return TraversalStatus::Continue;
	}

	// 继承ReflexBase，
	// 理解RObject，Repid Object 快速对象
	class RObject : public ReflexBase
	{
	protected:
		RObject(const wchar_t* name, RObject* parent)
			: ReflexBase(name)
		{
			if (parent)
				//该函数求出另一个对象和本对象在内存中的偏移量
				parent->Register(this);
		}
		RObject(const RObject& o) = delete;
		RObject& operator=(const RObject& o)
		{
			TRACE2(L"[RObject c=] l = %s, r = %s\n", _name, o._name);
			_disabled = o._disabled;
			return *this;
		}

	public:
		//求两个对象之间的偏移量
		void Register(ReflexBase* child)
		{
			//ulint 无符号长整型 8字节
			int offset = (int)(reinterpret_cast<ulint>(child) - reinterpret_cast<ulint>(this));
			_offsets.Add(offset);
		}

	private:
		//数据内存分配模板，它包含Memory类，对内存的管理
		MemoryT<int> _offsets;
		//返回一个偏移量为offset的子类对象，但返回类型是基类，所以用到了多态特征
		ReflexBase* _member(int offset) { return reinterpret_cast<ReflexBase*>(reinterpret_cast<ulint>(this) + offset); }

		//第二种写法，都是只读类型
		const ReflexBase* _member(int offset) const { return reinterpret_cast<const ReflexBase*>(reinterpret_cast<ulint>(this) + offset); }

		//序列化处理
		void _serialize(ValueW& val, DocumentW::AllocatorType& allocator) const
		{
			for (auto it : _offsets)
				//对内存中的每一个元素都要序列化
				_member(it)->Serialize(val, allocator);
		}

		//反序列化处理
		bool _deserialize(const ValueW& val)
		{
			bool re = true;
			for (auto it : _offsets)
				re &= _member(it)->Deserialize(val);
			return re;
		}

	public:

		//序列化用到了分配器
		void Serialize(ValueW& val, DocumentW::AllocatorType& allocator) const
		{
			if (_disabled)
				return;

			if (_name)
			{
				ValueW valt(kObjectType);
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
				json_node_object valt;
				//如果在当前的json对象中找到了，那么就同意反序列化json对象

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

				//否则直接反序列化该元素，而不是json对象
				return _deserialize(val);
			}
		}

		//代理（函数指针）
		TraversalStatus Traversal(MY_FUNC func, const void* param)
		{
			TraversalStatus ts = func(this, param);

			//如果当前的返回类型是Continue的话，才会继续进行序列化处理
			if (ts == TraversalStatus::Continue)
			{
				//offsets是整形类型的类模板
				for (auto it : _offsets)
				{
					ts = _member(it)->Traversal(func, param);

					//直到遇到返回值为Stop
					if (ts == TraversalStatus::Stop)
						break;
				}
			}
			return ts;
		}
	};



	template <typename T>
	class RObjectSAX : public RObject
	{
	private:
		RObjectSAX(const wchar_t* name, RObject* parent)
			: RObject(name, parent) {}
		RObjectSAX(const RObjectSAX& o) = delete;
		RObjectSAX& operator=(const RObjectSAX& o)
		{
			TRACE2(L"[RObjectSAX c=] l = %s, r = %s\n", _name, o._name);
			_disabled = o._disabled;
			return *this;
		}
		friend T;

		template <typename Handler>
		void _serialize_sax(Handler& writer) const
		{
			static_cast<const T*>(this)->_serialize_sax(writer);
		}

	public:
		template <typename Handler>
		void SerializeSAX(Handler& writer) const
		{
			if (_disabled)
				return;

			//字段不为空就继续
			if (_name)
			{
				writer.Key(_name);
				writer.StartObject();
				_serialize_sax(writer);
				writer.EndObject();
			}
			else
			{
				_serialize_sax(writer);
			}
		}
	};

	//========================================================================
	template <typename T>
	class R : public ReflexBase
	{
	private:
		T _value;

	public:
		R(const wchar_t* name, RObject* parent, T value = 0)
			: ReflexBase(name), _value(value)
		{
			ASSERT(_name);               // 必须有名（不能作为根节点和基础数组元素）
			ASSERT(rtraits_elem<T>::ok); // 必须是许可的基础元素类型
			if (parent)
				parent->Register(this);
		}
		R(const R& o) = delete;
		R& operator=(const R& o)
		{
			TRACE2(L"[R c=] l = %s, r = %s\n", _name, o._name);
			_disabled = o._disabled;
			_value = o._value;
			return *this;
		}

		// 基础类型直接传值
		void operator=(T value) { _value = value; }
		operator T() const { return _value; }

		void Serialize(ValueW& val, DocumentW::AllocatorType& allocator) const
		{
			if (_disabled)
				return;

			ASSERT(_name);
			ValueW valVal(_value);
			val.AddMember(StringRef(_name), valVal, allocator);
		}

		bool Deserialize(const ValueW& val)
		{
			ASSERT(_name);
			json_node_<T> valt;
			if (valt.Find(val, _name))
			{
				_value = valt;
				_deserialized = true;
				return true;
			}
			_deserialized = false;
			return false;
		}

		template <typename Handler>
		void SerializeSAX(Handler& writer) const
		{
			if (_disabled)
				return;

			ASSERT(_name);
			writer.Key(_name);
			sax_write(writer, _value);
		}
	};

	template <typename T>
	class RObjArray : public ReflexBase
	{
	protected:
		MemoryT<T> _mem;

	public:
		RObjArray(const wchar_t* name = nullptr, RObject* parent = nullptr)
			: ReflexBase(name)
		{
			if (parent)
				parent->Register(this);
		}
		RObjArray(const RObjArray& o) = delete;
		RObjArray& operator=(const RObjArray& o)
		{
			TRACE2(L"[RObjArray c=] l = %s, r = %s\n", _name, o._name);
			_disabled = o._disabled;
			T* t = _mem.New(o.Count());
			for (int i = 0; i < o.Count(); i++)
			{
				t[i] = o[i];
			}
			return *this;
		}
		RObjArray& operator=(RObjArray&& o) noexcept
		{
			TRACE2(L"[RObjArray m=] l = %s, r = %s\n", _name, o._name);
			_disabled = o._disabled;
			_mem = std::move(o._mem);
			return *this;
		}

		operator T* () { return _mem; }
		operator const T* () const { return _mem; }
		T& operator[](int idx) { return _mem[idx]; }
		const T& operator[](int idx) const { return _mem[idx]; }
		int Count() const { return _mem.Count(); }

		T* New(int count)
		{
			//#ifdef
						// 要求T必须从RObject继承
						//return _mem.New<const wchar_t *, RObject *>(count, nullptr, nullptr);

			//#else
						// Linux不支持上述写法
			return _mem.New(count);
			//#endif
		}

		// 如果不知道New多大空间，那么先New一个足够大的，
		// 最后用该函数截断为正确大小
		void Truncate(int count)
		{
			_mem.Truncate(count);
		}

	public:
		int Add(T&& val) // 添加元素
		{
			return _mem.Add(std::move(val));
		}
		void Erase(int idx) // 删除指定元素
		{
			_mem.Erase(idx, 1);
		}
		void Insert(int idx, T&& val) // 指定位置插入
		{
			_mem.Insert(idx, std::move(val));
		}
		T Detach(int idx) // 分离元素
		{
			T val = std::move(_mem[idx]);
			_mem.Erase(idx, 1);
			return val;
		}

		void Sort(MY_COMPARE compare)
		{
			qsort(_mem, Count(), sizeof(T), compare);
		}

	private:
		void _serialize(ValueW& val, DocumentW::AllocatorType& allocator) const
		{
			ASSERT(val.IsArray());
			const T* t = _mem;
			for (int i = 0; i < Count(); i++)
			{
				ValueW valObj(kObjectType);
				t[i].Serialize(valObj, allocator);
				val.PushBack(valObj, allocator);
			}
		}

		bool _deserialize(const ValueW& val)
		{
			ASSERT(val.IsArray());
			if (val.Size() > 0)
			{
				T* t = New(val.Size());
				for (int i = 0; i < Count(); i++)
				{
					t[i].Deserialize(val[i]);
				}
				return true;
			}
			return false;
		}

	public:
		void Serialize(ValueW& val, DocumentW::AllocatorType& allocator) const
		{
			if (_disabled)
				return;
#ifdef _DEBUG
			constexpr bool ok = is_derived<RObject, T>::ok;
			ASSERT(ok); // T必须从RObject继承，否则不支持序列化
#endif
			if (_name)
			{
				ValueW valArr(kArrayType);
				_serialize(valArr, allocator);
				val.AddMember(StringRef(_name), valArr, allocator);
			}
			else
			{
				_serialize(val, allocator);
			}
		}

		bool Deserialize(const ValueW& val)
		{
#ifdef _DEBUG
			constexpr bool ok = is_derived<RObject, T>::ok;
			ASSERT(ok); // T必须从RObject继承，否则不支持反序列化
#endif
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
				T* t = _mem;
				for (int i = 0; i < Count(); i++)
				{
					ts = t[i].Traversal(func, param);
					if (ts == TraversalStatus::Stop)
						break;
				}
			}
			return ts;
		}

	private:
		template <typename Handler>
		void _serialize_sax(Handler& writer) const
		{
			const T* t = _mem;
			for (int i = 0; i < Count(); i++)
			{
				writer.StartObject();
				t[i].SerializeSAX(writer);
				writer.EndObject();
			}
		}

	public:
		template <typename Handler>
		void SerializeSAX(Handler& writer) const
		{
			if (_disabled)
				return;

			if (_name)
			{
				writer.Key(_name);
				writer.StartArray();
				_serialize_sax(writer);
				writer.EndArray();
			}
			else
			{
				_serialize_sax(writer);
			}
		}
	};


	//=============================================

	template <typename T>
	class RArray : public ReflexBase
	{
	protected:
		MemoryT<T> _mem;

	public:
		RArray(const wchar_t* name, RObject* parent)
			: ReflexBase(name)
		{
			ASSERT(rtraits_array_elem<T>::ok); // 必须是许可的基础数组元素类型
			if (parent)
				parent->Register(this);
		}
		RArray(const RArray& o) = delete;
		RArray& operator=(const RArray& o)
		{
			TRACE2(L"[RArray c=] l = %s, r = %s\n", _name, o._name);
			_disabled = o._disabled;
			Set(o, o.Count());
			return *this;
		}
		RArray& operator=(RArray&& o) noexcept
		{
			TRACE2(L"[RArray m=] l = %s, r = %s\n", _name, o._name);
			_disabled = o._disabled;
			_mem = std::move(o._mem);
			return *this;
		}

		operator T* () { return _mem; }
		operator const T* () const { return _mem; }
		T& operator[](int idx) { return _mem[idx]; }
		const T& operator[](int idx) const { return _mem[idx]; }
		int Count() const { return _mem.Count(); }

		T* New(int count)
		{
			return _mem.New(count);
		}

		void Set(const T* value, int count)
		{
			ASSERT(value);
			memcpy(New(count), value, count * sizeof(T));
		}

		int Add(T val)
		{
			return _mem.Add(val);
		}

		void Serialize(ValueW& val, DocumentW::AllocatorType& allocator) const
		{
			if (_disabled)
				return;

			ASSERT(_name);
			const T* t = _mem;
			ValueW valArr(kArrayType);
			for (int i = 0; i < Count(); i++)
			{
				ValueW valt(t[i]);
				valArr.PushBack(valt, allocator);
			}
			val.AddMember(StringRef(_name), valArr, allocator);
		}

		bool Deserialize(const ValueW& val)
		{
			ASSERT(_name);
			json_node_array valt;
			if (valt.Find(val, _name) && valt.Size() > 0)
			{
				T* t = New(valt.Size());
				for (int i = 0; i < Count(); i++)
				{
					t[i] = json_node_<T>::Get(valt[i]);
				}
				_deserialized = true;
				return true;
			}
			_deserialized = false;
			return false;
		}

		template <typename Handler>
		void SerializeSAX(Handler& writer) const
		{
			if (_disabled)
				return;

			ASSERT(_name);
			const T* t = _mem;
			writer.Key(_name);
			writer.StartArray();
			for (int i = 0; i < Count(); i++)
			{
				sax_write(writer, t[i]);
			}
			writer.EndArray();
		}
	};

	template <typename T, int _count>
	class RFixArray : public ReflexBase
	{
	protected:
		T _value[_count] = { 0 };

	public:
		RFixArray(const wchar_t* name, RObject* parent)
			: ReflexBase(name)
		{
			ASSERT(_name);                     // 必须有名（不能作为根节点和基础数组元素）
			ASSERT(rtraits_array_elem<T>::ok); // 必须是许可的类型
			if (parent)
				parent->Register(this);
		}
		RFixArray(const RFixArray& o) = delete;
		RFixArray& operator=(const RFixArray& o)
		{
			TRACE2(L"[RFixArray c=] l = %s, r = %s\n", _name, o._name);
			_disabled = o._disabled;
			Set(o, o.Count());
			return *this;
		}

		// 数组名会被const限定，必须分开重载
		operator T* () { return _value; }
		operator const T* () const { return _value; }
		T& operator[](int idx) { return _value[idx]; }
		const T& operator[](int idx) const { return _value[idx]; }
		int Count() const { return _count; }

		void Set(const T* value, int count)
		{
			ASSERT(value);
			count = count < _count ? count : _count;
			memcpy(_value, value, count * sizeof(T));
		}

		void Serialize(ValueW& val, DocumentW::AllocatorType& allocator) const
		{
			if (_disabled)
				return;

			ASSERT(_name);
			ValueW valArr(kArrayType);
			for (int i = 0; i < _count; i++)
			{
				ValueW valt(_value[i]);
				valArr.PushBack(valt, allocator);
			}
			val.AddMember(StringRef(_name), valArr, allocator);
		}

		bool Deserialize(const ValueW& val)
		{
			ASSERT(_name);
			json_node_array valt;
			if (valt.Find(val, _name) && valt.Size() == _count)
			{
				for (int i = 0; i < _count; i++)
				{
					_value[i] = json_node_<T>::Get(valt[i]);
				}
				_deserialized = true;
				return true;
			}
			_deserialized = false;
			return false;
		}

		template <typename Handler>
		void SerializeSAX(Handler& writer) const
		{
			if (_disabled)
				return;

			ASSERT(_name);
			writer.Key(_name);
			writer.StartArray();
			for (int i = 0; i < _count; i++)
			{
				sax_write(writer, _value[i]);
			}
			writer.EndArray();
		}
	};

	class RString : public RArray<wchar_t>
	{
	public:
		RString(const wchar_t* name = nullptr, RObject* parent = nullptr, const wchar_t* value = nullptr)
			: RArray<wchar_t>(name, parent)
		{
			if (value)
				Set(value);
		}

		void Set(const wchar_t* value, int len = -1)
		{
			ASSERT(value);
			gen_string(_mem, value, len);
		}

#ifdef YSBASE_USE_STL
		// 注意，这里的string只是当作unicode字符串的容器使用！
		void Set(const std::string& value)
		{
			typedef unsigned short unicode;
			int len = (int)(value.length() / sizeof(unicode));
			const unicode* src = (const unicode*)value.c_str();
			wchar_t* dst = New(len);
			for (int i = 0; i < len; i++)
				dst[i] = src[i];
			dst[len] = 0;
		}
#endif

		// 将字符串清空，这会导致该JSON节点不输出
		void Empty()
		{
			_mem.Delete();
		}

		RString& operator=(StringW&& o) noexcept
		{
			_mem = std::move(o);
			return *this;
		}

		RString& operator=(const StringW& o)
		{
			gen_string(_mem, o);
			return *this;
		}

		void Serialize(ValueW& val, DocumentW::AllocatorType& allocator) const
		{
			if (_disabled)
				return;

			ASSERT(_name);
			const wchar_t* value = _mem;
			if (value)
				val.AddMember(StringRef(_name), StringRef(value), allocator);
		}

		bool Deserialize(const ValueW& val)
		{
			ASSERT(_name);
			json_node_string valt;
			if (valt.Find(val, _name))
			{
				Set(valt, valt.Length());
				_deserialized = true;
				return true;
			}
			_deserialized = false;
			return false;
		}

		template <typename Handler>
		void SerializeSAX(Handler& writer) const
		{
			if (_disabled)
				return;

			ASSERT(_name);
			const wchar_t* value = _mem;
			if (value)
			{
				writer.Key(_name);
				writer.String(value);
			}
		}

	private:
		using RArray::New;
	};

	template <int _count>
	class RFixString : public RFixArray<wchar_t, EXTALIGN2(_count)>
	{
		using ReflexBase::_deserialized;
		using ReflexBase::_disabled;
		using ReflexBase::_name;
		using RFixArray<wchar_t, EXTALIGN2(_count)>::_value;

	public:
		RFixString(const wchar_t* name, RObject* parent, const wchar_t* value = nullptr)
			: RFixArray<wchar_t, EXTALIGN2(_count)>(name, parent)
		{
			if (value)
				Set(value);
		}

		int Count() const { return _count; }

		void Set(const wchar_t* value, int len = -1)
		{
			ASSERT(value);
			if (len == -1)
				len = (int)wcslen(value);
			len = len < _count ? len : _count;
			memcpy(_value, value, len * sizeof(wchar_t));
			_value[len] = 0;
		}

#ifdef YSBASE_USE_STL
		// 注意，这里的string只是当作unicode字符串的容器使用！
		void Set(const std::string& value)
		{
			typedef unsigned short unicode;
			int len = (int)(value.length() / sizeof(unicode));
			len = len < _count ? len : _count;
			const unicode* src = (const unicode*)value.c_str();
			for (int i = 0; i < len; i++)
				_value[i] = src[i];
			_value[len] = 0;
		}
#endif

		void Serialize(ValueW& val, DocumentW::AllocatorType& allocator) const
		{
			if (_disabled)
				return;

			ASSERT(_name);
			val.AddMember(StringRef(_name), StringRef(_value), allocator);
		}

		bool Deserialize(const ValueW& val)
		{
			ASSERT(_name);
			json_node_string valt;
			if (valt.Find(val, _name))
			{
				Set(valt, valt.Length());
				_deserialized = true;
				return true;
			}
			_deserialized = false;
			return false;
		}

		template <typename Handler>
		void SerializeSAX(Handler& writer) const
		{
			if (_disabled)
				return;

			ASSERT(_name);
			writer.Key(_name);
			writer.String(_value);
		}
	};

	class RStringArray : public RObjArray<RString>
	{
	public:
		RStringArray(const wchar_t* name, RObject* parent)
			: RObjArray<RString>(name, parent) {}

		int Add(pcwchar val)
		{
			return RObjArray<RString>::Add(RString(nullptr, nullptr, val));
		}

		void Insert(int idx, pcwchar val)
		{
			RObjArray<RString>::Insert(idx, RString(nullptr, nullptr, val));
		}

		void Serialize(ValueW& val, DocumentW::AllocatorType& allocator) const
		{
			if (_disabled)
				return;

			ASSERT(_name);
			const RString* t = _mem;
			ValueW valArr(kArrayType);
			for (int i = 0; i < Count(); i++)
			{
				valArr.PushBack(StringRef((const wchar_t*)t[i]), allocator);
			}
			val.AddMember(StringRef(_name), valArr, allocator);
		}

		bool Deserialize(const ValueW& val)
		{
			ASSERT(_name);
			json_node_array valt;
			if (valt.Find(val, _name) && valt.Size() > 0)
			{
				RString* t = New(valt.Size());
				for (int i = 0; i < Count(); i++)
				{
					const ValueW& s = valt[i];
					t[i].Set(s.GetString(), s.GetStringLength());
				}
				_deserialized = true;
				return true;
			}
			_deserialized = false;
			return false;
		}

		template <typename Handler>
		void SerializeSAX(Handler& writer) const
		{
			if (_disabled)
				return;

			ASSERT(_name);
			const RString* t = _mem;
			writer.Key(_name);
			writer.StartArray();
			for (int i = 0; i < Count(); i++)
			{
				writer.String(t[i]);
			}
			writer.EndArray();
		}
	};
}

#define R_SAX_STRUCT_BEGIN(cls)                                           \
    struct cls : public ys::RObjectSAX<cls>                               \
    {                                                                     \
        cls(const wchar_t *name = nullptr, ys::RObject *parent = nullptr) \
            : RObjectSAX<cls>(name, parent) {}                            \
        friend ys::RObjectSAX<cls>;

#define R_SAX_STRUCT_END                        \
protected:                                      \
    template <typename Handler>                 \
    void _serialize_sax(Handler &writer) const; \
    }

#define R_SAX_FUNC_BEGIN(cls)                       \
    template <typename Handler>                     \
    void cls::_serialize_sax(Handler &writer) const \
    {

#define R_SAX(n) \
    n.SerializeSAX(writer);

#define R_SAX_FUNC_END \
    }

#define R_STRUCT_BEGIN(cls)                                               \
    struct cls : public ys::RObject                                       \
    {                                                                     \
        cls(const wchar_t *name = nullptr, ys::RObject *parent = nullptr) \
            : RObject(name, parent) {}

#define R_STRUCT_END \
    }

#define R_STRUCT_DERIVE(cls, parentcls)                                   \
    struct cls : public parentcls                                         \
    {                                                                     \
        cls(const wchar_t *name = nullptr, ys::RObject *parent = nullptr) \
            : parentcls(name, parent) {}

#define R_INT(n) ys::R<int> n{L## #n, this};
#define R_UINT(n) ys::R<uint> n{L## #n, this};
#define R_BOOL(n) ys::R<bool> n{L## #n, this};
#define R_DOUBLE(n) ys::R<double> n{L## #n, this};
#define R_OBJARRAY(n, o) ys::RObjArray<o> n{L## #n, this};
#define R_INTARRAY(n) ys::RArray<int> n{L## #n, this};
#define R_UINTARRAY(n) ys::RArray<uint> n{L## #n, this};
#define R_BOOLARRAY(n) ys::RArray<bool> n{L## #n, this};
#define R_DOUBLEARRAY(n) ys::RArray<double> n{L## #n, this};
#define R_FIXINTARRAY(n, c) ys::RFixArray<int, c> n{L## #n, this};
#define R_FIXUINTARRAY(n, c) ys::RFixArray<uint, c> n{L## #n, this};
#define R_FIXBOOLARRAY(n, c) ys::RFixArray<bool, c> n{L## #n, this};
#define R_FIXDOUBLEARRAY(n, c) ys::RFixArray<double, c> n{L## #n, this};
#define R_STRING(n) ys::RString n{L## #n, this};
#define R_FIXSTRING(n, c) ys::RFixString<c> n{L## #n, this};
#define R_STRINGARRAY(n) ys::RStringArray n{L## #n, this};
#define R_OBJ(n, o) o n{L## #n, this};
