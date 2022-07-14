#include "viewcheck.h"
#include "../MajorWindow/ToolDefine.h"
#include "ui_viewcheck.h"

#include "attribute.h"

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

    m_label.resize(m_centerWidget->size());
    m_titleWidget->resize(m_centerWidget->width(), 35);

    deleteBtn->move(m_titleWidget->width() - 35, 0);

    resetBtn->move(m_titleWidget->width() - 75, 0);
}

void ViewCheck::initViewCheck()
{
    m_centerWidget = new QWidget(this);
    m_centerWidget->resize(width(), height());
    m_titleWidget = new QWidget(this);

    // m_titleWidget->setStyleSheet("background-color: gray");

    loadBtn = new QPushButton(TR("载入"), m_centerWidget);
    m_attribute = new QAction(TR("属性"));

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
    connect(m_attribute, SIGNAL(triggered()), SLOT(slot_attribute()));

    connect(deleteBtn, SIGNAL(clicked()), this, SLOT(slot_deleteBtnClicked()));
    connect(resetBtn, SIGNAL(clicked()), this, SLOT(slot_resetBtnClicked()));
}

void ViewCheck::initTitleWidget()
{
    m_titleWidget->resize(m_centerWidget->width(), 35);
    //背景透明
    m_titleWidget->setAttribute(Qt::WA_TranslucentBackground);

    //删除和复原按钮
    deleteBtn = new QPushButton(m_titleWidget);
    resetBtn = new QPushButton(m_titleWidget);
    deleteBtn->setObjectName("deleteBtn");
    resetBtn->setObjectName("resetBtn");

    deleteBtn->resize(30, 30);
    resetBtn->resize(30, 30);
    deleteBtn->setEnabled(false);
    // deleteBtn->show();
    // resetBtn->show();
}

void ViewCheck::contextMenuEvent(QContextMenuEvent* event)
{
    Q_UNUSED(event)
    QMenu* menu = new QMenu();
    menu->addAction(m_attribute);
    menu->exec(cursor().pos());
}

void ViewCheck::slot_attribute()
{
    Attribute* attribute = new Attribute;
    attribute->show();
}

void ViewCheck::slot_deleteBtnClicked()
{
    QMessageBox::StandardButton standardBtn;

    standardBtn = QMessageBox::question(nullptr, "Tips", TR("是否重新上传?"));
    if (standardBtn == QMessageBox::Yes)
    {
        loadBtn->show();
        m_label.hide();
        deleteBtn->setEnabled(false);
    }
    else if (standardBtn == QMessageBox::No)
    {
        return;
    }
}

void ViewCheck::slot_resetBtnClicked()
{
    m_label.move(QPoint(0, 0));
}

void ViewCheck::slot_loadImage()
{

    m_curFilePth = QFileDialog::getOpenFileName(nullptr, TR("选择图片"), "./", "*.png;;*.jpg");
    if (m_curFilePth.isEmpty() || m_curFilePth.isNull())
    {
        return;
    }

    m_image.load(m_curFilePth);
    m_pixmap = QPixmap::fromImage(m_image);

    // Qt::KeepAspectRatioByExpanding左右全部填充,保持比例填充
    m_pixmap = m_pixmap.scaled(m_centerWidget->size(), Qt::KeepAspectRatio);

    m_label.setPixmap(m_pixmap);
    // printf("%d,%d\n", m_pixmap.width(), m_pixmap.height());
    m_label.setParent(m_centerWidget);

    m_label.resize(m_pixmap.size());
    m_label.show();
    loadBtn->hide();
    deleteBtn->setEnabled(true);
}