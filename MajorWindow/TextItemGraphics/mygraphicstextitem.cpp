#include "mygraphicstextitem.h"
#include <QCursor>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <qtextdocument.h>

// enum STATE_FLAG2
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

MyGraphicsTextItem::MyGraphicsTextItem(const QRectF& rt, QGraphicsItem* parent) : QGraphicsTextItem(parent), m_isMousePress(false), m_isRepeat(true)
{
    //设置属性，并初始化基本信息
    m_rect = rt;
    M_FLAG = DEFAULT_FLAG;
    initGraphicsTextItem();
    setRectSize(m_rect);
}

MyGraphicsTextItem::~MyGraphicsTextItem()
{
}

void MyGraphicsTextItem::initGraphicsTextItem()
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemSendsGeometryChanges);
    setTextInteractionFlags(Qt::TextEditorInteraction);
    setAcceptHoverEvents(true);
    setFocus();
}

void MyGraphicsTextItem::setRect(const QRectF& rect)
{
    m_rect = rect;
}
//更新框架大小
void MyGraphicsTextItem::updateFontInfo()
{
    m_fontWidth = document()->size().width();
    m_fontHeight = document()->size().height();
    QFontMetrics metrics(font());
    setTextWidth(m_rect.width());

    // setPlainText(m_text);
    //当输入文字长于文本框时
    // if (m_fontWidth > boundingRect().width())
    //{
    //    // qDebug() << "long";
    //    qreal adjust = metrics.width("中");
    //    this->setRect(QRectF(boundingRect().x(), boundingRect().y(), m_fontWidth + adjust, boundingRect().height()));
    //    // setRect(QRectF(boundingRect().x(), boundingRect().y(), m_fontWidth + adjust, boundingRect().height()));
    //    // this->setFont(m_font);
    //    this->setPlainText(m_text);

    //    //当更新文本框后，光标的位置设置，文本的末尾
    //    QTextCursor cursor = this->textCursor();
    //    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
    //    this->setTextCursor(cursor);
    //}

    //当输入文字高于文本框时
    if (m_fontHeight > boundingRect().height())
    {
        qreal adjust = metrics.height();
        this->setRect(QRectF(boundingRect().x(), boundingRect().y(), boundingRect().width(), m_fontHeight + adjust));
        // this->setFont(m_font);
        this->setPlainText(m_text);
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
        this->setTextCursor(cursor);
    }
}

void MyGraphicsTextItem::setText(QString text)
{
    m_text = text;
    updateFontInfo();
}

QRectF MyGraphicsTextItem::getBoundingRect()
{
    return boundingRect();
}

qreal MyGraphicsTextItem::getScale()
{
    return scale();
}

QString MyGraphicsTextItem::getStrText()
{
    return toPlainText();
}

void MyGraphicsTextItem::setRectSize(QRectF rect)
{
    m_rect = rect;

    m_oldRectPolygon = getRotatePolygonFromRect(m_rect);
    m_topRect = QRectF(m_rect.x() + 8, m_rect.y(), m_rect.width() - 8, 8);
    m_topPoly = getRotatePolygonFromRect(m_topRect);

    m_leftRect = QRectF(m_rect.x(), m_rect.y(), 8, m_rect.height() - 8);
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

QPolygonF MyGraphicsTextItem::getRotatePolygonFromRect(QRectF rectIn)
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

void MyGraphicsTextItem::focusInEvent(QFocusEvent* e)
{
    // qDebug() << "focus in";
    // setTextInteractionFlags(Qt::TextEditorInteraction);
    setCursor(Qt::IBeamCursor);
    QGraphicsTextItem::focusInEvent(e);
}

void MyGraphicsTextItem::focusOutEvent(QFocusEvent* e)
{

    // qDebug() << "focusOut";
    if (this->toPlainText().isNull() || this->toPlainText().isEmpty())
    {
        emit sig_loseFocusText(this);
        return;
    }
    // setFlag(QGraphicsItem::ItemStopsFocusHandling);

    int width = document()->size().width();
    int height = document()->size().height();
    setRect(QRectF(boundingRect().x(), boundingRect().y(), width, height));
    // document()->adjustSize();
    //  setTextInteractionFlags(Qt::NoTextInteraction);
    setSelected(false);
    // QTextCursor cursor = textCursor();
    // cursor.clearSelection();
    emit(sig_needSceneUpdate());
    emit(sig_hideRectMouse(true));

    setCursor(Qt::ArrowCursor);
    setFlag(QGraphicsItem::ItemStopsFocusHandling);

    QGraphicsTextItem::focusOutEvent(e);
}

QVariant MyGraphicsTextItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    // if (m_isExpand)
    //{
    return QGraphicsTextItem::itemChange(change, value);
    //}
    // else
    //{
    //    if (change == ItemPositionChange && scene()) // 控件发生移动
    //    {
    //        QPointF newPos = value.toPointF(); //即将要移动的位置
    //        // QRectF rect(0, 0, scene()->width(), scene()->height());
    //        QRectF rect = scene()->sceneRect();
    //        qDebug() << "scene()->sceneRect()" << rect;

    //        if (!rect.contains(newPos)) // 是否在区域内
    //        {
    //            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
    //            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
    //            // qDebug() << "text newPos: ";
    //            return newPos;
    //        }
    //    }
    //    return QGraphicsTextItem::itemChange(change, value);
    //}
}

void MyGraphicsTextItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{

    // painter->drawRect(m_topRect);
    // painter->drawRect(m_leftRect);
    // painter->drawRect(m_rightRect);
    // painter->drawRect(m_bottomRect);
    // painter->drawRect(m_rbRect);
    if (hasFocus())
    {
        painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
        painter->drawRect(boundingRect());
        QString text = toPlainText();
        setText(text);
        return QGraphicsTextItem::paint(painter, option, widget);
    }
    else
    {
        return QGraphicsTextItem::paint(painter, option, widget);
    }
}

QRectF MyGraphicsTextItem::boundingRect() const
{
    QRectF boundingRectF = m_rect;

    // return QRectF(boundingRectF.x() - 40, boundingRectF.y() - 40, boundingRectF.width() + 80, boundingRectF.height() + 80);
    return m_rect;
}

int MyGraphicsTextItem::type() const
{
    return QGraphicsTextItem::Type;
}

void MyGraphicsTextItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    m_startPos = event->pos();
    if (hasFocus())
    {
        return QGraphicsTextItem::mousePressEvent(event);
    }
    else
    {
        m_isMousePress = true;
        // m_startPos = event->scenePos();
        setTextInteractionFlags(Qt::NoTextInteraction);

        //隐藏场景的鼠标滑动形成的框架
        // qDebug() << "hideRect";
        emit sig_hideRectMouse(true);
        // QGraphicsTextItem::mousePressEvent(event);
    }

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
        m_topOldRect = m_rect;
        M_FLAG = MOV_TOP_LINE;
    }
    else if (m_leftPoly.containsPoint(m_startPos, Qt::WindingFill))
    {
        // setFlags(flags() ^ QGraphicsItem::ItemIsMovable);
        // setCursor(Qt::SizeHorCursor);
        m_leftOldRect = m_rect;
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
    QGraphicsTextItem::mousePressEvent(event);
}

void MyGraphicsTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    // qDebug() << "release";
    // killTimer(m_timeId);
    // setZValue(m_zValue);

    setCursor(Qt::ArrowCursor);
    if (M_FLAG == MOV_RECT)
    {
        M_FLAG = DEFAULT_FLAG;
    }

    if (M_FLAG == MOV_TOP_LINE)
    {
        // M_FLAG = DEFAULT_FLAG;
        setRect(m_topOldRect);

        scene()->update();
        // update();
    }

    if (M_FLAG == MOV_LEFT_LINE)
    {
        setRect(m_leftOldRect);
        scene()->update();
        // update();
    }
    setAcceptHoverEvents(true);
    m_isMousePress = false;
    emit sig_hideRectMouse(true);
    update();
    QGraphicsTextItem::mouseReleaseEvent(event);
}

void MyGraphicsTextItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    // qDebug() << "event->pos(): " << event->pos();
    if (M_FLAG == MOV_RECT)
    {
        QPointF point = (event->pos() - m_startPos);
        moveBy(point.x() * scale(), point.y() * scale());
        setRectSize(m_rect);
        scene()->update();
    }
    else if (M_FLAG == MOV_TOP_LINE)
    {

        // pf求出了矩形的中心点
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

            QRectF newRect(m_rect);
            newRect.setTop(m_rect.bottom() - dis);
            newRect.setBottom(m_rect.bottom());

            setRectSize(newRect);
            // setPos(m_rect.x(), m_rect.bottom() - dis);
            //  m_RotateCenter = QPointF((m_oldRectPolygon.at(0).x() + m_oldRectPolygon.at(2).x()) / 2, (m_oldRectPolygon.at(0).y() + m_oldRectPolygon.at(2).y()) / 2);
            //  m_rect.moveCenter(m_RotateCenter);

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
            setRectSize(newRect);

            // setTextWidth(newRect.right());
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

            setRectSize(newRect);
            setTextWidth(newRect.right());
            // setRectSize(m_rect);
            int width = document()->size().width();
            int height = document()->size().height();

            // qDebug() << "width: height:" << width << "," << height;
            // qDebug() << "rect: width ,height" << m_rect.width() << "," << m_rect.height();

            setRect(QRectF(boundingRect().x(), boundingRect().y(), width, height));

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
            int height = document()->size().height();
            if (newRect.bottom() <= height)
            {
                return;
            }
            setRectSize(newRect);

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
            setTextWidth(newRect.width());
            scene()->update();
        }
    }

    if (hasFocus() || m_isMousePress)
    {
        emit sig_hideRectMouse(false);
        // QGraphicsTextItem::mouseMoveEvent(event);
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
        ////重复的情况，提高ZValue值
        // QList<QGraphicsItem*> list = collidingItems();
        // for (QGraphicsItem* item : list)
        //{
        //     setZValue(item->zValue() + 0.01);
        // }
    }
    QGraphicsTextItem::mouseMoveEvent(event);
}

void MyGraphicsTextItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    // qDebug() << "hover: " << event->pos();
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
    return QGraphicsTextItem::hoverEnterEvent(event);
}

QPainterPath MyGraphicsTextItem::shape() const
{
    // qDebug() << "shape";
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void MyGraphicsTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e)
{

    if (e->button() == Qt::LeftButton)
    {
        // setTextWidth(-1);
        //  qDebug() << "double Mouse";
        setTextInteractionFlags(Qt::TextEditorInteraction);
        setFocus();
    }
    QGraphicsTextItem::mouseDoubleClickEvent(e);
}

void MyGraphicsTextItem::slot_repeat(bool flag)
{
    m_isRepeat = flag;
}
