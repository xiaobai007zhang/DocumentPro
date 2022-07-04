#include "mytable.h"
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextTable>
#include <QTextTableFormat>

void MyTable::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{

    //进行绘画出对应的表格
    //暂时决定，将鼠标聚焦放到对应的位置进行双击会出现编辑框

    painter->drawRect(m_rect);
    // painter->drawText(0, 0, TR("今天天气真不错"));
    for (int i = 1; i < m_row; ++i)
    {
        painter->drawLine(0, i * intervalH, 0 + m_rect.width(), i * intervalH);
    }

    for (int i = 1; i < m_col; ++i)
    {
        painter->drawLine(i * intervalW, 0, i * intervalW, 0 + m_rect.height());
    }
    updateHeight();
    // painter->drawLine(50, 0, 50, 100);
    QGraphicsTextItem::paint(painter, option, widget);
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

void MyTable::initTableWidget()
{
    setTextWidth(intervalW);
    // setTextWidth(m_rect.width());
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    // setTextInteractionFlags(Qt::NoTextInteraction);
    setTextInteractionFlags(Qt::TextEditorInteraction);
}

void MyTable::updateHeight()
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

void MyTable::setRect(QRectF rect)
{
    m_rect = rect;
}

MyTable::MyTable(int row, int col, QRectF rect)
{
    m_row = row;
    m_col = col;
    m_rect = rect;
    intervalW = m_rect.width() / m_col;
    intervalH = m_rect.height() / m_row;
    // setPlainText(TR("jkjkjkjkjkjjk"));

    setFocus();
    initTableWidget();
}

MyTable::~MyTable()
{
}

int MyTable::type() const
{
    return UserType + 1;
}

void MyTable::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    emit sig_hideRectMouse(false);
    setTextInteractionFlags(Qt::NoTextInteraction);
    QGraphicsTextItem::mousePressEvent(event);
}

void MyTable::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    emit sig_hideRectMouse(true);

    QGraphicsTextItem::mouseReleaseEvent(event);
}

void MyTable::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    emit sig_hideRectMouse(false);
    QGraphicsTextItem::mouseMoveEvent(event);
}

void MyTable::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    qDebug() << "double event: " << event->pos();

    // QTextList* list = item->textCursor().createList(QTextListFormat::ListDisc);

    // item->setTextInteractionFlags(Qt::TextEditorInteraction);
    // item->setFocus();
    setTextInteractionFlags(Qt::TextEditorInteraction);
    setFocus();
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void MyTable::focusInEvent(QFocusEvent* event)
{
    qDebug() << "focusInEvent ";
    setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::focusInEvent(event);
}

void MyTable::focusOutEvent(QFocusEvent* event)
{
    qDebug() << "focusInEvent ";
    // adjustSize();
    QGraphicsTextItem::focusOutEvent(event);
}

void MyTable::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Backspace)
    {
        int height = document()->size().height();
        QFontMetrics metrics(font());

        // qDebug() << "textCursor block: " << textCursor().block().text();

        // textCursor().joinPreviousEditBlock();
        int lastLength = document()->lastBlock().text().length();

        // qDebug() << "test: " << document()->lastBlock().text().size();

        if (height < intervalH && lastLength == 0)
        {
            if (document()->firstBlock().text().size() == 0)
            {
                return;
            }

            qreal adjust = metrics.height();
            // intervalH = height;
            /*qDebug() << "height: " << height;
            qDebug() << "intervalH: " << intervalH;
            qDebug() << "adjust: " << adjust;*/

            setRect(QRectF(boundingRect().x(), boundingRect().y(), boundingRect().width(), boundingRect().height() - adjust * m_row));

            intervalH = m_rect.height() / m_row;

            //当更新文本框后，光标的位置设置，文本的末尾
        }
    }

    QGraphicsTextItem::keyPressEvent(event);
}

//=======================================================================================
//=======================================================================================
MyTableText::MyTableText(int row, int col, QRectF rect)
{
    m_col = col;
    m_row = row;
    m_rect = rect;
}

MyTableText::~MyTableText()
{
}

void MyTableText::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->drawRect(m_rect);

    QGraphicsTextItem::paint(painter, option, widget);
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
    // setTextInteractionFlags(Qt::NoTextInteraction);

    QGraphicsTextItem::mousePressEvent(event);
}

void MyTableText::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsTextItem::mouseReleaseEvent(event);
}

void MyTableText::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsTextItem::mouseMoveEvent(event);
}

void MyTableText::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    // setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}
