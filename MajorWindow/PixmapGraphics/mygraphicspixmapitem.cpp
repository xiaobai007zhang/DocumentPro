#include "mygraphicspixmapitem.h"
#include <QCursor>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QWheelEvent>
#include <qmath.h>

// enum STATE_FLAG3
//{
//     DEFAULT_FLAG = 0,
//     MOV_LEFT_LINE,        //标记当前为用户按下矩形的左边界区域
//     MOV_TOP_LINE,         //标记当前为用户按下矩形的上边界区域
//     MOV_RIGHT_LINE,       //标记当前为用户按下矩形的右边界区域
//     MOV_BOTTOM_LINE,      //标记当前为用户按下矩形的下边界区域
//     MOV_RIGHTBOTTOM_RECT, //标记当前为用户按下矩形的右下角
//     MOV_RECT,             //标记当前为鼠标拖动图片移动状态
//
// } M_FLAG;
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

    if (m_insicsdPoly.containsPoint(m_startPos, Qt::WindingFill))
    {
        // setCursor(Qt::ArrowCursor);
        M_FLAG = MOV_RECT;
        // setFlag(QGraphicsItem::ItemIsMovable);
    }
    else if (m_topPoly.containsPoint(m_startPos, Qt::WindingFill))
    {
        // setFlags(flags() ^ QGraphicsItem::ItemIsMovable);
        // setCursor(Qt::SizeVerCursor);

        M_FLAG = MOV_TOP_LINE;
    }
    else if (m_leftPoly.containsPoint(m_startPos, Qt::WindingFill))
    {
        // setFlags(flags() ^ QGraphicsItem::ItemIsMovable);
        // setCursor(Qt::SizeHorCursor);
        M_FLAG = MOV_LEFT_LINE;
    }
    else if (m_rightPoly.containsPoint(m_startPos, Qt::WindingFill))
    {
        // setFlags(flags() ^ QGraphicsItem::ItemIsMovable);
        // setCursor(Qt::SizeHorCursor);
        M_FLAG = MOV_RIGHT_LINE;
    }
    else if (m_bottomPoly.containsPoint(m_startPos, Qt::WindingFill))
    {
        // setFlags(flags() ^ QGraphicsItem::ItemIsMovable);
        // setCursor(Qt::SizeVerCursor);
        M_FLAG = MOV_BOTTOM_LINE;
    }
    else if (m_rbRect.contains(m_startPos))
    {
        // setFlags(flags() ^ QGraphicsItem::ItemIsMovable);
        // setCursor(Qt::SizeFDiagCursor);
        M_FLAG = MOV_RIGHTBOTTOM_RECT;
    }
    else
    {
        // setFlags(flags() ^ QGraphicsItem::ItemIsMovable);
        // setCursor(Qt::ArrowCursor);
        M_FLAG = DEFAULT_FLAG;
    }
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
        // qDebug() << "top";
        //  pf求出了矩形的中心点
        QPointF pf = QPointF((m_oldRectPolygon.at(2).x() + m_oldRectPolygon.at(3).x()) / 2, ((m_oldRectPolygon.at(2).y() + m_oldRectPolygon.at(3).y()) / 2));

        //最终dis求出来的是，鼠标移动对于矩形中心点的距离
        qreal dis = sqrt((event->pos().x() - pf.x()) * (event->pos().x() - pf.x()) + (event->pos().y() - pf.y()) * (event->pos().y() - pf.y()));
        qreal dis2LT = sqrt((event->pos().x() - m_oldRectPolygon.at(0).x()) * (event->pos().x() - m_oldRectPolygon.at(0).x()) +
                            (event->pos().y() - m_oldRectPolygon.at(0).y()) * (event->pos().y() - m_oldRectPolygon.at(0).y()));
        qreal dis2LB = sqrt((event->pos().x() - m_oldRectPolygon.at(3).x()) * (event->pos().x() - m_oldRectPolygon.at(3).x()) +
                            (event->pos().y() - m_oldRectPolygon.at(3).y()) * (event->pos().y() - m_oldRectPolygon.at(3).y()));

        // dis2LT 和dis2LB 左上角大于左下角的话，那么就代表向下拉到极限了
        if (dis < 20 || dis2LT > dis2LB)
        {
            return;
        }
        else
        {
            // qDebug() << "dis: " << dis;
            // qDebug() << "bottom: " << m_rect.bottom();
            QRectF newRect(m_rect);
            newRect.setTop(m_rect.bottom() - dis);
            newRect.setBottom(m_rect.bottom());
            setRect(newRect);
            // m_RotateCenter = QPointF((m_oldRectPolygon.at(0).x() + m_oldRectPolygon.at(2).x()) / 2, (m_oldRectPolygon.at(0).y() + m_oldRectPolygon.at(2).y()) / 2);
            // m_rect.moveCenter(m_RotateCenter);
            setRect(m_rect);
            scene()->update(); //必须要用scene()->update()，不能用update();否则会出现重影
        }
    }
    else if (M_FLAG == MOV_LEFT_LINE)
    {
        QPointF pf = QPointF((m_oldRectPolygon.at(1).x() + m_oldRectPolygon.at(2).x()) / 2, ((m_oldRectPolygon.at(1).y() + m_oldRectPolygon.at(2).y()) / 2));
        //计算到右侧边中点的距离
        qreal dis = sqrt((event->pos().x() - pf.x()) * (event->pos().x() - pf.x()) + (event->pos().y() - pf.y()) * (event->pos().y() - pf.y()));
        qreal dis2LT = sqrt((event->pos().x() - m_oldRectPolygon.at(0).x()) * (event->pos().x() - m_oldRectPolygon.at(0).x()) +
                            (event->pos().y() - m_oldRectPolygon.at(0).y()) * (event->pos().y() - m_oldRectPolygon.at(0).y()));
        qreal dis2RT = sqrt((event->pos().x() - m_oldRectPolygon.at(1).x()) * (event->pos().x() - m_oldRectPolygon.at(1).x()) +
                            (event->pos().y() - m_oldRectPolygon.at(1).y()) * (event->pos().y() - m_oldRectPolygon.at(1).y()));
        if (dis < 20 || dis2LT > dis2RT)
        {
            return;
        }
        else
        {
            QRectF newRect(m_rect);
            newRect.setLeft(m_rect.right() - dis);
            newRect.setRight(m_rect.right());
            setRect(newRect);

            // setRect(m_rect);
            scene()->update(); //必须要用scene()->update()，不能用update();否则会出现重影
        }
    }
    else if (M_FLAG == MOV_RIGHT_LINE)
    {
        QPointF pf = QPointF((m_oldRectPolygon.at(0).x() + m_oldRectPolygon.at(3).x()) / 2, ((m_oldRectPolygon.at(0).y() + m_oldRectPolygon.at(3).y()) / 2));
        //计算到左侧边中点的距离
        qreal dis = sqrt((event->pos().x() - pf.x()) * (event->pos().x() - pf.x()) + (event->pos().y() - pf.y()) * (event->pos().y() - pf.y()));
        qreal dis2LT = sqrt((event->pos().x() - m_oldRectPolygon.at(0).x()) * (event->pos().x() - m_oldRectPolygon.at(0).x()) +
                            (event->pos().y() - m_oldRectPolygon.at(0).y()) * (event->pos().y() - m_oldRectPolygon.at(0).y()));
        qreal dis2RT = sqrt((event->pos().x() - m_oldRectPolygon.at(1).x()) * (event->pos().x() - m_oldRectPolygon.at(1).x()) +
                            (event->pos().y() - m_oldRectPolygon.at(1).y()) * (event->pos().y() - m_oldRectPolygon.at(1).y()));
        if (dis < 20 || dis2LT < dis2RT)
        {
            return;
        }
        else
        {
            QRectF newRect(m_rect);
            newRect.setLeft(m_rect.left());
            newRect.setRight(m_rect.left() + dis);
            setRect(newRect);

            // setRect(m_rect);
            scene()->update(); //必须要用scene()->update()，不能用update();否则会出现重影
        }
    }
    else if (M_FLAG == MOV_BOTTOM_LINE)
    {
        //顶边中点
        QPointF pf = QPointF((m_oldRectPolygon.at(0).x() + m_oldRectPolygon.at(1).x()) / 2, ((m_oldRectPolygon.at(0).y() + m_oldRectPolygon.at(1).y()) / 2));
        //计算到底边中点的距离
        qreal dis = sqrt((event->pos().x() - pf.x()) * (event->pos().x() - pf.x()) + (event->pos().y() - pf.y()) * (event->pos().y() - pf.y()));
        qreal dis2LT = sqrt((event->pos().x() - m_oldRectPolygon.at(0).x()) * (event->pos().x() - m_oldRectPolygon.at(0).x()) +
                            (event->pos().y() - m_oldRectPolygon.at(0).y()) * (event->pos().y() - m_oldRectPolygon.at(0).y()));
        qreal dis2LB = sqrt((event->pos().x() - m_oldRectPolygon.at(3).x()) * (event->pos().x() - m_oldRectPolygon.at(3).x()) +
                            (event->pos().y() - m_oldRectPolygon.at(3).y()) * (event->pos().y() - m_oldRectPolygon.at(3).y()));
        if (dis < 20 || dis2LT < dis2LB)
        {
            return;
        }
        else
        {
            QRectF newRect(m_rect);
            newRect.setTop(m_rect.top());
            newRect.setBottom(m_rect.top() + dis);
            setRect(newRect);
            // m_RotateCenter = QPointF((m_oldRectPolygon.at(0).x() + m_oldRectPolygon.at(2).x()) / 2, (m_oldRectPolygon.at(0).y() + m_oldRectPolygon.at(2).y()) / 2);
            // m_oldRect.moveCenter(m_RotateCenter);
            // setRect(m_rect);
            scene()->update(); //必须要用scene()->update()，不能用update();否则会出现重影
        }
    }
    else if (M_FLAG == MOV_RIGHTBOTTOM_RECT)
    {
        //中心坐标点
        QPointF pf = QPointF((m_oldRectPolygon.at(0).x() + m_oldRectPolygon.at(1).x()) / 2, ((m_oldRectPolygon.at(0).y() + m_oldRectPolygon.at(1).y()) / 2));

        qreal dis = sqrt((event->pos().x() - pf.x()) * (event->pos().x() - pf.x()) + (event->pos().y() - pf.y()) * (event->pos().y() - pf.y()));
        qreal dis2LT = sqrt((event->pos().x() - m_oldRectPolygon.at(0).x()) * (event->pos().x() - m_oldRectPolygon.at(0).x()) +
                            (event->pos().y() - m_oldRectPolygon.at(0).y()) * (event->pos().y() - m_oldRectPolygon.at(0).y()));
        qreal dis2RB = sqrt((event->pos().x() - m_oldRectPolygon.at(3).x()) * (event->pos().x() - m_oldRectPolygon.at(2).x()) +
                            (event->pos().y() - m_oldRectPolygon.at(3).y()) * (event->pos().y() - m_oldRectPolygon.at(2).y()));

        QPointF pf2 = QPointF((m_oldRectPolygon.at(0).x() + m_oldRectPolygon.at(3).x()) / 2, ((m_oldRectPolygon.at(0).y() + m_oldRectPolygon.at(3).y()) / 2));
        qreal dis2 = sqrt((event->pos().x() - pf2.x()) * (event->pos().x() - pf2.x()) + (event->pos().y() - pf2.y()) * (event->pos().y() - pf2.y()));

        if (dis < 20 || dis2LT < dis2RB)
        {
            return;
        }
        else
        {
            // qDebug() << "m_rect: bottom: " << m_rect.bottom() << ", right: " << m_rect.right();
            QRectF newRect(m_rect);
            // qDebug() << "dis1: " << dis;
            // qDebug() << "dis2: " << dis2;
            newRect.setBottom(m_rect.top() + dis);
            newRect.setRight(m_rect.left() + dis2);
            setRect(newRect);

            scene()->update();
        }
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
    // painter->setPen(Qt::red);
    // painter->drawRect(m_rbRect);
    // painter->drawRect(m_insicedRectf);
    // painter->drawRect(m_topRect);
    // painter->drawRect(m_bottomRect);
    // painter->drawRect(m_leftRect);
    // painter->drawRect(m_rightRect);
    QGraphicsPixmapItem::paint(painter, option, widget);
}

