#include "mylabel.h"
#include <QDebug>

void MyLabel::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_startPos = event->pos();
        m_isPress = true;
        setCursor(Qt::ClosedHandCursor);
    }
    return QLabel::mousePressEvent(event);
}

void MyLabel::mouseReleaseEvent(QMouseEvent* event)
{
    m_isPress = false;
    // qDebug() << m_isPress;
    return QLabel::mouseReleaseEvent(event);
}

void MyLabel::mouseMoveEvent(QMouseEvent* event)
{
    // qDebug() << m_isPress;
    if (m_isPress)
    {
        QPoint offset = event->pos() - m_startPos;
        setCursor(Qt::ClosedHandCursor);
        this->move(pos() + offset);
        return QLabel::mouseMoveEvent(event);
    }

    setCursor(Qt::OpenHandCursor);
    return QLabel::mouseMoveEvent(event);
}

void MyLabel::wheelEvent(QWheelEvent* event)
{
    if (event->delta() > 0)
    {
        // printf("out zero");
    }
    else
    {
        // printf("less zero");
    }
}

MyLabel::MyLabel(const QSize size, QWidget* parent) : QLabel(parent), m_isPress(false)
{
    resize(size);
    setMouseTracking(true);
}

MyLabel::MyLabel(QWidget* parent) : QLabel(parent), m_isPress(false)
{
    setMouseTracking(true);
}

MyLabel::~MyLabel()
{
}
