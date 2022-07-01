#include "abstractshape.h"
#include "../PixmapGraphics/mygraphicspixmapitem.h"
#include "../TextItemGraphics/mygraphicstextitem.h"

#include <QGraphicsTextItem>
QGraphicsItem* AbstractShape::copy()
{
    if (this->type() == QGraphicsTextItem::Type)
    {
        MyGraphicsTextItem* item = qgraphicsitem_cast<MyGraphicsTextItem*>(this);
        // new的时候就已经确定了矩形大小了
        MyGraphicsTextItem* tmpItem = new MyGraphicsTextItem(item->getBoundingRect());
        tmpItem->setScale(item->getScale());
        //保存文本
        tmpItem->setPlainText(item->getStrText());
        tmpItem->setPos(item->pos());

        return tmpItem;
        // return qgraphicsitem_cast<QGraphicsItem*>(tmpItem);
    }
    else if (this->type() == QGraphicsPixmapItem::Type)
    {

        MyGraphicsPixmapItem* item = qgraphicsitem_cast<MyGraphicsPixmapItem*>(this);
        MyGraphicsPixmapItem* tmpItem = new MyGraphicsPixmapItem(item->getBoundingRect());
        tmpItem->setImage(item->getImagePth());
        tmpItem->setScale(item->scale());
        tmpItem->setPos(item->pos());
        return tmpItem;
    }
    else
    {

        return nullptr;
    }
}

int AbstractShape::type() const
{
    if (type() == QGraphicsTextItem::Type)
    {
        return QGraphicsTextItem::Type;
    }
    else if (type() == QGraphicsPixmapItem::Type)
    {
        return QGraphicsPixmapItem::Type;
    }
    else
    {
        return 0;
    }
}

AbstractShape::AbstractShape(QObject* parent)
{
    Q_UNUSED(parent)
}

AbstractShape::~AbstractShape()
{
}
