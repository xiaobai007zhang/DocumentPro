#include "mytable.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QMenu>
#include <QMessageBox>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QTextBlock>
#include <QTextDocument>
#include <qmath.h>

void MyTable::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    // painter->save();
    painter->drawRect(m_rect);
    // painter->setPen(Qt::red);
    // painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
    painter->setBrush(Qt::white);
    painter->setPen(Qt::gray);

    painter->drawEllipse(m_bottomRect);
    painter->drawEllipse(m_topRect);
    painter->drawEllipse(m_rightRect);
    painter->drawEllipse(m_leftRect);

    painter->setPen(Qt::black);
    for (MyTableText* text : m_tableText)
    {
        QRectF rect = text->boundingRect();

        painter->drawRect(text->getX(), text->getY(), rect.width(), rect.height());
        // painter->drawRect(rect);
    }

    // painter->setPen(Qt::blue);
}

QRectF MyTable::boundingRect() const
{
    // return QRectF(m_rect.x() - 3, m_rect.y() - 3, m_rect.width() + 6, m_rect.height() + 6);
    // return m_rect;
    return QRectF(m_rect.x() - 4, m_rect.y() - 4, m_rect.width() + 8, m_rect.height() + 8);
}

QPainterPath MyTable::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    /* QPainterPathStroker pathStroker;

     QPainterPath path = pathStroker.createStroke(temp);*/

    return path;
}

void MyTable::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    // qDebug() << "table press";
    //  emit sig_hideRectMouse(false);
    m_startPos = event->pos();

    if (m_insicedPoly.containsPoint(m_startPos, Qt::WindingFill))
    {

        // M_FLAG = MOV_RECT;
        // setFlag(QGraphicsItem::ItemIsMovable);
        M_FLAG = MOV_RECT;
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
    return QGraphicsItem::mousePressEvent(event);
}

void MyTable::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    // qDebug() << "table release: " << event->pos();
    emit sig_hideRectMouse(true);
    return QGraphicsItem::mouseReleaseEvent(event);
}

void MyTable::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{

    if (m_insicedPoly.containsPoint(event->pos(), Qt::WindingFill))
    {
        setCursor(Qt::SizeAllCursor);
    }
    else if (m_topPoly.containsPoint(event->pos(), Qt::WindingFill))
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
    QGraphicsItem::hoverEnterEvent(event);
}

void MyTable::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{

    emit sig_hideRectMouse(false);

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
            // qDebug() << "dis: " << m_rect.bottom() - dis;
            emit sig_updateSize(MOV_TOP_LINE, m_rect.bottom() - dis);
            // qDebug() << "bottom: " << m_rect.bottom();
            QRectF newRect(m_rect);
            newRect.setTop(m_rect.bottom() - dis);
            newRect.setBottom(m_rect.bottom());
            setRect(newRect);

            // m_RotateCenter = QPointF((m_oldRectPolygon.at(0).x() + m_oldRectPolygon.at(2).x()) / 2, (m_oldRectPolygon.at(0).y() + m_oldRectPolygon.at(2).y()) / 2);
            // m_rect.moveCenter(m_RotateCenter);
            // setRect(m_rect);
            scene()->update(); //必须要用scene()->update()，不能用update();否则会出现重影

            //所有的孩子的高度都要变
            /* for (MyTableText* text : m_tableText)
             {
                 text->setRect(QRectF(text->getRect().x(), text->getRect().y(), text->intervalW, text->intervalH));
             }*/
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

    return QGraphicsItem::mouseMoveEvent(event);
}

void MyTable::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    return QGraphicsItem::mouseDoubleClickEvent(event);
}

int MyTable::type() const
{
    return TABLE_TYPE;
}

void MyTable::focusOutEvent(QFocusEvent* event)
{
    Q_UNUSED(event)
    // qDebug() << TR("table less focus");
}

QVariant MyTable::itemChange(GraphicsItemChange change, const QVariant& value)
{

    return QGraphicsItem::itemChange(change, value);
}

qreal MyTable::getIntervalW()
{
    return intervalW;
}

qreal MyTable::getIntervalH()
{
    return intervalH;
}

