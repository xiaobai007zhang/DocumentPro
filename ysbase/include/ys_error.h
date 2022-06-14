#pragma once
#include "cmn.h"
#include "ys_mem.h"
#include "ys_errorcode.h"

namespace ys
{
    class Error;
    class YSBASE_CLASS ErrorStack
    {
    private:
        Error* _error = nullptr;

    public:
        ErrorStack() {}
        ErrorStack(const ErrorStack& es) = delete;
        ErrorStack& operator=(const ErrorStack& o) = delete;
        ErrorStack(ErrorStack&& o) noexcept;
        ErrorStack& operator=(ErrorStack&& o) noexcept;
        ~ErrorStack();

        void AddError(ERRORCODE code, pcchar info, pcchar file, int line, pcchar func);
        bool Print();
        bool GetInfo(StringA& info);
        void Release();
    };
}

// 错误处理的辅助宏（确保宏用在大括号范围内！）
//
#define ADD_ERROR(es, code, info) \
if (es) es->AddError(code, info, __FILE__, __LINE__, __func__)

#define RETURN_CODE_INFO(es, code, info) \
ADD_ERROR(es, code, info); return code

#define RETURN_CODE(es, code) \
ADD_ERROR(es, code, #code); return code

#define UNIMPLEMENT(es) \
RETURN_CODE(es, EC_UNIMPLEMENT)
