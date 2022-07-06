#include "mytable.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QMenu>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QTextBlock>
#include <QTextDocument>

void MyTable::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    //进行绘画出对应的表格
    //暂时决定，将鼠标聚焦放到对应的位置进行双击会出现编辑框
    // qDebug() << "interval H: " << intervalH;

    painter->drawRect(m_rect);

    //画线条需要判断每一行最长的高度来决定
    int maxVal = m_tableText[0]->intervalH;
    int oldMax = 0;

    for (int i = 0; i < m_tableText.size(); ++i)
    {
        if (i % m_col != 0)
        {
            if (maxVal < m_tableText[i]->intervalH)
            {
                maxVal = m_tableText[i]->intervalH;
            }
        }
        else if (i != 0)
        {
            //比较完成一轮了，开始绘画
            painter->drawLine(0, oldMax + maxVal, m_rect.width(), oldMax + maxVal);

            // val++;
            oldMax += maxVal;
            maxVal = m_tableText[i]->intervalH;
            //还需要再次比较一次
        }
    }

    // for (int i = 1; i < m_row; ++i)
    //{
    //     painter->drawLine(0, i * intervalH, 0 + m_rect.width(), i * intervalH);
    // }

    for (int i = 1; i < m_col; ++i)
    {
        painter->drawLine(i * intervalW, 0, i * intervalW, 0 + m_rect.height());
    }
    // QGraphicsItem::paint(painter, option, widget);
}

QRectF MyTable::boundingRect() const
{
    return QRectF(m_rect.x() - 3.5, m_rect.y() - 3.5, m_rect.width() + 7, m_rect.height() + 7);
}

QPainterPath MyTable::shape() const
{
    QPainterPath temp;
    temp.addRect(boundingRect());
    QPainterPathStroker pathStroker;

    QPainterPath path = pathStroker.createStroke(temp);
    return path;
}

void MyTable::mousePressEvent(QGraphicsSceneMouseEvent* event)
{

    qDebug() << "table pressL " << event->pos();
    emit sig_hideRectMouse(false);
    // setTextInteractionFlags(Qt::NoTextInteraction);
    return QGraphicsItem::mousePressEvent(event);
}

void MyTable::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << "table release: " << event->pos();
    emit sig_hideRectMouse(true);
    return QGraphicsItem::mouseReleaseEvent(event);
}

void MyTable::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << "table move";

    emit sig_hideRectMouse(false);
    return QGraphicsItem::mouseMoveEvent(event);
}

void MyTable::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << "table double press";
    setHandlesChildEvents(true);
    event->ignore();
    return QGraphicsItem::mouseDoubleClickEvent(event);
}

void MyTable::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    // qDebug() << "hover";
    // qDebug() << event->pos();
    if (shape().contains(event->pos()))
        setCursor(Qt::SizeAllCursor);
}

bool MyTable::eventFilter(QObject* obj, QEvent*)
{
    for (QGraphicsItem* item : childItems())
    {
        MyTableText* tmp = dynamic_cast<MyTableText*>(item);
        MyTableText* tmp2 = dynamic_cast<MyTableText*>(obj);
        if (tmp2 == tmp)
        {
            return false;
        }
    }

    return true;
}