void MyTable::keyPressEvent(QKeyEvent* event)
{
    // qDebug() << "table key press";

    //在子类进行删除的时候进行判断
    // if (event->key() == Qt::Key_Backspace)
    //{
    //    QGraphicsItem* item = focusItem();
    //    MyTableText* text = dynamic_cast<MyTableText*>(item);

    //    if (text == nullptr)
    //    {
    //        return;
    //    }
    //    if (text->document()->blockCount() == 1 && text->document()->firstBlock().text().length() == 1)
    //    {
    //        return;
    //    }

    //    int height = text->intervalH;
    //    int docH = text->document()->size().height();

    //    int data = text->data(Qt::UserRole + 1).toInt();
    //    int maxVal = docH;
    //    int curVal = docH;
    //    QFontMetrics metrics(text->font());
    //    //如果文档内容小于当前的高度
    //    if (docH < height)
    //    {

    //        // if (m_isJoin)
    //        //{

    //        //} //没有合并过就按照原先的方式
    //        // else
    //        { //分为两种情况，一种是当前编辑的框的长度是最长的,那就把当前的行和总长度都要缩短，
    //            //第二种情况是当前行中第二或者第三长的，那就不管
    //            for (QGraphicsItem* item : childItems())
    //            {
    //                MyTableText* tmp = dynamic_cast<MyTableText*>(item);
    //                if (tmp->data(Qt::UserRole + 1).toInt() == data)
    //                {
    //                    if (maxVal < tmp->document()->size().height())
    //                    {
    //                        maxVal = tmp->document()->size().height();
    //                    }
    //                }
    //            }

    //            //如果相等，证明是当前一层中最高的,需要整体缩短
    //            if (maxVal == curVal)
    //            {
    //                qreal adjust = metrics.height();
    //                for (QGraphicsItem* item : childItems())
    //                {
    //                    MyTableText* tmp = dynamic_cast<MyTableText*>(item);
    //                    if (tmp->data(Qt::UserRole + 1).toInt() == data)
    //                    {
    //                        QRectF rect = tmp->getRect();
    //                        tmp->setRect(QRectF(rect.x(), rect.y(), rect.width(), rect.height() - adjust));
    //                    }
    //                    else if (tmp->data(Qt::UserRole + 1).toInt() > data)
    //                    {
    //                        QRectF rect = tmp->getRect();
    //                        tmp->moveBy(rect.x(), rect.y() - adjust);
    //                    }
    //                }
    //                setRect(QRectF(m_rect.x(), m_rect.y(), m_rect.width(), m_rect.height() - adjust));
    //            }
    //            else
    //            {
    //                //啥也不干
    //            }
    //        }
    //    }
    //}
    // qDebug() << "scene keypress";
    return QGraphicsItem::keyPressEvent(event);
}

void MyTable::initTableWidget()
{
    setFlags(QGraphicsItem::ItemIsSelectable);
    setAcceptHoverEvents(true);
    setFocus();
}

MyTableText* MyTable::findItem(int row, int col)
{

    for (MyTableText* text : sg_vecJoin)
    {
        if (text->getRow() == row && text->getCol() == col)
        {
            return text;
        }
    }

    return nullptr;
}

QPolygonF MyTable::getRotatePolygonFromRect(QRectF rectIn)
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

void MyTable::slot_MyTable(QRectF rect)
{
    Q_UNUSED(rect)
    // qDebug() << TR("清空");
    sg_vecJoin.clear();
    for (MyTableText* text : m_tableText)
    {
        text->m_isSelect = false;
    }

    // emit sig_changeSelect();
    //   prepareGeometryChange();
    //   scene()->update();
}

