#pragma once

#include "ToolDefine.h"

#include <QGraphicsRectItem>
#include <QObject>
#include <vector>
#define TABLE_TYPE QGraphicsItem::UserType + 1

class MyTableText;
class MyTable : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    QRectF boundingRect() const override;

    QPainterPath shape() const override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

    // void keyPressEvent(QKeyEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

    bool eventFilter(QObject* obj, QEvent*) override;

    int type() const override;

    void focusOutEvent(QFocusEvent* event) override;

    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

public:
    qreal getIntervalW();

    qreal getIntervalH();
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

public:
    QRectF getRect();

    int getRow();

    int getCol();

    void setRect(QRectF rect);

private:
    void initTableWidget();

signals:
    void sig_hideRectMouse(bool);

public slots:

    void slot_contentsChanged();
    //清空vector
    void slot_MyTable(QRectF);

    //合并单元格
    void slot_joinTable();

    void slot_expand(bool);

    void slot_repeat(bool flag);

private:
    //记录行列
    int m_row;
    int m_col;

    QRectF m_rect;

    QGraphicsTextItem* item;

    qreal intervalW;
    qreal intervalH;

    bool m_isExpand;
    bool m_isRepeat;

    bool m_isJoin;

public:
    MyTable(int row, int col, QRectF rect);
    ~MyTable();

public:
    //结构体数组

    std::vector<MyTableText*> m_tableText;
};

//=========================================================================================
class MyTableText : public QGraphicsTextItem
{
    Q_OBJECT
public:
    MyTableText(QRectF rect, QGraphicsItem* parent = nullptr);
    ~MyTableText();
    MyTableText()
    {
    }

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    QRectF boundingRect() const override;

    QPainterPath shape() const override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

    void keyPressEvent(QKeyEvent* event) override;

    // void keyReleaseEvent(QKeyEvent* event) override;

    void focusInEvent(QFocusEvent* event) override;

    void focusOutEvent(QFocusEvent* event) override;

    // bool eventFilter(QObject* obj, QEvent* event) override;

    void wheelEvent(QGraphicsSceneWheelEvent* event) override;

    int type() const override;

private:
    //初始化基本信息
    void initMyTableText();

    // void updateHeight();

signals:
    void sig_hideRectMouse(bool);

private slots:
    void slot_MyTable(QRectF rect);

public:
    qreal intervalW;
    qreal intervalH;

public:
    void setRect(QRectF rect);

    QRectF getRect();
    bool m_isSelect = false;

private:
    // QGraphicsProxyWidget* m_proxy;
    QRectF m_rect;
};