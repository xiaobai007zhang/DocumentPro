#pragma once
#define TABLE_TYPE QGraphicsItem::UserType + 1

#include "ToolDefine.h"
#include <QGraphicsRectItem>
#include <QObject>
#include <vector>

#include "ToolDefine.h"

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

    int type() const override;

public:
    qreal getIntervalW();
    qreal getIntervalH();

    QRectF boundingRect() const override;

    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;

public:
    QRectF getRect();

    int getRow();

    int getCol();

    void setRect(QRectF rect);

private:
    void initTableWidget();

    MyTableText* findItem(int row, int col);

    QPolygonF getRotatePolygonFromRect(QRectF rectIn);

signals:
    void sig_hideRectMouse(bool);

    void sig_changeSelect();

    void sig_updateSize(STATE_FLAG, qreal);

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

    QRectF m_rect, m_topRect, m_leftRect, m_rightRect, m_bottomRect;
    QPolygonF m_rectPolygon;

    QGraphicsTextItem* item;

    qreal intervalW;
    qreal intervalH;
    bool m_isRepeat;

    bool m_isJoin;

    QPointF m_startPos;

    STATE_FLAG M_FLAG;

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
//=========================================================================================
class MyTableText : public QGraphicsTextItem
{
    Q_OBJECT
public:
    MyTableText(QRectF rect, QGraphicsItem* parent = nullptr);
    ~MyTableText();
    MyTableText(QGraphicsItem* parent = nullptr) : m_rect(0, 0, 0, 0), QGraphicsTextItem(parent)
    {
        initMyTableText();
    }
    QRectF boundingRect() const override;

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    QPainterPath shape() const override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    // void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    // void keyReleaseEvent(QKeyEvent* event) override;

    void focusInEvent(QFocusEvent* event) override;

    void focusOutEvent(QFocusEvent* event) override;

    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;

    void wheelEvent(QGraphicsSceneWheelEvent* event) override;

    int type() const override;

private:
    //��ʼ��������Ϣ
    void initMyTableText();

    QPolygonF getPolygonFromRect(QRectF rectIn);

signals:
    void sig_hideRectMouse(bool);

private slots:
    void slot_MyTable(QRectF rect);
    void slot_changeSelect();

    // void slot_updateSize(STATE_FLAG, qreal distance);

public:
    qreal intervalW;
    qreal intervalH;

public:
    void setRect(QRectF rect);

    void setIndex(int row, int col);

    int getRow();
    int getCol();

    void setX(int);
    void setY(int);

    int getX();
    int getY();

    QRectF getRect();
    bool m_isSelect;

private:
    QRectF m_rect;

    //��¼��ǰ������������Ϊ����
    int m_row;
    int m_col;
    int m_x, m_y;

    QPolygonF m_oldRectPolygon;
    STATE_FLAG M_FLAG;
};