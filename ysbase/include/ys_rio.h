#pragma once
#include "ioinl.h"
#include "ys_rutil.h"
#include "ys_util.h"
#include "ys_rtypes.h"
#include <wchar.h>

#define PFLAG   32

namespace ys
{
	template<typename T>
	inline bool json_load(pcchar pathname, T& sr)
	{
		//sfile 是一个文件流处理类，包括写入读取等操作
		sfile file;
		if (file.OpenReadA(pathname))
		{
			//准备4k大小的缓冲区
			char buf[4096];
			FileReadStream bis(file, buf, sizeof(buf)); // 包装为文件流
			AutoUTFInputStream<unsigned, FileReadStream> eis(bis); // 自动判断编码

			DocumentW doc;
			if (!doc.ParseStream<PFLAG, AutoUTF<unsigned> >(eis).HasParseError())
			{
				//反序列化处理
				sr.Deserialize(doc);
				return true;
			}
		}
		return false;
	}

	template<typename T>
	inline bool json_parse(pcwchar json, T& sr)
	{
		DocumentW doc;
		//Parse方法如果就一个参数，默认为DocumentW的模板的第一个类型

		if (!doc.Parse<PFLAG>(json).HasParseError())
		{
			sr.Deserialize(doc);
			return true;
		}
		return false;
	}

	template<typename T>
	inline bool json_parse(pcchar json, T& sr)
	{
		DocumentW doc;
		//Parse方法明确指定了第二个参数，那么就用第二个参数作为编码格式要求
		if (!doc.Parse<PFLAG, UTF8<> >(json).HasParseError())
		{
			sr.Deserialize(doc);
			return true;
		}
		return false;
	}

	template<typename T>
	inline bool json_parse(pcchar json, T& sr, pcwchar ptype)
	{
		DocumentW doc;
		if (!doc.Parse<PFLAG, UTF8<> >(json).HasParseError())
		{
			/*
			R_STRUCT_BEGIN(RParamType)
			R_STRING(ptype)
			R_STRUCT_END;
			*/
			RParamType rpt;
			if (rpt.Deserialize(doc))
			{
				//提供两个宽字符串，如果相等才会反序列化到sr对象中
				if (wcscmp(ptype, (pcwchar)rpt.ptype) == 0)
				{
					sr.Deserialize(doc);
					return true;
				}
			}
		}
		return false;
	}

	template<typename T1, typename T2>
	inline bool json_parse(pcchar json, T1& sr1, T2& sr2)
	{
		GenericStringStream<UTF8<> > s(json);
		DocumentW doc;

		if (doc.ParseStream<PFLAG + 8, UTF8<> >(s).HasParseError())
			return false;
		sr1.Deserialize(doc);

		if (doc.ParseStream<PFLAG + 8, UTF8<> >(s).HasParseError())
			return false;
		sr2.Deserialize(doc);

		return true;
	}

	template<typename T>
	inline bool json_parse_insitu(pwchar json, T& sr)
	{
		DocumentW doc;
		if (!doc.ParseInsitu<PFLAG>(json).HasParseError())
		{
			sr.Deserialize(doc);
			return true;
		}
		return false;
	}

	template<typename T, typename Encoding = UTF8<> >
	inline bool json_save(szpath pathname, const T& sr, bool pretty)
	{
		//OutputStream是UTF8格式和FileWriteStream类型
		typedef EncodedOutputStream<Encoding, FileWriteStream> OutputStream;

		sfile file;
		if (file.OpenWrite(pathname))
		{
			char buf[4096];
			FileWriteStream bos(file, buf, sizeof(buf));
			OutputStream eos(bos, true); // 写入 BOM,写到文件指针中。（字符缓冲区 -> 文件指针）

			constexpr bool b = is_template_of<RObjArray, T>::ok;
			constexpr Type t = b ? kArrayType : kObjectType;
			DocumentW doc(t);
			DocumentW::AllocatorType& allocator = doc.GetAllocator();

			//序列化，不使用默认的分配器
			sr.Serialize(doc, allocator);


			//pretty来控制是否设置格式
			/*
			enum PrettyFormatOptions {

				kFormatDefault = 0,         //!< Default pretty formatting.
				kFormatSingleLineArray = 1  //!< Format arrays on a single line.
				};
			*/
			if (pretty)
			{
				PrettyWriter<OutputStream, UTF16<>, Encoding> writer(eos);
				writer.SetFormatOptions(kFormatSingleLineArray);
				return doc.Accept(writer);
			}
			else
			{
				Writer<OutputStream, UTF16<>, Encoding> writer(eos);
				return doc.Accept(writer);
			}
		}
		return false;
	}

