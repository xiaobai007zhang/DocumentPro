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

#ifndef RAPIDJSON_FILEREADSTREAM_H_
#define RAPIDJSON_FILEREADSTREAM_H_

#include "stream.h"
#include <cstdio>

#ifdef __clang__
RAPIDJSON_DIAG_PUSH
RAPIDJSON_DIAG_OFF(padded)
RAPIDJSON_DIAG_OFF(unreachable - code)
RAPIDJSON_DIAG_OFF(missing - noreturn)
#endif

RAPIDJSON_NAMESPACE_BEGIN

//! File byte stream for input using fread().
/*!
	\note implements Stream concept
*/

//文件读取流
class FileReadStream {
public:
	typedef char Ch;    //!< Character type (byte).

	//! Constructor.
	/*!
		\param fp File pointer opened for read.
		\param buffer user-supplied buffer.
		\param bufferSize size of buffer in bytes. Must >=4 bytes.
	*/
	FileReadStream(std::FILE* fp, char* buffer, size_t bufferSize) : fp_(fp), buffer_(buffer), bufferSize_(bufferSize), bufferLast_(0), current_(buffer_), readCount_(0), count_(0), eof_(false) {
		RAPIDJSON_ASSERT(fp_ != 0);
		//如果缓冲区的大小，小于等于四，那么也会退出
		RAPIDJSON_ASSERT(bufferSize >= 4);
		Read();
	}

	//返回当前的字符
	Ch Peek() const { return *current_; }
	//返回当前的字符，然后自动将指针往后挪动一个
	Ch Take() { Ch c = *current_; Read(); return c; }

	//返回当前的位置
	size_t Tell() const { return count_ + static_cast<size_t>(current_ - buffer_); }

	// Not implemented
	void Put(Ch) { RAPIDJSON_ASSERT(false); }
	void Flush() { RAPIDJSON_ASSERT(false); }
	Ch* PutBegin() { RAPIDJSON_ASSERT(false); return 0; }
	size_t PutEnd(Ch*) { RAPIDJSON_ASSERT(false); return 0; }

	// For encoding detection only.
	//测试往后挪动四个字节是否还是在合法范围内，如果不在，则返回0，如果在则非0
	const Ch* Peek4() const {
		return (current_ + 4 <= bufferLast_) ? current_ : 0;
	}

private:

	//如果没有到尾部，那么就往后挪动一个字节
	//如果已经到尾部了，判断eof_标志位，如果为false，直接读取缓冲区大小个数据，然后更新各个成员变量
	void Read() {
		//判断是否到尾部了，如果没有到，就可以移动一个字节 
		if (current_ < bufferLast_)
			++current_;
		else if (!eof_) {
			count_ += readCount_;
			readCount_ = fread(buffer_, 1, bufferSize_, fp_);
			bufferLast_ = buffer_ + readCount_ - 1;
			current_ = buffer_;

			//如果已经读取的字节个数小于缓冲区个数，那么会提前在当前结尾插入'\0'
			if (readCount_ < bufferSize_) {
				buffer_[readCount_] = '\0';
				++bufferLast_;
				eof_ = true;
			}
		}
	}

	std::FILE* fp_;	//文件流指针
	Ch* buffer_;	//缓冲区头指针
	size_t bufferSize_; //缓冲区大小
	Ch* bufferLast_;	//缓冲区尾指针
	Ch* current_;		//缓冲区当前的位置
	size_t readCount_;	//已经读取的字节个数
	size_t count_;  //!< Number of characters read
	bool eof_;		//是否错误
};

RAPIDJSON_NAMESPACE_END

#ifdef __clang__
RAPIDJSON_DIAG_POP
#endif

#endif // RAPIDJSON_FILESTREAM_H_
