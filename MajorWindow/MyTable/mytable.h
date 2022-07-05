#pragma once

#include "ToolDefine.h"
#include <QGraphicsRectItem>
#include <QObject>
#include <vector>

class MyTableText;
class MyTable : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    QRectF boundingRect() const override;

    QPainterPath shape() const override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

public:
    qreal getIntervalW();

    qreal getIntervalH();

public:
    QRectF getRect();

    int getRow();

    int getCol();

    void setRect(QRectF rect);

private:
    void initTableWidget();

    void updateHeight();

signals:
    void sig_hideRectMouse(bool);

private:
    // QGraphicsProxyWidget* m_proxy;

    //��¼����
    int m_row;
    int m_col;

    QRectF m_rect;

    QGraphicsTextItem* item;

    qreal intervalW;
    qreal intervalH;

    bool m_isPress;



public:
    MyTable(int row, int col, QRectF rect);
    ~MyTable();

public:
    //�ṹ������
    //MyTableText** m_tableText;
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

    // void focusInEvent(QFocusEvent* event) override;

    // void focusOutEvent(QFocusEvent* event) override;
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    //��ʼ��������Ϣ
    void initMyTableText();

    void updateHeight();

    void setRect(QRectF rect);

signals:
    void sig_hideRectMouse(bool);


public:
    qreal intervalW;
    qreal intervalH;

private:
    // QGraphicsProxyWidget* m_proxy;

    //��¼����
    int m_row;
    int m_col;
    QRectF m_rect;
};