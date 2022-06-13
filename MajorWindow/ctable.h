#ifndef CTABLE_H
#define CTABLE_H

#include <QWidget>

namespace Ui {
class CTable;
}

class CTable : public QWidget
{
    Q_OBJECT

public:
    explicit CTable(QWidget *parent = nullptr);
    ~CTable();


//!私有成员函数
private:
    void initTable();


//!行列成员变量
private:
    QString m_row,m_column;


signals:
    void sig_tableRowColumn(QString row,QString column);


private:
    Ui::CTable *ui;
};

#endif // CTABLE_H
