#pragma once
#include "cmn.h"
#include "ys_mem.h"

namespace ys
{
    // 本地字符串转宽字符串（失败返回nullptr）
    YSBASE_CLASS pcwchar atow(StringW& wstr, pcchar str, bool append = false);
    inline StringW atow(pcchar str) { StringW wstr; atow(wstr, str); return wstr; }

    // 宽字符串转本地字符串（失败返回nullptr）
    YSBASE_CLASS pcchar wtoa(StringA& str, pcwchar wstr, bool append = false);
    inline StringA wtoa(pcwchar wstr) { StringA str; wtoa(str, wstr); return str; }

    // base64编码转buffer（失败返回nullptr）
    YSBASE_CLASS pcuchar base64_to_buf(Buffer& buf, pcchar base, int len = -1);
    YSBASE_CLASS pcuchar base64_to_buf(Buffer& buf, pcwchar base, int len = -1);
    inline Buffer base64_to_buf(pcchar base, int len = -1) { Buffer buf; base64_to_buf(buf, base, len); return buf; }
    inline Buffer base64_to_buf(pcwchar base, int len = -1) { Buffer buf; base64_to_buf(buf, base, len); return buf; }

    // buffer转base64编码（失败返回nullptr）
    YSBASE_CLASS pcchar buf_to_base64(StringA& base, pcuchar buf, int len);
    YSBASE_CLASS pcwchar buf_to_base64(StringW& base, pcuchar buf, int len);
    inline StringA buf_to_base64a(pcuchar buf, int len) { StringA base; buf_to_base64(base, buf, len); return base; }
    inline StringW buf_to_base64w(pcuchar buf, int len) { StringW base; buf_to_base64(base, buf, len); return base; }

    // 创建字符串（失败返回nullptr）
    YSBASE_CLASS pcchar gen_string(StringA& o, pcchar sz, int len = -1);
    YSBASE_CLASS pcwchar gen_string(StringW& o, pcwchar sz, int len = -1);
    inline pcchar gen_string(StringA& o, const StringA& src) { return gen_string(o, src, src.Count()); }
    inline pcwchar gen_string(StringW& o, const StringW& src) { return gen_string(o, src, src.Count()); }
    inline StringA gen_string(pcchar sz, int len = -1) { StringA o; gen_string(o, sz, len); return o; }
    inline StringW gen_string(pcwchar sz, int len = -1) { StringW o; gen_string(o, sz, len); return o; }
    inline StringA gen_string(const StringA& src) { StringA o; gen_string(o, src); return o; }
    inline StringW gen_string(const StringW& src) { StringW o; gen_string(o, src); return o; }
    YSBASE_CLASS pcchar gen_string(StringA& o, char ch);
    YSBASE_CLASS pcwchar gen_string(StringW& o, wch ch);
    inline StringA gen_string(char ch) { StringA o; gen_string(o, ch); return o; }
    inline StringW gen_string(wch ch) { StringW o; gen_string(o, ch); return o; }

    // 连接字符串
    YSBASE_CLASS pcchar cat_string(StringA& o, pcchar sz, int len = -1);
    YSBASE_CLASS pcwchar cat_string(StringW& o, pcwchar sz, int len = -1);
    inline pcchar cat_string(StringA& o1, const StringA& o2) { return cat_string(o1, o2, o2.Count()); }
    inline pcwchar cat_string(StringW& o1, const StringW& o2) { return cat_string(o1, o2, o2.Count()); }
    YSBASE_CLASS pcchar cat_string(StringA& o, char ch);
    YSBASE_CLASS pcwchar cat_string(StringW& o, wch ch);

    // 连加重载
    YSBASE_CLASS StringA& operator+=(StringA& o, pcchar sz);
    YSBASE_CLASS StringW& operator+=(StringW& o, pcwchar sz);

