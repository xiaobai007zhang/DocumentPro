#include "cmajor.h"
#include "ui_cmajor.h"

#include <QCheckBox>
#include <QColorDialog>
#include <QCursor>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFontDialog>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMenu>
#include <QMessageBox>
#include <QScrollBar>

//#include <QMetaClassInfo>
//#include <QMetaObject>
#include <QPluginLoader>
#include <QResizeEvent>

#include <QDragEnterEvent>
#include <QGraphicsProxyWidget>
#include <QMimeData>
#include <QTableWidget>
#include <QTextBlock>
#include <QTextDocument>
#include <QTextLayout>
#include <QTextStream>
#include <QTextTable>

// clang-format off
//#include "ysbase.h"
//using namespace ys;

CMajor::CMajor(QWidget* parent) : QMainWindow(parent),m_imageStartPos(QPoint(0,0))
,ui(new Ui::CMajor), m_textEnable(false),viewPlugin(nullptr),view(nullptr),m_isRepeat(true) ,tableInfo(nullptr),m_tableEnable(false){
	ui->setupUi(this);

	//loadStyleSheet("./Action.qss");
	loadPlugin();
	initCMajor();
	initGraphics();
	initCenterWidget();
	initTimer();
	initMenuBar();
	initToolBar();
	
	initConnection();
	setAcceptDrops(true);
	m_view->setAcceptDrops(false);
	ui->statusbar->hide();

	repeat->setIcon(QIcon(":/yesExpand.png"));
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
		stream << time << TR("插件加载失败!");
		stream << endl;

		file.close();
	}
	else
	{
		stream<<time;
		stream << TR("插件加载成功");
		stream << endl;
		file.close();
	}
}

void CMajor::readJson(const QString & fileName)
{
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);
	if (!file.isOpen()){
		// QMessageBox::information(nullptr,"Tips","打开文件失败!");
		/*if(logFile != nullptr)
		logFile->errorLog(TR("打开 ”%1“ 文件失败!").arg(m_curFileName));*/
	}
	QByteArray byte = file.readAll();
	QJsonDocument doc (QJsonDocument::fromJson(byte));
	QJsonObject obj = doc.object();
	QJsonArray textArr = obj.value("text").toArray();
	QJsonArray pixArr = obj.value("pixmap").toArray();
	QJsonArray tableArr = obj.value("table").toArray();

	QJsonObject scene = obj.value("scene").toObject();
	
	
	//先设置好场景的大小
	qreal width = scene.value("width").toDouble();
	qreal height = scene.value("height").toDouble();
	m_scene->setSceneRect(0, 0, width, height);
	if (view != nullptr) {
		this->resize(height+view->width(),height);
	}
	

	//resizeEvent(nullptr);

	//写一个函数，来加载出不同的元素信息(json对象,type)
	for (QJsonValue value : textArr) {
		QJsonObject obj = value.toObject();
		loadJsonObj(obj,"text");
	}

	for (QJsonValue value : pixArr) {
		QJsonObject obj = value.toObject();
		loadJsonObj(obj,"pixmap");
	}


	for (QJsonValue value : tableArr) {
		//json对象中包括一个基本的信息，还有一个数组，数组记录着所有的单元格信息
		QJsonObject obj = value.toObject();
		loadJsonObj(obj,"table");
	}

	
	//m_scene->setSceneRect(QRectF());
	file.close();
}

void CMajor::initMenuBar()
{
	QMenu* file = new QMenu(TR("文件"));
	QMenu* edit = new QMenu(TR("编辑"));
	QMenu* search = new QMenu(TR("查看"));
	QMenu* insert = new QMenu(TR("插入"));
	QMenu* setting = new QMenu(TR("设置"));
	QMenu* domain = new QMenu(TR("功能区"));

	//! 文件功能 动作
	newCreat = new QAction(TR("新建"));
	newWindow = new QAction(TR("新建窗口"));
	open = new QAction(TR("打开"));
	save = new QAction(TR("保存"));
	otherSave = new QAction(TR("另存为"));
	exitDocument = new QAction(TR("退出"));
	
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
	connect(exitDocument, SIGNAL(triggered()), this, SLOT(slot_exitDocument()));

	//! 编辑
	revoke = new QAction(TR("撤销"));
	shear = new QAction(TR("剪切"));
	copy = new QAction(TR("复制"));
	paste = new QAction(TR("粘贴"));
	remove = new QAction(TR("删除"));
	typeface = new QAction(TR("字体"));
	color = new QAction(TR("颜色"));

	edit->addAction(revoke);
	edit->addAction(shear);
	edit->addAction(copy);
	edit->addAction(paste);
	edit->addAction(remove);
	
	edit->addAction(typeface);
	edit->addAction(color);

	connect(revoke, SIGNAL(triggered()), this, SLOT(slot_revoke()));
	connect(shear, SIGNAL(triggered()), this, SLOT(slot_shear()));
	connect(copy, SIGNAL(triggered()), this, SLOT(slot_copy()));
	connect(paste, SIGNAL(triggered()), this, SLOT(slot_paste()));
	connect(remove, SIGNAL(triggered()), this, SLOT(slot_remove()));
	connect(typeface, SIGNAL(triggered()), this, SLOT(slot_typeface()));
	connect(color, SIGNAL(triggered()), this, SLOT(slot_color()));

	//! 查看
	scale = new QMenu(TR("缩放"));
	search->addMenu(scale);
	zoomIn = new QAction(TR("放大"));
	zoomOut = new QAction(TR("缩小"));
	defaulted = new QAction(TR("默认比例"));
	scale->addAction(zoomIn);
	scale->addAction(zoomOut);
	scale->addAction(defaulted);

	connect(zoomIn, SIGNAL(triggered()), this, SLOT(slot_zoomIn()));
	connect(zoomOut, SIGNAL(triggered()), this, SLOT(slot_zoomOut()));
	connect(defaulted, SIGNAL(triggered()), this, SLOT(slot_defaulted()));

	//! 插入
	insertImage = new QAction(TR("插入图片"));
	//insertForm = new QAction(TR("插入表格"));
	insert->addAction(insertImage);
	//insert->addAction(insertForm);

	connect(insertImage, SIGNAL(triggered()), this, SLOT(slot_insertImage()));
	//connect(insertForm, SIGNAL(triggered()), this, SLOT(slot_insertForm()));

	//!设置
	size = new QAction(TR("任务栏字体"));
	setting->addAction(size);
	connect(size, SIGNAL(triggered()), this, SLOT(slot_menuBarFont()));

	//! 功能区
	//expand = new QAction(TR("扩展窗口"));
	repeat = new QAction(TR("重叠"));
	joinTable = new QAction(TR("合并单元格"));

	connect(repeat,SIGNAL(triggered()),this,SLOT(slot_repeat()));
	//connect(expand,SIGNAL(triggered()),this,SLOT(slot_expand()));
	connect(joinTable,SIGNAL(triggered()),this,SLOT(slot_joinTable()));


	//domain->addAction(expand);
	domain->addAction(repeat);


	ui->menubar->addMenu(file);
	ui->menubar->addMenu(edit);
	ui->menubar->addMenu(search);
	ui->menubar->addMenu(insert);
	ui->menubar->addMenu(setting);
	ui->menubar->addMenu(domain);

}

