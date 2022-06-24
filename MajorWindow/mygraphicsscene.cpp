#include "mygraphicsscene.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QPainter>


#include "mygraphicstextitem.h"
MyGraphicsScene::MyGraphicsScene(const int& width, const int& height, QObject* parent)
	: QGraphicsScene(parent), m_textItemFlag(false)
{
	setWidHei(width, height);
<<<<<<< HEAD


=======
>>>>>>> 4887f329125d6ae369b6e0babd1cb1e6d9eed5df
	//m_rectMouse = new QGraphicsRectItem;
	//m_rectMouse->setBrush(Qt::blue);

}

MyGraphicsScene::~MyGraphicsScene()
{
}

void MyGraphicsScene::setWidHei(const int& width, const int& height)
{
	//qDebug() << width << " " << height;
	setSceneRect(0, 0, width, height);
	//addRect(0, 0, width, height)->setBrush(Qt::blue);

}


void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	//qDebug() << "My mouse press event";
<<<<<<< HEAD

	m_rectMouse = new QGraphicsRectItem;

=======
	
	m_rectMouse = new QGraphicsRectItem;
	
>>>>>>> 4887f329125d6ae369b6e0babd1cb1e6d9eed5df


	addItem(m_rectMouse);


	m_isPress = true;
	m_curPos = event->scenePos();


	QGraphicsScene::mousePressEvent(event);

}

void MyGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	//只有点击了才会做处理
	if (m_isPress) {
		int width = event->scenePos().x() - m_curPos.x();
		int height = event->scenePos().y() - m_curPos.y();

		if (width > 0 && height > 0) {
			QSize size(width, height);
			m_rectMouse->setRect(QRectF(m_curPos.toPoint(), size));
		}

		if (width < 0 && height < 0) {
			QSize size(abs(width), abs(height));
			m_rectMouse->setRect(QRectF(m_curPos.toPoint() - QPoint(size.width(), size.height()), size));
		}

		if (width > 0 && height < 0) {
			QSize size(abs(width), abs(height));
			m_rectMouse->setRect(QRectF(m_curPos.toPoint() - QPoint(0, size.height()), size));
		}

		if (width < 0 && height > 0) {
			QSize size(abs(width), abs(height));
			m_rectMouse->setRect(QRectF(m_curPos.toPoint() - QPoint(size.width(), 0), size));
		}

	}

	QGraphicsScene::mouseMoveEvent(event);
}


void MyGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	//qDebug() << "MyScene mouse event";
	//QSize size = QSize(abs(event->scenePos().x() - m_curPos.x()), (abs(event->scenePos().y() - m_curPos.y())));

	QSize size = QSize(event->scenePos().x() - m_curPos.x(), (event->scenePos().y() - m_curPos.y()));
	//计算一下选中的文本框大小
	emit(sig_rectFrame(size, event->scenePos(), m_textItemFlag));
	m_isPress = false;

	QList<QGraphicsItem*>list = this->items();
	if (list.contains(m_rectMouse)) {
		removeItem(m_rectMouse);
	}


	QGraphicsScene::mouseReleaseEvent(event);

}

void MyGraphicsScene::wheelEvent(QGraphicsSceneWheelEvent* event)
{
	//qDebug() << "scene wheel event";
	QList<QGraphicsItem*> list = selectedItems();
	if (!list.isEmpty()) {
		for (QGraphicsItem* item : list) {


			if (event->delta() > 0) {
				//qDebug() << QStringLiteral("大于零");
				item->setScale(item->scale() + 0.1);
				//item->setPos((item->pos().x() - item->boundingRect().width() * 0.1), (item->pos().y() - item->boundingRect().height() * 0.1));
			}
			else {
				if (item->scale() - 0.1 <= 0) {
					continue;
				}
				else {
					item->setScale(item->scale() - 0.1);
				}
				//qDebug() << QStringLiteral("小于零");
				//item->setPos((item->pos().x() + item->boundingRect().width() * 0.1), (item->pos().y() + item->boundingRect().height() * 0.1));

			}
		}

		event->accept();
	}
	else {
		QGraphicsScene::wheelEvent(event);
	}


}


void MyGraphicsScene::slot_hideRectMouse(bool flag) {

	m_textItemFlag = flag;
	m_rectMouse->hide();

}