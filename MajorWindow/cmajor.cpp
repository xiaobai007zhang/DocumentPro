#include "cmajor.h"
#include "ui_cmajor.h"

#include <QColorDialog>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFontDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMenu>
#include <QMessageBox>
#include <QMetaClassInfo>
#include <QMetaObject>
#include <QPluginLoader>

#include <QTextBlock>
#include <QTextDocument>
#include <QTextLayout>
#include <QTextStream>
#include <QTextTable>


#include "ysbase.h"
using namespace ys;

static short g_textNumber;

// json对象1
R_STRUCT_BEGIN(CHILD1)
R_INTARRAY(intArray)
R_FIXSTRING(fixString, 10)
R_BOOLARRAY(boolArray)
R_STRUCT_END;

// json对象2
R_STRUCT_BEGIN(CHILD2)
R_DOUBLEARRAY(doubleArray)
R_FIXDOUBLEARRAY(fixDouble, 10)
R_OBJ(obj, CHILD1)
R_STRUCT_END;

// json对象3
R_STRUCT_BEGIN(CHILD3)
R_STRING(str)
R_STRINGARRAY(strArray)
R_STRUCT_END;

// json根对象，操作其他json子对象
R_STRUCT_BEGIN(PARENT)
R_OBJARRAY(objArray1, CHILD1)
R_OBJARRAY(objArray2, CHILD2)
R_OBJARRAY(objArray3, CHILD3)
R_INT(iInt)
R_OBJ(obj, CHILD3)
R_UTF8STRINGARRAY(utf8StrArray)
R_STRUCT_END;

//测试宏 R_SAX_STRUCT_BEGIN,包括SAX字段的类都是包含着友元类
R_SAX_STRUCT_BEGIN(TEST)
R_SAX_STRUCT_END;

//测试宏 R_SAX_FUNC_BEGIN ,定义了TEST的SAX序列化函数，并调用TEST类的普通序列化函数

RBoxD box;

//测试宏 R_STRUCT_DERIVE, 该宏的功能为：让第一个参数的类继承第二个参数的类
R_STRUCT_DERIVE(TDERIVE, PARENT)
R_STRUCT_END;

//测试宏 R_STRUCT_BASE， 跟普通的R_STRUCT_BEGIN一样，就是多了一个静态函数New(int type)，需要重写静态函数，否则编译报错
R_STRUCT_BASE(BASE)
R_STRUCT_END;

enum enum_num
{
	CFIND = 1,
	JOURNAL = 2
};

CMajor::CMajor(QWidget* parent) : QMainWindow(parent), ui(new Ui::CMajor), m_textEnable(false) {
	ui->setupUi(this);

	initTimer();
	initMenuBar();
	initToolBar();
	initGraphics();

	initCenterWidget();
	initFindWidget();
	initTableWidget();

	initConnection();

	//练习接口
	// printf("%d\n", arr.Count());
	// RArray<R<int>> arr(L"张佳旭", nullptr);
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
		stream << time << QStringLiteral("插件加载失败!");
		stream << endl;

		file.close();
	}
	else
	{
		stream << QStringLiteral("插件加载成功");
		stream << endl;
		file.close();
	}
}