    // 加号重载
    YSBASE_CLASS StringA operator+(const StringA& s1, const StringA& s2);
    YSBASE_CLASS StringW operator+(const StringW& s1, const StringW& s2);
    YSBASE_CLASS StringA operator+(const StringA& s1, pcchar s2);
    YSBASE_CLASS StringW operator+(const StringW& s1, pcwchar s2);
    YSBASE_CLASS StringA operator+(pcchar s1, const StringA& s2);
    YSBASE_CLASS StringW operator+(pcwchar s1, const StringW& s2);
    YSBASE_CLASS StringA operator+(const StringA& s1, char s2);
    YSBASE_CLASS StringW operator+(const StringW& s1, wch s2);
    YSBASE_CLASS StringA operator+(char s1, const StringA& s2);
    YSBASE_CLASS StringW operator+(wch s1, const StringW& s2);

    // 等号重载
    YSBASE_CLASS bool operator==(const StringA& s1, const StringA& s2);
    YSBASE_CLASS bool operator==(const StringW& s1, const StringW& s2);
    YSBASE_CLASS bool operator==(const StringA& s1, pcchar s2);
    YSBASE_CLASS bool operator==(const StringW& s1, pcwchar s2);
    YSBASE_CLASS bool operator==(pcchar s1, const StringA& s2);
    YSBASE_CLASS bool operator==(pcwchar s1, const StringW& s2);

    // 不等号重载
    YSBASE_CLASS bool operator!=(const StringA& s1, const StringA& s2);
    YSBASE_CLASS bool operator!=(const StringW& s1, const StringW& s2);
    YSBASE_CLASS bool operator!=(const StringA& s1, pcchar s2);
    YSBASE_CLASS bool operator!=(const StringW& s1, pcwchar s2);
    YSBASE_CLASS bool operator!=(pcchar s1, const StringA& s2);
    YSBASE_CLASS bool operator!=(pcwchar s1, const StringW& s2);

    // 生成子字符串（失败返回nullptr）（len，count可以为-1）
    YSBASE_CLASS pcchar mid_string(StringA& o, pcchar sz, int len, int first, int count);
    YSBASE_CLASS pcwchar mid_string(StringW& o, pcwchar sz, int len, int first, int count);
    inline pcchar mid_string(StringA& o, const StringA& src, int first, int count) { return mid_string(o, src, src.Count(), first, count); }
    inline pcwchar mid_string(StringW& o, const StringW& src, int first, int count) { return mid_string(o, src, src.Count(), first, count); }
    inline StringA mid_string(pcchar sz, int len, int first, int count) { StringA o; mid_string(o, sz, len, first, count); return o; }
    inline StringW mid_string(pcwchar sz, int len, int first, int count) { StringW o; mid_string(o, sz, len, first, count); return o; }
    inline StringA mid_string(const StringA& src, int first, int count) { return mid_string(src, src.Count(), first, count); }
    inline StringW mid_string(const StringW& src, int first, int count) { return mid_string(src, src.Count(), first, count); }

    // 获取首字符串
    inline StringA left_string(const StringA& src, int count) { return mid_string(src, 0, count); }
    inline StringW left_string(const StringW& src, int count) { return mid_string(src, 0, count); }

    // 获取尾字符串
    inline StringA right_string(const StringA& src, int count) { return mid_string(src, src.Count() - count, count); }
    inline StringW right_string(const StringW& src, int count) { return mid_string(src, src.Count() - count, count); }

    // 查找字符（找不到返回-1）
    YSBASE_CLASS int find_string(const StringA& src, char ch, int start = 0);
    YSBASE_CLASS int find_string(const StringW& src, wch ch, int start = 0);

    // 查找字符串（找不到返回-1）
    YSBASE_CLASS int find_string(const StringA& src, pcchar sz, int start = 0);
    YSBASE_CLASS int find_string(const StringW& src, pcwchar sz, int start = 0);

    // 倒序查找字符（找不到返回-1）
    YSBASE_CLASS int rfind_string(const StringA& src, char ch);
    YSBASE_CLASS int rfind_string(const StringW& src, wch ch);

    // 查找字符串中的一个字符（找不到返回-1）
    YSBASE_CLASS int findoneof_string(const StringA& src, pcchar sz, int start = 0);
    YSBASE_CLASS int findoneof_string(const StringW& src, pcwchar sz, int start = 0);

    // 倒序查找字符串中的一个字符（找不到返回-1）
    YSBASE_CLASS int rfindoneof_string(const StringA& src, pcchar sz);
    YSBASE_CLASS int rfindoneof_string(const StringW& src, pcwchar sz);

