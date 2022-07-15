#include "shapemimedata.h"
#include "AbstractShape/abstractshape.h"

ShapeMimeData::ShapeMimeData(QList<QGraphicsItem*> items)
{
    for (QGraphicsItem* item : items)
    {
        AbstractShape* sp = qgraphicsitem_cast<AbstractShape*>(item);
        m_items.append(sp->copy());
    }
}

ShapeMimeData::~ShapeMimeData()
{

    for (QGraphicsItem* item : m_items)
    {
        delete item;
    }
    m_items.clear();
}

QList<QGraphicsItem*> ShapeMimeData::items() const
{
    return m_items;
}

void ShapeMimeData::removeItem(QGraphicsItem* item)
{
    m_items.removeOne(item);
}