void CMajor::initMenuBar()
{
	QMenu* file = new QMenu(QStringLiteral("文件"));
	QMenu* edit = new QMenu(QStringLiteral("编辑"));
	QMenu* search = new QMenu(QStringLiteral("查看"));
	QMenu* insert = new QMenu(QStringLiteral("插入"));
	QMenu* setting = new QMenu(QStringLiteral("设置"));

	//! 文件功能 动作
	QAction* newCreat = new QAction(QStringLiteral("新建"));
	QAction* newWindow = new QAction(QStringLiteral("新建窗口"));
	QAction* open = new QAction(QStringLiteral("打开"));
	QAction* save = new QAction(QStringLiteral("保存"));
	QAction* otherSave = new QAction(QStringLiteral("另存为"));
	// QAction *printFile = new QAction("打印");
	QAction* exitDocument = new QAction(QStringLiteral("退出"));

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
	QAction* revoke = new QAction(QStringLiteral("撤销"));
	QAction* shear = new QAction(QStringLiteral("剪切"));
	QAction* copy = new QAction(QStringLiteral("复制"));
	QAction* paste = new QAction(QStringLiteral("粘贴"));
	QAction* remove = new QAction(QStringLiteral("删除"));
	//QAction* find = new QAction(QStringLiteral("查找"));
	QAction* typeface = new QAction(QStringLiteral("字体"));
	QAction* color = new QAction(QStringLiteral("颜色"));

	edit->addAction(revoke);
	edit->addAction(shear);
	edit->addAction(copy);
	edit->addAction(paste);
	edit->addAction(remove);
	//edit->addAction(find);
	edit->addAction(typeface);
	edit->addAction(color);

	connect(revoke, SIGNAL(triggered()), this, SLOT(slot_revoke()));
	connect(shear, SIGNAL(triggered()), this, SLOT(slot_shear()));
	connect(copy, SIGNAL(triggered()), this, SLOT(slot_copy()));
	connect(paste, SIGNAL(triggered()), this, SLOT(slot_paste()));
	connect(remove, SIGNAL(triggered()), this, SLOT(slot_remove()));
	//connect(find, SIGNAL(triggered()), this, SLOT(slot_search()));
	connect(typeface, SIGNAL(triggered()), this, SLOT(slot_typeface()));
	connect(color, SIGNAL(triggered()), this, SLOT(slot_color()));

	//! 查看
	QMenu* scale = new QMenu(QStringLiteral("缩放"));
	search->addMenu(scale);
	QAction* zoomIn = new QAction(QStringLiteral("放大"));
	QAction* zoomOut = new QAction(QStringLiteral("缩小"));
	QAction* defaulted = new QAction(QStringLiteral("默认比例"));
	scale->addAction(zoomIn);
	scale->addAction(zoomOut);
	scale->addAction(defaulted);

	connect(zoomIn, SIGNAL(triggered()), this, SLOT(slot_zoomIn()));
	connect(zoomOut, SIGNAL(triggered()), this, SLOT(slot_zoomOut()));
	connect(defaulted, SIGNAL(triggered()), this, SLOT(slot_defaulted()));

	//! 插入
	QAction* insertImage = new QAction(QStringLiteral("插入图片"));
	QAction* insertForm = new QAction(QStringLiteral("插入表格"));
	insert->addAction(insertImage);
	insert->addAction(insertForm);

	connect(insertImage, SIGNAL(triggered()), this, SLOT(slot_insertImage()));
	connect(insertForm, SIGNAL(triggered()), this, SLOT(slot_insertForm()));

	//!设置
	QAction* size = new QAction(QStringLiteral("任务栏字体"));
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

	QAction* newCreat = new QAction(QStringLiteral("新建"));
	QAction* open = new QAction(QStringLiteral("打开"));
	//QAction* shear = new QAction(QStringLiteral("剪切"));
	//QAction* copy = new QAction(QStringLiteral("复制"));
	//QAction* paste = new QAction(QStringLiteral("粘贴"));
	QAction* textFrame = new QAction(QStringLiteral("文本框"));
	QAction* remove = new QAction(QStringLiteral("删除"));


	m_tempTextFrame = textFrame;
	m_toolBar->addAction(newCreat);
	m_toolBar->addAction(open);
	//m_toolBar->addAction(shear);
	//m_toolBar->addAction(copy);
	//m_toolBar->addAction(paste);
	m_toolBar->addAction(textFrame);
	m_toolBar->addAction(remove);


	//对于临时的对象，他们的信号槽连接都是在本地连接的
	connect(newCreat, SIGNAL(triggered()), this, SLOT(slot_creatDocument()));
	connect(open, SIGNAL(triggered()), this, SLOT(slot_openFile()));
	//connect(shear, SIGNAL(triggered()), this, SLOT(slot_shear()));
	//connect(copy, SIGNAL(triggered()), this, SLOT(slot_copy()));
	//connect(paste, SIGNAL(triggered()), this, SLOT(slot_paste()));
	connect(textFrame, SIGNAL(triggered()), this, SLOT(slot_textFrame()));
	connect(remove, SIGNAL(triggered()), this, SLOT(slot_remove()));
	// connect(,SIGNAL(triggered()),this,SLOT());
}

void CMajor::initCenterWidget()
{
	m_wid = new CenterWidget;
	m_wid->resize(width(), height());
	m_wid->initWidget();
	//setCentralWidget(m_wid);
	//text = new MyGraphicsTextItem;
	//text->setPlainText("1231111111");
	//m_scene->addItem(text);
	setCentralWidget(m_view);
}

