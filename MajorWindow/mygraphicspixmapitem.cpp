#include "mygraphicspixmapitem.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QCursor>
#include <QWheelEvent>
#include <QPainter>

MyGraphicsPixmapItem::MyGraphicsPixmapItem(QRectF rect, QGraphicsItem* parent)
	: QGraphicsPixmapItem(parent)
{
	//m_rect = rect;
}

MyGraphicsPixmapItem::~MyGraphicsPixmapItem()
{
}

void MyGraphicsPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	m_isMousePress = true;
	m_startPos = event->scenePos();
	//��������
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
		emit sig_hideRectMouse(true);
	}
	QGraphicsPixmapItem::mouseMoveEvent(event);
}


QVariant MyGraphicsPixmapItem::itemChange(GraphicsItemChange change, const QVariant& value)
{

	if (change == ItemPositionChange && scene()) // �ؼ������ƶ�
	{
		//qDebug() << "boundRect :" << boundingRect().width() << "," << boundingRect().height();
		QPointF newPos = value.toPointF(); //����Ҫ�ƶ���λ��

		QRectF rect(0, 0, scene()->width() - scale() * boundingRect().width(), scene()->height() - scale() * boundingRect().height());

		//QRectF rect(0, 0, 800, 600); // ��Ҫ���Ƶ�����
		//qDebug() << "rect.left()" << rect.left();
		//qDebug() << "rect.right()" << rect.right();
		//qDebug() << "rect.top()" << rect.top();
		//qDebug() << "rect.bottom()" << rect.bottom();
		//qDebug() << "newPos: " << newPos;

		if (!rect.contains(newPos)) // �Ƿ���������
		{
			newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
			newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
			qDebug() << "newPos: " << newPos;
			return newPos;
		}
	}
	return QGraphicsItem::itemChange(change, value);
}


void MyGraphicsPixmapItem::setImage(const QString& filePth)
{
	//m_pixmap.load(filePth);
}

void MyGraphicsPixmapItem::setRect(QRectF rect)
{
	//m_rect = rect;
}
