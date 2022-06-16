#include "cmajor.h"
#include "ui_cmajor.h"

#include <QColorDialog>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFontDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMenu>
#include <QMessageBox>
#include <QMetaClassInfo>
#include <QMetaObject>
#include <QPluginLoader>
#include <QPrinter>
#include <QTextBlock>
#include <QTextDocument>
#include <QTextLayout>
#include <QTextStream>
#include <QTextTable>

//#include "ys_rio.h"
#include "ys_rbase.h"
//#include "ys_rnode.h"
//#include <iostream>

// using namespace std;
using namespace ys;

enum enum_num
{
    CFIND = 1,
    JOURNAL = 2
};

CMajor::CMajor(QWidget* parent) : QMainWindow(parent), ui(new Ui::CMajor)
{
    ui->setupUi(this);

    initTimer();
    initMenuBar();
    initToolBar();
    initCenterWidget();
    initFindWidget();
    initTableWidget();

    initConnection();
    m_font = m_wid->m_textEdit->font();

    RArray<int> array(L"张佳旭", nullptr);

    // json_node_<const uint> node;
    // RArray<int> array(nullptr,nullptr);
}

CMajor::~CMajor()
{
    delete ui;
}

void CMajor::isLoadPlugin()
{
    QFile file;
    file.setFileName("WatchLog.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    QString time = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    QTextStream stream(&file);

    if (!loadPlugin())
    {
        stream << time << QString("插件加载失败!");
        stream << endl;

        file.close();
    }
    else
    {
        stream << QString("插件加载成功");
        stream << endl;
        file.close();
    }
}

void CMajor::initMenuBar()
{
    QMenu* file = new QMenu("文件");
    QMenu* edit = new QMenu("编辑");
    QMenu* search = new QMenu("查看");
    QMenu* insert = new QMenu("插入");
    QMenu* setting = new QMenu("设置");

    //! 文件功能 动作
    QAction* newCreat = new QAction("新建");
    QAction* newWindow = new QAction("新建窗口");
    QAction* open = new QAction("打开");
    QAction* save = new QAction("保存");
    QAction* otherSave = new QAction("另存为");
    // QAction *printFile = new QAction("打印");
    QAction* exitDocument = new QAction("退出");

    //设置Save的快捷键
    save->setShortcut(QKeySequence::Save);
    newCreat->setShortcut(QKeySequence::New);
    open->setShortcut(QKeySequence::Open);
    otherSave->setShortcut(QKeySequence::SaveAs);
    // printFile->setShortcut(QKeySequence::Print);
    // printFile->setShortcut(QKeySequence::Quit);

    file->addAction(newCreat);
    file->addAction(newWindow);
    file->addAction(open);
    file->addAction(save);
    file->addAction(otherSave);
    // file->addAction(printFile);
    file->addAction(exitDocument);

    connect(newCreat, SIGNAL(triggered()), this, SLOT(slot_creatDocument()));
    connect(newWindow, SIGNAL(triggered()), this, SLOT(slot_creatDocumentWindow()));
    connect(open, SIGNAL(triggered()), this, SLOT(slot_openFile()));
    connect(save, SIGNAL(triggered()), this, SLOT(slot_save()));
    connect(otherSave, SIGNAL(triggered()), this, SLOT(slot_otherSave()));
    // connect(printFile,SIGNAL(triggered()),this,SLOT(slot_printFile()));
    connect(exitDocument, SIGNAL(triggered()), this, SLOT(slot_exitDocument()));

    //! 编辑
    QAction* revoke = new QAction("撤销");
    QAction* shear = new QAction("剪切");
    QAction* copy = new QAction("复制");
    QAction* paste = new QAction("粘贴");
    QAction* remove = new QAction("删除");
    QAction* find = new QAction("查找");
    QAction* typeface = new QAction("字体");
    QAction* color = new QAction("颜色");

    edit->addAction(revoke);
    edit->addAction(shear);
    edit->addAction(copy);
    edit->addAction(paste);
    edit->addAction(remove);
    edit->addAction(find);
    edit->addAction(typeface);
    edit->addAction(color);

    connect(revoke, SIGNAL(triggered()), this, SLOT(slot_revoke()));
    connect(shear, SIGNAL(triggered()), this, SLOT(slot_shear()));
    connect(copy, SIGNAL(triggered()), this, SLOT(slot_copy()));
    connect(paste, SIGNAL(triggered()), this, SLOT(slot_paste()));
    connect(remove, SIGNAL(triggered()), this, SLOT(slot_remove()));
    connect(find, SIGNAL(triggered()), this, SLOT(slot_search()));
    connect(typeface, SIGNAL(triggered()), this, SLOT(slot_typeface()));
    connect(color, SIGNAL(triggered()), this, SLOT(slot_color()));

    //! 查看
    QMenu* scale = new QMenu("缩放");
    search->addMenu(scale);
    QAction* zoomIn = new QAction("放大");
    QAction* zoomOut = new QAction("缩小");
    QAction* defaulted = new QAction("默认比例");
    scale->addAction(zoomIn);
    scale->addAction(zoomOut);
    scale->addAction(defaulted);

    connect(zoomIn, SIGNAL(triggered()), this, SLOT(slot_zoomIn()));
    connect(zoomOut, SIGNAL(triggered()), this, SLOT(slot_zoomOut()));
    connect(defaulted, SIGNAL(triggered()), this, SLOT(slot_defaulted()));

    //! 插入
    QAction* insertImage = new QAction("插入图片");
    QAction* insertForm = new QAction("插入表格");
    insert->addAction(insertImage);
    insert->addAction(insertForm);

    connect(insertImage, SIGNAL(triggered()), this, SLOT(slot_insertImage()));
    connect(insertForm, SIGNAL(triggered()), this, SLOT(slot_insertForm()));

    //!设置
    QAction* size = new QAction("任务栏字体");
    setting->addAction(size);
    connect(size, SIGNAL(triggered()), this, SLOT(slot_menuBarFont()));

    ui->menubar->addMenu(file);
    ui->menubar->addMenu(edit);
    ui->menubar->addMenu(search);
    ui->menubar->addMenu(insert);
    ui->menubar->addMenu(setting);
}

void CMajor::initToolBar()
{
    m_toolBar = new QToolBar("title");
    addToolBar(m_toolBar);

    QAction* newCreat = new QAction("新建");
    QAction* open = new QAction("打开");
    QAction* shear = new QAction("剪切");
    QAction* copy = new QAction("复制");
    QAction* paste = new QAction("粘贴");

    m_toolBar->addAction(newCreat);
    m_toolBar->addAction(open);
    m_toolBar->addAction(shear);
    m_toolBar->addAction(copy);
    m_toolBar->addAction(paste);

    //对于临时的对象，他们的信号槽连接都是在本地连接的
    connect(newCreat, SIGNAL(triggered()), this, SLOT(slot_creatDocument()));
    connect(open, SIGNAL(triggered()), this, SLOT(slot_openFile()));
    connect(shear, SIGNAL(triggered()), this, SLOT(slot_shear()));
    connect(copy, SIGNAL(triggered()), this, SLOT(slot_copy()));
    connect(paste, SIGNAL(triggered()), this, SLOT(slot_paste()));
    // connect(,SIGNAL(triggered()),this,SLOT());
}

void CMajor::initCenterWidget()
{
    m_wid = new CenterWidget;
    m_wid->resize(width(), height());
    m_wid->initWidget();
    setCentralWidget(m_wid);
}

//!所有的连接信号槽都在这里
void CMajor::initConnection()
{
    //定时器
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_timeOut()));
    // textEdit文本改变
    connect(m_wid->m_textEdit, SIGNAL(textChanged()), this, SLOT(slot_textChanged()));

    connect(m_findWid, SIGNAL(sig_findText(QString)), this, SLOT(slot_findBtnClicked(QString)));

    connect(m_table, SIGNAL(sig_tableRowColumn(QString, QString)), this, SLOT(slot_tableRowColumn(QString, QString)));
}

