/********************************************************************************
** Form generated from reading UI file 'mygraphicstextitem.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYGRAPHICSTEXTITEM_H
#define UI_MYGRAPHICSTEXTITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsTextItem>

QT_BEGIN_NAMESPACE

class Ui_MyGraphicsTextItem
{
public:

    void setupUi(QGraphicsTextItem *MyGraphicsTextItem)
    {
        if (MyGraphicsTextItem->objectName().isEmpty())
            MyGraphicsTextItem->setObjectName(QString::fromUtf8("MyGraphicsTextItem"));
        MyGraphicsTextItem->resize(400, 300);

        retranslateUi(MyGraphicsTextItem);

        QMetaObject::connectSlotsByName(MyGraphicsTextItem);
    } // setupUi

    void retranslateUi(QGraphicsTextItem *MyGraphicsTextItem)
    {
        MyGraphicsTextItem->setWindowTitle(QCoreApplication::translate("MyGraphicsTextItem", "MyGraphicsTextItem", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MyGraphicsTextItem: public Ui_MyGraphicsTextItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYGRAPHICSTEXTITEM_H
