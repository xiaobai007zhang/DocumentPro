#include "mygraphicstextitem.h"
#include <QCursor>
#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <qtextdocument.h>
#include <QDebug>
#include <QStyleOptionGraphicsItem>

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
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setTextInteractionFlags(Qt::TextSelectableByMouse);
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
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
	//当输入文字长于文本框时
	if (m_fontWidth > boundingRect().width())
	{
		qreal adjust = 5;
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
		qreal adjust = 5;
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
	setTextInteractionFlags(Qt::TextEditorInteraction);
	QGraphicsTextItem::focusInEvent(e);
}

void MyGraphicsTextItem::focusOutEvent(QFocusEvent* e)
{
	if (this->toPlainText().isNull() || this->toPlainText().isEmpty()) {
		emit sig_loseFocusText(this);
		return;
	}
	int width = document()->size().width();
	int height = document()->size().height();
	setRect(QRectF(boundingRect().x(), boundingRect().y(), width, height));
	setTextInteractionFlags(Qt::NoTextInteraction);

	setSelected(false);
	emit(sig_needSceneUpdate());

	QGraphicsTextItem::focusOutEvent(e);
}

void MyGraphicsTextItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	qDebug() << "paint and setText";

	if (hasFocus()) {
		//painter->setRenderHint(QPainter::SmoothPixmapTransform);
		//painter->setRenderHint(QPainter::Antialiasing);
		painter->drawRect(boundingRect());
		//QStyleOptionGraphicsItem options(*option);
		//options.state &= ~QStyle::State_None;

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
	Q_UNUSED(event);
	qDebug() << "myTextItem mousePressEvent";
	setSelected(true);
	setTextInteractionFlags(Qt::NoTextInteraction);
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
		setTextInteractionFlags(Qt::TextEditorInteraction);
		setFocus();

		QGraphicsTextItem::mouseDoubleClickEvent(e);

	}

}