void MyTable::keyReleaseEvent(QKeyEvent* event)
{
    qDebug() << "table key release ";
    if (event->key() == Qt::Key_Alt)
    {
        m_altPress = false;
        for (QGraphicsItem* item : childItems())
        {
            item->setFlags(item->flags() & ~ItemIsSelectable);
            for (int i = 0; i < m_vecJoin.size(); ++i)
            {
                qDebug() << m_vecJoin.at(i);
            }
        }
    }
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
    qDebug() << "table key press";
    if (event->key() == Qt::Key_Alt)
    {
        m_altPress = true;
        for (QGraphicsItem* item : childItems())
        {
            item->setFlag(QGraphicsItem::ItemIsSelectable);
        }
    }

    //在子类进行删除的时候进行判断
    if (event->key() == Qt::Key_Backspace)
    {
        QGraphicsItem* item = focusItem();
        MyTableText* text = dynamic_cast<MyTableText*>(item);
        if (text->document()->blockCount() == 1 && text->document()->firstBlock().text().length() == 1)
        {
            return;
        }

        int height = text->intervalH;
        int docH = text->document()->size().height();

        int data = text->data(Qt::UserRole + 1).toInt();
        int maxVal = docH;
        int curVal = docH;
        QFontMetrics metrics(text->font());
        //如果文档内容小于当前的高度
        if (docH < height)
        {
            //分为两种情况，一种是当前编辑的框的长度是最长的,那就把当前的行和总长度都要缩短，
            //第二种情况是当前行中第二或者第三长的，那就不管
            for (QGraphicsItem* item : childItems())
            {
                MyTableText* tmp = dynamic_cast<MyTableText*>(item);
                if (tmp->data(Qt::UserRole + 1).toInt() == data)
                {
                    if (maxVal < tmp->document()->size().height())
                    {
                        maxVal = tmp->document()->size().height();
                    }
                }
            }

            //如果相等，证明是当前一层中最高的,需要整体缩短
            if (maxVal == curVal)
            {
                qreal adjust = metrics.height();
                for (QGraphicsItem* item : childItems())
                {
                    MyTableText* tmp = dynamic_cast<MyTableText*>(item);
                    if (tmp->data(Qt::UserRole + 1).toInt() == data)
                    {
                        QRectF rect = tmp->getRect();
                        tmp->setRect(QRectF(rect.x(), rect.y(), rect.width(), rect.height() - adjust));
                    }
                    else if (tmp->data(Qt::UserRole + 1).toInt() > data)
                    {
                        QRectF rect = tmp->getRect();
                        tmp->moveBy(rect.x(), rect.y() - adjust);
                    }
                }
                setRect(QRectF(m_rect.x(), m_rect.y(), m_rect.width(), m_rect.height() - adjust));
            }
            else
            {
                //啥也不干
            }
        }
    }
    return QGraphicsItem::keyPressEvent(event);
}

void MyTable::initTableWidget()
{
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    setAcceptHoverEvents(true);
    setFocus();
    // installEventFilter(this);
}

void MyTable::slot_MyTable(QRectF rect)
{
    qDebug() << "MyTable recv: " << rect;
    qDebug() << "mapfromscene pos: " << this->mapFromScene(rect);
}

void MyTable::setRect(QRectF rect)
{
    m_rect = rect;
}

MyTable::MyTable(int row, int col, QRectF rect) : m_altPress(false), intervalW(0), intervalH(0)
{
    m_row = row;
    m_col = col;
    m_rect = rect;
    intervalW = m_rect.width() / m_col;
    intervalH = m_rect.height() / m_row;
    // setFocus();

    initTableWidget();
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
    MyTableText* child;

    //找到当前正在编辑的子项
    for (QGraphicsItem* item : childItems())
    {
        if (item->hasFocus())
        {
            child = dynamic_cast<MyTableText*>(item);
            if (child == nullptr)
            {
                assert(0);
            }
            break;
        }
    }

    int height = child->intervalH;
    int docH = child->document()->size().height();
    QFontMetrics metrics(child->font());

    if (docH > height)
    {
        qreal adjust = metrics.height();
        QRectF childRect = child->getRect();

        child->setRect(QRectF(childRect.x(), childRect.y(), childRect.width(), childRect.height() + adjust));
        setRect(QRectF(m_rect.x(), m_rect.y(), m_rect.width(), m_rect.height() + adjust));

        //当更新文本框后，光标的位置设置，文本的末尾
        /* QTextCursor cursor = child->textCursor();
         cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
         child->setTextCursor(cursor);*/

        int tmpData = child->data(Qt::UserRole + 1).toInt();

        for (QGraphicsItem* item : childItems())
        {
            MyTableText* tmp = dynamic_cast<MyTableText*>(item);
            if (tmp->data(Qt::UserRole + 1).toInt() == tmpData && tmp != child)
            {
                QRectF tmpRect = tmp->getRect();
                tmp->setRect(QRectF(tmpRect.x(), tmpRect.y(), tmpRect.width(), tmpRect.height() + adjust));
            }
        }

        //设置向下偏移
        int data = 20;
        for (QGraphicsItem* item : childItems())
        {
            MyTableText* tmp = dynamic_cast<MyTableText*>(item);
            if (tmp->hasFocus())
            {
                data = tmp->data(Qt::UserRole + 1).toInt();
            }

            if (tmp->data(Qt::UserRole + 1).toInt() > data)
            {
                QRectF rect = tmp->getRect();

                // tmp->setRect(QRectF(rect.x(), rect.y() + adjust + adjust, rect.width(), rect.height()));
                tmp->moveBy(rect.x(), rect.y() + adjust);
            }
        }
    }
}

