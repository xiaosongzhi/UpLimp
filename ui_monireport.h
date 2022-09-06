/********************************************************************************
** Form generated from reading UI file 'monireport.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MONIREPORT_H
#define UI_MONIREPORT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MoniReport
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLineEdit *UserID_LineEdit;
    QComboBox *trainMode_ComboBox;
    QLineEdit *Score_LineEdit;
    QComboBox *trainScorp_ComboBox;
    QPushButton *createReport_Btn;

    void setupUi(QWidget *MoniReport)
    {
        if (MoniReport->objectName().isEmpty())
            MoniReport->setObjectName(QString::fromUtf8("MoniReport"));
        MoniReport->resize(716, 654);
        label = new QLabel(MoniReport);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 40, 81, 31));
        label_2 = new QLabel(MoniReport);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(40, 80, 81, 31));
        label_3 = new QLabel(MoniReport);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(40, 160, 81, 31));
        label_4 = new QLabel(MoniReport);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(40, 120, 81, 31));
        label_5 = new QLabel(MoniReport);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(40, 240, 81, 31));
        label_6 = new QLabel(MoniReport);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(40, 200, 81, 31));
        label_7 = new QLabel(MoniReport);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(40, 320, 81, 31));
        label_8 = new QLabel(MoniReport);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(40, 280, 81, 31));
        UserID_LineEdit = new QLineEdit(MoniReport);
        UserID_LineEdit->setObjectName(QString::fromUtf8("UserID_LineEdit"));
        UserID_LineEdit->setGeometry(QRect(140, 44, 113, 21));
        trainMode_ComboBox = new QComboBox(MoniReport);
        trainMode_ComboBox->addItem(QString());
        trainMode_ComboBox->addItem(QString());
        trainMode_ComboBox->addItem(QString());
        trainMode_ComboBox->addItem(QString());
        trainMode_ComboBox->setObjectName(QString::fromUtf8("trainMode_ComboBox"));
        trainMode_ComboBox->setGeometry(QRect(140, 83, 111, 22));
        Score_LineEdit = new QLineEdit(MoniReport);
        Score_LineEdit->setObjectName(QString::fromUtf8("Score_LineEdit"));
        Score_LineEdit->setGeometry(QRect(140, 125, 113, 21));
        trainScorp_ComboBox = new QComboBox(MoniReport);
        trainScorp_ComboBox->addItem(QString());
        trainScorp_ComboBox->addItem(QString());
        trainScorp_ComboBox->addItem(QString());
        trainScorp_ComboBox->setObjectName(QString::fromUtf8("trainScorp_ComboBox"));
        trainScorp_ComboBox->setGeometry(QRect(140, 167, 111, 22));
        createReport_Btn = new QPushButton(MoniReport);
        createReport_Btn->setObjectName(QString::fromUtf8("createReport_Btn"));
        createReport_Btn->setGeometry(QRect(390, 160, 93, 28));

        retranslateUi(MoniReport);

        QMetaObject::connectSlotsByName(MoniReport);
    } // setupUi

    void retranslateUi(QWidget *MoniReport)
    {
        MoniReport->setWindowTitle(QApplication::translate("MoniReport", "Form", nullptr));
        label->setText(QApplication::translate("MoniReport", "\347\224\250\346\210\267ID", nullptr));
        label_2->setText(QApplication::translate("MoniReport", "\350\256\255\347\273\203\346\250\241\345\274\217", nullptr));
        label_3->setText(QApplication::translate("MoniReport", "\350\256\255\347\273\203\350\214\203\345\233\264", nullptr));
        label_4->setText(QApplication::translate("MoniReport", "\350\256\255\347\273\203\345\276\227\345\210\206", nullptr));
        label_5->setText(QApplication::translate("MoniReport", "\350\256\255\347\273\203\346\250\241\345\274\217", nullptr));
        label_6->setText(QApplication::translate("MoniReport", "\350\256\255\347\273\203\346\250\241\345\274\217", nullptr));
        label_7->setText(QApplication::translate("MoniReport", "\350\256\255\347\273\203\346\250\241\345\274\217", nullptr));
        label_8->setText(QApplication::translate("MoniReport", "\350\256\255\347\273\203\346\250\241\345\274\217", nullptr));
        trainMode_ComboBox->setItemText(0, QApplication::translate("MoniReport", "\350\242\253\345\212\250", nullptr));
        trainMode_ComboBox->setItemText(1, QApplication::translate("MoniReport", "\344\270\273\345\212\250", nullptr));
        trainMode_ComboBox->setItemText(2, QApplication::translate("MoniReport", "\345\212\251\345\212\233", nullptr));
        trainMode_ComboBox->setItemText(3, QApplication::translate("MoniReport", "\346\212\227\351\230\273", nullptr));

        trainScorp_ComboBox->setItemText(0, QApplication::translate("MoniReport", "\345\260\217", nullptr));
        trainScorp_ComboBox->setItemText(1, QApplication::translate("MoniReport", "\344\270\255", nullptr));
        trainScorp_ComboBox->setItemText(2, QApplication::translate("MoniReport", "\345\244\247", nullptr));

        createReport_Btn->setText(QApplication::translate("MoniReport", "\347\224\237\346\210\220\346\212\245\345\221\212", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MoniReport: public Ui_MoniReport {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MONIREPORT_H
