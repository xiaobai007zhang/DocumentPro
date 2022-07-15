#pragma once

#include <QGraphicsItem>
#include <QMimeData>

class ShapeMimeData : public QMimeData
{
    Q_OBJECT

public:
    ShapeMimeData(QList<QGraphicsItem*> items);
    ~ShapeMimeData();

public:
    QList<QGraphicsItem*> items() const;
    void removeItem(QGraphicsItem* item);

private:
    QList<QGraphicsItem*> m_items;
};
