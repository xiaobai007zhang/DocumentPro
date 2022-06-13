#include "ctable.h"
#include "ui_ctable.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>

CTable::CTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTable)
{
    ui->setupUi(this);

    initTable();
}

CTable::~CTable()
{
    delete ui;
}

void CTable::initTable()
{
    resize(400,60);
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    QLineEdit *lineRow = new QLineEdit;
    QLineEdit *lineColumn = new QLineEdit;
    QPushButton *creatBtn = new QPushButton("创建");

    lineRow->setInputMask("D");
    lineColumn->setInputMask("D");

    hLayout->addWidget(lineRow);
    hLayout->addWidget(lineColumn);
    hLayout->addWidget(creatBtn);


    connect(creatBtn,&QPushButton::clicked,[=](){
        m_row = lineRow->text();
        m_column = lineColumn->text();
        emit sig_tableRowColumn(m_row,m_column);
    });

}
