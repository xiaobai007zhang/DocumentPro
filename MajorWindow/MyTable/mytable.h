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
    QRectF boundingRect() const override;
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

    MyTableText* findItem(int row, int col);

signals:
    void sig_hideRectMouse(bool);

    void sig_changeSelect();
public slots:

    void slot_contentsChanged();
    //���vector
    void slot_MyTable(QRectF);

    //�ϲ���Ԫ��
    void slot_joinTable();

    void slot_repeat(bool flag);

private:
    //��¼����
    int m_row;
    int m_col;

    QRectF m_rect;

    QGraphicsTextItem* item;

    qreal intervalW;
    qreal intervalH;
    bool m_isRepeat;

    bool m_isJoin;

public:
    MyTable(int row, int col, QRectF rect);
    ~MyTable();

public:
    //�ṹ������

    std::vector<MyTableText*> m_tableText;

    //��¼��Ҫ�ϲ��ĵ�Ԫ��
    std::vector<MyTableText*> sg_vecJoin;
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
    //��ʼ��������Ϣ
    void initMyTableText();

    // void updateHeight();

signals:
    void sig_hideRectMouse(bool);

private slots:
    void slot_MyTable(QRectF rect);
    void slot_changeSelect();

public:
    qreal intervalW;
    qreal intervalH;

public:
    void setRect(QRectF rect);

    void setIndex(int row, int col);

    int getRow();
    int getCol();

    QRectF getRect();
    bool m_isSelect = false;

private:
    // QGraphicsProxyWidget* m_proxy;
    QRectF m_rect;

    //��¼��ǰ������������Ϊ����
    int m_row;
    int m_col;
};