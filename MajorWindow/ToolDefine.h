
#ifndef __TOOLDEFINE_H
#define __TOOLDEFINE_H
#define TR(str) QStringLiteral(str)

enum STATE_FLAG
{
    DEFAULT_FLAG = 0,
    MOV_LEFT_LINE,        //标记当前为用户按下矩形的左边界区域
    MOV_TOP_LINE,         //标记当前为用户按下矩形的上边界区域
    MOV_RIGHT_LINE,       //标记当前为用户按下矩形的右边界区域
    MOV_BOTTOM_LINE,      //标记当前为用户按下矩形的下边界区域
    MOV_RIGHTBOTTOM_RECT, //标记当前为用户按下矩形的右下角
    MOV_RECT,             //标记当前为鼠标拖动图片移动状态
};
#endif // !__TOOLDEFINE_H