void CMajor::initToolBar()
{
	m_toolBar = new QToolBar("title");
	//m_toolBar->setFixedHeight(20);
	m_toolBar->setMovable(false);


	addToolBar(m_toolBar);


	QAction* newCreat = new QAction(TR("新建"));
	QAction* open = new QAction(TR("打开"));
	QAction* shear = new QAction(TR("剪切"));
	QAction* copy = new QAction(TR("复制"));
	QAction* paste = new QAction(TR("粘贴"));
	//QAction* textFrame = new QAction(TR("文本框"));
	newCreat->setIcon(QIcon(":/newCreat.png"));
	open->setIcon(QIcon(":/open.png"));
	shear->setIcon(QIcon(":/shear.png"));
	copy->setIcon(QIcon(":/copy.png"));
	paste->setIcon(QIcon(":/paste.png"));




	m_tool = new QToolButton;
	m_tool->setText(TR("文本框"));
	m_tool->setObjectName("tool");
	m_tool->setCheckable(true);
	m_tool->setIcon(QIcon(":/textFrame.png"));
	m_tool->setToolTip(TR("文本框"));

	m_tableTool = new QToolButton;
	m_tableTool->setText(TR("表格框"));
	//m_tableTool->setCheckable(true);
	m_tableTool->setIcon(QIcon(":/tableFrame.png"));
	m_tableTool->setToolTip(TR("表格框"));
	//textFrame->setObjectName("tool");
	
	//m_tempTextFrame = textFrame;
	m_toolBar->addAction(newCreat);
	m_toolBar->addAction(open);
	m_toolBar->addAction(shear);
	m_toolBar->addAction(copy);
	m_toolBar->addAction(paste);
	//m_toolBar->addAction(textFrame);
	m_toolBar->addWidget(m_tool);
	m_toolBar->addWidget(m_tableTool);
	m_toolBar->addAction(remove);
	remove->setIcon(QIcon(":/delete.png"));

	//对于临时的对象，他们的信号槽连接都是在本地连接的
	connect(newCreat, SIGNAL(triggered()), this, SLOT(slot_creatDocument()));
	connect(open, SIGNAL(triggered()), this, SLOT(slot_openFile()));
	connect(shear, SIGNAL(triggered()), this, SLOT(slot_shear()));
	connect(copy, SIGNAL(triggered()), this, SLOT(slot_copy()));
	connect(paste, SIGNAL(triggered()), this, SLOT(slot_paste()));
	//connect(textFrame, SIGNAL(triggered()), this, SLOT(slot_textFrame()));
	connect(m_tool,SIGNAL(clicked()),this,SLOT(slot_textFrame()));
	connect(m_tableTool,SIGNAL(clicked()),this,SLOT(slot_setTableInfo()));
	connect(remove, SIGNAL(triggered()), this, SLOT(slot_remove()));
	
	
	// connect(,SIGNAL(triggered()),this,SLOT());
	//qDebug()<<m_tool->objectName();
}

void CMajor::initCenterWidget()
{
	//setCentralWidget(m_view);
	QHBoxLayout *layout = new QHBoxLayout;
	QSizePolicy sizePolicy = this->sizePolicy();
    sizePolicy.setHorizontalPolicy(QSizePolicy::Preferred);

	if (viewPlugin != nullptr) {
		//view = dynamic_cast<ViewCheck*>(viewPlugin);
		view = new ViewCheck;
		view->setSizePolicy(sizePolicy);
		layout->addWidget(view);
	}
	
	
	
	m_view->setSizePolicy(sizePolicy);
	
	
	layout->addWidget(m_view);

	//layout->addWidget(view);
	//view->setStyleSheet("background-color:blue");
	
	centralWidget()->setLayout(layout);
}

//!所有的连接信号槽都在这里
void CMajor::initConnection()
{
	//定时器
	//connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_timeOut()));
	// textEdit文本改变
	//connect(m_wid->m_textEdit, SIGNAL(textChanged()), this, SLOT(slot_textChanged()));
	connect(m_scene, SIGNAL(changed(const QList<QRectF>)), this, SLOT(slot_textChanged(const QList<QRectF>)));
	
	connect(m_repeatTime,SIGNAL(timeout()),this,SLOT(slot_repeatTime()));

}

void CMajor::initFindWidget()
{
	//m_findWid = new CFind;
}

void CMajor::initTableWidget()
{
	//m_table = new CTable;
}

void CMajor::initGraphics()
{
	//m_view = new QGraphicsView;
	m_view = new MyGraphicsView;
	m_scene = new MyGraphicsScene;

	connect(m_scene, SIGNAL(sig_rectFrame(QSize, QPointF, bool)), this, SLOT(slot_rectFrame(QSize, QPointF, bool)));
	connect(m_view,SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(slot_rightMenu(const QPoint &)));

	m_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	m_view->setContextMenuPolicy(Qt::CustomContextMenu);
	m_view->setScene(m_scene);
	m_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	

}

