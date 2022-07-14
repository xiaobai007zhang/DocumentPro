/********************************************************************************
** Form generated from reading UI file 'attribute.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ATTRIBUTE_H
#define UI_ATTRIBUTE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Attribute
{
public:

    void setupUi(QWidget *Attribute)
    {
        if (Attribute->objectName().isEmpty())
            Attribute->setObjectName(QString::fromUtf8("Attribute"));
        Attribute->resize(400, 300);

        retranslateUi(Attribute);

        QMetaObject::connectSlotsByName(Attribute);
    } // setupUi

    void retranslateUi(QWidget *Attribute)
    {
        Attribute->setWindowTitle(QCoreApplication::translate("Attribute", "Attribute", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Attribute: public Ui_Attribute {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ATTRIBUTE_H
