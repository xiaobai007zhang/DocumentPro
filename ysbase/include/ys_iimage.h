#pragma once
#include "ys_mem.h"
#include "ys_ibase.h"
typedef struct tagRECT RECT;

namespace ys
{
	// 图像数据结构
	struct IMAGEDATA
	{
		// 图像数据首地址
		const unsigned char* scan0 = 0;
		// 图像宽
		int width = 0;
		// 图像高
		int height = 0;
		// 图像步长
		int stride = 0;
		// 图像通道数
		int channel = 0;
		// 图像通道位深
		int depth = 0;
		// 保留参数
		int flag = 0;
	};

	// 图像数据接口
	interface IImageData : public IBase
	{
		// 是否存在有效图像数据
		virtual bool IsValid() const = 0;

		// 获取图像数据
		//（注意：如果组件内图像数据发生改变，则获取的数据不再有效，需要重新获取）
		virtual bool Get(IMAGEDATA& data, const RECT* rect = NULL) const = 0;

		// 设置图像数据（将外部数据复制到组件内）
		virtual bool Set(const IMAGEDATA& data, const RECT* rect = NULL) = 0;

		// 引用图像数据（一般可用作将图像数据包装成IImageData指针）
		//（注意：调用者需要自己保证被引用的图像数据的生命周期）
		virtual bool Ref(const IMAGEDATA& data, const RECT* rect = NULL) = 0;

		// 获取图像dpi
		virtual int GetDPI() const = 0;

		// 设置图像dpi
		virtual void SetDPI(int dpi) = 0;
	};

	// 这个类型表示该值当作常量看待直接使用
	typedef ICONST IImageData* IImageDataCPtr;

	// 图像数据来源接口
	interface IImageSource : public IBase
	{
		// 加载BGR格式的图像
		virtual bool LoadBGR(pcchar pathname) = 0;

		// 加载Grey格式的图像
		virtual bool LoadGrey(pcchar pathname) = 0;

		// 保存图像（根据扩展名来确定保存格式，默认保存为PNG）
		virtual bool Save(pcchar pathname) = 0;
	};

	// 图像数据来源接口2
	interface IImageSource2 : public IBase
	{
		// 加载BGR格式的图像
		virtual bool LoadBGRFromBuffer(pcuchar buf, int size) = 0;

		// 加载Grey格式的图像
		virtual bool LoadGreyFromBuffer(pcuchar buf, int size) = 0;

		// 保存图像（根据ext来确定保存格式，默认保存为PNG）
		virtual bool SaveToBuffer(Buffer& buf, pcchar ext = ".png") = 0;
	};

	// 图像数组数据接口
	interface IImageArray : public IBase
	{
		// 获取数组图像个数
		virtual int Count() const = 0;

		// 获取图像
		virtual IImageData* Get(int idx) = 0;

		// 添加图像（bref明确要求用引用的方式添加，某些实现可能要保证src的生命周期）
		virtual void Add(IImageDataCPtr src, bool bref = true) = 0;

		// 获取指定图像数据
		virtual bool GetData(IMAGEDATA& data, int idx) const = 0;
	};

	// 这个类型表示该值当作常量看待直接使用
	typedef ICONST IImageArray* IImageArrayCPtr;

	// 图像解析函数指针定义
	typedef IImageData* (*IMG_PARSE)(puchar data, int len);

	// 图像数组解析函数指针定义
	typedef IImageArray* (*IMGS_PARSE)(puchar data, int len);
}