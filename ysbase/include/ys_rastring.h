#pragma once
#include "ys_rbase.h"

namespace ys
{
    /* @hhoking@ char类型字符串
        一般来说所有用到字符串的地方尽量使用wchar_t（unicode），这里主要
        为了兼容一些使用char类型字符串的地方，这些地方在windows平台上表示
        本地字符集，在linux平台上表示utf8字符集
    */
    class RAString : public RArray<char>
    {
    public:
        RAString(const wchar_t* name = nullptr, RObject* parent = nullptr)
            : RArray<char>(name, parent) {}

        void Set(const char* value, int len = -1)
        {
            ASSERT(value);
            gen_string(_mem, value, len);
        }

        RAString& operator=(StringA&& o) noexcept
        {
            _mem = std::move(o);
            return *this;
        }

        RAString& operator=(const StringA& o)
        {
            gen_string(_mem, o);
            return *this;
        }

        void Serialize(ValueW& val, DocumentW::AllocatorType& allocator) const
        {
            ASSERT(_name);
            const wchar_t* value = atow(_wstr, _mem);
            if (value)
            {
                val.AddMember(StringRef(_name), StringRef(value), allocator);
            }
        }

        bool Deserialize(const ValueW& val)
        {
            ASSERT(_name);
            json_node_string valt;
            if (valt.Find(val, _name))
            {
                wtoa(_mem, valt);
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
            const wchar_t* value = atow(_wstr, _mem);
            if (value)
            {
                writer.Key(_name);
                writer.String(value);
            }
        }

    private:
        mutable StringW _wstr;
        using RArray::New;
    };
}

#define R_ASTRING(n)                 ys::RAString               n{ L ### n, this };