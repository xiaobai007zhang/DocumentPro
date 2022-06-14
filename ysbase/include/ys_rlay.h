#pragma once
#include "ys_rbase.h"
#include "ys_rptrarray.h"

/* @hhoking@ 标准数据交换结构
*/
namespace ys
{
    // 字 
    R_SAX_STRUCT_BEGIN(Char)
        R_FIXSTRING(code, 1)            // 字符 
        R_FIXSTRING(candi, 5)           // 候选字 
        R_FIXINTARRAY(coords, 8)        // 字外框坐标 
        R_INT(type)                     // 字类型（=0印刷体，=1手写体，=2非识别字） 
        R_INT(score)                    // 分数[0,1000] 
        R_FIXINTARRAY(candi_score, 5)   // 候选字分数[0,1000] 
        R_SAX_STRUCT_END;

    R_SAX_FUNC_BEGIN(Char)
        R_SAX(code)
        R_SAX(candi)
        R_SAX(coords)
        R_SAX(type)
        R_SAX(score)
        R_SAX(candi_score)
        R_SAX_FUNC_END;

    // 行 
    R_SAX_STRUCT_BEGIN(Line)
        R_OBJARRAY(chars, Char)     // 字数组 
        R_STRING(text)              // 行内容 
        R_FIXINTARRAY(coords, 8)    // 行外框坐标 
        R_INT(type)                 // 行类型 
        R_INT(score)                // 分数[0,1000] 
        R_SAX_STRUCT_END;

    R_SAX_FUNC_BEGIN(Line)
        R_SAX(chars)
        R_SAX(text)
        R_SAX(coords)
        R_SAX(type)
        R_SAX(score)
        R_SAX_FUNC_END;

    // 块
    R_SAX_STRUCT_BEGIN(Block)
        R_OBJARRAY(lines, Line)     // 行数组 
        R_FIXINTARRAY(coords, 8)    // 块外框坐标 
        R_INT(rotate)               // 旋转角（=0，=90原图及坐标需顺时针旋转90度，=180，=270） 
        R_SAX_STRUCT_END;

    R_SAX_FUNC_BEGIN(Block)
        R_SAX(lines)
        R_SAX(coords)
        R_SAX(rotate)
        R_SAX_FUNC_END;

    // 条目
    R_SAX_STRUCT_BEGIN(Item)
        R_STRING(text)              // 内容 
        R_FIXINTARRAY(coords, 8)    // 外框坐标（基于抠图的坐标系） 
        R_INTARRAY(polygon)         // 多点坐标
        R_INT(score)                // 分数[0,1000] 
        R_SAX_STRUCT_END;

    R_SAX_FUNC_BEGIN(Item)
        R_SAX(text)
        R_SAX(coords)
        R_SAX(polygon)
        R_SAX(score)
        R_SAX_FUNC_END;

    // 条目集 
    R_SAX_STRUCT_BEGIN(ItemCollection)
        R_OBJARRAY(items, Item)     // 条目数组 
        R_INT(type)                 // 卡证类型 
        R_INT(score)                // 卡证类型分数[0,1000] 
        R_FIXINTARRAY(coords, 8)    // 外框坐标（基于原图的坐标系） 
        R_INT(width)                // 抠图宽 
        R_INT(height)               // 抠图高 
        R_SAX_STRUCT_END;

    R_SAX_FUNC_BEGIN(ItemCollection)
        R_SAX(items)
        R_SAX(type)
        R_SAX(score)
        R_SAX(coords)
        R_SAX(width)
        R_SAX(height)
        R_SAX_FUNC_END;

    // 条目对
    R_SAX_STRUCT_BEGIN(Pair)
        R_STRING(key)               // 抽象键值 
        R_OBJ(pkey, Item)           // 识别键值（未能识别对应为空） 
        R_OBJ(value, Item)          // 内容值（未能识别对应为空） 
        R_SAX_STRUCT_END;

    R_SAX_FUNC_BEGIN(Pair)
        R_SAX(key)
        R_SAX(pkey)
        R_SAX(value)
        R_SAX_FUNC_END;

    // 条目对集
    R_SAX_STRUCT_BEGIN(PairCollection)
        R_OBJARRAY(items, Pair)     // 条目对数组 
        R_INT(type)                 // 卡证类型 
        R_INT(score)                // 卡证类型分数[0,1000] 
        R_INT(width)                // 抠图宽 
        R_INT(height)               // 抠图高 
        R_FIXINTARRAY(coords, 8)    // 外框坐标，基于原图坐标系 
        R_SAX_STRUCT_END;

