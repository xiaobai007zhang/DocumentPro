#include "managertableinfo.h"
#include "ToolDefine.h"
#include <QSizePolicy>

ManagerTableInfo::ManagerTableInfo(QWidget* parent) : QWidget(parent)
{
    ui.setupUi(this);
    initTableInfo();
    initConnection();
    setFixedSize(160, 150);
    setWindowTitle(TR("������"));
    setWindowFlags(Qt::WindowCloseButtonHint);
    setWindowModality(Qt::ApplicationModal);
    // setWindowState(Qt::WindowMinimized);
}

ManagerTableInfo::~ManagerTableInfo()
{
}

void ManagerTableInfo::initTableInfo()
{
    m_rowLabel = new QLabel(TR("��"));
    m_colLabel = new QLabel(TR("��"));
    m_row = new QSpinBox;
    m_col = new QSpinBox;
    m_okBtn = new QPushButton(TR("ȷ��"));
    m_cancelBtn = new QPushButton(TR("ȡ��"));

    m_row->setMinimum(1);
    m_row->setMaximum(8);
    m_col->setMinimum(1);
    m_col->setMaximum(8);

    m_row->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    m_col->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    //����ˮƽ�ģ�һ����ֱ��
    QHBoxLayout* hLayout1 = new QHBoxLayout;
    QHBoxLayout* hLayout2 = new QHBoxLayout;
    QHBoxLayout* hLayout3 = new QHBoxLayout;

    hLayout1->addWidget(m_rowLabel);
    hLayout1->addWidget(m_row);

    hLayout2->addWidget(m_colLabel);
    hLayout2->addWidget(m_col);

    hLayout3->addWidget(m_okBtn);
    hLayout3->addWidget(m_cancelBtn);

    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    vLayout->addLayout(hLayout3);
}

void ManagerTableInfo::initConnection()
{
    connect(m_okBtn, &QPushButton::clicked, [=] {
        emit sig_okClicked(m_row->value(), m_col->value());
        close();
    });
    connect(m_cancelBtn, SIGNAL(clicked()), this, SIGNAL(sig_cancelClicked()));
}
