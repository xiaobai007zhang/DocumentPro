#pragma once

#include "ToolDefine.h"
#include "ui_attribute.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QWidget>
enum PixStatus
{
    TOHEIGHT,    //填充高度
    TOWIDTH,     //填充宽度
    TOKEEPRATIO, //保持比例
    TOEXPAND     //填充宽和高
};
class Attribute : public QWidget
{
    Q_OBJECT

public:
    Attribute(QWidget* parent = Q_NULLPTR);
    ~Attribute();

private:
    void initAttribute();
    //初始化单选按钮
    void initAttrRadioBtn();

private slots:
    void slot_Htoggled(bool);
    void slot_Wtoggled(bool);
    void slot_Ktoggled(bool);
    void slot_Etoggled(bool);

signals:
    void sig_okBtnClick(PixStatus);
    void sig_cancelBtnClick(PixStatus);

private:
    // PixStatus M_FLAG;
    QRadioButton* toHeight;
    QRadioButton* toWidth;
    QRadioButton* toKeepRatio;
    QRadioButton* toExpand;

    PixStatus M_FLAG, ORIGIN;

private:
    Ui::Attribute ui;
};
