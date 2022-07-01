#pragma once

#include <QGraphicsScene>
#include <QMouseEvent>

class MyGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    MyGraphicsScene(const int& width, const int& height, QObject* parent = nullptr);
    MyGraphicsScene(QRectF rect, QObject* parent = nullptr);
    MyGraphicsScene(QObject* parent = nullptr);
    ~MyGraphicsScene();

public:
    void setWidHei(const int& width, const int& height);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void wheelEvent(QGraphicsSceneWheelEvent* event) override;
    // void contextMenuEvent(QGraphicsSceneContextMenuEvent* contextMenuEvent) override;

signals:
    void sig_rectFrame(QSize size, QPointF point, bool);

public slots:
    void slot_hideRectMouse(bool);

private:
    QPointF m_curPos;
    bool m_isPress;
    QGraphicsRectItem* m_rectMouse;
    //����1�Ļ���֤��Ҫ�϶��ˣ�������ʹ���ִ��Ĭ�ϲ���
    bool m_textItemFlag;

    //��� 0xcdcdcdcdҰָ��
    bool initFlag;
};