void CMajor::loadJsonObj(const QJsonObject & obj, const QString & type)
{
	if (type == TR("text")) {
	//文本: x、y、宽、高、缩放、内容、颜色、字体大小、加粗、倾斜、下划线，中划线、字体
	qreal x = obj.value("x").toDouble();
	qreal y = obj.value("y").toDouble();
	qreal width = obj.value("width").toDouble();
	qreal height = obj.value("height").toDouble();
	qreal scale = obj.value("scale").toDouble();
	QString contents = obj.value("contents").toString();
	int red = obj.value("fontColorRed").toInt();
	int green = obj.value("fontColorGreen").toInt();
	int blue = obj.value("fontColorBlue").toInt();
	QColor color(red,green,blue);
	int fontSize = obj.value("fontSize").toInt();
	bool bold = obj.value("fontBold").toBool();
	bool italic = obj.value("fontItalic").toBool();
	bool underLine = obj.value("fontUnderLine").toBool();
	bool strikeout = obj.value("fontStrikeout").toBool();
	qreal zValue = obj.value("zValue").toDouble();

	QString family = obj.value("fontFamily").toString();
	
	
	QFont font;
	font.setBold(bold);
	font.setItalic(italic);
	font.setFamily(family);
	font.setUnderline(underLine);
	font.setStrikeOut(strikeout);
	font.setPointSize(fontSize);
	

	QPointF point(x, y);
	
	MyGraphicsTextItem* myItem = new MyGraphicsTextItem(QRectF(0,0,width,height));
	myItem->setPos(point.x(),point.y());

	myItem->setZValue(zValue);
	myItem->setPlainText(contents) ;
	myItem->setFont(font);
	myItem->setDefaultTextColor(color);

	myItem->setScale(scale);

	connect(myItem, SIGNAL(sig_hideRectMouse(bool)), m_scene, SLOT(slot_hideRectMouse(bool)));
	//connect(this,SIGNAL(sig_expand(bool)),myItem,SLOT(slot_expand(bool)));
	connect(this,SIGNAL(sig_repeat(bool)),myItem,SLOT(slot_repeat(bool)));
	connect(myItem, SIGNAL(sig_loseFocusText(QGraphicsTextItem*)), this, SLOT(slot_eraseTextFrame(QGraphicsTextItem*)));
	connect(myItem, SIGNAL(sig_needSceneUpdate()), this, SLOT(slot_sceneUpdate()));
	myItem->setSelected(true);
	//connect(myItem, SIGNAL(sig_deleteKey()), this, SLOT(slot_remove()));
	myItem->setTextInteractionFlags(Qt::NoTextInteraction);
	m_scene->addItem(myItem);

	}
	else if (type == TR("pixmap")) {
		//图片：x、y、宽、高、比例、图片路径、
		qreal x = obj.value("x").toDouble();
		qreal y = obj.value("y").toDouble();
		//qDebug()<<"load x: "<<x<<",load y: "<<y;

		QPointF point(x, y);

		qreal width = obj.value("width").toDouble();
		qreal height = obj.value("height").toDouble();
		qreal scale = obj.value("scale").toDouble();
		QString imgPth = obj.value("imagePth").toString();
		qreal zValue = obj.value("zValue").toDouble();
		MyGraphicsPixmapItem *myItem = new MyGraphicsPixmapItem(QRectF(0,0,width,height));
		
		myItem->setPos(point.x(), point.y());
		
		//qDebug() << QStringLiteral("========打开========");
		//qDebug() << "myItem->pos(): " << point;
		//qDebug() << "mapFromScene(myItem->scenePos()): " << point;

		myItem->pos();
		myItem->setScale(scale);
		myItem->setImage(imgPth);
		myItem->setZValue(zValue);

		connect(myItem, SIGNAL(sig_hideRectMouse(bool)), m_scene, SLOT(slot_hideRectMouse(bool)));
		connect(this,SIGNAL(sig_repeat(bool)),myItem,SLOT(slot_repeat(bool)));
		m_scene->addItem(myItem);
		//resizeEvent(nullptr);
		//m_scene->update();
	}
	else if(type == TR("table")){
		qreal x = obj.value("x").toDouble();
		qreal y = obj.value("y").toDouble();
		qreal scale = obj.value("scale").toDouble();
		int col = obj.value("col").toInt();
		int row = obj.value("row").toInt();
		qreal height = obj.value("height").toDouble();
		qreal width = obj.value("width").toDouble();
		

		//接下来该读取单元格的信息了
		QJsonArray textArr  = obj.value("tableText").toArray();
		MyTable *table = new MyTable(row,col,QRectF(0,0,width-1,height));
		table->setPos(x,y);
		table->setScale(scale);

		connect(table, SIGNAL(sig_hideRectMouse(bool)), m_scene, SLOT(slot_hideRectMouse(bool)));
		connect(this,SIGNAL(sig_repeat(bool)),table,SLOT(slot_repeat(bool)));
		connect(this,SIGNAL(sig_joinTable()),table,SLOT(slot_joinTable()));
		connect(this,SIGNAL(sig_MyTable(QRectF)),table,SLOT(slot_MyTable(QRectF)));

		for (QJsonValue value : textArr) {
			QJsonObject tmpObj = value.toObject();
			loadTableText(tmpObj,table);
		}

		m_scene->addItem(table);	
		
		//给容器整理元素的排放位置
		for (QGraphicsItem* tmpItem : table->childItems()) {
			MyTableText* text = dynamic_cast<MyTableText*>(tmpItem);
			table->m_tableText.push_back(text);
		}
		//resizeEvent(nullptr);
		//m_scene->update();
	}
}

void CMajor::initTimer()
{
	m_repeatTime = new QTimer;
	m_repeatTime->setInterval(300);
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

//记录当前的高度
void CMajor::initCMajor()
{
	//qDebug() << "width: " << width() << ",height: " << height();
	m_curWidth = width();
	m_curHeight = height();
}

void CMajor::loadTableText(QJsonObject obj,MyTable* parent)
{
	 QString contents = obj.value("contents").toString();
	 qreal intervalH = obj.value("intervalH").toDouble();
	 qreal intervalW = obj.value("intervalW").toDouble();
	 qreal x = obj.value("x").toDouble();
	 qreal y = obj.value("y").toDouble();
	 int row = obj.value("rowIndex").toInt();
	 int col = obj.value("colIndex").toInt();

	 MyTableText* text = new MyTableText(QRect(0,0,intervalW,intervalH),parent);
	 
	 text->setIndex(row,col);
	 text->setPos(x,y);
	 text->setPlainText(contents);

	 text->setX(x);
	 text->setY(y);
	 
	 //text->setRect(QRectF(50,0,intervalW,intervalH));
	 connect(text, SIGNAL(sig_hideRectMouse(bool)), m_scene, SLOT(slot_hideRectMouse(bool)));
	 connect(text->document(),SIGNAL(contentsChanged()),parent,SLOT(slot_contentsChanged()));
	 connect(this,SIGNAL(sig_MyTable(QRectF)),text,SLOT(slot_MyTable(QRectF)));
	 connect(parent,SIGNAL(sig_changeSelect()),text,SLOT(slot_changeSelect()));
	 text->update();
}


void CMajor::resizeEvent(QResizeEvent * event)
{	//qDebug()<<"viewport size: "<<m_view->viewport()->size();
	//qDebug()<<"size: "<<m_view->size();
	Q_UNUSED(event)
	//if (m_isExpand) {
		//允许扩展
		m_scene->setSceneRect(QRectF());
		m_view->setAlignment(Qt::AlignLeft | Qt::AlignTop);
		this->resize(width(),height());
		//view->resize(width()/2,height());
		//m_view->resize(width()/2,height()-80);
		//m_view->move(width()/2,0);
		m_scene->update();
		

}

void CMajor::closeEvent(QCloseEvent* event)
{
	Q_UNUSED(event);
	if (windowTitle().contains("*"))
	{
	QMessageBox::StandardButton standard =
	QMessageBox::information(nullptr, "Tips", TR("是否保存对此文件的修改?"), QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);

		if (standard == QMessageBox::Save)
		{
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
//#ifdef PLUGIN_SUCCESS
					//logFile->errorLog(TR("文件“%1”保存失败").arg(m_curFileName));
//#endif

					return;
				}
			}else{
				//否则文件是已经打开的，可以直接保存
				slot_save();
				}
		
		}else if (standard == QMessageBox::Cancel){
			
			event->ignore();
			return;
		}


	//新的文档名称
	m_curFileName = "";
	m_curFilePath = "";
	
	}
}

//键盘事件
void CMajor::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
		slot_remove();
	}
	if (event->modifiers() == Qt::CTRL) {
		if (event->key() == Qt::Key_C) {
			slot_copy();
		}

		if (event->key() == Qt::Key_V) {
			slot_paste();
		}

		if (event->key() == Qt::Key_A) {
		
			for (QGraphicsItem* item : m_scene->items()) {
				item->setSelected(true);
			}
		}
		if (event->key() == Qt::Key_X) {
			slot_shear();
		}

	}
	//qDebug() << "cmajor key press";
	return QMainWindow::keyPressEvent(event);
}

