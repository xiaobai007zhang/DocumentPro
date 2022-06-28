#include "centerwidget.h"
#include "ui_centerwidget.h"
#include <QDebug>
#include <QPushButton>
#include <QRegularExpression>

CenterWidget::CenterWidget(QWidget* parent) : QWidget(parent), ui(new Ui::CenterWidget)
{
    ui->setupUi(this);
}

CenterWidget::~CenterWidget()
{
    delete ui;
}

void CenterWidget::initWidget()
{
    m_wid = new QWidget(this);
    m_textEdit = new QTextEdit(m_wid);
}

void CenterWidget::resizeWidget()
{
    m_wid->resize(width(), height() - 44);
    m_textEdit->resize(m_wid->width(), m_wid->height());
}
