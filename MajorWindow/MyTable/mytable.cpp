#include "mytable.h"
#include <QPainter>

void MyTable::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->drawRect(m_rect);
    painter->drawLine(50, 0, 50, 100);
    // QGraphicsItem::paint(painter, option, widget);
}

QRectF MyTable::boundingRect() const
{
    return m_rect;
}

QPainterPath MyTable::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());

    return path;
}

void MyTable::initTableWidget()
{
}

MyTable::MyTable(int row, int col, QRectF rect)
{
    m_row = row;
    m_col = col;
    m_rect = rect;
    initTableWidget();
}

MyTable::~MyTable()
{
}

int MyTable::type() const
{
    return 0;
}
