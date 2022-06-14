#pragma once
#include "ys_rbase.h"

namespace ys
{
    /* @hhoking@ 缓存类型
        从RArray派生一个专门的缓存类，主要为了特殊的序列化：
        缓存类的值将以base64字符串的方式序列化。这么做是为了性能。
    */
    class RBuffer : public RArray<uchar>
    {
    public:
        RBuffer(const wchar_t* name, RObject* parent) : RArray<uchar>(name, parent) {}

        void Serialize(ValueW& val, DocumentW::AllocatorType& allocator) const
        {
            ASSERT(_name);
            const wchar_t* t = buf_to_base64(_base64, _mem, _mem.Count());
            if (t)
            {
                val.AddMember(StringRef(_name), StringRef(t), allocator);
            }
        }

        bool Deserialize(const ValueW& val)
        {
            ASSERT(_name);
            json_node_string valt;
            if (valt.Find(val, _name))
            {
                base64_to_buf(_mem, valt, valt.Length());
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
            const wchar_t* t = buf_to_base64(_base64, _mem, _mem.Count());
            if (t)
            {
                writer.Key(_name);
                writer.String(t);
            }
        }

    private:
        mutable StringW _base64;
    };
}

#define R_BUFFER(n)         ys::RBuffer       n{ L ### n, this };