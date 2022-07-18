#include "attribute.h"

Attribute::Attribute(QWidget* parent) : QWidget(parent)
{
    ui.setupUi(this);
    initAttribute();

    initAttrRadioBtn();
}

Attribute::~Attribute()
{
}

void Attribute::initAttribute()
{
    setWindowModality(Qt::ApplicationModal);
    setFixedSize(310, 90);
    M_FLAG = TOKEEPRATIO;
    ORIGIN = M_FLAG;
}

void Attribute::initAttrRadioBtn()
{
    toHeight = new QRadioButton(TR("填充高度"));
    toWidth = new QRadioButton(TR("填充宽度"));
    toKeepRatio = new QRadioButton(TR("原图"));
    toExpand = new QRadioButton(TR("填充宽高"));
    toKeepRatio->connect(toHeight, SIGNAL(toggled(bool)), this, SLOT(slot_Htoggled(bool)));
    connect(toWidth, SIGNAL(toggled(bool)), this, SLOT(slot_Wtoggled(bool)));
    connect(toKeepRatio, SIGNAL(toggled(bool)), this, SLOT(slot_Ktoggled(bool)));
    connect(toExpand, SIGNAL(toggled(bool)), this, SLOT(slot_Etoggled(bool)));

    toKeepRatio->setChecked(true);

    QHBoxLayout* hLayoutTitle = new QHBoxLayout;
    hLayoutTitle->addWidget(toHeight);
    hLayoutTitle->addWidget(toWidth);
    hLayoutTitle->addWidget(toKeepRatio);
    hLayoutTitle->addWidget(toExpand);

    QPushButton* okBtn = new QPushButton(TR("确定"));
    QPushButton* cancelBtn = new QPushButton(TR("取消"));

    QHBoxLayout* hLayoutBottom = new QHBoxLayout;
    hLayoutBottom->addWidget(okBtn);
    hLayoutBottom->addWidget(cancelBtn);
    // hLayoutBottom->setSpacing(0);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addLayout(hLayoutTitle);
    layout->addLayout(hLayoutBottom);
    setLayout(layout);

    connect(okBtn, &QPushButton::clicked, [=]() {
        emit sig_okBtnClick(M_FLAG);
        ORIGIN = M_FLAG;
        hide();
    });
    connect(cancelBtn, &QPushButton::clicked, [=]() {
        M_FLAG = ORIGIN;
        switch (M_FLAG)
        {
        case TOHEIGHT:
            toHeight->setChecked(true);
            break;
        case TOWIDTH:
            toWidth->setChecked(true);
            break;
        case TOKEEPRATIO:
            toKeepRatio->setChecked(true);
            break;
        case TOEXPAND:
            toKeepRatio->setChecked(true);
            break;
        }
        hide();
    });
}
void Attribute::slot_Htoggled(bool flag)
{
    if (flag)
    {
        M_FLAG = TOHEIGHT;
    }
}

void Attribute::slot_Wtoggled(bool flag)
{
    if (flag)
    {
        M_FLAG = TOWIDTH;
    }
}

void Attribute::slot_Ktoggled(bool flag)
{
    if (flag)
    {
        M_FLAG = TOKEEPRATIO;
    }
}

void Attribute::slot_Etoggled(bool flag)
{
    if (flag)
    {
        M_FLAG = TOEXPAND;
    }
}
