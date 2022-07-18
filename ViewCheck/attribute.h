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
    TOHEIGHT,    //���߶�
    TOWIDTH,     //�����
    TOKEEPRATIO, //���ֱ���
    TOEXPAND     //����͸�
};
class Attribute : public QWidget
{
    Q_OBJECT

public:
    Attribute(QWidget* parent = Q_NULLPTR);
    ~Attribute();

private:
    void initAttribute();
    //��ʼ����ѡ��ť
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
