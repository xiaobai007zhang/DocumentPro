#pragma once

#include <QAbstractGraphicsShapeItem>
#include <QGraphicsItem>

class AbstractShape : public QAbstractGraphicsShapeItem
{
public:
    QGraphicsItem* copy();
    int type() const override;

public:
    AbstractShape(QObject* parent);
    ~AbstractShape();
};
