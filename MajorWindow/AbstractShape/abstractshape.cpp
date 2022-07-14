#include "abstractshape.h"
#include "MyTable/mytable.h"
#include "PixmapGraphics/mygraphicspixmapitem.h"
#include "TextItemGraphics/mygraphicstextitem.h"

#include <QGraphicsTextItem>
QGraphicsItem* AbstractShape::copy()
{
    if (this->type() == QGraphicsTextItem::Type)
    {
        MyGraphicsTextItem* item = qgraphicsitem_cast<MyGraphicsTextItem*>(this);
        // new��ʱ����Ѿ�ȷ���˾��δ�С��
        MyGraphicsTextItem* tmpItem = new MyGraphicsTextItem(item->getBoundingRect());
        tmpItem->setScale(item->getScale());
        //�����ı�
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
    else if (this->type() == TABLE_TYPE)
    {
        //���Ʊ�����ȸ���������,�ֱ�new��ÿ����Ԫ�񣬻��е�Ԫ�������
        MyTable* item = qgraphicsitem_cast<MyTable*>(this);
        MyTable* tmpItem = new MyTable(item->getRow(), item->getCol(), item->getRect());
        for (MyTableText* item : item->m_tableText)
        {
            // MyTableText* tmp = new MyTableText(item->getRect());
            // tmp->setX(item->getX());
            // tmp->setY(item->getY());
            // tmp->setIndex(item->getRow(), item->getCol());

            tmpItem->m_tableText.push_back(item);
        }
        return tmpItem;
    }

    return nullptr;
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
    else if (type() == TABLE_TYPE)
    {
        return TABLE_TYPE;
    }

    return 0;
}

AbstractShape::AbstractShape(QObject* parent)
{
    Q_UNUSED(parent)
}

AbstractShape::~AbstractShape()
{
}
