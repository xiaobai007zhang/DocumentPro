#pragma once

#include <QGraphicsView>

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    MyGraphicsView(QWidget* parent = nullptr);

    ~MyGraphicsView();

private:
    bool m_isCtrlPress;
    bool m_isFlag;
    bool m_isAltPress;

protected:
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    // void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
};
