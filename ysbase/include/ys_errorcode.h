#pragma once

namespace ys
{
	typedef int ERRORCODE; // 错误码返回类型

	/* @hhoking@ “全局错误码表”维护规则

	1. 所有2.0框架下实现的底层模块均共用“全局错误码表”，并共同维护
	   全局错误码。

	2. 各模块可根据需求自行扩充通用错误码，只能顺序添加，多个模块在同
	   时定义码值发生冲突时需及时协商解决，码值确定后不能再修改。

	3. 如果一些被调用的模块有定义自己的错误码，并需要返回这些错误码时，
	   可占用一个码值段，定义错误码基值。

	4. 如果模块希望返回自己独立的错误码时，也可以占用一个码值段。目前
	   每个码值段都定义为100个。
	*/
	enum ErrorCode
	{
#pragma region 0//通用错误码

		// 成功
		EC_OK = 0,

		// 未定义错误
		EC_ERROR = 1,

		// 无返回结果
		EC_NO_RESULT = 2,

		// 加载动态库失败
		EC_LOAD_DLL_FAILED = 3,

		// 功能未实现
		EC_UNIMPLEMENT = 4,

		// 参数错误
		EC_PARAM_ERROR = 5,

		// 控制选项设置错误
		EC_OPTION_ERROR = 6,

		// 加载模型文件错误
		EC_LOAD_MODEL_FAILED = 7,

		// 模型未初始化
		EC_MODEL_NOT_INIT = 8,

		// 图像数据错误
		EC_IMAGE_ERROR = 9,

		// 内存错误
		EC_MEMORY_ERROR = 10,

		// 接口不存在
		EC_INTERFACE_NOT_EXIST = 11,

		// 模型初始化失败
		EC_MODEL_INIT_FAILED = 12,

		// 平台调用失败
		EC_CLASSIFY_FAILED = 13,

		// 序列化失败
		EC_SERIALIZE_FAILED = 14,

		// 反序列化失败
		EC_DESERIALIZE_FAILED = 15,

		// 非法调用
		EC_INVALID_CALL = 16,

		// 值为空
		EC_VALUE_IS_EMPTY = 17,

		// 注册失败
		EC_REGISTER_FAILED = 18,

		// 加载配置文件失败
		EC_LOAD_CFG_FAILED = 19,

		// 转码失败(包括加解密)
		EC_TRANS_CODE_FAILED = 20,

		// 内核对象错误
		EC_CORE_OBJECT_ERROR = 21,

		// 超出并发限制
		EC_PARALLEL_LIMIT_ERROR = 22,

#pragma endregion

#pragma region 1//加密锁错误码(100)

		// 加密锁错误码基值
		EC_KEY_BASE = 100,

		// 加密锁错误
		EC_KEY_HASP_ERROR = EC_KEY_BASE + 1,

		// 加密锁未初始化
		EC_KEY_NO_INIT = EC_KEY_BASE + 2,

		// 加密锁产品错误
		EC_KEY_PRODUCT_ERROR = EC_KEY_BASE + 3,

		// 加密锁过期
		EC_KEY_OUT_OF_DATE = EC_KEY_BASE + 4,

		// 加密锁系统时间被篡改
		EC_KEY_TIME_ERROR = EC_KEY_BASE + 5,

		// 加密锁只读区错误
		EC_KEY_ROM_ERROR = EC_KEY_BASE + 6,

		// 加密锁没有使用次数了
		EC_KEY_NO_COUNT = EC_KEY_BASE + 7,

#pragma endregion

#pragma region 2//模块授权错误码(200)

		// 模块xx错误码基值
		EC_AUTH_BASE = 200,

		// 文件读写错误
		EC_AUTH_IO_ERROR = EC_AUTH_BASE + 1,

		// 授权过期
		EC_AUTH_OUT_OF_DATE = EC_AUTH_BASE + 2,

		// 系统时间被篡改
		EC_AUTH_TIME_ERROR = EC_AUTH_BASE + 3,

		// 包名错误
		EC_AUTH_PACKAGENAME_ERROR = EC_AUTH_BASE + 4,

		// 哈希错误
		EC_AUTH_HASH_ERROR = EC_AUTH_BASE + 5,

		// PPID错误
		EC_AUTH_PPID_ERROR = EC_AUTH_BASE + 6,

		// FID错误
		EC_AUTH_FID_ERROR = EC_AUTH_BASE + 7,

#pragma endregion

#pragma region 3//模块xx错误码(300)

		// 模块xx错误码基值
		EC_XX3_BASE = 300,

#pragma endregion

#pragma region 4//模块xx错误码(400)

		// 模块xx错误码基值
		EC_XX4_BASE = 400,

#pragma endregion

#pragma region 5//模块xx错误码(500)

		// 模块xx错误码基值
		EC_XX5_BASE = 500,

#pragma endregion

#pragma region 6//模块xx错误码(600)

		// 模块xx错误码基值
		EC_XX6_BASE = 600,

#pragma endregion
	};
}