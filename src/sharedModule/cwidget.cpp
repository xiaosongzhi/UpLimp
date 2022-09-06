#include "cwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
CWidget::CWidget(QWidget *parent) : QWidget(parent),
    m_rowCount(2),
    m_columnCount(5),
    m_totalWidgetCount(0)

{
    resize(800,600);
    m_HBoxLayoutUpper = new QHBoxLayout();
    m_HBoxLayoutLower = new QHBoxLayout();
    m_HBoxLayoutUpper->setSpacing(20);

    m_HBoxLayoutLower->setSpacing(20);
    m_VBoxLayout = new QVBoxLayout(this);
    m_VBoxLayout->setSpacing(20);
    m_VBoxLayout->setContentsMargins(80,80,80,80);
    m_VBoxLayout->insertLayout(0,m_HBoxLayoutUpper,1);
    m_VBoxLayout->insertLayout(1,m_HBoxLayoutLower,1);
    setLayout(m_VBoxLayout);
}

void CWidget::setColumnCount(int)
{

}
void CWidget::setRowCount()
{

}

int CWidget::getColumnCount()
{
    return m_columnCount;
}
int CWidget::getRowCount()
{
    return m_rowCount;
}

void CWidget::addWidgets(QList<QWidget*> widgetList)
{

    for(int i= 0;i < (widgetList.size() > 10 ? 10:widgetList.size());i++)
    {
        if(i < 5)
            m_HBoxLayoutUpper->addWidget(widgetList.at(i),QBoxLayout::LeftToRight);
        else
            m_HBoxLayoutLower->addWidget(widgetList.at(i),QBoxLayout::LeftToRight);
    }

    int toAddButtons = 10 - widgetList.size();
    for(int i = 0;i < toAddButtons;i++)
    {
        if(i < 5)
        {
            // 该处的属性必须使用Expanding
            QSpacerItem *item = new QSpacerItem(200,200,QSizePolicy::Expanding,QSizePolicy::Fixed);
            m_HBoxLayoutLower->addSpacerItem(item);
        }
        else
        {
            QSpacerItem *item = new QSpacerItem(200,200,QSizePolicy::Expanding,QSizePolicy::Fixed);
            m_HBoxLayoutUpper->addSpacerItem(item);
        }
    }

    //    m_HBoxLayoutUpper->addStretch();

    //    widgetList.append();

    //    if(widgetList.size() <= 5)
    //    {
    //        m_HBoxLayoutUpper->addWidget(widget);
    //    }
    //    else
    //    {
    //        //两行
    //        m_HBoxLayoutLower->addWidget(widget);
    //    }
    //    m_totalWidgetCount++;

}

void CWidget::addWidget(QWidget* widget,int row,int column)
{
    m_totalWidgetCount++;
}
