/********************************************************************************
** Form generated from reading UI file 'cbasewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CBASEWIDGET_H
#define UI_CBASEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "titleform.h"

QT_BEGIN_NAMESPACE

class Ui_CBaseWidget
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *leftTop_Label;
    QLabel *top_Label;
    QLabel *rightTop_Label;
    QHBoxLayout *horizontalLayout;
    QLabel *left_Label;
    QVBoxLayout *verticalLayout;
    TitleForm *title_Widget;
    QWidget *central_Widget;
    QLabel *right_Label;
    QHBoxLayout *horizontalLayout_3;
    QLabel *leftBottom_Label;
    QLabel *bottom_Label;
    QLabel *rightBottom_Label;

    void setupUi(QWidget *CBaseWidget)
    {
        if (CBaseWidget->objectName().isEmpty())
            CBaseWidget->setObjectName(QString::fromUtf8("CBaseWidget"));
        CBaseWidget->resize(1024, 716);
        gridLayout = new QGridLayout(CBaseWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        leftTop_Label = new QLabel(CBaseWidget);
        leftTop_Label->setObjectName(QString::fromUtf8("leftTop_Label"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(leftTop_Label->sizePolicy().hasHeightForWidth());
        leftTop_Label->setSizePolicy(sizePolicy);
        leftTop_Label->setMinimumSize(QSize(5, 5));
        leftTop_Label->setMaximumSize(QSize(5, 5));
        leftTop_Label->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);\n"
""));

        horizontalLayout_2->addWidget(leftTop_Label);

        top_Label = new QLabel(CBaseWidget);
        top_Label->setObjectName(QString::fromUtf8("top_Label"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(top_Label->sizePolicy().hasHeightForWidth());
        top_Label->setSizePolicy(sizePolicy1);
        top_Label->setMaximumSize(QSize(16777215, 5));
        top_Label->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 255, 0);\n"
""));

        horizontalLayout_2->addWidget(top_Label);

        rightTop_Label = new QLabel(CBaseWidget);
        rightTop_Label->setObjectName(QString::fromUtf8("rightTop_Label"));
        sizePolicy.setHeightForWidth(rightTop_Label->sizePolicy().hasHeightForWidth());
        rightTop_Label->setSizePolicy(sizePolicy);
        rightTop_Label->setMinimumSize(QSize(5, 5));
        rightTop_Label->setMaximumSize(QSize(5, 5));
        rightTop_Label->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));

        horizontalLayout_2->addWidget(rightTop_Label);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        left_Label = new QLabel(CBaseWidget);
        left_Label->setObjectName(QString::fromUtf8("left_Label"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(left_Label->sizePolicy().hasHeightForWidth());
        left_Label->setSizePolicy(sizePolicy2);
        left_Label->setMinimumSize(QSize(5, 0));
        left_Label->setMaximumSize(QSize(5, 16777215));
        left_Label->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 170, 0);"));

        horizontalLayout->addWidget(left_Label);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        title_Widget = new TitleForm(CBaseWidget);
        title_Widget->setObjectName(QString::fromUtf8("title_Widget"));
        sizePolicy1.setHeightForWidth(title_Widget->sizePolicy().hasHeightForWidth());
        title_Widget->setSizePolicy(sizePolicy1);
        title_Widget->setMinimumSize(QSize(35, 35));
        title_Widget->setMaximumSize(QSize(16777215, 35));
        title_Widget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 255);"));

        verticalLayout->addWidget(title_Widget);

        central_Widget = new QWidget(CBaseWidget);
        central_Widget->setObjectName(QString::fromUtf8("central_Widget"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(central_Widget->sizePolicy().hasHeightForWidth());
        central_Widget->setSizePolicy(sizePolicy3);
        central_Widget->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 85, 127);"));

        verticalLayout->addWidget(central_Widget);


        horizontalLayout->addLayout(verticalLayout);

        right_Label = new QLabel(CBaseWidget);
        right_Label->setObjectName(QString::fromUtf8("right_Label"));
        sizePolicy2.setHeightForWidth(right_Label->sizePolicy().hasHeightForWidth());
        right_Label->setSizePolicy(sizePolicy2);
        right_Label->setMaximumSize(QSize(5, 16777215));
        right_Label->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 170, 0);"));

        horizontalLayout->addWidget(right_Label);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        leftBottom_Label = new QLabel(CBaseWidget);
        leftBottom_Label->setObjectName(QString::fromUtf8("leftBottom_Label"));
        sizePolicy.setHeightForWidth(leftBottom_Label->sizePolicy().hasHeightForWidth());
        leftBottom_Label->setSizePolicy(sizePolicy);
        leftBottom_Label->setMinimumSize(QSize(5, 5));
        leftBottom_Label->setMaximumSize(QSize(5, 5));
        leftBottom_Label->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));

        horizontalLayout_3->addWidget(leftBottom_Label);

        bottom_Label = new QLabel(CBaseWidget);
        bottom_Label->setObjectName(QString::fromUtf8("bottom_Label"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(bottom_Label->sizePolicy().hasHeightForWidth());
        bottom_Label->setSizePolicy(sizePolicy4);
        bottom_Label->setMinimumSize(QSize(0, 5));
        bottom_Label->setMaximumSize(QSize(16777215, 5));
        bottom_Label->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 255, 0);"));

        horizontalLayout_3->addWidget(bottom_Label);

        rightBottom_Label = new QLabel(CBaseWidget);
        rightBottom_Label->setObjectName(QString::fromUtf8("rightBottom_Label"));
        sizePolicy.setHeightForWidth(rightBottom_Label->sizePolicy().hasHeightForWidth());
        rightBottom_Label->setSizePolicy(sizePolicy);
        rightBottom_Label->setMinimumSize(QSize(5, 5));
        rightBottom_Label->setMaximumSize(QSize(5, 5));
        rightBottom_Label->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);"));

        horizontalLayout_3->addWidget(rightBottom_Label);


        verticalLayout_2->addLayout(horizontalLayout_3);


        gridLayout->addLayout(verticalLayout_2, 0, 0, 1, 1);


        retranslateUi(CBaseWidget);

        QMetaObject::connectSlotsByName(CBaseWidget);
    } // setupUi

    void retranslateUi(QWidget *CBaseWidget)
    {
        CBaseWidget->setWindowTitle(QApplication::translate("CBaseWidget", "CBaseWidget", nullptr));
        leftTop_Label->setText(QString());
        top_Label->setText(QString());
        rightTop_Label->setText(QString());
        left_Label->setText(QString());
        right_Label->setText(QString());
        leftBottom_Label->setText(QString());
        bottom_Label->setText(QString());
        rightBottom_Label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CBaseWidget: public Ui_CBaseWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CBASEWIDGET_H