//!所有的连接信号槽都在这里
void CMajor::initConnection()
{
	//定时器
	connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_timeOut()));
	// textEdit文本改变
	//connect(m_wid->m_textEdit, SIGNAL(textChanged()), this, SLOT(slot_textChanged()));
	connect(m_scene, SIGNAL(changed(const QList<QRectF>)), this, SLOT(slot_textChanged(const QList<QRectF>)));

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

void CMajor::initGraphics()
{
	resize(width(), height());
	m_view = new QGraphicsView(this);
	m_scene = new MyGraphicsScene(width(), height());

	connect(m_scene, SIGNAL(sig_rectFrame(QSize, QPointF, bool)), this, SLOT(slot_rectFrame(QSize, QPointF, bool)));

	m_view->setFixedSize(width(), height());
	m_view->setScene(m_scene);
	m_view->setAcceptDrops(true);

	CenterWidget().setFocusPolicy(Qt::NoFocus);
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

	ui->statusbar->showMessage(QStringLiteral("行：") + m_row + QStringLiteral(" 列：") + m_column);
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

BASE* BASE::New(int type)
{
	printf("type:%d\n", type);
	return nullptr;
}
void CMajor::getJson()
{
	QFile file("./zjx.json");
	file.open(QIODevice::ReadOnly);
	if (!file.isOpen())
	{
		logFile->errorLog("json file open failed!");
		return;
	}
	QTextStream stream(&file);
	stream.setCodec("UTF-8");
	QString strFile = stream.readAll();

	//====================================================
	//测试接口功能
	//CHILD1 child1;
	//CHILD2 child2;
	//CHILD3 child3;
	//PARENT parent;
	//BASE base;

	//base.New(10);

	//for (int i = 0; i < 10; i++)
	//{
	//	child1.intArray.Add(i);
	//}
	//child1.boolArray.Add(true);
	//child1.boolArray.Add(false);
	//child1.boolArray.Add(false);

	//child1.fixString[0] = child1.fixString.Count();
	//child1.fixString.Set(L"zhang", strlen("zhang"));

	//// child2
	//child2.doubleArray.Add(1.1234);
	//child2.doubleArray.Add(2.1234);
	//child2.obj = child1;
	//double* dou = new double(10.5);
	//child2.fixDouble.Set(dou, 1);

	//// child3
	//child3.str.Add(L'a');
	//child3.strArray.Add(L"奥里给");
	//child3.strArray.Add(L"兄弟们");
	//////parent

	//parent.iInt = 10;
	//parent.iInt.SetName(L"整形");
	//parent.obj = child3;
	//parent.objArray1.Add(std::move(child1));

	//parent.objArray3.Add(std::move(child3));
	//// child3.SetName(L"child4");
	//parent.objArray2.Add(std::move(child2));
	//// parent.objArray3.Add(std::move(child3));
	//parent.objArray1.Add(std::move(child1));

	//json_save(L"./test.json", parent, true);

	/*if (json_save(L"./test.json", child3, true) == true) {
		m_wid->m_textEdit->setText("success");
	}
	else {
		m_wid->m_textEdit->setText("failed");
	}*/
	//=====================================================

	// QJsonParseError error;

	// QJsonDocument doc = QJsonDocument::fromJson(strFile.toUtf8(), &error);
	//  logFile->errorLog("");
	// QJsonObject root = doc.object();
	// QJsonValue zhangjiaxu = root.value("zhangjiaxu");
	// if (zhangjiaxu.type() == QJsonValue::Object)
	//{
	//	QJsonObject obj = zhangjiaxu.toObject();
	//	qDebug() << obj.value("name").toString();
	//	qDebug() << obj.value("age").toString();
	//	qDebug() << obj.value("sex").toString();
	// }
}

void CMajor::setJson(const QString& fileName)
{
	QFile file(fileName);
	file.open(QIODevice::WriteOnly | QIODevice::Truncate);

	if (!file.isOpen())
	{
		logFile->errorLog("Json File to failed!");
		return;
	}

	QJsonObject root;
	QJsonObject obj1;

	obj1.insert("age", QJsonValue("22"));
	obj1.insert("name", QStringLiteral("张佳旭"));
	obj1.insert("sex", QStringLiteral("男"));

	QJsonObject obj2({ {"age", "21"}, {"name", QStringLiteral("刘家宝")}, {"sex", QStringLiteral("男")} });
	QJsonObject obj3({ {"age", "22"}, {"name", QStringLiteral("王世博")}, {"sex", QStringLiteral("男")} });

	QJsonArray array;
	array.append("language");
	array.append("math");
	array.append("calculate");
	QJsonDocument doc;

	root.insert("zhangjiaxu", obj1);
	root.insert("liujiabao", obj2);
	root.insert("wangshibo", obj3);

	root.insert("array", array);
	doc.setObject(root);

	QTextStream stream(&file);
	stream.setCodec("UTF-8");

	stream << doc.toJson();

	file.close();
}

void CMajor::resizeEvent(QResizeEvent* event)
{
	Q_UNUSED(event);

	QWidget::resizeEvent(event);
}

void CMajor::closeEvent(QCloseEvent* event)
{
	Q_UNUSED(event);
	if (windowTitle().contains("*"))
	{

		//另存为或者不保存
		QMessageBox::StandardButton standard =
			QMessageBox::information(nullptr, "Tips", QStringLiteral("是否保存对此文件的修改"), QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);

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
				QPixmap pix = m_view->grab();
				pix.save(fileName, "PNG");
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
				QPixmap pix = m_view->grab();
				pix.save(m_curFileName, "PNG");

			}
		}
		else if (standard == QMessageBox::Cancel)
		{

			event->ignore();
			return;
		}
	}

	//m_wid->close();
	m_curFileName = "";
	m_curFilePath = "";
}


