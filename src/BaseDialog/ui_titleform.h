/********************************************************************************
** Form generated from reading UI file 'titleform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TITLEFORM_H
#define UI_TITLEFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TitleForm
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *close_Btn;
    QLabel *title_Label;
    QPushButton *max_Btn;
    QPushButton *min_Btn;
    QPushButton *help_Btn;

    void setupUi(QWidget *TitleForm)
    {
        if (TitleForm->objectName().isEmpty())
            TitleForm->setObjectName(QString::fromUtf8("TitleForm"));
        TitleForm->resize(920, 38);
        gridLayout = new QGridLayout(TitleForm);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(419, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1);

        close_Btn = new QPushButton(TitleForm);
        close_Btn->setObjectName(QString::fromUtf8("close_Btn"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(close_Btn->sizePolicy().hasHeightForWidth());
        close_Btn->setSizePolicy(sizePolicy);

        gridLayout->addWidget(close_Btn, 0, 6, 1, 1);

        title_Label = new QLabel(TitleForm);
        title_Label->setObjectName(QString::fromUtf8("title_Label"));

        gridLayout->addWidget(title_Label, 0, 0, 1, 1);

        max_Btn = new QPushButton(TitleForm);
        max_Btn->setObjectName(QString::fromUtf8("max_Btn"));
        sizePolicy.setHeightForWidth(max_Btn->sizePolicy().hasHeightForWidth());
        max_Btn->setSizePolicy(sizePolicy);

        gridLayout->addWidget(max_Btn, 0, 5, 1, 1);

        min_Btn = new QPushButton(TitleForm);
        min_Btn->setObjectName(QString::fromUtf8("min_Btn"));
        sizePolicy.setHeightForWidth(min_Btn->sizePolicy().hasHeightForWidth());
        min_Btn->setSizePolicy(sizePolicy);

        gridLayout->addWidget(min_Btn, 0, 3, 1, 1);

        help_Btn = new QPushButton(TitleForm);
        help_Btn->setObjectName(QString::fromUtf8("help_Btn"));
        sizePolicy.setHeightForWidth(help_Btn->sizePolicy().hasHeightForWidth());
        help_Btn->setSizePolicy(sizePolicy);

        gridLayout->addWidget(help_Btn, 0, 2, 1, 1);


        retranslateUi(TitleForm);

        QMetaObject::connectSlotsByName(TitleForm);
    } // setupUi

    void retranslateUi(QWidget *TitleForm)
    {
        TitleForm->setWindowTitle(QApplication::translate("TitleForm", "Form", nullptr));
        close_Btn->setText(QApplication::translate("TitleForm", "X-", nullptr));
        title_Label->setText(QApplication::translate("TitleForm", "titelName1111111111111111111", nullptr));
        max_Btn->setText(QApplication::translate("TitleForm", "max", nullptr));
        min_Btn->setText(QApplication::translate("TitleForm", "min", nullptr));
        help_Btn->setText(QApplication::translate("TitleForm", "help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TitleForm: public Ui_TitleForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TITLEFORM_H