void MyTable::slot_joinTable()
{

    // qDebug() << "triggered";
    if (sg_vecJoin.empty() || sg_vecJoin.size() == 1)
    {
        // QMessageBox::information(nullptr, TR("Tips"), TR("当前选中的单元格为空！"));
        return;
    }

    qreal maxH = 0;
    qreal maxW = 0;

    // MyTableText* minItem = *sg_vecJoin.begin();
    MyTableText* minItem = sg_vecJoin.at(0);
    // MyTableText* minItem = new MyTableText(QRectF(0, 0, 0, 0));

    std::vector<MyTableText*> tmpVec;
    //求出最小的x
    for (MyTableText* item : sg_vecJoin)
    {

        if (minItem->x() > item->x())
        {
            minItem = item;
        }
    }
    for (MyTableText* item : sg_vecJoin)
    {

        if (minItem->x() == item->x() && item != minItem)
        {
            tmpVec.push_back(item);
        }
    }

    for (MyTableText* item : tmpVec)
    {
        if (minItem->y() > item->y())
        {
            minItem = item;
        }
    }

    //右下角的文本框
    MyTableText* maxTableXY = minItem;

    //找出xy中最大的一个，当作右下角数据
    for (MyTableText* item : sg_vecJoin)
    {
        if (maxTableXY->y() <= item->y() && maxTableXY->x() <= item->x())
        {
            maxTableXY = item;
        }
    }

    // int orw = maxTableXY->getRow();
    for (int i = minItem->getRow(); i <= maxTableXY->getRow(); ++i)
    {
        //写一个函数把i , j 传进去，找到对应的节点，然后返回，
        MyTableText* item = findItem(i, minItem->getCol());
        if (item)
        {
            maxH += item->intervalH;
        }
    }

    for (int j = minItem->getCol(); j <= maxTableXY->getCol(); ++j)
    {
        //写一个函数把i , j 传进去，找到对应的节点，然后返回，
        MyTableText* item = findItem(minItem->getRow(), j);
        if (item)
        {
            maxW += item->intervalW;
        }
    }

    if (maxH == 0 || maxW == 0)
    {
        return;
    }

    minItem->intervalH = maxH;
    minItem->intervalW = maxW;
    minItem->setTextWidth(maxW);

    // minItem->setData(Qt::UserRole + 1, maxTableXY->data(Qt::UserRole + 1).toInt());
    minItem->setRect(QRectF(minItem->getRect().x(), minItem->getRect().y(), maxW, maxH));

    for (MyTableText* item : sg_vecJoin)
    {
        if (item != minItem)
        {
            m_tableText.erase(std::find(m_tableText.begin(), m_tableText.end(), item));
            delete item;
        }
    }
    sg_vecJoin.clear();
    minItem->m_isSelect = false;
}

void MyTable::slot_repeat(bool flag)
{
    m_isRepeat = flag;
}

void MyTable::setRect(QRectF rect)
{
    m_rect = rect;
    m_oldRectPolygon = getRotatePolygonFromRect(m_rect);

    // m_topRect = QRectF(m_rect.x() + m_rect.width() / 2 - 5, m_rect.y() - 7, 10, 10);
    m_topRect = QRectF(m_rect.x() + m_rect.width() / 2 - 5, m_rect.y() - 10, 10, 10);
    m_topPoly = getRotatePolygonFromRect(m_topRect);

    m_leftRect = QRectF(m_rect.x() - 10, m_rect.y() + m_rect.height() / 2 - 5, 10, 10);
    m_leftPoly = getRotatePolygonFromRect(m_leftRect);

    m_rightRect = QRectF(m_rect.right(), m_rect.y() + m_rect.height() / 2 - 5, 10, 10);
    m_rightPoly = getRotatePolygonFromRect(m_rightRect);

    m_bottomRect = QRectF(m_rect.x() + m_rect.width() / 2 - 5, m_rect.bottom(), 10, 10);
    m_bottomPoly = getRotatePolygonFromRect(m_bottomRect);

    m_insicedRectf = QRectF(m_rect.x() - 2, m_rect.y() - 2, m_rect.width() + 4, m_rect.height() + 4);
    // m_insicedRectf = QRectF(m_rect.x(), m_rect.y(), m_rect.width(), m_rect.height());
    m_insicedPoly = getRotatePolygonFromRect(m_insicedRectf);
}

MyTable::MyTable(int row, int col, QRectF rect) : intervalW(0), intervalH(0)
{
    m_row = row;
    m_col = col;
    m_rect = rect;
    intervalW = m_rect.width() / m_col;
    intervalH = m_rect.height() / m_row;
    // setFocus();

    initTableWidget();
    setRect(m_rect);
}

MyTable::~MyTable()
{
}

QRectF MyTable::getRect()
{
    return m_rect;
}

int MyTable::getRow()
{
    return m_row;
}

int MyTable::getCol()
{
    return m_col;
}

