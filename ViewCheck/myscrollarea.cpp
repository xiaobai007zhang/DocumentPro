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
    int adjustSize = 20; //�ı�ĳߴ�
    QRect tmp = label->geometry();
    //����������ϻ����������λ����scrollArea����
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

    //���¶�λ������Ϊ���λ��
    QScrollBar* tmph = horizontalScrollBar();
    QScrollBar* tmpv = verticalScrollBar();
    QPoint pos = event->pos();
    tmph->setValue(pos.x());
    tmpv->setValue(pos.y());
}