void CMajor::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Delete) {
		slot_remove();
	}
	QMainWindow::keyPressEvent(event);
}


void CMajor::slot_creatDocument()
{

	if (windowTitle().contains("*"))
	{

		//另存为或者不保存
		QMessageBox::StandardButton standard =
			QMessageBox::information(nullptr, "Tips", QStringLiteral("是否保存对此文件的修改"), QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);

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
	setWinTitle(QStringLiteral("新建文件"));
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
	//添加功能，在读取json的时候需要固定住大小
	//=======================================

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
	file.remove();

	QPixmap pix = m_view->grab();
	pix.save(fileName, "PNG");

	QMessageBox::information(nullptr, "Tips", QStringLiteral("保存成功!"));

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
		QMessageBox::StandardButton standard =
			QMessageBox::information(nullptr, "Tips", QStringLiteral("是否保存对此文件的修改"), QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);

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
					QMessageBox::critical(nullptr, "Tips", QStringLiteral("文件保存失败，请重试!"));
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
					QMessageBox::critical(nullptr, "Tips", QStringLiteral("文件打开失败!"));
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

	//m_itemGroup = m_scene->createItemGroup(m_scene->selectedItems());
	//qDebug() << "group bound: " << m_itemGroup->boundingRect().toRect();
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


}

//如果有选中的就删除选中的，否则删除当前的文本框
void CMajor::slot_remove()
{
	//删除的话分为两种情况，第一种情况是获得了焦点状态，再细分为焦点状态的选中文本
	QGraphicsTextItem* item = dynamic_cast<QGraphicsTextItem*>(m_scene->focusItem());
	if (item) {
		if (item->textCursor().selectedText().isNull() || item->textCursor().selectedText().isEmpty()) {
			m_scene->removeItem(item);
		}
		else {
			item->textCursor().removeSelectedText();

		}
	}
	else {
		//第二种情况就是选中状态，而非焦点状态
		QList<QGraphicsItem*>list = m_scene->selectedItems();
		for (const auto& value : list) {
			m_scene->removeItem(value);
		}
	}


}

void CMajor::slot_search()
{
	m_findWid->show();
}

void CMajor::slot_insertImage()
{
	QString imageName = QFileDialog::getOpenFileName(nullptr, "Tips", "./", "*.png;;*.jpg;;*.gif");
	//QGraphicsPixmapItem* pixmap = new QGraphicsPixmapItem;
	MyGraphicsPixmapItem* pixmap = new MyGraphicsPixmapItem;

	QPixmap pix(imageName);
	pix.scaled(25, 25, Qt::KeepAspectRatio);

	pixmap->setScale(0.5);

	pixmap->setPixmap(pix);
	m_scene->addItem(pixmap);
	pixmap->setFlag(QGraphicsItem::ItemIsMovable);
	pixmap->setFlag(QGraphicsItem::ItemIsSelectable);

	connect(pixmap, SIGNAL(sig_hideRectMouse(bool)), m_scene, SLOT(slot_hideRectMouse(bool)));


	//m_scene->addItem(pixmap);
	/*QTextCursor cursor = m_wid->m_textEdit->textCursor();
	QTextImageFormat imageFormat;
	imageFormat.setName(imageName);
	imageFormat.setWidth(100);
	imageFormat.setHeight(100);

	cursor.insertImage(imageFormat);*/
}

void CMajor::slot_insertForm()
{
	m_table->show();
}

void CMajor::slot_typeface()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok);

	//先暂定为只要是焦点的都可以改变字体,后期在区分各种不同的结构
	QGraphicsItem* itemTmp = m_scene->focusItem();
	QGraphicsTextItem* item = dynamic_cast<QGraphicsTextItem*>(itemTmp);
	//ASSERT(item);

	if (item) {
		//区分选中和未选中两种
		if (item->textCursor().selectedText().isEmpty() || item->textCursor().selectedText().isNull()) {
			item->setFont(font);
		}
		else {
			QTextCharFormat fmt;        //文本字符格式
			fmt.setFont(font);
			QTextCursor cursor = item->textCursor();
			cursor.setCharFormat(fmt);
			//item->setTextCursor(cursor);

		}

	}
	else {

		//还有一种是选中状态的字体
		QList<QGraphicsItem*> list = m_scene->selectedItems();
		if (list.isEmpty()) {
			return;
		}

		for (QGraphicsItem* tmp : list) {
			MyGraphicsTextItem* item = dynamic_cast<MyGraphicsTextItem*>(tmp);
			item->setFont(font);
			item->setText(item->toPlainText());
		}

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
		file.remove();

		QPixmap pix = m_view->grab();
		pix.save(m_curFilePath, "PNG");
		// QMessageBox::information(nullptr,"Tips","保存成功!");
		m_curFileName.remove("*");
		setWindowTitle(m_curFileName);
	}
}