void MyTable::slot_contentsChanged()
{
    // setFocus();
    // QGraphicsItem* itemFocus = focusItem();
    // MyTableText* child = dynamic_cast<MyTableText*>(itemFocus);
    // if (child == nullptr)
    //{
    //    return;
    //}
    // int height = child->intervalH;
    // int docH = child->document()->size().height();
    // QFontMetrics metrics(child->font());

    // if (docH > height)
    //{
    //     qreal adjust = metrics.height();
    //     QRectF childRect = child->getRect();

    //    child->setRect(QRectF(childRect.x(), childRect.y(), childRect.width(), childRect.height() + adjust));
    //    setRect(QRectF(m_rect.x(), m_rect.y(), m_rect.width(), m_rect.height() + adjust));

    //    //当更新文本框后，光标的位置设置，文本的末尾
    //    /* QTextCursor cursor = child->textCursor();
    //     cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
    //     child->setTextCursor(cursor);*/

    //    int tmpData = child->data(Qt::UserRole + 1).toInt();

    //    for (QGraphicsItem* item : childItems())
    //    {
    //        MyTableText* tmp = dynamic_cast<MyTableText*>(item);
    //        if (tmp->data(Qt::UserRole + 1).toInt() == tmpData && tmp != child)
    //        {
    //            QRectF tmpRect = tmp->getRect();
    //            tmp->setRect(QRectF(tmpRect.x(), tmpRect.y(), tmpRect.width(), tmpRect.height() + adjust));
    //        }
    //    }

    //    //设置向下偏移
    //    int data = 20;
    //    for (QGraphicsItem* item : childItems())
    //    {
    //        MyTableText* tmp = dynamic_cast<MyTableText*>(item);
    //        if (tmp->hasFocus())
    //        {
    //            data = tmp->data(Qt::UserRole + 1).toInt();
    //        }

    //        if (tmp->data(Qt::UserRole + 1).toInt() > data)
    //        {
    //            QRectF rect = tmp->getRect();

    //            // tmp->setRect(QRectF(rect.x(), rect.y() + adjust + adjust, rect.width(), rect.height()));
    //            tmp->moveBy(rect.x(), rect.y() + adjust);
    //            // tmp->setRect(QRectF(rect.x(), rect.y() + adjust, tmp->intervalW, tmp->intervalH));
    //        }
    //    }
    //}
}

//=======================================================================================
//=======================================================================================
MyTableText::MyTableText(QRectF rect, QGraphicsItem* parent) : QGraphicsTextItem(parent)
{
    m_rect = rect;

    initMyTableText();
}

MyTableText::~MyTableText()
{
}

void MyTableText::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    // painter->drawText(0, 0, "123");
    // Q_UNUSED(option)
    // QPen pen;
    // pen.setColor(Qt::red);
    // painter->setPen(pen);
    // painter->drawRect(m_rect);
    //// painter->setRenderHint(QPainter::Antialiasing);
    // painter->setRenderHint(QPainter::SmoothPixmapTransform);
    // painter->save();
    //// painter->drawRect(m_rect);
    ////  painter->setBrush(Qt::gray);
    ////  painter->drawEllipse(m_rect.x() + m_rect.width() / 2, m_rect.y() - 5, 10, 10);
    if (m_isSelect == true)
    {
        painter->setBrush(Qt::gray);
        painter->drawRect(m_rect);
    }
    else
    {
        painter->setBrush(Qt::black);
    }
    //// 原来什么属性就要什么属性,只不过去掉多余的选中状态
    QStyleOptionGraphicsItem op;
    op.initFrom(widget);
    op.state = QStyle::State_None;
    // painter->restore();
    QGraphicsTextItem::paint(painter, &op, widget);
}

QRectF MyTableText::boundingRect() const
{
    return m_rect;
}

QPainterPath MyTableText::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void MyTableText::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    // qDebug() << "text press";
    if (!m_rect.contains(event->pos()))
    {
        m_isSelect = false;
    }
    // m_isSelect = true;
    //  setTextInteractionFlags(Qt::TextEditorInteraction);
    //   setFocus();
    return QGraphicsTextItem::mousePressEvent(event);
}

void MyTableText::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    // emit sig_hideRectMouse(false);
    return QGraphicsTextItem::mouseMoveEvent(event);
}

void MyTableText::slot_changeSelect()
{
    m_isSelect = false;
}

