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

//�ļ���ȡ��
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
		//����������Ĵ�С��С�ڵ����ģ���ôҲ���˳�
		RAPIDJSON_ASSERT(bufferSize >= 4);
		Read();
	}

	//���ص�ǰ���ַ�
	Ch Peek() const { return *current_; }
	//���ص�ǰ���ַ���Ȼ���Զ���ָ������Ų��һ��
	Ch Take() { Ch c = *current_; Read(); return c; }

	//���ص�ǰ��λ��
	size_t Tell() const { return count_ + static_cast<size_t>(current_ - buffer_); }

	// Not implemented
	void Put(Ch) { RAPIDJSON_ASSERT(false); }
	void Flush() { RAPIDJSON_ASSERT(false); }
	Ch* PutBegin() { RAPIDJSON_ASSERT(false); return 0; }
	size_t PutEnd(Ch*) { RAPIDJSON_ASSERT(false); return 0; }

	// For encoding detection only.
	//��������Ų���ĸ��ֽ��Ƿ����ںϷ���Χ�ڣ�������ڣ��򷵻�0����������0
	const Ch* Peek4() const {
		return (current_ + 4 <= bufferLast_) ? current_ : 0;
	}

private:

	//���û�е�β������ô������Ų��һ���ֽ�
	//����Ѿ���β���ˣ��ж�eof_��־λ�����Ϊfalse��ֱ�Ӷ�ȡ��������С�����ݣ�Ȼ����¸�����Ա����
	void Read() {
		//�ж��Ƿ�β���ˣ����û�е����Ϳ����ƶ�һ���ֽ� 
		if (current_ < bufferLast_)
			++current_;
		else if (!eof_) {
			count_ += readCount_;
			readCount_ = fread(buffer_, 1, bufferSize_, fp_);
			bufferLast_ = buffer_ + readCount_ - 1;
			current_ = buffer_;

			//����Ѿ���ȡ���ֽڸ���С�ڻ�������������ô����ǰ�ڵ�ǰ��β����'\0'
			if (readCount_ < bufferSize_) {
				buffer_[readCount_] = '\0';
				++bufferLast_;
				eof_ = true;
			}
		}
	}

	std::FILE* fp_;	//�ļ���ָ��
	Ch* buffer_;	//������ͷָ��
	size_t bufferSize_; //��������С
	Ch* bufferLast_;	//������βָ��
	Ch* current_;		//��������ǰ��λ��
	size_t readCount_;	//�Ѿ���ȡ���ֽڸ���
	size_t count_;  //!< Number of characters read
	bool eof_;		//�Ƿ����
};

RAPIDJSON_NAMESPACE_END

#ifdef __clang__
RAPIDJSON_DIAG_POP
#endif

#endif // RAPIDJSON_FILESTREAM_H_
