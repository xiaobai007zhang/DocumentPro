#ifndef CFIND_H
#define CFIND_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

namespace Ui {
	class CFind;
}

class CFind : public QWidget
{
	Q_OBJECT

public:
	explicit CFind(QWidget* parent = nullptr);
	~CFind();


	//!普通成员函数
private:
	void initFindWidget();

	void initConnection();

	//!成员变量
private:
	QString m_findText;
	QPushButton* m_findBtn;
	QLineEdit* m_lineEdit;

signals:
	void sig_findText(QString);

private:
	Ui::CFind* ui;
};

#endif // CFIND_H
