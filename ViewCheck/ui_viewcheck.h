/********************************************************************************
** Form generated from reading UI file 'viewcheck.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEWCHECK_H
#define UI_VIEWCHECK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ViewCheck
{
public:

    void setupUi(QWidget *ViewCheck)
    {
        if (ViewCheck->objectName().isEmpty())
            ViewCheck->setObjectName(QString::fromUtf8("ViewCheck"));
        ViewCheck->resize(400, 300);

        retranslateUi(ViewCheck);

        QMetaObject::connectSlotsByName(ViewCheck);
    } // setupUi

    void retranslateUi(QWidget *ViewCheck)
    {
        ViewCheck->setWindowTitle(QCoreApplication::translate("ViewCheck", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ViewCheck: public Ui_ViewCheck {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWCHECK_H