void CMajor::dragEnterEvent(QDragEnterEvent* event)
{

	//qDebug() << event->mimeData()->hasUrls();
	QStringList FileTypes;
	FileTypes.append("jpg");
	FileTypes.append("png");
	FileTypes.append("txt");
	FileTypes.append("json");
	//FileTypes.append("gif");
	if (event->mimeData()->hasUrls() && event->mimeData()->urls().count() == 1) {

		QFileInfo file(event->mimeData()->urls().at(0).toLocalFile());
		//qDebug() << "event->mimeData()->urls().at(0).toLocalFile(): " << event->mimeData()->urls().at(0).toLocalFile();

		if (FileTypes.contains(file.suffix().toLower())) {
			event->acceptProposedAction();
		}

	}
	//qDebug()<<m_scene->width()<<" "<<m_scene->height();
	QMainWindow::dragEnterEvent(event);
}

void CMajor::dropEvent(QDropEvent* event)
{
	//读取图片的信息
	//qDebug()<<"pos: "<<pos();

	QFileInfo file(event->mimeData()->urls().at(0).toLocalFile());
	QString fileName = event->mimeData()->urls().at(0).toLocalFile();
	if (fileName.contains("txt") || fileName.contains("json")) {
	
		for (QGraphicsItem* item : m_scene->items()) {
		m_scene->removeItem(item);
		delete item; 
		item = nullptr;
		}
		readJson(fileName);
		setFilePathAName(fileName);
		setWindowTitle(m_curFileName);
	}
	else {
	QPixmap pixmap(file.absoluteFilePath());
	
	//QPointF point = m_view->mapToScene(cursor().pos().x() - pos().x(),cursor().pos().y() - pos().y());
	
	//QPoint point = cursor().pos() - pos();
	//qDebug()<<pos();
	//qDebug()<<"frame: "<<frameGeometry().topLeft();
	MyGraphicsPixmapItem* item = new MyGraphicsPixmapItem(QRectF(0,0, pixmap.width(), pixmap.height()));
	//MyGraphicsPixmapItem* item = new MyGraphicsPixmapItem(QRectF(0,0,pixmap.width(), pixmap.height()));
	item->setImage(file.absoluteFilePath());
	m_scene->addItem(item);
	item->moveBy(event->pos().x()/2,event->pos().y()/2);
	//connect(this,SIGNAL(sig_expand(bool)),item,SLOT(slot_expand(bool)));
	connect(this,SIGNAL(sig_repeat(bool)),item,SLOT(slot_repeat(bool)));
	connect(item, SIGNAL(sig_hideRectMouse(bool)), m_scene, SLOT(slot_hideRectMouse(bool)));
	//解决了根据当前视图按照比例缩放
	if(pixmap.width() > m_view->viewport()->width() || pixmap.height() > m_view->viewport()->height())
	m_view->fitInView(item,Qt::KeepAspectRatio);
	
	//emit(sig_expand(true));
	}
	

	QMainWindow::dropEvent(event);
}
void CMajor::slot_creatDocument()
{

	if (windowTitle().contains("*"))
	{
	QMessageBox::StandardButton standard =
	QMessageBox::information(nullptr, "Tips", TR("是否保存对此文件的修改?"), QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);

		if (standard == QMessageBox::Save)
		{
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
//#ifdef PLUGIN_SUCCESS
					//logFile->errorLog(TR("文件“%1”保存失败").arg(m_curFileName));
//#endif

					//return;
				}
			}else{
				//否则文件是已经打开的，可以直接保存
				slot_save();
				}
		
		}else if (standard == QMessageBox::Cancel){
			return;
		}


	//新的文档名称
	

	
	}
	setWinTitle(QStringLiteral("新建文件"));
	m_scene->clear();
	m_curFileName = "";
	m_curFilePath = "";
}

bool CMajor::slot_creatDocumentWindow()
{
	CMajor* major = new CMajor;
	major->show();
	return true;
}

void CMajor::slot_openFile()
{
	//添加功能，在读取json的时候需要固定住大小
	//=======================================


	//刷新一次
	//slot_sceneUpdate();

	QString fileName = QFileDialog::getOpenFileName(nullptr, "Tips", ".");
	if (fileName.isEmpty() || fileName.isNull()) {
		return;
	}

	//如果真的成功打开了文件，那么才会清楚当前所有的内容
	for (QGraphicsItem* item : m_scene->items()) {
		m_scene->removeItem(item);
		delete item;
		item = nullptr;
	}

	//从文件路径中获取文件名称,并显示到窗口上
	setFilePathAName(fileName);

	readJson(fileName);
	
	setWindowTitle(m_curFileName);

	//记录日志
	//#ifdef PLUGIN_SUCCESS
		//logFile->PrintLog(TR("打开文件:{文件名:%1,文件路径:%2").arg(m_curFileName).arg(m_curFilePath));
	//#endif
	return;
}

