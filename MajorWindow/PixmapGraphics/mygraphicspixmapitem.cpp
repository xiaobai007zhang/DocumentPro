#include "mygraphicspixmapitem.h"
#include <QCursor>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QWheelEvent>
#include <qmath.h>

MyGraphicsPixmapItem::MyGraphicsPixmapItem(QRectF rect, QGraphicsItem* parent) : QGraphicsPixmapItem(parent), m_isRepeat(true)
{
    m_rect = rect;

    // setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setRect(m_rect);
    setAcceptHoverEvents(true);
}

MyGraphicsPixmapItem::~MyGraphicsPixmapItem()
{
}

void MyGraphicsPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    // qDebug() << "mouse press";
    m_isMousePress = true;
    m_startPos = event->pos();

    //开启隐藏
    emit sig_hideRectMouse(true);

    return QGraphicsPixmapItem::mousePressEvent(event);
}

void MyGraphicsPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    // setZValue(m_zValue);
    setCursor(Qt::ArrowCursor);
    if (M_FLAG == MOV_RECT)
    {
        M_FLAG = DEFAULT_FLAG;
    }
    // qDebug() << flags();
    m_isMousePress = false;
    emit sig_hideRectMouse(true);
    return QGraphicsPixmapItem::mouseReleaseEvent(event);
}

void MyGraphicsPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_isMousePress)
    {
        emit sig_hideRectMouse(false);
    }

    if (M_FLAG == MOV_RECT)
    {
        QPointF point = event->pos() - m_startPos;
        moveBy(point.x() * scale(), point.y() * scale());
        setRect(m_rect);
        scene()->update();
    }
    else if (M_FLAG == MOV_TOP_LINE)
    {
        handle(0, 3, 2, 3, event->pos(), MOV_TOP_LINE);
    }
    else if (M_FLAG == MOV_LEFT_LINE)
    {
        handle(0, 1, 1, 2, event->pos(), MOV_LEFT_LINE);
    }
    else if (M_FLAG == MOV_RIGHT_LINE)
    {
        handle(0, 1, 0, 3, event->pos(), MOV_RIGHT_LINE);
    }
    else if (M_FLAG == MOV_BOTTOM_LINE)
    {
        handle(0, 3, 0, 1, event->pos(), MOV_BOTTOM_LINE);
    }
    else if (M_FLAG == MOV_RIGHTBOTTOM_RECT)
    {
        handle(0, 3, 0, 1, event->pos(), MOV_RIGHTBOTTOM_RECT);
    }

    if (m_isRepeat == false)
    { //不重复
        QList<QGraphicsItem*> list = collidingItems();
        for (QGraphicsItem* item : list)
        { //比较坐标
            // qDebug() << "item->pos(): " << item->pos();
            // qDebug() << "pos(): " << pos();

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
        // QList<QGraphicsItem*> list = collidingItems();
        // for (QGraphicsItem* item : list)
        //{
        //     // setZValue(item->zValue() + 0.01);
        // }
    }

    return QGraphicsPixmapItem::mouseMoveEvent(event);
}

void MyGraphicsPixmapItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{

    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    painter->drawPixmap(m_rect.toRect(), m_pixmap);

    return QGraphicsPixmapItem::paint(painter, option, widget);
}

void MyGraphicsPixmapItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{

    if (m_insicedRectf.contains(event->pos()))
    {
        setCursor(Qt::SizeAllCursor);
        M_FLAG = MOV_RECT;
    }
    else if (m_topRect.contains(event->pos()))
    {

        setCursor(Qt::SizeVerCursor);

        M_FLAG = MOV_TOP_LINE;
    }
    else if (m_leftRect.contains(event->pos()))
    {

        setCursor(Qt::SizeHorCursor);
        M_FLAG = MOV_LEFT_LINE;
    }
    else if (m_rightRect.contains(event->pos()))
    {

        setCursor(Qt::SizeHorCursor);
        M_FLAG = MOV_RIGHT_LINE;
    }
    else if (m_bottomRect.contains(event->pos()))
    {

        setCursor(Qt::SizeVerCursor);
        M_FLAG = MOV_BOTTOM_LINE;
    }
    else if (m_rbRect.contains(event->pos()))
    {

        setCursor(Qt::SizeFDiagCursor);
        M_FLAG = MOV_RIGHTBOTTOM_RECT;
    }
    else
    {
        setCursor(Qt::ArrowCursor);
        M_FLAG = DEFAULT_FLAG;
    }

    return QGraphicsPixmapItem::hoverMoveEvent(event);
}

