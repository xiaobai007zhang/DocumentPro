#pragma once

#include "ToolDefine.h"
#include <QGraphicsItem>
#include <QObject>

class MyTable : public QGraphicsTextItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    QRectF boundingRect() const override;

    QPainterPath shape() const override;

    int type() const override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);

    void focusInEvent(QFocusEvent* event) override;

    void focusOutEvent(QFocusEvent* event) override;

    void keyPressEvent(QKeyEvent* event) override;

private:
    void initTableWidget();

    void updateHeight();

    void setRect(QRectF rect);

signals:
    void sig_hideRectMouse(bool);

private:
    // QGraphicsProxyWidget* m_proxy;

    //记录行列
    int m_row;
    int m_col;

    QRectF m_rect;

    QGraphicsTextItem* item;

    qreal intervalW;
    qreal intervalH;

public:
    MyTable(int row, int col, QRectF rect);
    ~MyTable();
};

class MyTableText : public QGraphicsTextItem
{
    Q_OBJECT
public:
    MyTableText(int row, int col, QRectF rect);
    ~MyTableText();

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    QRectF boundingRect() const override;

    QPainterPath shape() const override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);

    // void focusInEvent(QFocusEvent* event) override;

    // void focusOutEvent(QFocusEvent* event) override;

signals:
    void sig_hideRectMouse(bool);

private:
    // QGraphicsProxyWidget* m_proxy;

    //记录行列
    int m_row;
    int m_col;

    QRectF m_rect;
};