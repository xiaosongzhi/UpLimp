/********************************************************************************
** Form generated from reading UI file 'cmessagebox.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CMESSAGEBOX_H
#define UI_CMESSAGEBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CMessageBox
{
public:
    QLabel *label;

    void setupUi(QWidget *CMessageBox)
    {
        if (CMessageBox->objectName().isEmpty())
            CMessageBox->setObjectName(QString::fromUtf8("CMessageBox"));
        CMessageBox->resize(400, 300);
        label = new QLabel(CMessageBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 100, 371, 101));
        QFont font;
        font.setPointSize(50);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 255, 127);"));
        label->setAlignment(Qt::AlignCenter);

        retranslateUi(CMessageBox);

        QMetaObject::connectSlotsByName(CMessageBox);
    } // setupUi

    void retranslateUi(QWidget *CMessageBox)
    {
        CMessageBox->setWindowTitle(QApplication::translate("CMessageBox", "Form", nullptr));
        label->setText(QApplication::translate("CMessageBox", "!\350\277\236\346\216\245\345\267\262\346\226\255\345\274\200", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CMessageBox: public Ui_CMessageBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CMESSAGEBOX_H
