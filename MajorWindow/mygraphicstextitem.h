#pragma once

#include <QGraphicsTextItem>
#include <QPen>
#include <QFontMetricsF>
#include <QPainterPath>
#include <QTextCursor>
#include <QColor>


class MyGraphicsTextItem : public QGraphicsTextItem
{
	Q_OBJECT

public:
	MyGraphicsTextItem(const QRectF& rt, QGraphicsItem* parent = nullptr);
	~MyGraphicsTextItem();
public:
	void initGraphicsTextItem();
	//根据键值对来区分json文件中的各个模块
	void setName(const int& key, const QString& name);

	const QString& getName(const int& key);

	void setRect(const QRectF& rect);

	void updateFontInfo();

	void setText(QString text);

protected:
	//void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
	void focusInEvent(QFocusEvent* e)override;
	void focusOutEvent(QFocusEvent* e)override;

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)override;
	QRectF boundingRect()const override;

	int     type() const;
	void mousePressEvent(QGraphicsSceneMouseEvent* event)override;

	QPainterPath shape() const;

	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e)override;
signals:
	void sig_loseFocusText(QGraphicsTextItem* item);

	//需要刷新一下场景
	void sig_needSceneUpdate();

private:
	QRectF m_rect;
	QString m_text;

	//QFontMetricsF* m_fontMetricsF = NULL;
	qreal m_fontWidth;
	qreal m_fontHeight;
	//QFont m_font;
	bool isMousePress = false;

};