//!另存为
bool CMajor::slot_otherSave()
{

	QString fileName = QFileDialog::getSaveFileName(nullptr, "Tips", "./");
	if (fileName.isEmpty() || fileName.isNull()) {
		return false;
	}
	QFile file(fileName);
	file.open(QIODevice::WriteOnly);

	QJsonObject scene;
	QJsonArray textArr;
	QJsonArray pixmapArr;
	QJsonArray tableArr;
	
	QJsonObject root;

	QList<QGraphicsItem*>list = m_scene->items();
	for (QGraphicsItem* item : list) {
		QJsonObject obj;
		if (item->type() == QGraphicsTextItem::Type) {
			MyGraphicsTextItem* myItem = dynamic_cast<MyGraphicsTextItem*>(item);
			//文本: x、y、宽、高、缩放、内容、颜色、字体大小、加粗、倾斜、下划线，中划线、字体
			
			//QPoint point(m_view->mapFromScene(myItem->scenePos()));
			QPointF point(myItem->pos());
			obj.insert("x",point.x());
			obj.insert("y",point.y());
			obj.insert("width",myItem->getBoundingRect().width());
			obj.insert("height",myItem->getBoundingRect().height());
			obj.insert("scale",myItem->scale());
			obj.insert("contents",myItem->getStrText());
			obj.insert("zValue",myItem->zValue());
			//读取颜色的RGB值，并保存为字符串
			QColor color = myItem->defaultTextColor();
			
			int red = color.red();
			int green = color.green();
			int blue = color.blue();

			//读取字体大小、加粗、倾斜、下划线，中划线、字体
			QFont font = myItem->font();
			int fontSize = font.pointSize();
			bool fontBold = font.bold();
			bool fontItalic = font.italic();
			bool fontUnderLine = font.underline();
			bool fontStrikeout = font.strikeOut();
			QString fontFamily = font.family();

			obj.insert("fontColorRed",red);
			obj.insert("fontColorGreen",green);
			obj.insert("fontColorBlue",blue);
			obj.insert("fontSize",fontSize);
			obj.insert("fontBold",fontBold);
			obj.insert("fontItalic",fontItalic);
			obj.insert("fontUnderLine",fontUnderLine);
			obj.insert("fontStrikeout",fontStrikeout);
			obj.insert("fontFamily",fontFamily);
			
			textArr.append(obj);
		}
		else if (item->type() == QGraphicsPixmapItem::Type) {
			MyGraphicsPixmapItem* myItem = dynamic_cast<MyGraphicsPixmapItem*>(item);
			//图片：x、y、宽、高、比例、图片路径、

			QPointF point = myItem->pos();

			
			//qDebug()<<TR("========另存为========");
			//qDebug()<<"pos(): "<<myItem->pos();
			//qDebug()<<"scene to view pos(): "<<point;
			////qDebug()<<"transform view pos: "<<m_view->mapFromScene(myItem->scenePos());
			//qDebug()<<"========================";
			obj.insert("x",point.x());
			obj.insert("y",point.y());
			obj.insert("width",myItem->getBoundingRect().width());
			obj.insert("height",myItem->getBoundingRect().height());
			obj.insert("scale",myItem->scale());
			obj.insert("imagePth",myItem->getImagePth());
			obj.insert("zValue",myItem->zValue());
			pixmapArr.append(obj);
			
		}else if (item->type() == TABLE_TYPE) {
			MyTable* myItem = dynamic_cast<MyTable*>(item);
			if (myItem) {
			QPointF point(myItem->pos());
			

			obj.insert("x",point.x());
			obj.insert("y",point.y());
			obj.insert("width",myItem->getRect().width());
			obj.insert("height",myItem->getRect().height());
			obj.insert("row",myItem->getRow());
			obj.insert("col",myItem->getCol());
			obj.insert("scale",myItem->scale());
			
			QJsonArray tmpArr;
			QJsonObject tmpObj;
			for(int i = 0 ; i < myItem->m_tableText.size();++i)
			{	//qDebug()<<"table point: "<<myItem->m_tableText.at(i)->pos();
				//后期可以增加每个文本框的字体，颜色，倾斜等等信息
				tmpObj.insert("x",myItem->m_tableText.at(i)->x());
				tmpObj.insert("y",myItem->m_tableText.at(i)->y());
				tmpObj.insert("intervalW",myItem->m_tableText.at(i)->intervalW);
				tmpObj.insert("intervalH",myItem->m_tableText.at(i)->intervalH);
				tmpObj.insert("contents",myItem->m_tableText.at(i)->toPlainText());
				tmpObj.insert("rowIndex",myItem->m_tableText.at(i)->getRow());
				tmpObj.insert("colIndex",myItem->m_tableText.at(i)->getCol());
				tmpArr.append(tmpObj);
				//qDebug()<<myItem->m_tableText.at(i)->getRow()<<", "<<myItem->m_tableText.at(i)->getCol();
			}

			obj.insert("tableText",tmpArr);
			
			tableArr.append(obj);
			
			
			}
			else {
				continue;
			}

		
	}
		
	}


	//========================SCENE======================//
	scene.insert("width",m_scene->itemsBoundingRect().width());
	scene.insert("height",m_scene->itemsBoundingRect().height());
	
	//===================================================//
	root.insert("scene",scene);
	root.insert("text",textArr);
	root.insert("pixmap",pixmapArr);
	root.insert("table",tableArr);
	
	QJsonDocument doc(root);
	
	
	file.write(doc.toJson());

	file.close();

	QMessageBox::information(nullptr, "Tips", TR("保存成功!"));

	setFilePathAName(fileName);

	setWindowTitle(m_curFileName);


	//记录日志
	//#ifdef PLUGIN_SUCCESS
		//logFile->PrintLog(TR("另存为:{文件名:%1,文件路径:%2").arg(m_curFileName).arg(m_curFilePath));
	//#endif // PLUGIN_SUCCESS

	

	return true;
}

void CMajor::slot_printFile()
{
	//qDebug() << "slot_printFile";
}

void CMajor::slot_exitDocument()
{
	if (m_curFileName.contains("*") || windowTitle().contains("*"))
	{
		//另存为或者不保存
		QMessageBox::StandardButton standard =
		QMessageBox::information(nullptr, "Tips", TR("是否保存对此文件的修改"), QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);

		if (standard == QMessageBox::Save)
		{

			//处理保存逻辑
			QFile file(m_curFilePath);

			//判断是否存在，如果不存在那么就另存为后关闭
			if (!file.exists())
			{
				slot_otherSave();
			}
			else
			{
				slot_save();
			}
		}
		else if (standard == QMessageBox::Cancel)
		{
			return;
		}
	}
	m_curFileName = "";
	m_curFilePath = "";

	qApp->closeAllWindows();
	qApp->quit();
}

void CMajor::slot_copy()
{
	
	ShapeMimeData *data = new ShapeMimeData( m_scene->selectedItems() );
    QApplication::clipboard()->setMimeData(data);
	
}

void CMajor::slot_revoke()
{
	//m_wid->m_textEdit->undo();
}

//剪切
void CMajor::slot_shear()
{
	ShapeMimeData *data = new ShapeMimeData( m_scene->selectedItems() );
    QApplication::clipboard()->setMimeData(data);

	for (QGraphicsItem* item : m_scene->selectedItems()) {
		m_scene->removeItem(item);
		item = nullptr;
	}
}

//粘贴
void CMajor::slot_paste()
{
	QMimeData * mp = const_cast<QMimeData *>(QApplication::clipboard()->mimeData()) ;
    ShapeMimeData * data = dynamic_cast<ShapeMimeData*>( mp );
    if ( data ){
        m_scene->clearSelection();
        for (QGraphicsItem * item : data->items() ) {
            
			AbstractShape *sp = qgraphicsitem_cast<AbstractShape*>(item);
			QGraphicsItem * copy = sp->copy();
			if (sp->type() == QGraphicsTextItem::Type) {
				MyGraphicsTextItem* myItem = dynamic_cast<MyGraphicsTextItem*>(copy);
				connect(myItem, SIGNAL(sig_hideRectMouse(bool)), m_scene, SLOT(slot_hideRectMouse(bool)));
				connect(this,SIGNAL(sig_repeat(bool)),myItem,SLOT(slot_repeat(bool)));
			}
			else if (sp->type() == QGraphicsPixmapItem::Type) {
				MyGraphicsPixmapItem* myItem = dynamic_cast<MyGraphicsPixmapItem*>(copy);
				connect(myItem, SIGNAL(sig_hideRectMouse(bool)), m_scene, SLOT(slot_hideRectMouse(bool)));
				
				connect(this,SIGNAL(sig_repeat(bool)),myItem,SLOT(slot_repeat(bool)));
			}
			else if(sp->type() == TABLE_TYPE){
				MyTable* myItem = dynamic_cast<MyTable*>(copy);
				connect(myItem,SIGNAL(sig_hideRectMouse(bool)),m_scene,SLOT(slot_hideRectMouse(bool)));
				connect(this,SIGNAL(sig_MyTable(QRectF)),myItem,SLOT(slot_MyTable(QRectF)));
				connect(this,SIGNAL(sig_joinTable()),myItem,SLOT(slot_joinTable()));
				
				std::vector<MyTableText*> tableVec;

				for (MyTableText* text : myItem->m_tableText) {
					if (text == nullptr) {
						return;
					}
					MyTableText* item = new MyTableText(QRectF(0, 0, text->intervalW,text->intervalH), myItem);
					item->setIndex(text->getRow(),text->getCol());
					item->setPos(text->getX(),text->getY());
					qDebug()<<item->getX()<<item->getY();
					item->setX(text->getX());
					item->setY(text->getY());
					//item->setPos(text->getX(),text->getY());
					connect(item, SIGNAL(sig_hideRectMouse(bool)), m_scene, SLOT(slot_hideRectMouse(bool)));
					//connect(item->document(),SIGNAL(contentsChanged()),myItem,SLOT(slot_contentsChanged()));
					connect(this,SIGNAL(sig_MyTable(QRectF)),item,SLOT(slot_MyTable(QRectF)));
					connect(myItem,SIGNAL(sig_changeSelect()),item,SLOT(slot_changeSelect()));
					//myItem->m_tableText.push_back(item);
					tableVec.push_back(item);
					
					
			}
				//m_scene->update();
				
				//myItem->update();
				myItem->m_tableText.clear();
				for (MyTableText* text : myItem->m_tableText) {
					delete text;
					text = nullptr;
				}

				for (MyTableText* text : tableVec) {
					myItem->m_tableText.push_back(text);
				}
				
				tableVec.clear();
				//qDebug()<<myItem->m_tableText;

				//tableVec.clear();
				//std::vector<QString> tmpVec;
				
				//MyTable* parent = qgraphicsitem_cast<MyTable*>(sp);
				////复制文本信息

				//for (QGraphicsItem* text : parent->childItems()) {
				//	MyTableText* tmp = dynamic_cast<MyTableText*>(text);
				//	tmpVec.push_back(tmp->toPlainText());
				//}

				//for (int i = 0; i < tmpVec.size(); ++i) {
				//	myItem->m_tableText.at(i)->setPlainText(tmpVec.at(i));
				//}

			}
			
			if ( copy ){
				copy->setSelected(true);
				copy->moveBy(30,30);
				m_scene->addItem(copy);
            }
        }
		//m_scene->clearFocus();
		m_scene->update();
		//m_view->update();
		m_scene->clearFocus();
	}
}

