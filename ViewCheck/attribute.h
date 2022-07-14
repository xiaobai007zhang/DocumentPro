#pragma once

#include "ui_attribute.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

class Attribute : public QWidget
{
    Q_OBJECT

public:
    Attribute(QWidget* parent = Q_NULLPTR);
    ~Attribute();

private:
    void initAttribute();

private:
    Ui::Attribute ui;
};