	template<typename T, typename Encoding = UTF8<> >
	inline bool json_save(GenericStringBuffer<Encoding>& buffer, const T& sr, bool pretty)
	{
		constexpr bool b = is_template_of<RObjArray, T>::ok;
		constexpr Type t = b ? kArrayType : kObjectType;
		DocumentW doc(t);
		DocumentW::AllocatorType& allocator = doc.GetAllocator();
		sr.Serialize(doc, allocator);

		if (pretty)
		{
			PrettyWriter<GenericStringBuffer<Encoding>, UTF16<>, Encoding> writer(buffer);
			writer.SetFormatOptions(kFormatSingleLineArray);
			return doc.Accept(writer);
		}
		else
		{
			Writer<GenericStringBuffer<Encoding>, UTF16<>, Encoding> writer(buffer);
			return doc.Accept(writer);
		}
		return false;
	}

	template<typename T>
	inline bool json_save(StringA& buffer, const T& sr, bool pretty)
	{
		StringBuffer sb;
		if (json_save(sb, sr, pretty))
		{
			gen_string(buffer, sb.GetString(), (int)sb.GetSize());
			return true;
		}
		return false;
	}

	/* @hhoking@ SAX方式的序列化
		需要自己实现成员函数：
		template<typename Handler>
		void T::_serialize_sax(Handler& writer) const
	*/
	template<typename T, typename Encoding = UTF8<> >
	inline bool json_save_sax(szpath pathname, const T& sr, bool pretty)
	{
		typedef EncodedOutputStream<Encoding, FileWriteStream> OutputStream;

		sfile file;
		if (file.OpenWrite(pathname))
		{
			char buf[4096];
			FileWriteStream bos(file, buf, sizeof(buf));
			OutputStream eos(bos, true);

			constexpr bool b = is_template_of<RObjArray, T>::ok;

			if (pretty)
			{
				PrettyWriter<OutputStream, UTF16<>, Encoding> writer(eos);
				writer.SetFormatOptions(kFormatSingleLineArray);
				if (b)
				{
					writer.StartArray();
					sr.SerializeSAX(writer);
					writer.EndArray();
				}
				else
				{
					writer.StartObject();
					sr.SerializeSAX(writer);
					writer.EndObject();
				}
				return writer.IsComplete();
			}
			else
			{
				Writer<OutputStream, UTF16<>, Encoding> writer(eos);
				if (b)
				{
					writer.StartArray();
					sr.SerializeSAX(writer);
					writer.EndArray();
				}
				else
				{
					writer.StartObject();
					sr.SerializeSAX(writer);
					writer.EndObject();
				}
				return writer.IsComplete();
			}
		}
		return false;
	}

	template<typename T, typename Encoding = UTF8<> >
	inline bool json_save_sax(GenericStringBuffer<Encoding>& buffer, const T& sr, bool pretty)
	{
		constexpr bool b = is_template_of<RObjArray, T>::ok;

		if (pretty)
		{
			PrettyWriter<GenericStringBuffer<Encoding>, UTF16<>, Encoding> writer(buffer);
			writer.SetFormatOptions(kFormatSingleLineArray);
			if (b)
			{
				writer.StartArray();
				sr.SerializeSAX(writer);
				writer.EndArray();
			}
			else
			{
				writer.StartObject();
				sr.SerializeSAX(writer);
				writer.EndObject();
			}
			return writer.IsComplete();
		}
		else
		{
			Writer<GenericStringBuffer<Encoding>, UTF16<>, Encoding> writer(buffer);
			if (b)
			{
				writer.StartArray();
				sr.SerializeSAX(writer);
				writer.EndArray();
			}
			else
			{
				writer.StartObject();
				sr.SerializeSAX(writer);
				writer.EndObject();
			}
			return writer.IsComplete();
		}
	}

	template<typename T>
	inline bool json_save_sax(StringA& buffer, const T& sr, bool pretty)
	{
		StringBuffer sb;
		if (json_save_sax(sb, sr, pretty))
		{
			gen_string(buffer, sb.GetString(), (int)sb.GetSize());
			return true;
		}
		return false;
	}
}