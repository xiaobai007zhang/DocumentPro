#ifndef CMAJOR_H
#define CMAJOR_H

#include <QMainWindow>
#include <QTimer>
#include <QToolBar>

#include "centerwidget.h"
#include "cfind.h"
#include "ctable.h"
#include "myhighlighter.h"
#include "plugin.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
	class CMajor;
}
QT_END_NAMESPACE

//!主窗体类
class CMajor : public QMainWindow
{
	Q_OBJECT

public:
	CMajor(QWidget* parent = nullptr);
	~CMajor();

	void getJson();

	void setJson(const QString& fileName);

public:
	//判断插件是否真正的加载成功了，如果失败会在当前的目录中产生日志
	void isLoadPlugin();

private:
	void initMenuBar();

	void initToolBar();

	void initCenterWidget();

	void initConnection();

	void initFindWidget();

	void initTableWidget();

	//!普通成员函数
private:
	//加载日志类的插件
	bool loadPlugin();

	//实时更新行数和列数
	void updateStatusBar();

	//初始化定时器
	void initTimer();

	//设置文档标题
	void setWinTitle(QString winTitle);

	//给文件路径和文件名赋值
	void setFilePathAName(QString path);

	//!重写的函数
private:
	void resizeEvent(QResizeEvent* event);
	void closeEvent(QCloseEvent* event);

	// bool eventFilter(QObject*,QEvent*);

	//!文档槽函数
private slots:

	//创建新的文档
	void slot_creatDocument();
	//创建新的窗口和文档
	bool slot_creatDocumentWindow();
	//打开文件
	bool slot_openFile();
	//另存为
	bool slot_otherSave();
	//打印文件
	void slot_printFile();
	//退出文档
	void slot_exitDocument();
	//复制
	void slot_copy();
	//撤回
	void slot_revoke();
	//剪切
	void slot_shear();
	//粘贴
	void slot_paste();
	//删除
	void slot_remove();
	//查找
	void slot_search();
	//插入图片
	void slot_insertImage();
	//插入表格
	void slot_insertForm();
	//字体
	void slot_typeface();
	//保存
	void slot_save();

	//缩放
	void slot_zoomOut();
	//扩大
	void slot_zoomIn();
	//恢复默认
	void slot_defaulted();

	void slot_findBtnClicked(QString);

	void slot_color();

	//!其他槽函数
private slots:
	//定时器到期槽函数
	void slot_timeOut();

	//如果改动了当前的文件，那么就会加上一个*号
	void slot_textChanged();

	void slot_tableRowColumn(QString, QString);

	void slot_menuBarFont();

	//!成员变量
private:
	//记录列数
	QString m_column;

	//记录行数
	QString m_row;

	//工具栏
	QToolBar* m_toolBar;

	QTimer* m_timer;

	//当前编辑的文件名
	QString m_curFileName;

	//当前打开的文件路径
	QString m_curFilePath;

	//记录默认字体,用于恢复字体
	QFont m_font;

	//中心部件的对象，也就是文档对象
	CenterWidget* m_wid;

	CFind* m_findWid;

	CTable* m_table;

	MyHighLighter* myhighlighter;

	//!插件变量
private:
	//插件抽象类对象
	Plugin* logFile;

private:
	Ui::CMajor* ui;
};
#endif // CMAJOR_H