void CMajor::initFindWidget()
{
    m_findWid = new CFind;
}

void CMajor::initTableWidget()
{
    m_table = new CTable;
}

//!实时更新状态栏的 光标 行列数
void CMajor::updateStatusBar()
{

    QTextCursor tc = m_wid->m_textEdit->textCursor();
    // QTextLayout *pLayout = tc.block().layout();

    //光标在一行中的位置
    //光标的绝对位置-当前的文本块的起始位置，得出光标所在列数
    m_column = QString::number(tc.position() - tc.block().position());

    m_row = QString::number(tc.block().firstLineNumber());
    // m_lineCount = QString::number(pLayout->lineForTextPosition(nCurpos).lineNumber() + tc.block().firstLineNumber());

    ui->statusbar->showMessage("行：" + m_row + " 列：" + m_column);
}

void CMajor::initTimer()
{
    m_timer = new QTimer;
    m_timer->setInterval(600);
    m_timer->start();
}

void CMajor::setWinTitle(QString winTitle)
{
    setWindowTitle(winTitle);
}

void CMajor::setFilePathAName(QString path)
{
    m_curFilePath = path;
    QStringList list = path.split("/");
    int len = list.count();
    m_curFileName = list.at(len - 1);
}

void CMajor::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event)
    m_wid->resize(width(), height() - 30);
    m_wid->resizeWidget();

    // qDebug()<<QString("CMajor 宽：%1，高：%2").arg(width()).arg(height());
    // qDebug()<<"触发resizeEvent";
    QWidget::resizeEvent(event);
}

