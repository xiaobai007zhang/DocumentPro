#pragma once

#include <QColor>
#include <QFontMetricsF>
#include <QGraphicsTextItem>
#include <QPainterPath>
#include <QPen>
#include <QTextCursor>
#include <QTimer>

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

    // const QString& getName(const int& key);

    void setRect(const QRectF& rect);

    void updateFontInfo();

    void setText(QString text);

    //返回Rect的大小
    QRectF getBoundingRect();

    //返回当前的比例
    qreal getScale();

    //返回文本信息
    QString getStrText();

protected:
    // void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void focusInEvent(QFocusEvent* e) override;
    void focusOutEvent(QFocusEvent* e) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    QRectF boundingRect() const override;

    int type() const;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    QPainterPath shape() const;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e) override;

public slots:
    void slot_expand(bool);
    void slot_repeat(bool);

signals:
    void sig_loseFocusText(QGraphicsTextItem* item);

    //需要刷新一下场景
    void sig_needSceneUpdate();

    //删除键上传
    // void sig_deleteKey();

    //隐藏鼠标移动形成的框
    void sig_hideRectMouse(bool);

private:
    QRectF m_rect;

    QString m_text;

    qreal m_fontWidth;

    qreal m_fontHeight;
    // QFont m_font;

    bool m_isMousePress = false;

    QPointF m_startPos;

    bool m_isExpand;

    bool m_isRepeat;

    short m_zValue;
};