void CMajor::slot_zoomOut()
{	//第一种情况，有选中的就会方法选中的进行调整
	//第二种情况，没有选中的会全部进行调整
	if (m_scene->selectedItems().isEmpty()) {

		for (QGraphicsItem* tmp : m_scene->items()) {
			//缩放比例最小为原型
			if (tmp->scale() - 0.5 <= 0) {
				return;
			}
			tmp->setScale(tmp->scale() - 0.5);
		}
	}
	else {
		for (QGraphicsItem* tmp : m_scene->selectedItems()) {
			//缩放比例最小为原型
			if (tmp->scale() - 0.5 <= 0) {
				return;
			}
			tmp->setScale(tmp->scale() - 0.5);
		}
	}
}

void CMajor::slot_zoomIn()
{
	/*m_wid->m_textEdit->zoomIn(2);*/
	//第一种情况，有选中的就会方法选中的进行调整
	//第二种情况，没有选中的会全部进行调整

	if (m_scene->selectedItems().isEmpty()) {

		for (QGraphicsItem* tmp : m_scene->items()) {

			if (tmp->scale() + 0.5 >= 5) {
				return;
			}

			tmp->setScale(tmp->scale() + 0.5);
		}
	}
	else {
		for (QGraphicsItem* tmp : m_scene->selectedItems()) {
			if (tmp->scale() + 0.5 >= 5) {
				return;
			}
			tmp->setScale(tmp->scale() + 0.5);
		}
	}


}

//恢复默认比例
void CMajor::slot_defaulted()
{
	for (const auto& tmp : m_scene->items()) {
		tmp->setScale(1);
	}

}

//!把查找到的文本进行添加蓝色高光
void CMajor::slot_findBtnClicked(QString findText)
{
	myhighlighter = new MyHighLighter(m_wid->m_textEdit->document(), findText);
}

void CMajor::slot_color()
{
	QColor color = QColorDialog::getColor();

	//三种情况，第一种焦点状态，分为两个子部分，光标选中和未选中，
	//选择多个item都要改变

	QGraphicsItem* tmp = m_scene->focusItem();
	QGraphicsTextItem* item = dynamic_cast<QGraphicsTextItem*>(tmp);
	if (item) {
		QString flags = item->textCursor().selectedText();
		if (flags.isEmpty() || flags.isNull()) {
			//当前的焦点Item都要改变颜色
			item->setDefaultTextColor(color);

		}
		else {
			//只改变选中的部分颜色
			QTextCharFormat format;
			format.setForeground(color);
			QTextCursor cursor = item->textCursor();
			cursor.setCharFormat(format);
			//item->setTextCursor(cursor);
		}
	}
	else {
		//选中的Item都要改变颜色
		QList<QGraphicsItem*>list = m_scene->selectedItems();
		for (QGraphicsItem* tmp : list) {
			QGraphicsTextItem* item = dynamic_cast<QGraphicsTextItem*>(tmp);
			if (item) {
				item->setDefaultTextColor(color);
			}
		}
	}


	{
		//只改变选中的部分

		QTextCharFormat format;
		format.setForeground(color);
		//QTextCursor cursor = ->textCursor();
		//cursor.setCharFormat(format);
	}
}

