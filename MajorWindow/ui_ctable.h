/********************************************************************************
** Form generated from reading UI file 'ctable.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CTABLE_H
#define UI_CTABLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CTable
{
public:

    void setupUi(QWidget *CTable)
    {
        if (CTable->objectName().isEmpty())
            CTable->setObjectName(QString::fromUtf8("CTable"));
        CTable->resize(400, 300);

        retranslateUi(CTable);

        QMetaObject::connectSlotsByName(CTable);
    } // setupUi

    void retranslateUi(QWidget *CTable)
    {
        CTable->setWindowTitle(QCoreApplication::translate("CTable", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CTable: public Ui_CTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CTABLE_H
