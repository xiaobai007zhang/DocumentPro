#pragma once
#include "ys_rbase.h"

namespace ys
{
    // 分词和词性标注结果
    R_STRUCT_BEGIN(NlpWord)
        R_STRING(text)                        // 对应文本，全闭区间[startChar,endChar]
        R_STRING(lemma)                       // 词元
        R_INT(startChar)                      // from index 0
        R_INT(endChar)                        // last char index
        R_STRING(upos)                        // pos类型，和POS模块定义的类型一致
        R_INT(head)                           // id of the syntactic head of this word
        R_STRING(deprel)
        R_STRUCT_END;

    R_STRUCT_BEGIN(NlpToken)
        R_STRING(text)
        R_INT(startChar)                      // from index 0
        R_INT(endChar)                        // last char index
        R_INTARRAY(words)                     // indexes in NlpDocument::words
        R_STRUCT_END;

    // NER结果
    R_STRUCT_BEGIN(NlpSpan)
        R_STRING(text)
        R_STRING(type)                        // ner类型, 和NER模块定义的type一致
        R_INT(startChar)                      // from index 0
        R_INT(endChar)                        // last char index
        R_INTARRAY(words)                     // indexes in NlpDocument::words
        R_STRUCT_END;

    // 依存句法分析结果
    R_STRUCT_BEGIN(NlpDependency)
        R_INT(head)                            // index in NlpDocument::words
        R_STRING(type)                         // 依存类型
        R_INT(dependent)                       // index in NlpDocument::words
        R_STRUCT_END;

    // 整个页面的所有信息
    R_STRUCT_BEGIN(NlpDocument)
        R_STRING(text)
        R_OBJARRAY(words, NlpWord)
        R_OBJARRAY(tokens, NlpToken)
        R_OBJARRAY(entities, NlpSpan)
        R_OBJARRAY(dependencies, NlpDependency)
        R_STRUCT_END;
}

