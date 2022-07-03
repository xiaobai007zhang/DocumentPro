#include "mygraphicstextitem.h"
#include <QCursor>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <qtextdocument.h>

MyGraphicsTextItem::MyGraphicsTextItem(const QRectF& rt, QGraphicsItem* parent) : QGraphicsTextItem(parent), m_isMousePress(false), m_isRepeat(true), m_isExpand(true)
{
    //�������ԣ�����ʼ��������Ϣ
    m_rect = rt;
    initGraphicsTextItem();
}

MyGraphicsTextItem::~MyGraphicsTextItem()
{
}

void MyGraphicsTextItem::initGraphicsTextItem()
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setTextInteractionFlags(Qt::TextEditorInteraction);
    setFocus();
}

void MyGraphicsTextItem::setName(const int& key, const QString& name)
{
    setData(key, name);
}

// const QString& MyGraphicsTextItem::getName(const int& key)
//{
//     Q_UNUSED(key)
//     // return data(key).toString();
//     return nullptr;
// }

void MyGraphicsTextItem::setRect(const QRectF& rect)
{
    m_rect = rect;
}
//���¿�ܴ�С
void MyGraphicsTextItem::updateFontInfo()
{
    m_fontWidth = document()->size().width();
    m_fontHeight = document()->size().height();
    QFontMetrics metrics(font());
    // setPlainText(m_text);
    //���������ֳ����ı���ʱ
    if (m_fontWidth > boundingRect().width())
    {
        qreal adjust = metrics.width("��");
        this->setRect(QRectF(boundingRect().x(), boundingRect().y(), m_fontWidth + adjust, boundingRect().height()));
        // setRect(QRectF(boundingRect().x(), boundingRect().y(), m_fontWidth + adjust, boundingRect().height()));
        // this->setFont(m_font);
        this->setPlainText(m_text);

        //�������ı���󣬹���λ�����ã��ı���ĩβ
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
        this->setTextCursor(cursor);
    }
    //���������ָ����ı���ʱ
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
    // setTextInteractionFlags(Qt::NoTextInteraction);
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
    if (m_isExpand)
    {
        return QGraphicsTextItem::itemChange(change, value);
    }
    else
    {
        if (change == ItemPositionChange && scene()) // �ؼ������ƶ�
        {
            QPointF newPos = value.toPointF(); //����Ҫ�ƶ���λ��
            // QRectF rect(0, 0, scene()->width(), scene()->height());
            QRectF rect = scene()->sceneRect();
            qDebug() << "scene()->sceneRect()" << rect;

            if (!rect.contains(newPos)) // �Ƿ���������
            {
                newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
                newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
                // qDebug() << "text newPos: ";
                return newPos;
            }
        }
        return QGraphicsTextItem::itemChange(change, value);
    }
}

void MyGraphicsTextItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{

    if (hasFocus())
    {
        // painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
        painter->drawRect(boundingRect());
        QString text = toPlainText();
        setText(text);
        QGraphicsTextItem::paint(painter, option, widget);
    }
    else
    {
        QGraphicsTextItem::paint(painter, option, widget);
    }
}

QRectF MyGraphicsTextItem::boundingRect() const
{
    // return QRectF(m_rect.x(), m_rect.y(), m_rect.width(), m_rect.height());
    return m_rect;
}

int MyGraphicsTextItem::type() const
{
    return QGraphicsTextItem::Type;
}

void MyGraphicsTextItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    // qDebug() << "press";
    // setSelected(true);
    // m_zValue = zValue();
    // setZValue(1);
    if (hasFocus())
    {
        QGraphicsTextItem::mousePressEvent(event);
    }
    else
    {
        m_isMousePress = true;
        m_startPos = event->scenePos();
        setTextInteractionFlags(Qt::NoTextInteraction);
        // qDebug() << "m_startPos" << m_startPos;
        //���س�������껬���γɵĿ��
        // qDebug() << "hideRect";
        emit sig_hideRectMouse(true);
        QGraphicsTextItem::mousePressEvent(event);
    }
}

void MyGraphicsTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    // qDebug() << "release";
    // killTimer(m_timeId);
    // setZValue(m_zValue);
    m_isMousePress = false;
    emit sig_hideRectMouse(true);
    QGraphicsTextItem::mouseReleaseEvent(event);
}

void MyGraphicsTextItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (hasFocus() || m_isMousePress)
    {
        emit sig_hideRectMouse(false);
        // QGraphicsTextItem::mouseMoveEvent(event);
    }

    if (m_isRepeat == false)
    { //���ظ�
        QList<QGraphicsItem*> list = collidingItems();
        for (QGraphicsItem* item : list)
        { //�Ƚ�����
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
        //�ظ�����������ZValueֵ
        QList<QGraphicsItem*> list = collidingItems();
        for (QGraphicsItem* item : list)
        {
            setZValue(item->zValue() + 0.01);
        }
    }
    QGraphicsTextItem::mouseMoveEvent(event);
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
        // qDebug() << "double Mouse";
        setTextInteractionFlags(Qt::TextEditorInteraction);
        setFocus();
    }
    QGraphicsTextItem::mouseDoubleClickEvent(e);
}

void MyGraphicsTextItem::slot_repeat(bool flag)
{
    m_isRepeat = flag;
}

void MyGraphicsTextItem::slot_expand(bool flag)
{
    m_isExpand = flag;
}
