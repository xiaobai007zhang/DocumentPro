#pragma once

#include "ToolDefine.h"
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QRectF>

class MyGraphicsPixmapItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    MyGraphicsPixmapItem(QRectF rect, QGraphicsItem* parent = nullptr);
    ~MyGraphicsPixmapItem();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    virtual QRectF boundingRect() const override;
    QPainterPath shape() const override;

public:
    void setImage(const QString& filePth);
    void setRect(QRectF rect);
    QRectF getBoundingRect();
    QString getImagePth();
    QPolygonF getPolygonFromRect(QRectF rectIn);
signals:
    void sig_hideRectMouse(bool);

public slots:
    // void slot_expand(bool);
    void slot_repeat(bool);

private:
    void handle(int pos1, int pos2, int pos3, int pos4, QPointF eventPos, STATE_FLAG FLAGS);

private:
    bool m_isMousePress;
    bool m_isRepeat;

    QPointF m_startPos;
    QPixmap m_pixmap;
    QString m_imagePth;

    //记录原先的z值，并用于恢复z值
    short m_zValue;

    QPolygonF m_rectPolygon;

    STATE_FLAG M_FLAG;

    QRectF m_rect, m_topRect, m_leftRect, m_rightRect, m_bottomRect, m_rbRect;
    QRectF m_insicedRectf;
};
