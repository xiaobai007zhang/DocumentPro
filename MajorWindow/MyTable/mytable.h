#pragma once

#include <QGraphicsItem>
#include <QObject>
class MyTable : public QObject, public QGraphicsItem
{
    Q_OBJECT
    // Q_INTERFACES(QGraphicsItem)
public:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    QRectF boundingRect() const override;

    QPainterPath shape() const override;

    int type() const override;

private:
    void initTableWidget();

private:
    // QGraphicsProxyWidget* m_proxy;

    //¼ÇÂ¼ÐÐÁÐ
    int m_row;
    int m_col;

    QRectF m_rect;

public:
    MyTable(int row, int col, QRectF rect);
    ~MyTable();
};