void CMajor::closeEvent(QCloseEvent* event)
{
    Q_UNUSED(event);
    if (windowTitle().contains("*"))
    {

        //另存为或者不保存
        QMessageBox::StandardButton standard = QMessageBox::information(nullptr, "Tips", "是否保存对此文件的修改", QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);

        if (standard == QMessageBox::Save)
        {

            //处理保存逻辑
            QFile file(m_curFilePath);

            //判断是否存在，如果不存在那么就另存为后关闭
            if (!file.exists())
            {
                //不存在
                QString fileName = QFileDialog::getSaveFileName(nullptr, "Save", "./");
                QFile file(fileName);
                file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
                if (!file.isOpen())
                {
                    // QMessageBox::critical(nullptr,"Tips","文件保存失败，请重试!");
                    return;
                }
                QTextStream stream(&file);
                stream << m_wid->m_textEdit->toHtml();
            }
            else
            {
                file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
                if (!file.isOpen())
                {
                    // QMessageBox::critical(nullptr,"Tips","文件打开失败!");
                    return;
                }
                //存在
                QTextStream stream(&file);
                stream << m_wid->m_textEdit->toHtml();
            }
        }
        else if (standard == QMessageBox::Cancel)
        {

            event->ignore();
            return;
        }
    }

    m_wid->close();
    m_curFileName = "";
    m_curFilePath = "";
}

void CMajor::slot_creatDocument()
{

    if (windowTitle().contains("*"))
    {

        //另存为或者不保存
        QMessageBox::StandardButton standard = QMessageBox::information(nullptr, "Tips", "是否保存对此文件的修改", QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);

        if (standard == QMessageBox::Save)
        {

            //处理保存逻辑

            QFile file(m_curFilePath);

            //判断是否存在，如果不存在那么就另存为后关闭
            if (!file.exists())
            {
                //不存在
                QString fileName = QFileDialog::getSaveFileName(nullptr, "Save", "./");
                QFile file(fileName);
                file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
                if (!file.isOpen())
                {
                    // QMessageBox::critical(nullptr,"Tips","文件保存失败，请重试!");
                    return;
                }

                QTextStream stream(&file);
                stream << m_wid->m_textEdit->toHtml();
                file.close();
                QMessageBox::information(nullptr, "Tips", "保存成功!");
            }
            else
            {
                file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
                if (!file.isOpen())
                {
                    // QMessageBox::critical(nullptr,"Tips","文件打开失败!");
                    return;
                }
                //存在
                QTextStream stream(&file);
                stream << m_wid->m_textEdit->toHtml();
                file.close();
            }
        }
        else if (standard == QMessageBox::Cancel)
        {

            return;
        }
    }

    m_wid->close();

    //新的文档名称
    setWinTitle("新建文件");
    initCenterWidget();
    initConnection();
    m_wid->resizeWidget();
    m_curFileName = "";
    m_curFilePath = "";
}

