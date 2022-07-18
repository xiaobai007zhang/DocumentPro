#include "viewcheck.h"
#include "../MajorWindow/ToolDefine.h"
#include "ui_viewcheck.h"

#include "attribute.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>

ViewCheck::ViewCheck(QWidget* parent) : QWidget(parent), ui(new Ui::ViewCheck)
{
    ui->setupUi(this);
    loadStyleSheet(":/ViewCheck.qss");
    initViewCheck();
    initTitleWidget();
    initConnection();

    // printf("%d,%d", loadBtn->width(), loadBtn->height());
}

ViewCheck::~ViewCheck()
{
    delete ui;
}

void ViewCheck::showTime()
{
}

void ViewCheck::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event)

    loadBtn->move(width() / 2 - 50, height() / 2 - 50);
    m_centerWidget->resize(width(), height());

    //m_label->resize(m_centerWidget->size());
    m_titleWidget->resize(m_centerWidget->width(), 35);
    //m_scroll->resize(width(), height());
    center->resize(width(), height());
    uploadBtn->move(m_titleWidget->width() - 35, 0);


    resetPosBtn->move(m_titleWidget->width() - 75, 0);
}

void ViewCheck::initViewCheck()
{
    m_centerWidget = new QWidget(this);
    m_centerWidget->resize(width(), height());
    m_titleWidget = new QWidget(this);

    //m_scroll = new MyScrollArea(m_centerWidget);
    //m_label = m_scroll->getLabel();
    uploadBtn = new QPushButton(m_titleWidget);
    resetPosBtn = new QPushButton(m_titleWidget);

    center = new QtDrawingPaperEditor(m_centerWidget,m_centerWidget->width(),m_centerWidget->height());

    loadBtn = new QPushButton(TR("载入"), m_centerWidget);
    

    //设置按钮的字体大小
    QFont font = loadBtn->font();
    font.setPointSize(15);
    loadBtn->setFont(font);
    loadBtn->resize(100, 100);
}

void ViewCheck::loadStyleSheet(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        // logfile
        return;
    }

    QString contents = file.readAll();

    setStyleSheet(contents);
}

void ViewCheck::initConnection()
{
    connect(loadBtn, SIGNAL(clicked()), this, SLOT(slot_loadImage()));
    

    connect(uploadBtn, SIGNAL(clicked()), this, SLOT(slot_uploadBtnClicked()));
    connect(resetPosBtn, SIGNAL(clicked()), this, SLOT(slot_resetPosBtnClicked()));
}

void ViewCheck::initTitleWidget()
{
    m_titleWidget->resize(m_centerWidget->width(), 35);
    //背景透明
    m_titleWidget->setAttribute(Qt::WA_TranslucentBackground);
    
    //删除和复原按钮

    uploadBtn->setParent(m_titleWidget);
    resetPosBtn->setParent(m_titleWidget);
    uploadBtn->setObjectName("deleteBtn");
    resetPosBtn->setObjectName("resetBtn");
    
    uploadBtn->resize(30, 30);
    resetPosBtn->resize(30, 30);
    uploadBtn->setEnabled(false);
    // deleteBtn->show();
    // resetBtn->show();
}

void ViewCheck::contextMenuEvent(QContextMenuEvent* event)
{
    Q_UNUSED(event)
    QMenu menu;
    QAction *action = menu.addAction(TR("属性"));

    connect(action, SIGNAL(triggered()), this,SLOT(slot_attribute()));
    menu.exec(cursor().pos());
    
    //disconnect(action, SIGNAL(triggered()), this,SLOT(slot_attribute()));
}

void ViewCheck::slot_attribute()
{
    qDebug() << "slot";
    Attribute* attribute = new Attribute;
    attribute->show();
}

void ViewCheck::slot_uploadBtnClicked()
{
    QMessageBox::StandardButton standardBtn;

    standardBtn = QMessageBox::question(nullptr, "Tips", TR("是否重新上传?"));
    if (standardBtn == QMessageBox::Yes)
    {
        loadBtn->show();
        //m_label->hide();
        QPixmap* pix = center->getPixmap();
        pix->load("");
        uploadBtn->setEnabled(false);
    }
    else if (standardBtn == QMessageBox::No)
    {
        return;
    }
}

void ViewCheck::slot_resetPosBtnClicked()
{
    //*pix = pix->scaled(m_centerWidget->size(), Qt::KeepAspectRatio);
    center->resizeEvent(nullptr);
    //center->setPaperWH(m_centerWidget->width(), m_centerWidget->height());
    //*pix = pix->scaled(m_centerWidget->size(), Qt::IgnoreAspectRatio);
    //pix->scaledToHeight(m_centerWidget->height());
    center->update();
}

void ViewCheck::slot_loadImage()
{

    QString fileName = QFileDialog::getOpenFileName(nullptr, TR("选择图片"), "./", "*.png;;*.jpg");
    if (fileName.isEmpty() || fileName.isNull())
    {
        return;
    }
    
    QPixmap tmpPix(fileName);
    QPixmap * pix = center->getPixmap();
    //*pix = tmpPix.scaled(200,700, Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
    pix->load(fileName);
    
    //*pix = pix->scaled(center->size(), Qt::KeepAspectRatio);
    loadBtn->hide();
    uploadBtn->setEnabled(true);
}