#include "mygraphicstextitem.h"
#include <QCursor>
#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <qtextdocument.h>
#include <QDebug>
#include <QStyleOptionGraphicsItem>
#include <QKeyEvent>

MyGraphicsTextItem::MyGraphicsTextItem(const QRectF& rt, QGraphicsItem* parent) :QGraphicsTextItem(parent)
{
	//设置属性，并初始化基本信息
	m_rect = rt;

	initGraphicsTextItem();

}

MyGraphicsTextItem::~MyGraphicsTextItem()
{
}

void MyGraphicsTextItem::initGraphicsTextItem()
{
	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsFocusable);
	setTextInteractionFlags(Qt::TextEditorInteraction);

	setFocus();

}

void MyGraphicsTextItem::setName(const int& key, const QString& name)
{
	setData(key, name);
}

const QString& MyGraphicsTextItem::getName(const int& key)
{
	return data(key).toString();
}

void MyGraphicsTextItem::setRect(const QRectF& rect)
{
	m_rect = rect;
}
//更新框架大小
void MyGraphicsTextItem::updateFontInfo()
{

	m_fontWidth = document()->size().width();
	m_fontHeight = document()->size().height();
	QFontMetrics metrics(font());

	//当输入文字长于文本框时
	if (m_fontWidth > boundingRect().width())
	{
		qreal adjust = metrics.width("张");
		this->setRect(QRectF(boundingRect().x(), boundingRect().y(), m_fontWidth + adjust, boundingRect().height()));
		//this->setFont(m_font);
		this->setPlainText(m_text);

		//当更新文本框后，光标的位置设置，文本的末尾
		QTextCursor cursor = this->textCursor();
		cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
		this->setTextCursor(cursor);
	}
	//当输入文字高于文本框时
	if (m_fontHeight > boundingRect().height())
	{
		qreal adjust = metrics.height();
		this->setRect(QRectF(boundingRect().x(), boundingRect().y(), boundingRect().width(), m_fontHeight + adjust));
		//this->setFont(m_font);
		this->setPlainText(m_text);
		QTextCursor cursor = this->textCursor();
		cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
		this->setTextCursor(cursor);
	}
}

void MyGraphicsTextItem::setText(QString text)
{
	m_text = text;
	updateFontInfo();
}


void MyGraphicsTextItem::focusInEvent(QFocusEvent* e)
{
	//qDebug() << "focus in";

	setCursor(Qt::IBeamCursor);
	QGraphicsTextItem::focusInEvent(e);
}

void MyGraphicsTextItem::focusOutEvent(QFocusEvent* e)
{

	qDebug() << "focusOut";
	if (this->toPlainText().isNull() || this->toPlainText().isEmpty()) {
		emit sig_loseFocusText(this);
		return;
	}
	int width = document()->size().width();
	int height = document()->size().height();
	setRect(QRectF(boundingRect().x(), boundingRect().y(), width, height));
	//setTextInteractionFlags(Qt::NoTextInteraction);
	setSelected(false);
	//QTextCursor cursor = textCursor();
	//cursor.clearSelection();
	emit(sig_needSceneUpdate());
	emit(sig_hideRectMouse(false));
	setFlag(QGraphicsItem::ItemStopsFocusHandling);
	setCursor(Qt::ArrowCursor);

	QGraphicsTextItem::focusOutEvent(e);
}

void MyGraphicsTextItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	//qDebug() << "paint and setText";

	if (hasFocus()) {
		//painter->setRenderHint(QPainter::SmoothPixmapTransform);
		//painter->setRenderHint(QPainter::Antialiasing);
		painter->drawRect(boundingRect());

		QString text = toPlainText();
		setText(text);
		QGraphicsTextItem::paint(painter, option, widget);
	}
	else {
		QGraphicsTextItem::paint(painter, option, widget);
	}


}

QRectF MyGraphicsTextItem::boundingRect() const
{
	return QRectF(m_rect.x(), m_rect.y(), m_rect.width(), m_rect.height());
}

int MyGraphicsTextItem::type() const
{
	return UserType + 5;
}


void MyGraphicsTextItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	//qDebug() << "press";
	if (hasFocus()) {
		QGraphicsTextItem::mousePressEvent(event);
	}
	else {
		m_isMousePress = true;
		m_startPos = event->scenePos();
		//qDebug() << "m_startPos" << m_startPos;
		setTextInteractionFlags(Qt::NoTextInteraction);

		//隐藏场景的鼠标滑动形成的框架
		emit sig_hideRectMouse(true);
		//m_timeId = startTimer(800);
		QGraphicsTextItem::mousePressEvent(event);
	}

}

void MyGraphicsTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	//qDebug() << "release";
	//killTimer(m_timeId);
	m_isMousePress = false;
	emit sig_hideRectMouse(false);
	QGraphicsTextItem::mouseReleaseEvent(event);
}

void MyGraphicsTextItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	if (hasFocus()) {
		emit sig_hideRectMouse(true);
		QGraphicsTextItem::mouseMoveEvent(event);
	}
	else {

	}
	if (m_isMousePress) {
		//qDebug() << "scenePos: " << event->scenePos();
		moveBy(event->scenePos().rx() - m_startPos.rx(), event->scenePos().ry() - m_startPos.ry());
	}
	QGraphicsTextItem::mouseMoveEvent(event);
}


QPainterPath MyGraphicsTextItem::shape() const
{
	//qDebug() << "shape";
	QPainterPath path;
	path.addRect(boundingRect());
	return path;
}

void MyGraphicsTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e)
{

	if (e->button() == Qt::LeftButton) {
		//qDebug() << "double Mouse";
		setTextInteractionFlags(Qt::TextEditorInteraction);
		//setTextInteractionFlags(Qt::TextSelectableByMouse);
		setFocus();
		//setSelected(true);
		QGraphicsTextItem::mouseDoubleClickEvent(e);
		//return;
	}
	//emit sig_hideRectMouse(false);
	//textCursor().clearSelection();
	//QGraphicsTextItem::mouseDoubleClickEvent(e);
}
