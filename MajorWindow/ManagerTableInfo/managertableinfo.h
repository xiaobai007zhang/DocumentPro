#pragma once

#include "ui_managertableinfo.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

class ManagerTableInfo : public QWidget
{
    Q_OBJECT

public:
    ManagerTableInfo(QWidget* parent = Q_NULLPTR);
    ~ManagerTableInfo();

private:
    void initTableInfo();

    void initConnection();

protected:
     void closeEvent(QCloseEvent* event) override;

signals:
    void sig_okClicked(int, int);
    void sig_cancelClicked();

private:
    QPushButton *m_okBtn, *m_cancelBtn;
    QSpinBox *m_row, *m_col;
    QLabel *m_rowLabel, *m_colLabel;

private:
    Ui::ManagerTableInfo ui;
};
