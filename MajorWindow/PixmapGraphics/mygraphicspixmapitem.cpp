#include "mygraphicspixmapitem.h"
#include <QCursor>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QWheelEvent>

MyGraphicsPixmapItem::MyGraphicsPixmapItem(QRectF rect, QGraphicsItem* parent) : QGraphicsPixmapItem(parent), m_isRepeat(true)
{
    m_rect = rect;

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
}

MyGraphicsPixmapItem::~MyGraphicsPixmapItem()
{
}

void MyGraphicsPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << zValue();
    // m_zValue = zValue();
    // setZValue(1);
    m_isMousePress = true;
    m_startPos = event->scenePos();

    //开启隐藏
    emit sig_hideRectMouse(true);

    QGraphicsPixmapItem::mousePressEvent(event);
}

void MyGraphicsPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    // setZValue(m_zValue);

    m_isMousePress = false;
    emit sig_hideRectMouse(true);
    QGraphicsPixmapItem::mouseReleaseEvent(event);
}

void MyGraphicsPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{

    if (m_isMousePress)
    {
        emit sig_hideRectMouse(false);
    }

    if (m_isRepeat == false)
    { //不重复
        QList<QGraphicsItem*> list = collidingItems();
        for (QGraphicsItem* item : list)
        { //比较坐标
            qDebug() << "item->pos(): " << item->pos();
            qDebug() << "pos(): " << pos();

            if (item->pos().x() < pos().x() && item->pos().y() < pos().y())
            {

                item->setPos(item->pos().x() - 2.5, item->pos().y() - 2.5);
            }

            if (item->pos().x() < pos().x() && item->pos().y() > pos().y())
            {
                item->setPos(item->pos().x() - 2.5, item->pos().y() + 2.5);
            }

            if (item->pos().x() > pos().x() && item->pos().y() > pos().y())
            {
                item->setPos(item->pos().x() + 2.5, item->pos().y() + 2.5);
            }

            if (item->pos().x() > pos().x() && item->pos().y() < pos().y())
            {
                item->setPos(item->pos().x() + 2.5, item->pos().y() - 2.5);
            }

            if (item->pos().x() == pos().x() && item->pos().y() < pos().y())
            {
                item->setPos(item->pos().x(), item->pos().y() - 2.5);
            }

            if (item->pos().x() == pos().x() && item->pos().y() > pos().y())
            {
                item->setPos(item->pos().x(), item->pos().y() + 2.5);
            }

            if (item->pos().x() > pos().x() && item->pos().y() == pos().y())
            {
                item->setPos(item->pos().x() + 2.5, item->pos().y());
            }

            if (item->pos().x() < pos().x() && item->pos().y() == pos().y())
            {
                item->setPos(item->pos().x() - 2.5, item->pos().y());
            }
        }
    }
    else
    {
        QList<QGraphicsItem*> list = collidingItems();
        for (QGraphicsItem* item : list)
        {
            setZValue(item->zValue() + 0.01);
        }
    }

    QGraphicsPixmapItem::mouseMoveEvent(event);
}

void MyGraphicsPixmapItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    // qDebug() << "paint";

    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    painter->drawPixmap(m_rect.toRect(), m_pixmap);

    // m_rect = QRectF(scenePos(), QSizeF(m_rect.width(), m_rect.height()));

    QGraphicsPixmapItem::paint(painter, option, widget);
}

QVariant MyGraphicsPixmapItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (m_isExpand)
    { //允许扩展
        return QGraphicsItem::itemChange(change, value);
    }
    else
    {
        if (change == ItemPositionChange && scene()) // 控件发生移动
        {
            // qDebug() << "boundRect :" << boundingRect().width() << "," << boundingRect().height();
            QPointF newPos = value.toPointF(); //即将要移动的位置

            // QRectF rect(0, 0, scene()->width() - scale() * boundingRect().width(), scene()->height() - scale() * boundingRect().height());
            QRectF rect = scene()->sceneRect();
            qDebug() << "pix scene()->sceneRect(): " << rect;

            // qDebug() << "scene rect: " << rect;
            // QRectF rect(0, 0, 800, 600); // 你要限制的区域
            // qDebug() << "rect.left()" << rect.left();
            // qDebug() << "rect.right()" << rect.right();
            // qDebug() << "rect.top()" << rect.top();
            // qDebug() << "rect.bottom()" << rect.bottom();
            // qDebug() << "newPos: " << newPos;

            if (!rect.contains(newPos)) // 是否在区域内
            {
                newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
                newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
                // qDebug() << "newPos: " << newPos;
                return newPos;
            }
        }
        return QGraphicsItem::itemChange(change, value);
    }
}

QRectF MyGraphicsPixmapItem::boundingRect() const
{
    return QRectF(m_rect.x(), m_rect.y(), m_rect.width(), m_rect.height());
}

QPainterPath MyGraphicsPixmapItem::shape() const
{
    QPainterPath pth;
    pth.addRect(boundingRect());
    return pth;
}

void MyGraphicsPixmapItem::setImage(const QString& filePth)
{
    m_imagePth = filePth;
    m_pixmap.load(filePth);
}

void MyGraphicsPixmapItem::setRect(QRectF rect)
{
    m_rect = rect;
}

QRectF MyGraphicsPixmapItem::getBoundingRect()
{
    return boundingRect();
}

QString MyGraphicsPixmapItem::getImagePth()
{
    return m_imagePth;
}

void MyGraphicsPixmapItem::slot_repeat(bool flag)
{
    m_isRepeat = flag;
}

void MyGraphicsPixmapItem::slot_expand(bool flag)
{
    m_isExpand = flag;
}