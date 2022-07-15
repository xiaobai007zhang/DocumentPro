#pragma once

#include "ToolDefine.h"
#include <QColor>
#include <QFontMetricsF>
#include <QGraphicsTextItem>
#include <QPainterPath>
#include <QPen>
#include <QTextCursor>
#include <QTimer>

class MyGraphicsTextItem : public QGraphicsTextItem
{
    Q_OBJECT

public:
    MyGraphicsTextItem(const QRectF& rt, QGraphicsItem* parent = nullptr);
    ~MyGraphicsTextItem();

public:
    void initGraphicsTextItem();

    void updateFontInfo();

    void setText(QString text);

    //返回Rect的大小
    QRectF getBoundingRect();

    //返回当前的比例
    qreal getScale();

    //返回文本信息
    QString getStrText();

    void setRectSize(QRectF rect);

private:
    QPolygonF getPolygonFromRect(QRectF rectIn);

    //处理各个方向的拉伸,需要传入五个参数，关心的两条边，和底边的中心点
    void handle(int pos1, int pos2, int pos3, int pos4, QPointF eventPos, STATE_FLAG FLAGS);

    void updateWidHei();

protected:
    // void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void focusInEvent(QFocusEvent* e) override;
    void focusOutEvent(QFocusEvent* e) override;

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

    //需要刷新一下场景
    void sig_needSceneUpdate();

    //删除键上传
    // void sig_deleteKey();

    //隐藏鼠标移动形成的框
    void sig_hideRectMouse(bool);

private:
    QRectF m_rect, m_leftRect, m_rightRect, m_topRect, m_bottomRect, m_rbRect;

    QRectF m_insicedRectf;

    // QRectF m_topOldRect, m_leftOldRect;
    QPolygonF m_rectPolygon;

    QString m_text;

    bool m_isMousePress;
    bool m_isRepeat;

    QPointF m_startPos;

    short m_zValue;

    //记录操作的枚举
    STATE_FLAG M_FLAG;
};
