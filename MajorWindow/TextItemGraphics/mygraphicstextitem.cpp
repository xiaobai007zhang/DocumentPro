#include "mygraphicstextitem.h"
#include <QCursor>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <qmath.h>
#include <qtextdocument.h>

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
    setFocus();
    setAcceptHoverEvents(true);
}

//更新框架大小
void MyGraphicsTextItem::updateFontInfo()
{
    int width = document()->size().width();

    int height = document()->size().height();
    QFontMetrics metrics(font());
    setTextWidth(m_rect.width());

    Q_UNUSED(width)

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
    if (height > boundingRect().height())
    {
        qreal adjust = metrics.height();
        this->setRectSize(QRectF(boundingRect().x(), boundingRect().y(), boundingRect().width(), height + adjust));
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

    m_rectPolygon = getPolygonFromRect(m_rect);

    m_topRect = QRectF(m_rect.x() + 8, m_rect.y(), m_rect.width() - 8, 8);

    m_leftRect = QRectF(m_rect.x(), m_rect.y(), 8, m_rect.height() - 8);

    m_rightRect = QRectF(m_rect.right() - 8, m_rect.y(), 8, m_rect.height() - 8);

    m_bottomRect = QRectF(m_rect.x(), m_rect.bottom() - 8, m_rect.width() - 8, 8);

    m_insicedRectf = QRectF(m_rect.x() + 8, m_rect.y() + 8, m_rect.width() - 16, m_rect.height() - 16);

    //右下角的小矩形
    m_rbRect = QRectF(m_rect.right() - 8, m_rect.bottom() - 8, 8, 8);
}

QPolygonF MyGraphicsTextItem::getPolygonFromRect(QRectF rectIn)
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

void MyGraphicsTextItem::handle(int pos1, int pos2, int pos3, int pos4, QPointF eventPos, STATE_FLAG FLAGS)
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
        // moveBy(m_rect.right() - dis, m_rect.y());
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
    setRectSize(newRect);

    scene()->update(); //必须要用scene()->update()，不能用update();否则会出现重影
}

void MyGraphicsTextItem::updateWidHei()
{
    // int width = document()->size().width();
    int height = document()->size().height();
    // setRectSize(QRect(m_rect.x(), m_rect.y(), width, height));
    setRectSize(QRect(m_rect.x(), m_rect.y(), m_rect.width(), height));
}

void MyGraphicsTextItem::focusInEvent(QFocusEvent* e)
{
    // qDebug() << "focus in";
    setCursor(Qt::IBeamCursor);
    QGraphicsTextItem::focusInEvent(e);
}

void MyGraphicsTextItem::focusOutEvent(QFocusEvent* e)
{

    setCursor(Qt::ArrowCursor);
    setSelected(false);
    setAcceptHoverEvents(true);

    //如果没有内容，那么就消失
    if (this->toPlainText().isNull() || this->toPlainText().isEmpty())
    {
        emit sig_loseFocusText(this);
        return;
    }

    //根据内容更新宽高
    updateWidHei();

    emit(sig_hideRectMouse(true));

    emit(sig_needSceneUpdate());

    setFlag(QGraphicsItem::ItemStopsFocusHandling);
    return QGraphicsTextItem::focusOutEvent(e);
}

void MyGraphicsTextItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{

    // painter->drawRect(m_topRect);
    // painter->drawRect(m_leftRect);
    // painter->drawRect(m_rightRect);
    // painter->drawRect(m_bottomRect);
    // painter->drawRect(m_rbRect);

    // if (hasFocus())
    //{
    //     painter->drawRect(boundingRect());
    //     painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
    //     QString text = toPlainText();
    //     setText(text);
    // }

    QString text = toPlainText();
    setText(text);
    if (hasFocus())
    {
        painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
        painter->drawRect(boundingRect());
    }

    return QGraphicsTextItem::paint(painter, option, widget);
}

QRectF MyGraphicsTextItem::boundingRect() const
{
    // QRectF boundingRectF = m_rect;

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
        setAcceptHoverEvents(false);
        return QGraphicsTextItem::mousePressEvent(event);
    }
    else if (event->buttons() == Qt::LeftButton)
    {

        m_isMousePress = true;
        setTextInteractionFlags(Qt::NoTextInteraction);

        //隐藏场景的鼠标滑动形成的框架
        // qDebug() << "hideRect";
        emit sig_hideRectMouse(true);
    }

    QGraphicsTextItem::mousePressEvent(event);
}

void MyGraphicsTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{

    setCursor(Qt::ArrowCursor);
    if (M_FLAG == MOV_RECT)
    {
        M_FLAG = DEFAULT_FLAG;
    }

    if (M_FLAG == MOV_RIGHT_LINE || M_FLAG == MOV_LEFT_LINE)
    {
        //自动更新宽高
        updateWidHei();
    }

    m_isMousePress = false;
    emit sig_hideRectMouse(true);
    // update();

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
    // handle函数，前四个坐标为关心的四个顶点
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

    if (hasFocus() || m_isMousePress)
    {
        emit sig_hideRectMouse(false);
        return QGraphicsTextItem::mouseMoveEvent(event);
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
    if (m_insicedRectf.contains(event->pos()))
    {
        setCursor(Qt::SizeAllCursor);
        M_FLAG = MOV_RECT;
    }
    else if (m_topRect.contains(event->pos()))
    {
        // setFlags(flags() ^ QGraphicsItem::ItemIsMovable);
        setCursor(Qt::SizeVerCursor);

        M_FLAG = MOV_TOP_LINE;
    }
    else if (m_leftRect.contains(event->pos()))
    {
        // setFlags(flags() ^ QGraphicsItem::ItemIsMovable);
        setCursor(Qt::SizeHorCursor);
        M_FLAG = MOV_LEFT_LINE;
    }
    else if (m_rightRect.contains(event->pos()))
    {
        // setFlags(flags() ^ QGraphicsItem::ItemIsMovable);
        setCursor(Qt::SizeHorCursor);
        M_FLAG = MOV_RIGHT_LINE;
    }
    else if (m_bottomRect.contains(event->pos()))
    {
        // setFlags(flags() ^ QGraphicsItem::ItemIsMovable);
        setCursor(Qt::SizeVerCursor);
        M_FLAG = MOV_BOTTOM_LINE;
    }
    else if (m_rbRect.contains(event->pos()))
    {
        // setFlags(flags() ^ QGraphicsItem::ItemIsMovable);
        setCursor(Qt::SizeFDiagCursor);
        M_FLAG = MOV_RIGHTBOTTOM_RECT;
    }
    else
    {
        // setFlags(flags() ^ QGraphicsItem::ItemIsMovable);
        setCursor(Qt::ArrowCursor);
        M_FLAG = DEFAULT_FLAG;
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