void MyTableText::slot_updateSize(STATE_FLAG flags, qreal distance)
{
    M_FLAG = flags;
    if (M_FLAG == MOV_TOP_LINE)
    {
        // qDebug() << "distance" << distance;
        //  pf求出了矩形的中心点
        QPointF pf = QPointF((m_oldRectPolygon.at(2).x() + m_oldRectPolygon.at(3).x()) / 2, ((m_oldRectPolygon.at(2).y() + m_oldRectPolygon.at(3).y()) / 2));

        //最终dis求出来的是，鼠标移动对于矩形中心点的距离
        qreal dis = distance;

        // qDebug() << "dis: " << dis;
        //  qDebug() << "bottom: " << m_rect.bottom();
        QRectF newRect(m_rect);
        newRect.setTop(m_rect.top() + dis);
        newRect.setBottom(m_rect.bottom());
        //// qDebug() << "y: " << getY();
        // setPos(getX(), getY());
        //  setRect(newRect);

        // m_RotateCenter = QPointF((m_oldRectPolygon.at(0).x() + m_oldRectPolygon.at(2).x()) / 2, (m_oldRectPolygon.at(0).y() + m_oldRectPolygon.at(2).y()) / 2);
        // m_rect.moveCenter(m_RotateCenter);
        // setRect(m_rect);
        scene()->update(); //必须要用scene()->update()，不能用update();否则会出现重影
    }
    // else if (M_FLAG == MOV_LEFT_LINE)
    //{
    //     QPointF pf = QPointF((m_oldRectPolygon.at(1).x() + m_oldRectPolygon.at(2).x()) / 2, ((m_oldRectPolygon.at(1).y() + m_oldRectPolygon.at(2).y()) / 2));
    //     //计算到右侧边中点的距离
    //     qreal dis = sqrt((event->pos().x() - pf.x()) * (event->pos().x() - pf.x()) + (event->pos().y() - pf.y()) * (event->pos().y() - pf.y()));
    //     qreal dis2LT = sqrt((event->pos().x() - m_oldRectPolygon.at(0).x()) * (event->pos().x() - m_oldRectPolygon.at(0).x()) +
    //                         (event->pos().y() - m_oldRectPolygon.at(0).y()) * (event->pos().y() - m_oldRectPolygon.at(0).y()));
    //     qreal dis2RT = sqrt((event->pos().x() - m_oldRectPolygon.at(1).x()) * (event->pos().x() - m_oldRectPolygon.at(1).x()) +
    //                         (event->pos().y() - m_oldRectPolygon.at(1).y()) * (event->pos().y() - m_oldRectPolygon.at(1).y()));
    //     if (dis < 20 || dis2LT > dis2RT)
    //     {
    //         return;
    //     }
    //     else
    //     {
    //         QRectF newRect(m_rect);
    //         newRect.setLeft(m_rect.right() - dis);
    //         newRect.setRight(m_rect.right());
    //         setRect(newRect);

    //        // setRect(m_rect);
    //        scene()->update(); //必须要用scene()->update()，不能用update();否则会出现重影
    //    }
    //}
    // else if (M_FLAG == MOV_RIGHT_LINE)
    //{
    //    QPointF pf = QPointF((m_oldRectPolygon.at(0).x() + m_oldRectPolygon.at(3).x()) / 2, ((m_oldRectPolygon.at(0).y() + m_oldRectPolygon.at(3).y()) / 2));
    //    //计算到左侧边中点的距离
    //    qreal dis = sqrt((event->pos().x() - pf.x()) * (event->pos().x() - pf.x()) + (event->pos().y() - pf.y()) * (event->pos().y() - pf.y()));
    //    qreal dis2LT = sqrt((event->pos().x() - m_oldRectPolygon.at(0).x()) * (event->pos().x() - m_oldRectPolygon.at(0).x()) +
    //                        (event->pos().y() - m_oldRectPolygon.at(0).y()) * (event->pos().y() - m_oldRectPolygon.at(0).y()));
    //    qreal dis2RT = sqrt((event->pos().x() - m_oldRectPolygon.at(1).x()) * (event->pos().x() - m_oldRectPolygon.at(1).x()) +
    //                        (event->pos().y() - m_oldRectPolygon.at(1).y()) * (event->pos().y() - m_oldRectPolygon.at(1).y()));
    //    if (dis < 20 || dis2LT < dis2RT)
    //    {
    //        return;
    //    }
    //    else
    //    {
    //        QRectF newRect(m_rect);
    //        newRect.setLeft(m_rect.left());
    //        newRect.setRight(m_rect.left() + dis);
    //        setRect(newRect);

    //        // setRect(m_rect);
    //        scene()->update(); //必须要用scene()->update()，不能用update();否则会出现重影
    //    }
    //}
    // else if (M_FLAG == MOV_BOTTOM_LINE)
    //{
    //    //顶边中点
    //    QPointF pf = QPointF((m_oldRectPolygon.at(0).x() + m_oldRectPolygon.at(1).x()) / 2, ((m_oldRectPolygon.at(0).y() + m_oldRectPolygon.at(1).y()) / 2));
    //    //计算到底边中点的距离
    //    qreal dis = sqrt((event->pos().x() - pf.x()) * (event->pos().x() - pf.x()) + (event->pos().y() - pf.y()) * (event->pos().y() - pf.y()));
    //    qreal dis2LT = sqrt((event->pos().x() - m_oldRectPolygon.at(0).x()) * (event->pos().x() - m_oldRectPolygon.at(0).x()) +
    //                        (event->pos().y() - m_oldRectPolygon.at(0).y()) * (event->pos().y() - m_oldRectPolygon.at(0).y()));
    //    qreal dis2LB = sqrt((event->pos().x() - m_oldRectPolygon.at(3).x()) * (event->pos().x() - m_oldRectPolygon.at(3).x()) +
    //                        (event->pos().y() - m_oldRectPolygon.at(3).y()) * (event->pos().y() - m_oldRectPolygon.at(3).y()));
    //    if (dis < 20 || dis2LT < dis2LB)
    //    {
    //        return;
    //    }
    //    else
    //    {
    //        QRectF newRect(m_rect);
    //        newRect.setTop(m_rect.top());
    //        newRect.setBottom(m_rect.top() + dis);
    //        setRect(newRect);
    //        // m_RotateCenter = QPointF((m_oldRectPolygon.at(0).x() + m_oldRectPolygon.at(2).x()) / 2, (m_oldRectPolygon.at(0).y() + m_oldRectPolygon.at(2).y()) / 2);
    //        // m_oldRect.moveCenter(m_RotateCenter);
    //        // setRect(m_rect);
    //        scene()->update(); //必须要用scene()->update()，不能用update();否则会出现重影
    //    }
    //}
    // else if (M_FLAG == MOV_RIGHTBOTTOM_RECT)
    //{
    //    //中心坐标点
    //    QPointF pf = QPointF((m_oldRectPolygon.at(0).x() + m_oldRectPolygon.at(1).x()) / 2, ((m_oldRectPolygon.at(0).y() + m_oldRectPolygon.at(1).y()) / 2));

    //    qreal dis = sqrt((event->pos().x() - pf.x()) * (event->pos().x() - pf.x()) + (event->pos().y() - pf.y()) * (event->pos().y() - pf.y()));
    //    qreal dis2LT = sqrt((event->pos().x() - m_oldRectPolygon.at(0).x()) * (event->pos().x() - m_oldRectPolygon.at(0).x()) +
    //                        (event->pos().y() - m_oldRectPolygon.at(0).y()) * (event->pos().y() - m_oldRectPolygon.at(0).y()));
    //    qreal dis2RB = sqrt((event->pos().x() - m_oldRectPolygon.at(3).x()) * (event->pos().x() - m_oldRectPolygon.at(2).x()) +
    //                        (event->pos().y() - m_oldRectPolygon.at(3).y()) * (event->pos().y() - m_oldRectPolygon.at(2).y()));

    //    QPointF pf2 = QPointF((m_oldRectPolygon.at(0).x() + m_oldRectPolygon.at(3).x()) / 2, ((m_oldRectPolygon.at(0).y() + m_oldRectPolygon.at(3).y()) / 2));
    //    qreal dis2 = sqrt((event->pos().x() - pf2.x()) * (event->pos().x() - pf2.x()) + (event->pos().y() - pf2.y()) * (event->pos().y() - pf2.y()));

    //    if (dis < 20 || dis2LT < dis2RB)
    //    {
    //        return;
    //    }
    //    else
    //    {
    //        // qDebug() << "m_rect: bottom: " << m_rect.bottom() << ", right: " << m_rect.right();
    //        QRectF newRect(m_rect);
    //        // qDebug() << "dis1: " << dis;
    //        // qDebug() << "dis2: " << dis2;
    //        newRect.setBottom(m_rect.top() + dis);
    //        newRect.setRight(m_rect.left() + dis2);
    //        setRect(newRect);

    //        scene()->update();
    //    }
    //}
}