bool CMajor::slot_creatDocumentWindow()
{
    CMajor* major = new CMajor;
    major->show();
    return true;
}

bool CMajor::slot_openFile()
{

    QString fileName = QFileDialog::getOpenFileName(nullptr, "Tips", "./");

    //从文件路径中获取文件名称,并显示到窗口上
    setFilePathAName(fileName);

    QFile file(m_curFilePath);
    file.open(QIODevice::ReadOnly);
    if (!file.isOpen())
    {
        // QMessageBox::information(nullptr,"Tips","打开文件失败!");
        return false;
    }

    QTextStream stream(&file);
    m_wid->m_textEdit->setText(stream.readAll());

    file.close();
    setWindowTitle(m_curFileName);

    //记录日志
    logFile->PrintLog(QString("打开文件:{文件名:%1,文件路径:%2").arg(m_curFileName).arg(m_curFilePath));

    return true;
}

//!另存为
bool CMajor::slot_otherSave()
{

    QString fileName = QFileDialog::getSaveFileName(nullptr, "Tips", "./");
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    if (!file.isOpen())
    {
        // QMessageBox::information(nullptr,"Tips","打开文件失败!");
        return false;
    }
    QTextStream stream(&file);
    stream << m_wid->m_textEdit->toHtml();
    QMessageBox::information(nullptr, "Tips", "保存成功!");

    setFilePathAName(fileName);

    setWindowTitle(m_curFileName);

    //记录日志
    logFile->PrintLog(QString("另存为:{文件名:%1,文件路径:%2").arg(m_curFileName).arg(m_curFilePath));

    return true;
}

void CMajor::slot_printFile()
{

    qDebug() << "slot_printFile";
}

void CMajor::slot_exitDocument()
{
    if (m_curFileName.contains("*") || windowTitle().contains("*"))
    {
        //另存为或者不保存
        QMessageBox::StandardButton standard = QMessageBox::information(nullptr, "Tips", "是否保存对此文件的修改", QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);

        if (standard == QMessageBox::Save)
        {

            //处理保存逻辑
            QFile file(m_curFilePath);

            //判断是否存在，如果不存在那么就另存为后关闭
            if (!file.exists())
            {
                //不存在
                QString fileName = QFileDialog::getSaveFileName(nullptr, "Save", "./");
                QFile file(fileName);
                file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
                if (!file.isOpen())
                {
                    QMessageBox::critical(nullptr, "Tips", "文件保存失败，请重试!");
                    return;
                }

                QTextStream stream(&file);
                stream << m_wid->m_textEdit->toHtml();
                file.close();
            }
            else
            {
                file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
                if (!file.isOpen())
                {
                    QMessageBox::critical(nullptr, "Tips", "文件打开失败!");
                    return;
                }
                //存在
                QTextStream stream(&file);
                stream << m_wid->m_textEdit->toHtml();
                file.close();
            }
        }
        else if (standard == QMessageBox::Cancel)
        {

            return;
        }
    }
    m_curFileName = "";
    m_curFilePath = "";

    m_findWid->close();
    m_table->close();

    qApp->closeAllWindows();
    qApp->quit();
}

void CMajor::slot_copy()
{
    m_wid->m_textEdit->copy();
}

void CMajor::slot_revoke()
{
    m_wid->m_textEdit->undo();
}

void CMajor::slot_shear()
{
    m_wid->m_textEdit->cut();
}

void CMajor::slot_paste()
{
    m_wid->m_textEdit->paste();
}

void CMajor::slot_remove()
{
    m_wid->m_textEdit->textCursor().removeSelectedText();
}

void CMajor::slot_search()
{
    m_findWid->show();
}

