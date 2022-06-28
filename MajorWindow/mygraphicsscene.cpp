#include "mygraphicsscene.h"
#include <QDebug>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QMenuBar>
#include <QPainter>

#include "mygraphicstextitem.h"
MyGraphicsScene::MyGraphicsScene(const int& width, const int& height, QObject* parent) : QGraphicsScene(parent), m_textItemFlag(true), m_rectMouse(nullptr)
{
    setWidHei(width, height);

    // m_rectMouse = new QGraphicsRectItem;
    // m_rectMouse->setBrush(Qt::blue);
}

MyGraphicsScene::MyGraphicsScene(QObject* parent) : QGraphicsScene(parent)
{
}

MyGraphicsScene::~MyGraphicsScene()
{
}

void MyGraphicsScene::setWidHei(const int& width, const int& height)
{
    setSceneRect(0, 0, width, height);
}

void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    // qDebug() << "My mouse press event";
    // qDebug() << "mouse press: " << ;
    if (event->button() == Qt::LeftButton)
    {
        m_rectMouse = new QGraphicsRectItem;
        addItem(m_rectMouse);

        m_isPress = true;
        m_curPos = event->scenePos();
        QGraphicsScene::mousePressEvent(event);
    }
    else
    {
        QGraphicsScene::mousePressEvent(event);
    }
}

void MyGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    //只有点击了才会做处理
    if (m_isPress)
    {
        int width = event->scenePos().x() - m_curPos.x();
        int height = event->scenePos().y() - m_curPos.y();

        if (width > 0 && height > 0)
        {
            QSize size(width, height);
            m_rectMouse->setRect(QRectF(m_curPos.toPoint(), size));
        }

        if (width < 0 && height < 0)
        {
            QSize size(abs(width), abs(height));
            m_rectMouse->setRect(QRectF(m_curPos.toPoint() - QPoint(size.width(), size.height()), size));
        }

        if (width > 0 && height < 0)
        {
            QSize size(abs(width), abs(height));
            m_rectMouse->setRect(QRectF(m_curPos.toPoint() - QPoint(0, size.height()), size));
        }

        if (width < 0 && height > 0)
        {
            QSize size(abs(width), abs(height));
            m_rectMouse->setRect(QRectF(m_curPos.toPoint() - QPoint(size.width(), 0), size));
        }

        QGraphicsScene::mouseMoveEvent(event);
    }
    else
    {

        QGraphicsScene::mouseMoveEvent(event);
    }
}

void MyGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    // qDebug() << "MyScene mouse event";
    // QSize size = QSize(abs(event->scenePos().x() - m_curPos.x()), (abs(event->scenePos().y() - m_curPos.y())));

    QSize size = QSize(event->scenePos().x() - m_curPos.x(), (event->scenePos().y() - m_curPos.y()));

    //计算一下选中的文本框大小
    // qDebug() << "startPos: " << m_curPos;
    // qDebug() << "scenePos: " << event->scenePos();
    emit(sig_rectFrame(size, event->scenePos(), m_textItemFlag));
    m_isPress = false;

    QList<QGraphicsItem*> list = this->items();
    if (list.contains(m_rectMouse))
    {
        removeItem(m_rectMouse);
    }

    QGraphicsScene::mouseReleaseEvent(event);
}

void MyGraphicsScene::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    // qDebug() << "scene wheel event";
    QList<QGraphicsItem*> list = selectedItems();
    if (!list.isEmpty())
    {
        for (QGraphicsItem* item : list)
        {

            if (event->delta() > 0)
            {
                // qDebug() << QStringLiteral("大于零");
                item->setScale(item->scale() + 0.02);
                item->setPos((item->pos().x() - item->boundingRect().width() * 0.02), (item->pos().y() - item->boundingRect().height() * 0.02));
                // item->setPos(item->pos().x() - item->pos().x() * 0.02, item->pos().y() - item->pos().y() * 0.02);
            }
            else
            {
                if (item->scale() - 0.01 <= 0.02)
                {
                    continue;
                }
                else
                {
                    item->setScale(item->scale() - 0.02);
                }
                // qDebug() << QStringLiteral("小于零");
                item->setPos((item->pos().x() + item->boundingRect().width() * 0.02), (item->pos().y() + item->boundingRect().height() * 0.02));
                // item->setPos(item->pos().x() - item->pos().x() * 0.02, item->pos().y() - item->pos().y() * 0.02);
            }
        }

        event->accept();
    }
    else
    {
        QGraphicsScene::wheelEvent(event);
    }
}

void MyGraphicsScene::slot_hideRectMouse(bool flag)
{
    m_textItemFlag = flag;
    m_rectMouse->hide();
}