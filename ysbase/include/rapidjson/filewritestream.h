// Tencent is pleased to support the open source community by making RapidJSON available.
// 
// Copyright (C) 2015 THL A29 Limited, a Tencent company, and Milo Yip. All rights reserved.
//
// Licensed under the MIT License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// http://opensource.org/licenses/MIT
//
// Unless required by applicable law or agreed to in writing, software distributed 
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
// CONDITIONS OF ANY KIND, either express or implied. See the License for the 
// specific language governing permissions and limitations under the License.

#ifndef RAPIDJSON_FILEWRITESTREAM_H_
#define RAPIDJSON_FILEWRITESTREAM_H_

#include "stream.h"
#include <cstdio>

#ifdef __clang__
RAPIDJSON_DIAG_PUSH
RAPIDJSON_DIAG_OFF(unreachable - code)
#endif

RAPIDJSON_NAMESPACE_BEGIN

//! Wrapper of C file stream for input using fread().
/*!
	\note implements Stream concept
*/
class FileWriteStream {
public:
	typedef char Ch;    //!< Character type. Only support char.

	FileWriteStream(std::FILE* fp, char* buffer, size_t bufferSize) : fp_(fp), buffer_(buffer), bufferEnd_(buffer + bufferSize), current_(buffer_) {
		RAPIDJSON_ASSERT(fp_ != 0);
	}

	//如果当前的位置已经超出了缓冲区大小了，那么直接刷新缓冲区。
	//否则将char c加入到缓冲区中，添加新数据时，会把旧数据全部flush
	void Put(char c) {
		if (current_ >= bufferEnd_)
			Flush();

		*current_++ = c;
	}

	//打印n个字符c
	void PutN(char c, size_t n) {

		//avail 是空闲的空间
		size_t avail = static_cast<size_t>(bufferEnd_ - current_);

		//当前需要插入的个数n 大于空闲个数，那么只会插入avail个char c，并且刷新缓冲区
		while (n > avail) {
			std::memset(current_, c, avail);
			current_ += avail;
			Flush();
			n -= avail;
			avail = static_cast<size_t>(bufferEnd_ - current_);

		}

		//如果需要插入的字符个数小于空闲个数，并且大于零，那么就调用memset来操作内存
		if (n > 0) {
			std::memset(current_, c, n);
			current_ += n;
		}
	}

	void Flush() {
		//如果当前缓冲区 位置 不等于 起始位置，就代表缓冲区已经有数据了，可以写到文件指针里
		if (current_ != buffer_) {
			size_t result = fwrite(buffer_, 1, static_cast<size_t>(current_ - buffer_), fp_);

			//result 是写入的字符数，判断是否将缓冲区所有的字节都写到文件指针中了

			if (result < static_cast<size_t>(current_ - buffer_)) {
				// failure deliberately ignored at this time
				// added to avoid warn_unused_result build errors
			}
			//写完缓冲区数据后，将缓冲区指针重新指向首地址
			current_ = buffer_;
		}
	}

	// Not implemented
	char Peek() const { RAPIDJSON_ASSERT(false); return 0; }
	char Take() { RAPIDJSON_ASSERT(false); return 0; }
	size_t Tell() const { RAPIDJSON_ASSERT(false); return 0; }
	char* PutBegin() { RAPIDJSON_ASSERT(false); return 0; }
	size_t PutEnd(char*) { RAPIDJSON_ASSERT(false); return 0; }

private:
	// Prohibit copy constructor & assignment operator.
	FileWriteStream(const FileWriteStream&);
	FileWriteStream& operator=(const FileWriteStream&);

	std::FILE* fp_;
	char* buffer_;
	char* bufferEnd_;
	char* current_;
};

//! Implement specialized version of PutN() with memset() for better performance.
template<>
inline void PutN(FileWriteStream& stream, char c, size_t n) {
	stream.PutN(c, n);
}

RAPIDJSON_NAMESPACE_END

#ifdef __clang__
RAPIDJSON_DIAG_POP
#endif

#endif // RAPIDJSON_FILESTREAM_H_
