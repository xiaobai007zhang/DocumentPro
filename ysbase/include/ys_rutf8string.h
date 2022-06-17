#pragma once
#include "ys_rbase.h"

namespace ys
{
    /* @hhoking@ utf8类型字符串
        一般来说所有用到字符串的地方尽量使用wchar_t（unicode），这里主要
        为了一些明确要求utf8类型字符串的地方
    */
    class RUTF8String : public RArray<char>
    {
    public:
        RUTF8String(const wchar_t* name = nullptr, RObject* parent = nullptr)
            : RArray<char>(name, parent) {}

        void Set(const char* value, int len = -1)
        {
            ASSERT(value);
            gen_string(_mem, value, len);
        }

        void Set(const wchar_t* value, int len = -1)
        {
            ASSERT(value);
            utf16to8(_mem, value, len);
        }

        RUTF8String& operator=(StringA&& o) noexcept
        {
            _mem = std::move(o);
            return *this;
        }

        RUTF8String& operator=(const StringA& o)
        {
            gen_string(_mem, o);
            return *this;
        }

        const StringBufferW& Get() const
        {
            utf8to16(_wbuf, _mem);
            return _wbuf;
        }

        void Serialize(ValueW& val, DocumentW::AllocatorType& allocator) const
        {
            ASSERT(_name);
            if (utf8to16(_wbuf, _mem))
            {
                val.AddMember(StringRef(_name), StringRef(_wbuf.GetString(), _wbuf.GetSize()/sizeof(wchar_t)), allocator);
            }
        }

        bool Deserialize(const ValueW& val)
        {
            ASSERT(_name);
            json_node_string valt;
            if (valt.Find(val, _name))
            {
                utf16to8(_mem, valt, valt.Length());
                _deserialized = true;
                return true;
            }
            _deserialized = false;
            return false;
        }

        template<typename Handler>
        void SerializeSAX(Handler& writer) const
        {
            ASSERT(_name);
            if (utf8to16(_wbuf, _mem))
            {
                writer.Key(_name);
                writer.String(_wbuf.GetString(), (SizeType)(_wbuf.GetSize()/sizeof(wchar_t)));
            }
        }

    private:
        mutable StringBufferW _wbuf;
        using RArray::New;
    };

    class RUTF8StringArray : public RObjArray<RUTF8String>
    {
    public:
        RUTF8StringArray(const wchar_t* name, RObject* parent)
            : RObjArray<RUTF8String>(name, parent) {}

        void Serialize(ValueW& val, DocumentW::AllocatorType& allocator) const
        {
            ASSERT(_name);
            const RUTF8String* t = _mem;
            ValueW valArr(kArrayType);
            for (int i = 0; i < Count(); i++)
            {
                const StringBufferW& sb = t[i].Get();
                valArr.PushBack(StringRef(sb.GetString(), sb.GetSize()/sizeof(wchar_t)), allocator);
            }
            val.AddMember(StringRef(_name), valArr, allocator);
        }

        bool Deserialize(const ValueW& val)
        {
            ASSERT(_name);
            json_node_array valt;
            if (valt.Find(val, _name) && valt.Size() > 0)
            {
                RUTF8String* t = New(valt.Size());
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

        template<typename Handler>
        void SerializeSAX(Handler& writer) const
        {
            ASSERT(_name);
            const RUTF8String* t = _mem;
            writer.Key(_name);
            writer.StartArray();
            for (int i = 0; i < Count(); i++)
            {
                const StringBufferW& sb = t[i].Get();
                writer.String(sb.GetString(), (SizeType)(sb.GetSize()/sizeof(wchar_t)));
            }
            writer.EndArray();
        }
    };
}

#define R_UTF8STRING(n)         ys::RUTF8String         n{ L ### n, this };
#define R_UTF8STRINGARRAY(n)    ys::RUTF8StringArray    n{ L ### n, this };
