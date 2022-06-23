#include "mygraphicspixmapitem.h"

#include <QGraphicsSceneMouseEvent>

MyGraphicsPixmapItem::MyGraphicsPixmapItem(QGraphicsItem* parent)
	: QGraphicsPixmapItem(parent)
{
}

MyGraphicsPixmapItem::~MyGraphicsPixmapItem()
{
}

void MyGraphicsPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	m_isMousePress = true;
	m_startPos = event->scenePos();
	//¿ªÆôÒþ²Ø
	emit sig_hideRectMouse(true);

	QGraphicsPixmapItem::mousePressEvent(event);
}

void MyGraphicsPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	m_isMousePress = false;
	emit sig_hideRectMouse(false);
	QGraphicsPixmapItem::mouseReleaseEvent(event);
}

void MyGraphicsPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	if (m_isMousePress) {
		moveBy(event->scenePos().rx() - m_startPos.rx(), event->scenePos().ry() - m_startPos.ry());
	}

	QGraphicsPixmapItem::mouseMoveEvent(event);
}