QRectF MyGraphicsPixmapItem::boundingRect() const
{
    // return QRectF(m_rect.x(), m_rect.y(), m_rect.width(), m_rect.height());
    return m_rect;
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

    m_rectPolygon = getPolygonFromRect(m_rect);

    m_topRect = QRectF(m_rect.x() + 8, m_rect.y(), m_rect.width() - 8, 8);

    m_leftRect = QRectF(m_rect.x(), m_rect.y() + 8, 8, m_rect.height() - 8);

    m_rightRect = QRectF(m_rect.right() - 8, m_rect.y(), 8, m_rect.height() - 8);

    m_bottomRect = QRectF(m_rect.x(), m_rect.bottom() - 8, m_rect.width() - 8, 8);

    m_insicedRectf = QRectF(m_rect.x() + 8, m_rect.y() + 8, m_rect.width() - 16, m_rect.height() - 16);

    //右下角的小矩形
    m_rbRect = QRectF(m_rect.right() - 8, m_rect.bottom() - 8, 8, 8);
}

QRectF MyGraphicsPixmapItem::getBoundingRect()
{
    return boundingRect();
}

QString MyGraphicsPixmapItem::getImagePth()
{
    return m_imagePth;
}

QPolygonF MyGraphicsPixmapItem::getPolygonFromRect(QRectF rectIn)
{
    QVector<QPointF> vpt;

    QPointF pf = rectIn.topLeft();
    vpt.append(pf);
    pf = rectIn.topRight();
    vpt.append(pf);
    pf = rectIn.bottomRight();
    vpt.append(pf);
    pf = rectIn.bottomLeft();
    vpt.append(pf);
    pf = rectIn.topLeft();
    vpt.append(pf);

    return QPolygonF(vpt);
}

void MyGraphicsPixmapItem::handle(int pos1, int pos2, int pos3, int pos4, QPointF eventPos, STATE_FLAG FLAGS)
{
    // pf求出了矩形的中心点
    QPointF pf = QPointF((m_rectPolygon.at(pos3).x() + m_rectPolygon.at(pos4).x()) / 2, ((m_rectPolygon.at(pos3).y() + m_rectPolygon.at(pos4).y()) / 2));

    //最终dis求出来的是，鼠标移动对于矩形中心点的距离
    qreal dis = sqrt((eventPos.x() - pf.x()) * (eventPos.x() - pf.x()) + (eventPos.y() - pf.y()) * (eventPos.y() - pf.y()));
    qreal dis2LT = sqrt((eventPos.x() - m_rectPolygon.at(pos1).x()) * (eventPos.x() - m_rectPolygon.at(pos1).x()) +
                        (eventPos.y() - m_rectPolygon.at(pos1).y()) * (eventPos.y() - m_rectPolygon.at(pos1).y()));
    qreal dis2LB = sqrt((eventPos.x() - m_rectPolygon.at(pos2).x()) * (eventPos.x() - m_rectPolygon.at(pos2).x()) +
                        (eventPos.y() - m_rectPolygon.at(pos2).y()) * (eventPos.y() - m_rectPolygon.at(pos2).y()));
    if (dis <= 20)
    {
        return;
    }

    if (FLAGS == MOV_TOP_LINE || FLAGS == MOV_LEFT_LINE)
    {
        if (dis2LT > dis2LB)
        {
            return;
        }
    }
    else if (FLAGS == MOV_RIGHT_LINE || FLAGS == MOV_BOTTOM_LINE || FLAGS == MOV_RIGHTBOTTOM_RECT)
    {
        if (dis2LT < dis2LB)
        {
            return;
        }
    }

    QRectF newRect(m_rect);
    QPointF pf2;
    qreal dis2;
    switch (FLAGS)
    {
    case MOV_LEFT_LINE:
        newRect.setLeft(m_rect.right() - dis);
        newRect.setRight(m_rect.right());

        break;
    case MOV_TOP_LINE:
        newRect.setTop(m_rect.bottom() - dis);
        newRect.setBottom(m_rect.bottom());
        break;

    case MOV_RIGHT_LINE:
        newRect.setLeft(m_rect.left());
        newRect.setRight(m_rect.left() + dis);
        break;

    case MOV_BOTTOM_LINE:
        newRect.setTop(m_rect.top());
        newRect.setBottom(m_rect.top() + dis);
        break;

    case MOV_RIGHTBOTTOM_RECT:
        pf2 = QPointF((m_rectPolygon.at(pos1).x() + m_rectPolygon.at(pos2).x()) / 2, ((m_rectPolygon.at(pos1).y() + m_rectPolygon.at(pos2).y()) / 2));
        dis2 = sqrt((eventPos.x() - pf2.x()) * (eventPos.x() - pf2.x()) + (eventPos.y() - pf2.y()) * (eventPos.y() - pf2.y()));
        newRect.setBottom(m_rect.top() + dis);
        newRect.setRight(m_rect.left() + dis2);
        break;
    default:
        return;
    }
    // qDebug() << eventPos;
    // qDebug() << newRect;
    setRect(newRect);

    scene()->update(); //必须要用scene()->update()，不能用update();否则会出现重影
}

void MyGraphicsPixmapItem::slot_repeat(bool flag)
{
    m_isRepeat = flag;
}
