#include "mygraphicsview.h"
#include <QDebug>
#include <QScrollBar>
#include <QWheelEvent>
#include <QKeyEvent>

MyGraphicsView::MyGraphicsView(QWidget* parent) : m_isCtrlPress(false), QGraphicsView(parent), m_isFlag(true)
{
}

MyGraphicsView::~MyGraphicsView()
{
}

void MyGraphicsView::wheelEvent(QWheelEvent* event)
{

    if (m_isCtrlPress)
    {
        QPointF cursorPoint = event->pos();
        // 获取当前鼠标相对于scene的位置;
        QPointF scenePos = this->mapToScene(QPoint(cursorPoint.x(), cursorPoint.y()));
        // qDebug() << cursorPoint;
        // qDebug() << scenePos;

        // 获取view的宽高;
        qreal viewWidth = this->viewport()->width();
        qreal viewHeight = this->viewport()->height();

        // 获取当前鼠标位置相当于view大小的横纵比例;
        qreal hScale = cursorPoint.x() / viewWidth;
        qreal vScale = cursorPoint.y() / viewHeight;

        // 当前放缩倍数;
        // qreal scaleFactor = this->matrix().m11();
        int wheelDeltaValue = event->delta();
        // 向上滚动，放大;
        if (wheelDeltaValue > 0)
        {
            this->scale(1.01, 1.01);
        }
        // 向下滚动，缩小;
        else
        {
            // this->scale(1.0 / 1.01, 1.0 / 1.1);
            this->scale(1.0 / 1.01, 1.0 / 1.01);
        }

        // 将scene坐标转换为放大缩小后的坐标;
        QPointF viewPoint = this->matrix().map(scenePos);
        // 通过滚动条控制view放大缩小后的展示scene的位置;
        horizontalScrollBar()->setValue(int(viewPoint.x() - viewWidth * hScale));
        verticalScrollBar()->setValue(int(viewPoint.y() - viewHeight * vScale));
    }
    else
    {

        QGraphicsView::wheelEvent(event);
    }
}

void MyGraphicsView::keyPressEvent(QKeyEvent* event)
{
    // qDebug() << "view keyPress";
    if (event->key() == Qt::Key_Control) {
        m_isCtrlPress = true;
    }
    
    QGraphicsView::keyPressEvent(event);
}

void MyGraphicsView::keyReleaseEvent(QKeyEvent* event)
{
    m_isCtrlPress = false;
    QGraphicsView::keyReleaseEvent(event);
}

void MyGraphicsView::paintEvent(QPaintEvent* event)
{
    /*if (m_isFlag)
    {
        this->horizontalScrollBar()->setSliderPosition(0);
        this->verticalScrollBar()->setSliderPosition(0);
        m_isFlag = false;
    }*/
    QGraphicsView::paintEvent(event);
}

void MyGraphicsView::mousePressEvent(QMouseEvent* event)
{
    //qDebug() << "view pos: " << event->pos();
    QGraphicsView::mousePressEvent(event);
}
