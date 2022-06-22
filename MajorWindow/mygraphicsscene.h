#pragma once

#include <QGraphicsScene>
#include <QMouseEvent>

class MyGraphicsScene : public QGraphicsScene
{
	Q_OBJECT

public:
	MyGraphicsScene(const int& width, const int& height, QObject* parent = nullptr);
	~MyGraphicsScene();
private:

	void setWidHei(const int& width, const int& height);
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event)override;

	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)override;

signals:
	void sig_rectFrame(QSize size, QPointF point);
private:
	QPointF m_curPos;

	bool m_isPress;
};
