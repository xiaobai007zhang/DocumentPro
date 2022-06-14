#pragma once

namespace ys
{
    enum class Language : int
    {
        /// special
        Unknown = -1,
        Blank = 0,              /// non-text

        /// language family
        EastAsia = 1,           /// 中文语系
        Latin = 2,              /// 拉丁语系
        ArabicFamily = 3,       /// 阿拉伯语系
        TibetanFamily = 4,      /// 藏文语系
        ThaiFamily = 5,         /// 泰文语系
        Digit = 6,              /// 纯数字，类别数：10

        /// language
        Chinese = 100,          /// 中文，类别数: 7678(GB2312), 11515(GB2312+常用繁体), 21490(GBK)
        Japanese = 101,         /// 日文，
        Korean = 102,           /// 韩文，

        English = 200,          /// 英语，类别数:196
        French = 201,           /// 法语，类别数:228
        Pinyin = 202,           /// 汉语拼音，类别数:233
        German = 203,           /// 德语，类别数:204
        Spanish = 204,          /// 西班牙语
        Portuguese = 205,       /// 葡萄牙语
        Italian = 206,          /// 意大利语
        Russian = 207,          /// 俄语
        Danish = 208,           /// 丹麦语
        Dutch = 209,            /// 荷兰语
        Swedish = 210,          /// 瑞典语
        Turkish = 211,          /// 土耳其语
        Polish = 212,           /// 波兰语
        Greek = 213,            /// 希腊语
        Finnish = 214,          /// 芬兰语
        Norwegian = 215,        /// 挪威语
        Hungarian = 216,        /// 匈牙利语
        Romanian = 217,         /// 罗马尼亚语
        Vietnamese = 218,       /// 越南语
        Malay = 219,            /// 马来语
        Indonesian = 220,       /// 印尼语

        Arabic = 300,           /// 阿拉伯文
        Uyghur = 301,           /// 维文

        Tibetan = 400,          /// 藏文

        Thai = 500,             /// 泰语
    };
}