    // 替换字符（返回成功替换的个数）
    YSBASE_CLASS int replace_string(StringA& o, char chold, char chnew);
    YSBASE_CLASS int replace_string(StringW& o, wch chold, wch chnew);

    // 替换字符串（返回成功替换的个数）
    YSBASE_CLASS int replace_string(StringA& o, pcchar szold, pcchar sznew);
    YSBASE_CLASS int replace_string(StringW& o, pcwchar szold, pcwchar sznew);

    // 替换指定段字符串
    YSBASE_CLASS bool replace_string(StringA& o, int idx, int count, pcchar sznew);
    YSBASE_CLASS bool replace_string(StringW& o, int idx, int count, pcwchar sznew);

    // 拆分字符串（len可以为-1）
    YSBASE_CLASS bool split_string(MemoryT<StringA>& arr, pcchar sz, int len, pcchar seps);
    YSBASE_CLASS bool split_string(MemoryT<StringW>& arr, pcwchar sz, int len, pcwchar seps);
    inline bool split_string(MemoryT<StringA>& arr, const StringA& src, pcchar seps) { return split_string(arr, src, src.Count(), seps); }
    inline bool split_string(MemoryT<StringW>& arr, const StringW& src, pcwchar seps) { return split_string(arr, src, src.Count(), seps); }

    // 格式化字符串
    YSBASE_CLASS void CC_C format_string(StringA& o, pcchar szformat, ...);
    YSBASE_CLASS void CC_C format_string(StringW& o, pcwchar szformat, ...);

    // 解析字符串
    YSBASE_CLASS int CC_C parse_string(pcchar sz, pcchar szformat, ...);
    YSBASE_CLASS int CC_C parse_string(pcwchar sz, pcwchar szformat, ...);

    // 合并路径
    YSBASE_CLASS pcchar combine_path(StringA& path, pcchar p1, pcchar p2);
    YSBASE_CLASS pcwchar combine_path(StringW& path, pcwchar p1, pcwchar p2);
    inline StringA combine_path(pcchar p1, pcchar p2) { StringA o; combine_path(o, p1, p2); return o; }
    inline StringW combine_path(pcwchar p1, pcwchar p2) { StringW o; combine_path(o, p1, p2); return o; }

    // 提取路径（如"C:\myPath\myFile.txt",得到"C:\myPath\"）
    YSBASE_CLASS pcchar get_path(StringA& path, pcchar pathname);
    YSBASE_CLASS pcwchar get_path(StringW& path, pcwchar pathname);
    inline StringA get_path(pcchar pathname) { StringA o; get_path(o, pathname); return o; }
    inline StringW get_path(pcwchar pathname) { StringW o; get_path(o, pathname); return o; }

    // UNICODE字符编码转换
    YSBASE_CLASS pcchar utf16to8(StringA& utf8, pcwchar utf16, int len = -1);
    inline StringA utf16to8(pcwchar utf16, int len = -1) { StringA o; utf16to8(o, utf16, len); return o; }
    inline StringA utf16to8(const StringW& utf16) { StringA o; utf16to8(o, utf16, utf16.Count()); return o; }

    // UNICODE字符编码转换
    YSBASE_CLASS pcwchar utf8to16(StringW& utf16, pcchar utf8, int len = -1);
    inline StringW utf8to16(pcchar utf8, int len = -1) { StringW o; utf8to16(o, utf8, len); return o; }
    inline StringW utf8to16(const StringA& utf8) { StringW o; utf8to16(o, utf8, utf8.Count()); return o; }

    // 创建目录（目录已存在返回true）
    YSBASE_CLASS bool create_dir(pcchar dir);
    // 创建多级目录
    YSBASE_CLASS bool create_mdir(pcchar dir);
}

#define EXTALIGN2(c)   ((c + 2) & ~1)
#define EXTALIGN4(c)   ((c + 4) & ~3)

#ifdef PF_WINDOWS
#define YS_SEP     '\\'
#define YS_SEPS    "\\/"
#else
#define YS_SEP     '/'
#define YS_SEPS    "/"
#endif
