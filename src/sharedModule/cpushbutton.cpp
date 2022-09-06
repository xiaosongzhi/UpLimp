#include "cpushbutton.h"
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QColor>
#include <QSizePolicy>
#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
//CPushButton::CPushButton()
//{

//}
CPushButton::CPushButton(const QString& iconPath,const QString& upper,const QString& lower)
{
    setFixedSize(320,230);
//    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
//    setSizePolicy(sizePolicy);
    m_icon = new QPixmap(iconPath);
    m_upper = upper;
    m_lower = lower;
    pixLabel = new QLabel();
    upperTextLabel = new QLabel();
    lowerTextLabel = new QLabel();
    pixLabel->setAlignment(Qt::AlignHCenter);
    upperTextLabel->setAlignment(Qt::AlignHCenter);
    lowerTextLabel->setAlignment(Qt::AlignHCenter);
    pixLabel->setPixmap(*m_icon);
    upperTextLabel->setText(upper);
    lowerTextLabel->setText(lower);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(pixLabel);
    layout->addWidget(upperTextLabel);
    layout->addWidget(lowerTextLabel);
    layout->setStretch(0,3);
    layout->setStretch(1,1);
    layout->setStretch(2,1);

    this->setLayout(layout);
#if 0
    //设置自定义控件的最小值 每行字最小高度为15
    setMinimumHeight(m_icon->size().height() + 30);
    setMinimumWidth(m_icon->size().width());

    qDebug()<<rect().width()<<m_icon->rect().width()<<this->rect()<<m_icon->rect();

    m_IconRect.setX((rect().width()-m_icon->width())/4);
    m_IconRect.setY(10);

    m_IconRect.setHeight(m_icon->height());
    m_IconRect.setWidth(m_icon->width());

    m_upperTextRect.setX(rect().x());
    m_upperTextRect.setY(m_IconRect.height()+20);
    m_upperTextRect.setWidth(rect().width());
    m_upperTextRect.setHeight(20);

    qDebug()<<m_upperTextRect;
#endif
}

CPushButton::~CPushButton()
{

    if(m_icon)
    {
        delete m_icon;
        m_icon = NULL;
    }
    if(pixLabel)
    {
        delete  pixLabel;
        pixLabel = NULL;
    }
    if(upperTextLabel)
    {
        delete  upperTextLabel;
        upperTextLabel = NULL;
    }
    if(lowerTextLabel)
    {
        delete  lowerTextLabel;
        lowerTextLabel = NULL;
    }

}
/*
void CPushButton::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

//    painter.drawPixmap(m_IconRect,*m_icon);
//    painter.drawText(m_upperTextRect, Qt::AlignHCenter | Qt::AlignVCenter, m_upper);
//    painter.drawText(event->rect(), Qt::AlignHCenter | Qt::AlignBottom, m_lower);
    QPen pen;
    pen.setWidth(5);
    pen.setColor(QColor(220,220,220));
    painter.setPen(pen);
    painter.drawRoundRect(rect(),20,20);
}
*/

QString CPushButton::getId()
{
    return m_lower;
}
QString CPushButton::getName()
{
    return m_upper;
}
