#include "cfind.h"
#include "ui_cfind.h"

#include <QHBoxLayout>


CFind::CFind(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFind)
{
    ui->setupUi(this);

    initFindWidget();
    initConnection();
}

CFind::~CFind()
{
    delete ui;
}

void CFind::initFindWidget()
{

    QHBoxLayout *hLayout = new QHBoxLayout(this);

    m_lineEdit = new QLineEdit;
    m_findBtn = new QPushButton("查找");

    hLayout->addWidget(m_lineEdit);
    hLayout->addWidget(m_findBtn);
    resize(400,60);

}

void CFind::initConnection()
{
    connect(m_findBtn,&QPushButton::clicked,[=](){
        m_findText = m_lineEdit->text();
        if(m_findText.isNull() || m_findText.isEmpty()){
            return;
        }

        emit sig_findText(m_findText);
    });
}