void CMajor::slot_insertImage()
{
    QString imageName = QFileDialog::getOpenFileName(nullptr, "Tips", "./", "*.png;;*.jpg;;*.gif");
    QTextCursor cursor = m_wid->m_textEdit->textCursor();
    QTextImageFormat imageFormat;
    imageFormat.setName(imageName);
    imageFormat.setWidth(100);
    imageFormat.setHeight(100);

    cursor.insertImage(imageFormat);
}

void CMajor::slot_insertForm()
{
    m_table->show();
}

void CMajor::slot_typeface()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok);
    //设置全部字体
    if (m_wid->m_textEdit->textCursor().selectedText().isNull() || m_wid->m_textEdit->textCursor().selectedText().isEmpty())
    {

        m_wid->m_textEdit->setFont(font);
    }
    else
    {
        //只设置选中的字体
        QTextCharFormat fmt;                                  //文本字符格式
        fmt.setFont(font);                                    //字体
        QTextCursor cursor = m_wid->m_textEdit->textCursor(); //获取文本光标
        // cursor.mergeCharFormat(fmt);//光标后的文字就用该格式显示
        cursor.setCharFormat(fmt);
    }
}

//保存文件
void CMajor::slot_save()
{
    //路径为空执行 另存为 功能
    if (m_curFilePath.isNull() || m_curFilePath.isEmpty())
    {
        slot_otherSave();
    }
    else
    {

        QFile file(m_curFilePath);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
        if (!file.isOpen())
        {
            // QMessageBox::information(nullptr,"Tips","打开文件失败!");
            return;
        }

        QTextStream stream(&file);
        stream << m_wid->m_textEdit->toHtml();

        // QMessageBox::information(nullptr,"Tips","保存成功!");
        m_curFileName.remove("*");
        setWindowTitle(m_curFileName);
        file.close();
    }
}

void CMajor::slot_zoomOut()
{
    m_wid->m_textEdit->zoomOut(2);
}

void CMajor::slot_zoomIn()
{
    m_wid->m_textEdit->zoomIn(2);
}

void CMajor::slot_defaulted()
{
    m_wid->m_textEdit->setFont(m_font);
}

//!把查找到的文本进行添加蓝色高光
void CMajor::slot_findBtnClicked(QString findText)
{
    myhighlighter = new MyHighLighter(m_wid->m_textEdit->document(), findText);
}

void CMajor::slot_color()
{
    QColor color = QColorDialog::getColor();
    QString flags = m_wid->m_textEdit->textCursor().selectedText();

    //如果没有选中的，那么就会全部变颜色
    if (flags.isNull() || flags.isEmpty())
    {

        m_wid->m_textEdit->setTextColor(color);
    }
    else
    {
        //只改变选中的部分

        QTextCharFormat format;
        format.setForeground(color);
        QTextCursor cursor = m_wid->m_textEdit->textCursor();
        cursor.setCharFormat(format);
    }
}

void CMajor::slot_timeOut()
{
    updateStatusBar();
}

void CMajor::slot_textChanged()
{
    QString documentName = windowTitle();
    if (documentName.contains("*"))
    {
        return;
    }
    setWindowTitle(documentName + "*");
}

void CMajor::slot_tableRowColumn(QString row, QString column)
{
    QTextCursor cursor = m_wid->m_textEdit->textCursor();

    QTextTable* table = cursor.insertTable(row.toInt(), column.toInt());
    QTextTableFormat format = table->format();
    format.setWidth(200);
    format.setBorder(1);
    format.setBorderCollapse(1);

    // format.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    // format.setBorderBrush(Qt::black);

    table->setFormat(format);
}

void CMajor::slot_menuBarFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok);
    ui->menubar->setFont(font);
}

bool CMajor::loadPlugin()
{
    QDir curPath("../bin");

    for (const QString& fileName : curPath.entryList(QDir::Files))
    {

        QPluginLoader loader(curPath.absoluteFilePath(fileName));
        QObject* obj = loader.instance();

        if (obj)
        {
            logFile = qobject_cast<Plugin*>(obj);
            if (logFile)
            {
                return true;
            }
        }
    }

    return false;
}
