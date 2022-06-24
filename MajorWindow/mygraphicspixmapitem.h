#pragma once

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QRectF>


class MyGraphicsPixmapItem :public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT

public:
	MyGraphicsPixmapItem(QRectF rect, QGraphicsItem* parent = nullptr);
	~MyGraphicsPixmapItem();

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event)override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event)override;

	//void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)override;
	QVariant itemChange(GraphicsItemChange change, const QVariant& value)override;
	//QRectF boundingRect()override;

public:
	void setImage(const QString& filePth);
	void setRect(QRectF rect);

signals:
	void sig_hideRectMouse(bool);

private:
	bool m_isMousePress = false;
	QPointF m_startPos;
	QRectF m_rect;
	QPixmap m_pixmap;
};
