#pragma once
#include "ys_rbase.h"

namespace ys
{
    /* @hhoking@ json节点提取
      1.有些时候json节点不能按照一个统一的结构解析，那么将这个节点的
        字符串直接提取出来。
      2.RNode1、RNode2 是相同结果的不同实现。
      3.RNode 是添加规则的实现，只支持节点类型是字符串和对象的情况，
        且不支持序列化。
    */
    class RNode1 : public ReflexBase
    {
    private:
        StringBuffer _sb;

    public:
        RNode1(const wchar_t* name, RObject* parent)
            : ReflexBase(name)
        {
            ASSERT(_name); // 必须有名（不能作为根节点和基础数组元素）
            if (parent) parent->Register(this);
        }

    public:
        void Serialize(ValueW& val, DocumentW::AllocatorType& allocator) const
        {
            ASSERT(_name);
            DocumentW node;
            if (!node.Parse<0, UTF8<> >(_sb.GetString()).HasParseError())
            {
                ValueW valVal(node, allocator);
                val.AddMember(StringRef(_name), valVal, allocator);
            }
        }

        bool Deserialize(const ValueW& val)
        {
            ASSERT(_name);
            ValueW::ConstMemberIterator itr = val.FindMember(_name);
            if (itr != val.MemberEnd() && !itr->value.IsNull())
            {
                _sb.Clear();
                Writer<StringBuffer, UTF16<>, UTF8<> > writer(_sb);
                _deserialized = true;
                return itr->value.Accept(writer);
            }
            _deserialized = false;
            return false;
        }

        pcchar GetString() const
        {// UTF8字符串
            return _sb.GetString();
        }
    };

    class RNode2 : public ReflexBase
    {
    private:
        DocumentW _node;

    public:
        RNode2(const wchar_t* name, RObject* parent)
            : ReflexBase(name)
        {
            ASSERT(_name); // 必须有名（不能作为根节点和基础数组元素）
            if (parent) parent->Register(this);
        }

    public:
        void Serialize(ValueW& val, DocumentW::AllocatorType& allocator) const
        {
            ASSERT(_name);
            if (!_node.IsNull())
            {
                ValueW valVal(_node, allocator);
                val.AddMember(StringRef(_name), valVal, allocator);
            }
        }

        bool Deserialize(const ValueW& val)
        {
            ASSERT(_name);
            ValueW::ConstMemberIterator itr = val.FindMember(_name);
            if (itr != val.MemberEnd() && !itr->value.IsNull())
            {
                _node.CopyFrom(itr->value, _node.GetAllocator());
                _deserialized = true;
                return true;
            }
            _deserialized = false;
            return false;
        }

        bool GetString(StringBuffer& sb) const
        {// UTF8字符串
            if (!_node.IsNull())
            {
                Writer<StringBuffer, UTF16<>, UTF8<> > writer(sb);
                return _node.Accept(writer);
            }
            return false;
        }
    };

    class RNode : public ReflexBase
    {
    private:
        StringBuffer    _sb;

    public:
        RNode(const wchar_t* name, RObject* parent)
            : ReflexBase(name)
        {
            ASSERT(_name); // 必须有名（不能作为根节点和基础数组元素）
            if (parent) parent->Register(this);
        }

    public:
        void Serialize(ValueW& val, DocumentW::AllocatorType& allocator) const
        {
            ASSERT(0); // 不支持
        }

        bool Deserialize(const ValueW& val)
        {
            ASSERT(_name);
            ValueW::ConstMemberIterator itr = val.FindMember(_name);
            if (itr != val.MemberEnd())
            {
                _deserialized = true;
                _sb.Clear();
                if (itr->value.IsString())
                {
                    return utf16to8(_sb, itr->value.GetString());
                }
                if (itr->value.IsObject())
                {
                    Writer<StringBuffer, UTF16<>, UTF8<> > writer(_sb);
                    return itr->value.Accept(writer);
                }
            }
            _deserialized = false;
            return false;
        }

        pcchar GetString() const
        {// UTF8字符串
            return _sb.GetString();
        }
    };
}

#define R_NODE1(n)      ys::RNode1          n{ L ### n, this };
#define R_NODE2(n)      ys::RNode2          n{ L ### n, this };
#define R_NODE(n)       ys::RNode           n{ L ### n, this };
