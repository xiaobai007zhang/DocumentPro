#pragma once

#include <QGraphicsPixmapItem>
#include <QObject>

class MyGraphicsPixmapItem :public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT

public:
	MyGraphicsPixmapItem(QGraphicsItem* parent = nullptr);
	~MyGraphicsPixmapItem();

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event)override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event)override;


signals:
	void sig_hideRectMouse(bool);

private:
	bool m_isMousePress = false;
	QPointF m_startPos;
};