//如果有选中的就删除选中的，否则删除当前的文本框
void CMajor::slot_remove()
{



	//
	//QMimeData* mime  = const_cast<QMimeData*> (QApplication::clipboard()->mimeData());
 //   ShapeMimeData *data = dynamic_cast<ShapeMimeData*>(mime);
	//
	//qDebug()<<"begin: " <<data->items().size();

	//if (data) {
	//	QList<QGraphicsItem*>list = m_scene->selectedItems();
	//	for (QGraphicsItem* value : list) {
	//		for (QGraphicsItem* item : data->items()) {
	//			if (item == value) {
	//				data->removeItem(item);
	//				break;
	//			}
	//		}
	//	}
	//}	
	// //QApplication::clipboard()->setMimeData(data);
	////qDebug()<<"end: " <<data->items().size();
//第二种情况就是选中状态，而非焦点状态
		QList<QGraphicsItem*>list = m_scene->selectedItems();
		for (QGraphicsItem* value : list) {
			m_scene->removeItem(value);
			
			delete value;

			value = nullptr;
		}
		//m_scene->update();
}

void CMajor::slot_insertImage()
{
	QString imageName = QFileDialog::getOpenFileName(nullptr, "Tips", "./", "*.png;;*.jpg;;*.gif");
	//QGraphicsPixmapItem* pixmap = new QGraphicsPixmapItem;

	QPixmap pix(imageName);
	//QPoint point = 
	MyGraphicsPixmapItem* pixmap = new MyGraphicsPixmapItem(QRectF(0,0 , pix.width(), pix.height()));;
	
	//while (pix.width() * pixmap->scale() > width() || pix.height()* pixmap->scale() > height()) {
	//	pixmap->setScale(pixmap->scale() - 0.1);
	//}

	pixmap->setImage(imageName);
	
	m_scene->addItem(pixmap);
	//pixmap->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
	
	//connect(this,SIGNAL(sig_expand(bool)),pixmap,SLOT(slot_expand(bool)));
	connect(this,SIGNAL(sig_repeat(bool)),pixmap,SLOT(slot_repeat(bool)));
	connect(pixmap, SIGNAL(sig_hideRectMouse(bool)), m_scene, SLOT(slot_hideRectMouse(bool)));
	
	
	if (pix.width() > m_view->viewport()->width() || pix.height() > m_view->viewport()->height())
	m_view->fitInView(pixmap, Qt::KeepAspectRatio);
	
	//emit(sig_expand(true));
}

void CMajor::slot_insertForm()
{
	//m_table->show();
}

