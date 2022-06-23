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
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event)override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)override;

signals:
	void sig_rectFrame(QSize size, QPointF point, bool);

public slots:
	void slot_hideRectMouse(bool);
private:
	QPointF m_curPos;

	bool m_isPress;
	QGraphicsRectItem* m_rectMouse;
	//等于1的话就证明要拖动了，等于零就代表执行默认操作
	bool m_textItemFlag;
};
