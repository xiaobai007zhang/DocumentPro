#pragma once

#include <QScrollArea>

class MyScrollArea : public QScrollArea
{
    Q_OBJECT

public:
    MyScrollArea(QWidget* parent);
    ~MyScrollArea();

protected:
    void wheelEvent(QWheelEvent* event) override;
};