//设置标志位，证明要创建文本框了
void CMajor::slot_textFrame()
{
	m_scene->clearFocus();

	//m_tempTextFrame->setCheckable(true);
	//m_scene->slot_hideRectMouse(false);
	//printf("slot_textFrame\n");
	if (m_textEnable == false) {
		m_textEnable = true;

	}
	else if (m_textEnable == true) {
		m_textEnable = false;
	}
}

void CMajor::slot_eraseTextFrame(QGraphicsTextItem* item)
{
	m_scene->removeItem(item);

}

void CMajor::slot_rectFrame(QSize size, QPointF point, bool flag)
{
	//证明当前要拖动窗口，而不是要选择或者创建窗口
	if (flag) {
		return;
	}

	if (m_textEnable) {
		MyGraphicsTextItem* item = new MyGraphicsTextItem(QRectF(0, 0, abs(size.width()), abs(size.height())));
		m_scene->addItem(item);
		if (size.height() < 0 && size.width() < 0) {
			item->moveBy(point.rx(), point.ry());
		}

		if (size.height() < 0 && size.width() > 0) {
			item->moveBy(point.rx() - abs(size.width()), point.ry());
		}

		if (size.height() > 0 && size.width() > 0)
			item->moveBy(point.rx() - size.width(), point.ry() - size.height());

		if (size.width() < 0 && size.height() > 0) {
			item->moveBy(point.rx(), point.ry() - abs(size.height()));
		}
		connect(item, SIGNAL(sig_loseFocusText(QGraphicsTextItem*)), this, SLOT(slot_eraseTextFrame(QGraphicsTextItem*)));
		connect(item, SIGNAL(sig_needSceneUpdate()), this, SLOT(slot_sceneUpdate()));
		connect(item, SIGNAL(sig_deleteKey()), this, SLOT(slot_remove()));
		connect(item, SIGNAL(sig_hideRectMouse(bool)), m_scene, SLOT(slot_hideRectMouse(bool)));

		//item->setName(g_textNumber++, QString::number(g_textNumber));
		//item->setPlainText("11111111111");
	}
	else {
		if (size.height() > 0 && size.width() > 0) {
			QRect rect(point.toPoint() - QPoint(size.width(), size.height()), size);
			QPainterPath path;
			path.addRect(rect);
			m_scene->setSelectionArea(path);
		}

		if (size.height() < 0 && size.width() < 0) {
			size = QSize(abs(size.width()), abs(size.height()));
			QRect rect(point.toPoint(), size);
			QPainterPath path;
			path.addRect(rect);
			m_scene->setSelectionArea(path);

			//QGraphicsRectItem* tmp = new QGraphicsRectItem;
			//tmp->setRect(rect);
			//tmp->setBrush(Qt::blue);
			//m_scene->addItem(tmp);
		}

		if (size.height() < 0 && size.width() > 0) {
			size = QSize(abs(size.width()), abs(size.height()));
			QRect rect(point.toPoint() - QPoint(size.width(), 0), size);
			QPainterPath path;
			path.addRect(rect);
			m_scene->setSelectionArea(path);
		}

		if (size.height() > 0 && size.width() < 0) {
			size = QSize(abs(size.width()), abs(size.height()));
			QRect rect(point.toPoint() - QPoint(0, size.height()), size);
			QPainterPath path;
			path.addRect(rect);
			m_scene->setSelectionArea(path);
		}

	}


	m_textEnable = false;
	//m_tempTextFrame->setCheckable(false);
}

void CMajor::slot_timeOut()
{
	//updateStatusBar();

}

void CMajor::slot_textChanged(const QList<QRectF>)
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

void CMajor::slot_sceneUpdate()
{
	m_scene->update();
}

bool CMajor::loadPlugin()
{
	QDir curPath("../bin");
	qDebug() << QDir::currentPath();

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