void CMajor::slot_typeface()
{
		//先暂定为只要是焦点的都可以改变字体,后期在区分各种不同的结构
	bool ok;
	QFont font = QFontDialog::getFont(&ok);

		//不是焦点的处理
		QList<QGraphicsItem*> list = m_scene->selectedItems();
		if (list.isEmpty()) {
			
		}
		else {
			for (QGraphicsItem* tmp : list) {
			MyGraphicsTextItem* item = dynamic_cast<MyGraphicsTextItem*>(tmp);
			
			if (item) {
			QColor color = item->defaultTextColor();
			item->setDefaultTextColor(color);
			item->setFont(font);

			item->setText(item->toPlainText());
			}
			
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
	file.open(QIODevice::WriteOnly);

	if (file.isOpen()) {
		//qDebug()<<TR("打开文件");
	}
	else {
		//qDebug()<<TR("失败");
	}
	QJsonObject scene;
	QJsonArray textArr;
	QJsonArray pixmapArr;
	QJsonArray tableAndTextArr;
	QJsonArray tableArr;
	
	QJsonObject root;

	
	QList<QGraphicsItem*>list = m_scene->items();
	for (QGraphicsItem* item : list) {
		QJsonObject obj;
		if (item->type() == QGraphicsTextItem::Type) {
			MyGraphicsTextItem* myItem = dynamic_cast<MyGraphicsTextItem*>(item);
			
			//QPoint point(m_view->mapFromScene(myItem->scenePos()));

			QPointF point(myItem->pos());

			//qDebug() << "m_view viewport pos: " << m_view->viewport()->pos();

			//文本: x、y、宽、高、缩放、内容、颜色、字体大小、加粗、倾斜、下划线，中划线、字体
			obj.insert("x",point.x());
			obj.insert("y",point.y());
			obj.insert("width",myItem->getBoundingRect().width());
			obj.insert("height",myItem->getBoundingRect().height());
			obj.insert("scale",myItem->scale());
			obj.insert("contents",myItem->toPlainText());
			obj.insert("zValue",myItem->zValue());
			//读取颜色的RGB值，并保存为字符串
			QColor color = myItem->defaultTextColor();
			QString colorStr = QString("%1,%2,%3").arg(color.red()).arg(color.green()).arg(color.blue());

			int red = color.red();
			int green = color.green();
			int blue = color.blue();

			QColor();
			//读取字体大小、加粗、倾斜、下划线，中划线、字体
			QFont font = myItem->font();
			int fontSize = font.pointSize();
			bool fontBold = font.bold();
			bool fontItalic = font.italic();
			bool fontUnderLine = font.underline();
			bool fontStrikeout = font.strikeOut();
			QString fontFamily = font.family();
			
			obj.insert("fontColorRed",red);
			obj.insert("fontColorGreen",green);
			obj.insert("fontColorBlue",blue);
			obj.insert("fontSize",fontSize);
			obj.insert("fontBold",fontBold);
			obj.insert("fontItalic",fontItalic);
			obj.insert("fontUnderLine",fontUnderLine);
			obj.insert("fontStrikeout",fontStrikeout);
			obj.insert("fontFamily",fontFamily);

			//obj.insert("")
			textArr.append(obj);
		}
		else if (item->type() == QGraphicsPixmapItem::Type) {
			MyGraphicsPixmapItem* myItem = dynamic_cast<MyGraphicsPixmapItem*>(item);


			//图片：x、y、宽、高、比例、图片路径、
			//现在用的坐标是场景坐标
			QPointF point(myItem->pos());

			//qDebug() << TR("========保存========");
			//qDebug() << "myItem->pos(): " << point;
			
			//qDebug() << "myItem->scenePos(): " << myItem->scenePos();
			//qDebug() << "mapFromScene(): " << m_view->mapFromScene(myItem->pos());
			

			//qDebug() << "mapFromScene(myItem->scenePos()): " << m_view->mapFromScene(myItem->pos());
			
			obj.insert("x",point.x());
			obj.insert("y",point.y());
			obj.insert("width",myItem->getBoundingRect().width());
			obj.insert("height",myItem->getBoundingRect().height());
			obj.insert("scale",myItem->scale());
			obj.insert("imagePth",myItem->getImagePth());
			obj.insert("zValue",myItem->zValue());
			pixmapArr.append(obj);
			//qDebug()<<"save point: "<<point;
			//qDebug()<<"save item point: "<<item->mapRectFromItem(item->pos());
		}
		//表格json格式
		else if (item->type() == TABLE_TYPE) {
			MyTable* myItem = dynamic_cast<MyTable*>(item);
			if (myItem) {
			QPointF point(myItem->pos());

			obj.insert("x",point.x());
			obj.insert("y",point.y());
			obj.insert("width",myItem->getRect().width());
			obj.insert("height",myItem->getRect().height());
			obj.insert("row",myItem->getRow());
			obj.insert("col",myItem->getCol());
			obj.insert("scale",myItem->scale());
			QJsonArray tmpArr;
			QJsonObject tmpObj;

			for(int i = 0 ; i < myItem->m_tableText.size();++i)
			{
				//qDebug()<<"table point: "<<myItem->m_tableText.at(i)->scenePos();
				//后期可以增加每个文本框的字体，颜色，倾斜等等信息
				tmpObj.insert("x",myItem->m_tableText.at(i)->x());
				tmpObj.insert("y",myItem->m_tableText.at(i)->y());
				tmpObj.insert("intervalW",myItem->m_tableText.at(i)->intervalW);
				tmpObj.insert("intervalH",myItem->m_tableText.at(i)->intervalH);
				tmpObj.insert("contents",myItem->m_tableText.at(i)->toPlainText());
				tmpObj.insert("rowIndex",myItem->m_tableText.at(i)->getRow());
				tmpObj.insert("colIndex",myItem->m_tableText.at(i)->getCol());
				tmpArr.append(tmpObj);
			}

			obj.insert("tableText",tmpArr);
			
			tableArr.append(obj);
			
			}
			else {
				continue;
			}

		
	}
	}
	//========================SCENE======================//
	scene.insert("width",m_scene->itemsBoundingRect().width());
	scene.insert("height",m_scene->itemsBoundingRect().height());
	
	//===================================================//
	root.insert("scene",scene);
	root.insert("text",textArr);
	root.insert("pixmap",pixmapArr);
	root.insert("table",tableArr);
	QJsonDocument doc(root);
	
	file.write(doc.toJson());
	
	file.close();

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
			if (tmp->scale() - 0.3 <= 0) {
				return;
			}
			tmp->setScale(tmp->scale() - 0.3);
		}
	}
	else {
		for (QGraphicsItem* tmp : m_scene->selectedItems()) {
			//缩放比例最小为原型
			if (tmp->scale() - 0.3 <= 0) {
				return;
			}
			tmp->setScale(tmp->scale() - 0.3);
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

			if (tmp->scale() + 0.3 >= 5) {
				return;
			}

			tmp->setScale(tmp->scale() + 0.3);
		}
	}
	else {
		for (QGraphicsItem* tmp : m_scene->selectedItems()) {
			if (tmp->scale() + 0.3 >= 5) {
				return;
			}
			tmp->setScale(tmp->scale() + 0.3);
		}
	}


}

//恢复默认比例
void CMajor::slot_defaulted()
{
	for (QGraphicsItem* tmp : m_scene->items()) {
		tmp->setScale(1);
	}

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
			cursor.mergeCharFormat(format);
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
	if (m_tool->isCheckable()) {
		m_tool->setCheckable(false);
		m_tool->setChecked(false);
	}
	else {
		m_tool->setCheckable(true);
		m_tool->setChecked(true);
	}

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
	delete item;
	item = nullptr;
}

