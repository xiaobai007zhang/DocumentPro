/********************************************************************************
** Form generated from reading UI file 'cfind.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CFIND_H
#define UI_CFIND_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CFind
{
public:

    void setupUi(QWidget *CFind)
    {
        if (CFind->objectName().isEmpty())
            CFind->setObjectName(QString::fromUtf8("CFind"));
        CFind->resize(400, 300);

        retranslateUi(CFind);

        QMetaObject::connectSlotsByName(CFind);
    } // setupUi

    void retranslateUi(QWidget *CFind)
    {
        CFind->setWindowTitle(QCoreApplication::translate("CFind", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CFind: public Ui_CFind {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CFIND_H
