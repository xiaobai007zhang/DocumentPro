#pragma once

#include <QColor>
#include <QFontMetricsF>
#include <QGraphicsTextItem>
#include <QPainterPath>
#include <QPen>
#include <QTextCursor>
#include <QTimer>
#define M_PI 3.14

class MyGraphicsTextItem : public QGraphicsTextItem
{
    Q_OBJECT

public:
    MyGraphicsTextItem(const QRectF& rt, QGraphicsItem* parent = nullptr);
    ~MyGraphicsTextItem();

public:
    void initGraphicsTextItem();

    void setRect(const QRectF& rect);

    void updateFontInfo();

    void setText(QString text);

    //����Rect�Ĵ�С
    QRectF getBoundingRect();

    //���ص�ǰ�ı���
    qreal getScale();

    //�����ı���Ϣ
    QString getStrText();

    void setRectSize(QRectF rect);

    QPolygonF getRotatePolygonFromRect(QRectF rectIn);

protected:
    // void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void focusInEvent(QFocusEvent* e) override;
    void focusOutEvent(QFocusEvent* e) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    QRectF boundingRect() const override;

    int type() const;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    QPainterPath shape() const;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e) override;

public slots:
    void slot_repeat(bool);

signals:
    void sig_loseFocusText(QGraphicsTextItem* item);

    //��Ҫˢ��һ�³���
    void sig_needSceneUpdate();

    //ɾ�����ϴ�
    // void sig_deleteKey();

    //��������ƶ��γɵĿ�
    void sig_hideRectMouse(bool);

private:
    QRectF m_rect, m_leftRect, m_rightRect, m_topRect, m_bottomRect, m_rbRect;

    QRectF m_insicedRectf;

    QPolygonF m_oldRectPolygon;

    QString m_text;

    qreal m_fontWidth;

    qreal m_fontHeight;
    // QFont m_font;

    bool m_isMousePress = false;

    QPointF m_startPos;

    bool m_isRepeat;

    short m_zValue;

    QPolygonF m_topPoly, m_leftPoly, m_rightPoly, m_bottomPoly, m_insicsdPoly;
};