void CMajor::slot_rectFrame(QSize size, QPointF point, bool flag)
{
	//Q_UNUSED(flag);
	//qDebug()<<"point: "<<point;
	
	if (m_textEnable) {
		MyGraphicsTextItem* item = new MyGraphicsTextItem(QRectF(0, 0, abs(size.width()), abs(size.height())));
		//connect(this,SIGNAL(sig_expand(bool)),item,SLOT(slot_expand(bool)));
		connect(this,SIGNAL(sig_repeat(bool)),item,SLOT(slot_repeat(bool)));
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
		//connect(item, SIGNAL(sig_deleteKey()), this, SLOT(slot_remove()));
		connect(item, SIGNAL(sig_hideRectMouse(bool)), m_scene, SLOT(slot_hideRectMouse(bool)));
		//emit item->sig_hideRectMouse(true);
		m_tool->setCheckable(false);
		m_tool->setChecked(false);
		update();

	}
	else if(m_tableEnable){
		MyTable *table;
		if (abs(size.width()) <= 100) {
			table = new MyTable(m_tableRow,m_tableCol,QRectF(0, 0, 200, 100));
		}
		else {

			table = new MyTable(m_tableRow,m_tableCol,QRectF(0, 0, abs(size.width()), abs(size.height())));
		}
		
		connect(table,SIGNAL(sig_hideRectMouse(bool)),m_scene,SLOT(slot_hideRectMouse(bool)));
		connect(this,SIGNAL(sig_MyTable(QRectF)),table,SLOT(slot_MyTable(QRectF)));
		connect(this,SIGNAL(sig_joinTable()),table,SLOT(slot_joinTable()));
		
		m_scene->addItem(table);
		
		if (table->getCol() > table->getRow()) {
			for (int i = 0; i < table->getRow(); ++i) {
				for (int j = 0; j < table->getCol(); ++j) {
					//MyTableText* item = new MyTableText(QRectF(j * table->getIntervalW(), i * table->getIntervalH(), table->getIntervalW(), table->getIntervalH()), table);
					MyTableText* item = new MyTableText(QRectF(0,0, table->getIntervalW(), table->getIntervalH()), table);
					
					item->setPos(j * table->getIntervalW(), i * table->getIntervalH());
					item->setX(j * table->getIntervalW());
					item->setY(i * table->getIntervalH());
					//item->setRect(QRectF(j * table->getIntervalW(), i * table->getIntervalH(), item->getRect().width(), item->getRect().height()));
					connect(item, SIGNAL(sig_hideRectMouse(bool)), m_scene, SLOT(slot_hideRectMouse(bool)));
					//connect(item->document(),SIGNAL(contentsChanged()),table,SLOT(slot_contentsChanged()));
					connect(this,SIGNAL(sig_MyTable(QRectF)),item,SLOT(slot_MyTable(QRectF)));
					connect(table,SIGNAL(sig_changeSelect()),item,SLOT(slot_changeSelect()));
					connect(table,SIGNAL(sig_updateSize(STATE_FLAG, qreal)),item,SLOT(slot_updateSize(STATE_FLAG, qreal)));
					item->setIndex(i,j);

					table->m_tableText.push_back(item);

				}
			}
		}
		else if (table->getCol() <= table->getRow()) {
			for (int i = 0; i < table->getRow(); ++i) {
				for (int j = 0; j < table->getCol(); ++j) {
					
					MyTableText* item = new MyTableText(QRectF(0, 0, table->getIntervalW(), table->getIntervalH()), table);

					item->setPos(j * table->getIntervalW(), i * table->getIntervalH());
					
					item->setX(j * table->getIntervalW());
					item->setY(i * table->getIntervalH());
					
					item->setIndex(i,j);
					connect(item, SIGNAL(sig_hideRectMouse(bool)), m_scene, SLOT(slot_hideRectMouse(bool)));
					//connect(item->document(),SIGNAL(contentsChanged()),table,SLOT(slot_contentsChanged()));
					connect(this,SIGNAL(sig_MyTable(QRectF)),item,SLOT(slot_MyTable(QRectF)));
					connect(table,SIGNAL(sig_changeSelect()),item,SLOT(slot_changeSelect()));
					connect(table,SIGNAL(sig_updateSize(STATE_FLAG, qreal)),item,SLOT(slot_updateSize(STATE_FLAG, qreal)));
					table->m_tableText.push_back(item);
				}
			}
		}
		m_tableEnable = false;

	if (size.height() < 0 && size.width() < 0) {
			table->moveBy(point.rx(), point.ry());
		}
		if (size.height() < 0 && size.width() > 0) {
			table->moveBy(point.rx() - abs(size.width()), point.ry());
		}

		if (size.height() > 0 && size.width() > 0)
			table->moveBy(point.rx() - size.width(), point.ry() - size.height());

		if (size.width() < 0 && size.height() > 0) {
			table->moveBy(point.rx(), point.ry() - abs(size.height()));
		}

		m_textEnable = false;
		
		return;
	}
	
	emit sig_MyTable(QRectF(point,size));
	
	if(flag) {
		
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
}

void CMajor::slot_rightMenu(const QPoint &point)
{
	Q_UNUSED(point);
	//qDebug()<<"slot_rightMenu";
	QMenu *menu = new QMenu();
	//menu->addAction(expand);
	menu->addAction(repeat);
	menu->addAction(joinTable);
	//menu->move(point);
	//qDebug()<<point;
	menu->exec(cursor().pos());
}

void CMajor::slot_repeat()
{
	if (m_isRepeat == true) {
		m_isRepeat = false;
		repeat->setIcon(QIcon(""));
		
		//定时器开启
		m_repeatTime->start();
	}
	else {
		m_isRepeat = true;
		//QIcon icon("../yesExpand.png");
		QIcon icon(":/yesExpand.png");
		repeat->setIcon(icon);
		//定时器关闭
		emit sig_repeat(true); 
		m_repeatTime->stop();
	}
}

void CMajor::slot_joinTable()
{
	//第一步 首先获得 记录选中的单元格，然后在进行合并划分
	emit sig_joinTable();
}

void CMajor::slot_setTableInfo()
{
	//new出来一个管理框
	if (tableInfo == nullptr) {
		tableInfo = new ManagerTableInfo;
		connect(tableInfo,SIGNAL(sig_okClicked(int,int)),this,SLOT(slot_okClicked(int,int)));
		connect(tableInfo,SIGNAL(sig_cancelClicked()),this,SLOT(slot_cancelClicked()));
	}

	tableInfo->show();
	tableInfo->move(pos()+QPoint(width()/2,height()/2));
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

void CMajor::slot_menuBarFont()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok);
	ui->menubar->setFont(font);
	QList<QAction*> list = ui->menubar->actions();

	for (QAction* action : list) {
		action->setFont(font);
	}
	newCreat->setFont(font);
    newWindow->setFont(font);
    open->setFont(font);
    save->setFont(font);
    otherSave->setFont(font);
    exitDocument->setFont(font);

    revoke->setFont(font);
    shear->setFont(font);
    copy->setFont(font);
    paste->setFont(font);
    remove->setFont(font);
    typeface->setFont(font);
    color->setFont(font);
    //! 查看
    zoomIn->setFont(font);
    zoomOut->setFont(font);
    defaulted->setFont(font);

    //! 插入
    insertImage->setFont(font);

	scale->setFont(font);
}

void CMajor::slot_sceneUpdate()
{
	m_scene->update();
}
void CMajor::slot_repeatTime()
{
	if (m_isRepeat == false) {
		emit sig_repeat(false);
	}
}
void CMajor::slot_okClicked(int row,int col)
{	
	m_tableEnable = true;
	m_tableCol = col;
	m_tableRow = row;
	
}
void CMajor::slot_cancelClicked()
{
	//qDebug()<<"slot_okClicked";
	m_tableEnable = false;
	tableInfo->deleteLater();
	tableInfo->close();
	tableInfo = nullptr;
}

bool CMajor::loadPlugin()
{

    //QDir curPath("../output/windows_x64_bin/");
    //QDir curPath("./");

    QDir curPath("../output/windows_x64_bin/");
    
    //QDir curPath("./");

    //qDebug() <<curPath.currentPath();
    //qDebug()<<curPath.path();
	for (const QString& fileName : curPath.entryList(QDir::Files))
	{

		QPluginLoader loader(curPath.absoluteFilePath(fileName));
		QObject* obj = loader.instance();

		if (obj)
		{
			logFile = qobject_cast<Plugin*>(obj);
			if (logFile)
			{

				continue;
			}
            viewPlugin = qobject_cast<PluginView*>(obj);
            if(viewPlugin){

                continue;
            }
        }

	}
	/*if (logFile != nullptr && viewPlugin != nullptr) {
		return true;
	}*/

	return false;
}

void CMajor::loadStyleSheet(const QString & fileName)
{
	
	QFile file(fileName);

	file.open(QIODevice::ReadOnly|QIODevice::Text);
	if (!file.isOpen()) {
	//logFile->errorLog(TR("加载 ”%1“ 样式表失败！").arg(fileName));
	}
	QTextStream stream(&file);
	QString styleCons = file.readAll();
	setStyleSheet(styleCons);
}
