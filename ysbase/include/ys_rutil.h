#pragma once
#include "cmn.h"
#undef max
#undef min
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/encodedstream.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
using namespace rapidjson;

//都设置为宽字节
typedef GenericDocument< UTF16<> > DocumentW;
typedef GenericValue< UTF16<> > ValueW;
typedef GenericStringBuffer< UTF16<> > StringBufferW;
typedef GenericStringStream< UTF16<> > StringStreamW;

namespace ys
{
    /* @hhoking@ 访问json节点的辅助类
        主要节点有：对象(object)、数组(array)、基本数据类型、字符串
    */
    class json_node
    {
    protected:
        //只读成员迭代器
        ValueW::ConstMemberIterator itr;
        virtual bool IsOk() = 0;
    public:
        //pcwchar  const属性的宽字节符
        bool Find(const ValueW& val, pcwchar name)
        {
            itr = val.FindMember(name);
            //只要返回的迭代器不是最后一个代表在当前的字段中找到了name信息

            return (itr != val.MemberEnd()) ? IsOk() : false;
        
        }
    };

    class json_node_object : public json_node
    {
    protected:
        bool IsOk() { return itr->value.IsObject(); }
    public:
        operator const ValueW&() const { return itr->value; }
    };

    class json_node_array : public json_node
    {
    protected:
        bool IsOk() { return itr->value.IsArray(); }
    public:
        operator const ValueW&() const { return itr->value; }
    public:
        int Size() const { return itr->value.Size(); }
        const ValueW& operator[](int idx) const { return itr->value[idx]; }
    };

    template<typename T>
    class json_node_ : public json_node
    {
    protected:
        bool IsOk() { return itr->value.IsInt(); }
    public:
        operator const T() const { return (T)itr->value.GetInt(); }
    public:
        static T Get(const ValueW& val) { return (T)val.GetInt(); }
    };

    template<>
    class json_node_<uint> : public json_node
    {
    protected:
        bool IsOk() { return itr->value.IsUint(); }
    public:
        operator const uint() const { return itr->value.GetUint(); }
    public:
        static uint Get(const ValueW& val) { return val.GetUint(); }
    };

    template<>
    class json_node_<bool> : public json_node
    {
    protected:
        bool IsOk() { return itr->value.IsBool(); }
    public:
        operator const bool() const { return itr->value.GetBool(); }
    public:
        static bool Get(const ValueW& val) { return val.GetBool(); }
    };

    template<>
    class json_node_<double> : public json_node
    {
    protected:
        bool IsOk() { return itr->value.IsDouble(); }
    public:
        operator const double() const { return itr->value.GetDouble(); }
    public:
        static double Get(const ValueW& val) { return val.GetDouble(); }
    };

    template<>
    class json_node_<float> : public json_node
    {
    protected:
        bool IsOk() { return itr->value.IsDouble(); }
    public:
        operator const float() const { return itr->value.GetFloat(); }
    public:
        static float Get(const ValueW& val) { return val.GetFloat(); }
    };

    class json_node_string : public json_node
    {
    protected:
        bool IsOk() { return itr->value.IsString(); }
    public:
        int Length() const { return itr->value.GetStringLength(); }
        operator pcwchar() const { return itr->value.GetString(); }
    };


    /* @hhoking@ json节点数据获取及写入操作
        DOM（get、set）
        SAX（read、write）
        一般来说这时的节点数据都是整型数据（char、uchar、int、uint），
        设计数据结构时要避免浮点数据形式。
    */
    template<typename Handler, typename T>
    inline void sax_write(Handler& writer, T value)
    {
        writer.Int(value);
    }

    template<typename Handler>
    inline void sax_write(Handler& writer, uint value)
    {
        writer.Uint(value);
    }

    template<typename Handler>
    inline void sax_write(Handler& writer, bool value)
    {
        writer.Bool(value);
    }

    template<typename Handler>
    inline void sax_write(Handler& writer, float value)
    {
        writer.Double(value);
    }

    template<typename Handler>
    inline void sax_write(Handler& writer, double value)
    {
        writer.Double(value);
    }


    inline bool utf16to8(StringBuffer& dst, pcwchar src, bool append = false)
    {
        if (!append) dst.Clear();
        StringStreamW ss(src);
        bool re = true;
        while (ss.Peek() != 0)
        {
            if (!Transcoder<UTF16<>, UTF8<> >::Transcode(ss, dst))
            {
                re = false;
                break;
            }
        }
        return re;
    }

    inline bool utf8to16(StringBufferW& dst, pcchar src, bool append = false)
    {
        if (!append) dst.Clear();
        StringStream ss(src);
        bool re = true;
        while (ss.Peek() != 0)
        {
            if (!Transcoder<UTF8<>, UTF16<> >::Transcode(ss, dst))
            {
                re = false;
                break;
            }
        }
        return re;
    }
}
