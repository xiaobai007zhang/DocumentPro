#pragma once

#include <QLabel>
#include <QMouseEvent>
#include <QWidget>
class MyLabel : public QLabel
{
    Q_OBJECT

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    QPoint m_startPos;
    bool m_isPress;

public:
    MyLabel(const QSize size, QWidget* parent = nullptr);
    MyLabel(QWidget* parent = nullptr);
    ~MyLabel();
};
