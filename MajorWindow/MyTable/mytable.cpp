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

    painter->drawRect(m_rect);

    // painter->drawRect(m_topRect);
    // painter->drawRect(m_leftRect);
    // painter->drawRect(m_rightRect);
    // painter->drawRect(m_bottomRect);

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
    //解决编辑和移动的冲突
    return QRectF(m_rect.x() - 5, m_rect.y() - 5, m_rect.width() + 10, m_rect.height() + 10);
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

    m_startPos = event->pos();
    return QGraphicsItem::mousePressEvent(event);
}

void MyTable::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    emit sig_hideRectMouse(true);

    if (M_FLAG == MOV_RECT)
    {
        M_FLAG = DEFAULT_FLAG;
        setCursor(Qt::ArrowCursor);
    }

    return QGraphicsItem::mouseReleaseEvent(event);
}

void MyTable::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    // qDebug() << event->pos();
    //  if (m_topRect.contains(event->pos()) || m_leftRect.contains(event->pos()) || m_rightRect.contains(event->pos()) || m_bottomRect.contains(event->pos()))
    //{
    M_FLAG = MOV_RECT;
    setCursor(Qt::SizeAllCursor);
    //}
    return QGraphicsItem::hoverEnterEvent(event);
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

    MyTableText* minItem = sg_vecJoin.at(0);

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
    m_rectPolygon = getRotatePolygonFromRect(m_rect);

    // m_topRect = QRectF(m_rect.x() + m_rect.width() / 2 - 5, m_rect.y() - 7, 10, 10);
    m_topRect = QRectF(m_rect.x(), m_rect.y(), m_rect.width(), 10);

    m_leftRect = QRectF(m_rect.x(), m_rect.y(), 10, m_rect.height());

    m_rightRect = QRectF(m_rect.right(), m_rect.y(), 10, m_rect.height());

    m_bottomRect = QRectF(m_rect.x(), m_rect.bottom(), m_rect.width(), 10);
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
    Q_UNUSED(option);
    if (m_isSelect == true)
    {
        painter->setBrush(Qt::gray);
        painter->drawRect(m_rect);
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

    return QRectF(m_rect.x(), m_rect.y(), m_rect.width(), m_rect.height());
}

QPainterPath MyTableText::shape() const
{
    QPainterPath path;
    path.addRect(m_rect);
    return path;
}

void MyTableText::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    // qDebug() << "text press";
    if (!m_rect.contains(event->pos()))
    {
        m_isSelect = false;
    }

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

void MyTableText::setRect(QRectF rect)
{
    m_rect = rect;
    intervalH = m_rect.height();
    intervalW = m_rect.width();

    m_oldRectPolygon = getPolygonFromRect(m_rect);
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

void MyTableText::focusInEvent(QFocusEvent* event)
{

    // setCursor(Qt::IBeamCursor);
    return QGraphicsTextItem::focusInEvent(event);
}

void MyTableText::focusOutEvent(QFocusEvent* event)
{

    setCursor(Qt::ArrowCursor);

    return QGraphicsTextItem::focusOutEvent(event);
}

void MyTableText::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    setCursor(Qt::ArrowCursor);
    // return QGraphicsTextItem::hoverMoveEvent(event);
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
    setAcceptHoverEvents(true);
}

QPolygonF MyTableText::getPolygonFromRect(QRectF rectIn)
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
