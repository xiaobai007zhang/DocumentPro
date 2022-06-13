/********************************************************************************
** Form generated from reading UI file 'cmajor.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CMAJOR_H
#define UI_CMAJOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CMajor
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *CMajor)
    {
        if (CMajor->objectName().isEmpty())
            CMajor->setObjectName(QString::fromUtf8("CMajor"));
        CMajor->resize(800, 600);
        centralwidget = new QWidget(CMajor);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        CMajor->setCentralWidget(centralwidget);
        menubar = new QMenuBar(CMajor);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        CMajor->setMenuBar(menubar);
        statusbar = new QStatusBar(CMajor);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        CMajor->setStatusBar(statusbar);

        retranslateUi(CMajor);

        QMetaObject::connectSlotsByName(CMajor);
    } // setupUi

    void retranslateUi(QMainWindow *CMajor)
    {
        CMajor->setWindowTitle(QCoreApplication::translate("CMajor", "CMajor", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CMajor: public Ui_CMajor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CMAJOR_H
