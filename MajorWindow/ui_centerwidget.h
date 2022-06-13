/********************************************************************************
** Form generated from reading UI file 'centerwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CENTERWIDGET_H
#define UI_CENTERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CenterWidget
{
public:

    void setupUi(QWidget *CenterWidget)
    {
        if (CenterWidget->objectName().isEmpty())
            CenterWidget->setObjectName(QString::fromUtf8("CenterWidget"));
        CenterWidget->resize(400, 300);

        retranslateUi(CenterWidget);

        QMetaObject::connectSlotsByName(CenterWidget);
    } // setupUi

    void retranslateUi(QWidget *CenterWidget)
    {
        CenterWidget->setWindowTitle(QCoreApplication::translate("CenterWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CenterWidget: public Ui_CenterWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CENTERWIDGET_H