QVariant MyGraphicsPixmapItem::itemChange(GraphicsItemChange change, const QVariant& value)
{

    return QGraphicsItem::itemChange(change, value);
}

void MyGraphicsPixmapItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    // qDebug() << "hover";
    // m_startPos = event->pos();
    if (m_topPoly.containsPoint(event->pos(), Qt::WindingFill))
    {
        // setFlags(flags() ^ QGraphicsItem::ItemIsMovable);
        setCursor(Qt::SizeVerCursor);

        // M_FLAG = MOV_TOP_LINE;
    }
    else if (m_leftPoly.containsPoint(event->pos(), Qt::WindingFill))
    {
        // setFlags(flags() ^ QGraphicsItem::ItemIsMovable);
        setCursor(Qt::SizeHorCursor);
        // M_FLAG = MOV_LEFT_LINE;
    }
    else if (m_rightPoly.containsPoint(event->pos(), Qt::WindingFill))
    {
        // setFlags(flags() ^ QGraphicsItem::ItemIsMovable);
        setCursor(Qt::SizeHorCursor);
        // M_FLAG = MOV_RIGHT_LINE;
    }
    else if (m_bottomPoly.containsPoint(event->pos(), Qt::WindingFill))
    {
        // setFlags(flags() ^ QGraphicsItem::ItemIsMovable);
        setCursor(Qt::SizeVerCursor);
        // M_FLAG = MOV_BOTTOM_LINE;
    }
    else if (m_rbRect.contains(event->pos()))
    {
        // setFlags(flags() ^ QGraphicsItem::ItemIsMovable);
        setCursor(Qt::SizeFDiagCursor);
        // M_FLAG = MOV_RIGHTBOTTOM_RECT;
    }
    else
    {
        // setFlags(flags() ^ QGraphicsItem::ItemIsMovable);
        setCursor(Qt::ArrowCursor);
        // M_FLAG = DEFAULT_FLAG;
    }

    return QGraphicsPixmapItem::hoverMoveEvent(event);
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

    m_oldRectPolygon = getRotatePolygonFromRect(m_rect);

    m_topRect = QRectF(m_rect.x() + 8, m_rect.y(), m_rect.width() - 8, 8);
    m_topPoly = getRotatePolygonFromRect(m_topRect);

    m_leftRect = QRectF(m_rect.x(), m_rect.y() + 8, 8, m_rect.height() - 8);
    m_leftPoly = getRotatePolygonFromRect(m_leftRect);

    m_rightRect = QRectF(m_rect.right() - 8, m_rect.y(), 8, m_rect.height() - 8);
    m_rightPoly = getRotatePolygonFromRect(m_rightRect);

    m_bottomRect = QRectF(m_rect.x(), m_rect.bottom() - 8, m_rect.width() - 8, 8);
    m_bottomPoly = getRotatePolygonFromRect(m_bottomRect);

    m_insicedRectf = QRectF(m_rect.x() + 8, m_rect.y() + 8, m_rect.width() - 16, m_rect.height() - 16);
    m_insicsdPoly = getRotatePolygonFromRect(m_insicedRectf);

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

QPolygonF MyGraphicsPixmapItem::getRotatePolygonFromRect(QRectF rectIn)
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

void MyGraphicsPixmapItem::slot_repeat(bool flag)
{
    m_isRepeat = flag;
}
