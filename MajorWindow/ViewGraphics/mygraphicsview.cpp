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
        // ��ȡ��ǰ��������scene��λ��;
        QPointF scenePos = this->mapToScene(QPoint(cursorPoint.x(), cursorPoint.y()));
        // qDebug() << cursorPoint;
        // qDebug() << scenePos;

        // ��ȡview�Ŀ��;
        qreal viewWidth = this->viewport()->width();
        qreal viewHeight = this->viewport()->height();

        // ��ȡ��ǰ���λ���൱��view��С�ĺ��ݱ���;
        qreal hScale = cursorPoint.x() / viewWidth;
        qreal vScale = cursorPoint.y() / viewHeight;

        // ��ǰ��������;
        // qreal scaleFactor = this->matrix().m11();
        int wheelDeltaValue = event->delta();
        // ���Ϲ������Ŵ�;
        if (wheelDeltaValue > 0)
        {
            this->scale(1.01, 1.01);
        }
        // ���¹�������С;
        else
        {
            // this->scale(1.0 / 1.01, 1.0 / 1.1);
            this->scale(1.0 / 1.01, 1.0 / 1.01);
        }

        // ��scene����ת��Ϊ�Ŵ���С�������;
        QPointF viewPoint = this->matrix().map(scenePos);
        // ͨ������������view�Ŵ���С���չʾscene��λ��;
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
