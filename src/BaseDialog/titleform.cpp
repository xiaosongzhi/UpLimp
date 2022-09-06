#include "titleform.h"
#include "ui_titleform.h"
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
TitleForm::TitleForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TitleForm),
    m_isPressed(false)
{
    ui->setupUi(this);
}

TitleForm::~TitleForm()
{
    delete ui;
}

void TitleForm::on_min_Btn_clicked()
{
    emit signalButtonEvent(E_MINEVENT);
}

void TitleForm::on_max_Btn_clicked()
{
    emit signalButtonEvent(E_MAXEVENT);
}

void TitleForm::on_close_Btn_clicked()
{
    emit signalButtonEvent(E_QUITEVENT);
}

void TitleForm::on_help_Btn_clicked()
{
    emit signalButtonEvent(E_HELPEVENT);
}

void TitleForm::mousePressEvent(QMouseEvent *event)
{
    //左键按下
    if(event->button() == Qt::LeftButton)
    {
        m_isPressed = true;
        m_pressdPoint = event->pos();
    }
}
void TitleForm::mouseMoveEvent(QMouseEvent * event)
{
    if(m_isPressed)
    {
        QPoint globalPoint = event->globalPos()-m_pressdPoint;
        //将移动位置告知给主窗体
        emit signalWindowMove(globalPoint);
    }
}
void TitleForm::mouseReleaseEvent(QMouseEvent * event)
{
    if(event->button() == Qt::LeftButton)
        m_isPressed = false;
}
void TitleForm::mouseDoubleClickEvent(QMouseEvent *event)
{
    //最大化和正常状态的切换
    if(event->button() == Qt::LeftButton)
        signalSwitchWindowSize();
}