//=======================================================================================
//=======================================================================================
MyTableText::MyTableText(QRectF rect, QGraphicsItem* parent) : QGraphicsTextItem(parent), m_altPress(false)
{
    m_rect = rect;
    // installEventFilter(this);
    initMyTableText();
}

MyTableText::~MyTableText()
{
}

void MyTableText::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    QPen pen;
    pen.setColor(Qt::red);
    painter->setPen(pen);
    painter->drawRect(m_rect);

    {
        // 原来什么属性就要什么属性,只不过去掉多余的选中状态
        QStyleOptionGraphicsItem op;
        op.initFrom(widget);
        op.state = QStyle::State_None;
        QGraphicsTextItem::paint(painter, &op, widget);
    }

    // QGraphicsTextItem::paint(painter, option, widget);
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

    // if (hasFocus())
    //{
    //     return QGraphicsTextItem::mousePressEvent(event);
    // }
    // else
    //{
    //     // setSelected(true);
    //     //  qDebug() << "table text: mousePressEvent";
    //     // setTextInteractionFlags(Qt::NoTextInteraction);
    //     emit sig_hideRectMouse(false);

    //    MyTable* table = dynamic_cast<MyTable*>(parentItem());
    //    table->mousePressEvent(event);
    //    return QGraphicsTextItem::mousePressEvent(event);
    //}
    QGraphicsTextItem::mousePressEvent(event);
}

void MyTableText::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << "text: mouseReleaseEvent";

    MyTable* table = dynamic_cast<MyTable*>(parentItem());
    table->mouseReleaseEvent(event);
    return QGraphicsTextItem::mouseReleaseEvent(event);
}

void MyTableText::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    // qDebug() << "table text: mouseMoveEvent";

    emit sig_hideRectMouse(false);
    return QGraphicsTextItem::mouseMoveEvent(event);
}

// MyTableText::wheelEvent(QGraphicsSceneWheelEvent* event)
//{
// }

void MyTableText::setRect(QRectF rect)
{
    m_rect = rect;
    intervalH = m_rect.height();
}

QRectF MyTableText::getRect()
{
    return m_rect;
}

void MyTableText::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    // qDebug() << "table text: mouseDoubleClickEvent";
    event->accept();
    if (event->button() == Qt::LeftButton)
    {
        setTextInteractionFlags(Qt::TextEditorInteraction);
        setFocus();
    }

    return QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void MyTableText::keyPressEvent(QKeyEvent* event)
{

    qDebug() << "child keyPress";
    MyTable* table = dynamic_cast<MyTable*>(parentItem());
    table->keyPressEvent(event);

    return QGraphicsTextItem::keyPressEvent(event);
}

void MyTableText::focusInEvent(QFocusEvent* event)
{
    qDebug() << "text focusIn event";
    setCursor(Qt::IBeamCursor);
    return QGraphicsTextItem::focusInEvent(event);
}

void MyTableText::focusOutEvent(QFocusEvent* event)
{
    qDebug() << "text focusOut event";
    // setSelected(false);
    setCursor(Qt::ArrowCursor);

    setTextInteractionFlags(Qt::NoTextInteraction);
    QGraphicsTextItem::focusOutEvent(event);
}

void MyTableText::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    event->ignore();
}

void MyTableText::initMyTableText()
{
    intervalH = m_rect.height();
    intervalW = m_rect.width();
    // this->setFlag(QGraphicsItem::ItemIsSelectable);
    //   setTextInteractionFlags(Qt::TextEditorInteraction);
    document()->setTextWidth(intervalW);
    // setFlag(QGraphicsItem::ItemIsFocusable);
    // setFocus();
    // setPlainText("");
}