void MyTableText::setRect(QRectF rect)
{
    m_rect = rect;
    intervalH = m_rect.height();
    intervalW = m_rect.width();

    m_oldRectPolygon = getRotatePolygonFromRect(m_rect);
}

void MyTableText::setIndex(int row, int col)
{
    m_row = row;
    m_col = col;
}

int MyTableText::getRow()
{
    return m_row;
}

int MyTableText::getCol()
{
    return m_col;
}

void MyTableText::setX(int sx)
{
    m_x = sx;
}

void MyTableText::setY(int sy)
{
    m_y = sy;
}

int MyTableText::getX()
{
    return m_x;
}

int MyTableText::getY()
{
    return m_y;
}

QRectF MyTableText::getRect()
{
    return m_rect;
}

void MyTableText::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    return QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void MyTableText::keyPressEvent(QKeyEvent* event)
{

    // qDebug() << "child keyPress";
    //  MyTable* table = dynamic_cast<MyTable*>(parentItem());
    //  table->keyPressEvent(event);

    return QGraphicsTextItem::keyPressEvent(event);
}

void MyTableText::focusInEvent(QFocusEvent* event)
{
    // qDebug() << "text focusIn event";
    setCursor(Qt::IBeamCursor);
    return QGraphicsTextItem::focusInEvent(event);
}

