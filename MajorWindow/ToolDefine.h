
#ifndef __TOOLDEFINE_H
#define __TOOLDEFINE_H
#define TR(str) QStringLiteral(str)

enum STATE_FLAG
{
    DEFAULT_FLAG = 0,
    MOV_LEFT_LINE,        //��ǵ�ǰΪ�û����¾��ε���߽�����
    MOV_TOP_LINE,         //��ǵ�ǰΪ�û����¾��ε��ϱ߽�����
    MOV_RIGHT_LINE,       //��ǵ�ǰΪ�û����¾��ε��ұ߽�����
    MOV_BOTTOM_LINE,      //��ǵ�ǰΪ�û����¾��ε��±߽�����
    MOV_RIGHTBOTTOM_RECT, //��ǵ�ǰΪ�û����¾��ε����½�
    MOV_RECT,             //��ǵ�ǰΪ����϶�ͼƬ�ƶ�״̬
};
#endif // !__TOOLDEFINE_H
