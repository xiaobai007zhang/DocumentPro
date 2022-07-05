#include "mytable.h"
#include <QCursor>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextTable>
#include <QTextTableFormat>

void MyTable::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{

    //进行绘画出对应的表格
    //暂时决定，将鼠标聚焦放到对应的位置进行双击会出现编辑框

    painter->drawRect(m_rect);
    for (int i = 1; i < m_row; ++i)
    {
        painter->drawLine(0, i * intervalH, 0 + m_rect.width(), i * intervalH);
    }

    for (int i = 1; i < m_col; ++i)
    {
        painter->drawLine(i * intervalW, 0, i * intervalW, 0 + m_rect.height());
    }
    // QGraphicsItem::paint(painter, option, widget);
}

QRectF MyTable::boundingRect() const
{
    return m_rect;
}

QPainterPath MyTable::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());

    return path;
}

void MyTable::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << "table press";
    emit sig_hideRectMouse(false);
    // setTextInteractionFlags(Qt::NoTextInteraction);
    return QGraphicsItem::mousePressEvent(event);
}

void MyTable::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << "table release";
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

qreal MyTable::getIntervalW()
{
    return intervalW;
}

qreal MyTable::getIntervalH()
{
    return intervalH;
}

void MyTable::initTableWidget()
{
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
}

void MyTable::updateHeight()
{

    // int height = document()->size().height();

    // QFontMetrics metrics(font());

    //// int height = metrics.height();

    // if (height > intervalH)
    //{
    //     qreal adjust = metrics.height();

    //    setRect(QRectF(boundingRect().x(), boundingRect().y(), boundingRect().width(), boundingRect().height() + adjust));
    //    intervalH = m_rect.height() / m_row;
    //    // this->setPlainText(toPlainText());

    //    //当更新文本框后，光标的位置设置，文本的末尾
    //    QTextCursor cursor = this->textCursor();
    //    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
    //    this->setTextCursor(cursor);
    //}
}

void MyTable::setRect(QRectF rect)
{
    m_rect = rect;
}

MyTable::MyTable(int row, int col, QRectF rect) : m_isPress(false), intervalW(0), intervalH(0)
{
    m_row = row;
    m_col = col;
    m_rect = rect;
    intervalW = m_rect.width() / m_col;
    intervalH = m_rect.height() / m_row;
    // setFocus();
    initTableWidget();

    m_tableText = new MyTableText*[m_col * m_row];
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

//=======================================================================================
//=======================================================================================
MyTableText::MyTableText(QRectF rect, QGraphicsItem* parent) : QGraphicsTextItem(parent)
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

    {
        // 原来什么属性就要什么属性,只不过去掉多余的选中状态
        QStyleOptionGraphicsItem op;
        op.initFrom(widget);
        op.state = QStyle::State_None;
        QGraphicsTextItem::paint(painter, &op, widget);
    }
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
    if (hasFocus())
    {
        return QGraphicsTextItem::mousePressEvent(event);
    }
    else
    {
        qDebug() << "table text: mousePressEvent";
        setTextInteractionFlags(Qt::NoTextInteraction);
        emit sig_hideRectMouse(false);
        // return QGraphicsTextItem::mousePressEvent(event);
    }
}

void MyTableText::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << "table text: mouseReleaseEvent";
    return QGraphicsTextItem::mouseReleaseEvent(event);
}

void MyTableText::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << "table text: mouseMoveEvent";

    emit sig_hideRectMouse(false);
    return QGraphicsTextItem::mouseMoveEvent(event);
}

void MyTableText::setRect(QRectF rect)
{
    m_rect = rect;
}

void MyTableText::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << "table text: mouseDoubleClickEvent";
    event->accept();
    if (event->button() == Qt::LeftButton)
    {
        setTextInteractionFlags(Qt::TextEditorInteraction);
        setFocus();
    }

    return QGraphicsTextItem::mouseDoubleClickEvent(event);
}

bool MyTableText::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == this)
    {
        if (event->type() == QEvent::GraphicsSceneMouseDoubleClick)
        {
            qDebug() << "filter GraphicsSceneMouseDoubleClick";
            event->accept();
        }
    }

    return QGraphicsTextItem::eventFilter(obj, event);
}

void MyTableText::updateHeight()
{

    int height = document()->size().height();

    QFontMetrics metrics(font());

    // int height = metrics.height();

    if (height > intervalH)
    {
        qreal adjust = metrics.height();

        setRect(QRectF(boundingRect().x(), boundingRect().y(), boundingRect().width(), boundingRect().height() + adjust));
        intervalH = m_rect.height() / m_row;
        // this->setPlainText(toPlainText());

        //当更新文本框后，光标的位置设置，文本的末尾
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
        this->setTextCursor(cursor);
    }
}

void MyTableText::initMyTableText()
{
    intervalH = m_rect.height();
    intervalW = m_rect.width();
    // setTextInteractionFlags(Qt::TextEditorInteraction);
    document()->setTextWidth(intervalW);
}