void MyTableText::focusOutEvent(QFocusEvent* event)
{
    // qDebug() << "text focusOut event";
    //   setSelected(false);
    setCursor(Qt::ArrowCursor);

    // setTextInteractionFlags(Qt::NoTextInteraction);
    return QGraphicsTextItem::focusOutEvent(event);
}

void MyTableText::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    event->ignore();
}

int MyTableText::type() const
{
    return TABLE_TYPE;
}

void MyTableText::initMyTableText()
{
    intervalH = m_rect.height();
    intervalW = m_rect.width();
    setTextInteractionFlags(Qt::TextEditorInteraction);
    document()->setTextWidth(intervalW);
    setRect(m_rect);
}

QPolygonF MyTableText::getRotatePolygonFromRect(QRectF rectIn)
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

void MyTableText::slot_MyTable(QRectF rect)
{
    QGraphicsItem* parentTmp = parentItem();
    MyTable* parent = dynamic_cast<MyTable*>(parentTmp);

    // qDebug() << "rect: " << rect;
    // m_isSelect = false;
    QPointF point = this->mapFromScene(rect.x(), rect.y());

    QPainterPath path;

    QRectF rect2;
    QSize size(rect.size().toSize());
    if (size.height() > 0 && size.width() > 0)
    {
        rect2.setRect(point.toPoint().x() - size.width(), point.toPoint().y() - size.height(), size.width(), size.height());
    }

    if (size.height() < 0 && size.width() < 0)
    {
        size = QSize(abs(size.width()), abs(size.height()));
        rect2.setRect(point.toPoint().x(), point.toPoint().y(), size.width(), size.height());
    }

    if (size.height() < 0 && size.width() > 0)
    {
        size = QSize(abs(size.width()), abs(size.height()));
        rect2.setRect(point.toPoint().x() - size.width(), point.toPoint().y(), size.width(), size.height());
    }

    if (size.height() > 0 && size.width() < 0)
    {
        size = QSize(abs(size.width()), abs(size.height()));
        rect2.setRect(point.toPoint().x(), point.toPoint().y() - size.height(), size.width(), size.height());
    }

    path.addRect(rect2);
    // qDebug() << rect2;
    //碰撞检测路径
    if (collidesWithPath(path))
    {

        for (MyTableText* text : parent->sg_vecJoin)
        {
            //遍历一下有没有自己，如果有自己的话就退出
            if (this == text)
            {
                return;
            }
        }
        // sg_isSelect = true;
        m_isSelect = true;
        parent->sg_vecJoin.push_back(this);
        return;
    }
    m_isSelect = false;
}
