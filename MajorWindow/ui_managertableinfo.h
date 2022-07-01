/********************************************************************************
** Form generated from reading UI file 'managertableinfo.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MANAGERTABLEINFO_H
#define UI_MANAGERTABLEINFO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ManagerTableInfo
{
public:

    void setupUi(QWidget *ManagerTableInfo)
    {
        if (ManagerTableInfo->objectName().isEmpty())
            ManagerTableInfo->setObjectName(QString::fromUtf8("ManagerTableInfo"));
        ManagerTableInfo->resize(400, 300);

        retranslateUi(ManagerTableInfo);

        QMetaObject::connectSlotsByName(ManagerTableInfo);
    } // setupUi

    void retranslateUi(QWidget *ManagerTableInfo)
    {
        ManagerTableInfo->setWindowTitle(QCoreApplication::translate("ManagerTableInfo", "ManagerTableInfo", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ManagerTableInfo: public Ui_ManagerTableInfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MANAGERTABLEINFO_H