    R_SAX_FUNC_BEGIN(PairCollection)
        R_SAX(items)
        R_SAX(type)
        R_SAX(score)
        R_SAX(width)
        R_SAX(height)
        R_SAX(coords)
        R_SAX_FUNC_END;

    // 卡证条目
    R_SAX_STRUCT_BEGIN(KVItem)
        R_STRING(key)               // 键 
        R_STRING(value)             // 值 
        R_SAX_STRUCT_END;

    R_SAX_FUNC_BEGIN(KVItem)
        R_SAX(key)
        R_SAX(value)
        R_SAX_FUNC_END;

    // 卡证
    R_SAX_STRUCT_BEGIN(KVCard)
        R_OBJARRAY(items, KVItem)   // 条目数组 
        R_INT(type)                 // 卡证类型 
        R_INT(score)                // 卡证类型分数[0-1000] 
        R_FIXINTARRAY(coords, 8)    // 外框坐标，基于原图坐标系 
        R_SAX_STRUCT_END;

    R_SAX_FUNC_BEGIN(KVCard)
        R_SAX(items)
        R_SAX(type)
        R_SAX(score)
        R_SAX(coords)
        R_SAX_FUNC_END;

    // 单元格 
    R_SAX_STRUCT_BEGIN(LayCell)
        R_OBJARRAY(lines, Line)     // 表格文本 
        R_INT(rowstart)             // 行起始逻辑号(0起始) 
        R_INT(rowend)               // 行结束逻辑号 
        R_INT(colstart)             // 列起始逻辑号(0起始) 
        R_INT(colend)               // 列结束逻辑号 
        R_FIXINTARRAY(coords, 8)    // 单元格坐标 
        R_SAX_STRUCT_END;

    R_SAX_FUNC_BEGIN(LayCell)
        R_SAX(lines)
        R_SAX(rowstart)
        R_SAX(rowend)
        R_SAX(colstart)
        R_SAX(colend)
        R_SAX(coords)
        R_SAX_FUNC_END;

    // 版面类型
    enum class LayType : int
    {
        Unknown = 0,
        Text = 1,                   // 文本
        Table = 2,                  // 表格
        Image = 3,                  // 图像
        Formula = 4,                // 公式
    };

    // 版面元素基类
    R_STRUCT_BASE(LayElem)
        R_INT(type)                 // 版面类型
        R_FIXINTARRAY(coords, 8)    // 外框坐标
        R_STRUCT_END;

    // 文本版面
    R_STRUCT_DERIVE(LayText, LayElem)
        R_INT(subtype)              // 文本类型（标题、正文等）
        R_OBJARRAY(lines, Line)     // 行数组
        R_STRUCT_END;

    // 表格版面
    R_STRUCT_DERIVE(LayTable, LayElem)
        R_OBJARRAY(cells, LayCell)  // 单元格数组 
        R_INT(rows)                 // 逻辑行数 
        R_INT(cols)                 // 逻辑列数 
        R_STRUCT_END;

    // 图像版面
    R_STRUCT_DERIVE(LayImage, LayElem)
        R_INT(subtype)              // 图像类型（签名、印章、贴图）
        R_STRING(base64)            // 图像的base编码 
        R_STRUCT_END;

    // 公式版面
    R_STRUCT_DERIVE(LayFormula, LayElem)
        R_STRING(latex)
        R_STRUCT_END;

    inline LayElem* LayElem::New(int type)
    {
        switch ((LayType)type)
        {
        case LayType::Text:
            return new LayText();
        case LayType::Table:
            return new LayTable();
        case LayType::Image:
            return new LayImage();
        case LayType::Formula:
            return new LayFormula();
        default:
            return nullptr;
        }
    }

    // 版面
    R_STRUCT_BEGIN(Layout)
        R_PTRARRAY(elems, LayElem)  // 版面元素集
        R_FIXINTARRAY(coords, 8)    // 版面外框坐标 
        R_INT(dpi)                  // 图像dpi 
        R_INT(rotate)               // 旋转角（=0，=90原图及坐标需顺时针旋转90度，=180，=270）
        R_STRUCT_END;
}
