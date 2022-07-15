#include "myscrollarea.h"
#include <QDebug>
#include <QLabel>
#include <QScrollBar>
#include <QWheelEvent>

MyScrollArea::MyScrollArea(QWidget* parent) : QScrollArea(parent)
{
}

MyScrollArea::~MyScrollArea()
{
}

void MyScrollArea::wheelEvent(QWheelEvent* event)
{

    QLabel* label = dynamic_cast<QLabel*>(this->children().at(0));
    int adjustSize = 20; //改变的尺寸
    QRect tmp = label->geometry();
    //如果滚轮向上滑动，且鼠标位置在scrollArea里面
    if (event->delta() > 0 && tmp.contains(event->pos()))
    {
        tmp.setWidth(tmp.width() + adjustSize);
        tmp.setHeight(tmp.height() + adjustSize);
        setFixedSize(tmp.width(), tmp.height());
    }
    if (event->delta() < 0 && tmp.contains(event->pos()))
    {
        tmp.setWidth(tmp.width() - adjustSize);
        tmp.setHeight(tmp.height() - adjustSize);
        setFixedSize(tmp.width(), tmp.height());
    }

    //重新定位滚动条为鼠标位置
    QScrollBar* tmph = horizontalScrollBar();
    QScrollBar* tmpv = verticalScrollBar();
    QPoint pos = event->pos();
    tmph->setValue(pos.x());
    tmpv->setValue(pos.y());
}
