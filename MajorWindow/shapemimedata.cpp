#include "shapemimedata.h"

ShapeMimeData::ShapeMimeData(QList<QGraphicsItem*> items)
{

    m_items = items;
}

ShapeMimeData::~ShapeMimeData()
{
}

QList<QGraphicsItem*> ShapeMimeData::items() const
{
    return m_items;
}
