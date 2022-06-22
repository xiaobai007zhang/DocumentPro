#include "mygraphicsscene.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
MyGraphicsScene::MyGraphicsScene(const int& width, const int& height, QObject* parent)
	: QGraphicsScene(parent)
{
	setWidHei(width, height);
}

MyGraphicsScene::~MyGraphicsScene()
{
}

void MyGraphicsScene::setWidHei(const int& width, const int& height)
{
	setSceneRect(0, 0, width, height);
}

void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	m_isPress = true;
	m_curPos = event->scenePos();

}

void MyGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	if (m_isPress) {
		//QSize size = QSize(abs(event->scenePos().x() - m_curPos.x()), (abs(event->scenePos().y() - m_curPos.y())));

		QSize size = QSize(event->scenePos().x() - m_curPos.x(), (event->scenePos().y() - m_curPos.y()));
		//计算一下选中的文本框大小
		emit(sig_rectFrame(size, event->scenePos()));

	}
	m_isPress = false;

}
