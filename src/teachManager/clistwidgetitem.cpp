#include "clistwidgetitem.h"
#include "ui_clistwidgetitem.h"

CListWidgetItem::CListWidgetItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CListWidgetItem)
{

}

CListWidgetItem::CListWidgetItem(QString text,QPoint point):
    ui(new Ui::CListWidgetItem)
{
    ui->setupUi(this);
    m_text = " "+text;
    m_point = point;
    ui->icon_Label->setText(text);
    ui->x_Label->setText(" X "+QString::number(point.x()));
    ui->y_Label->setText("Y "+QString::number(point.y()));

}

CListWidgetItem::~CListWidgetItem()
{
    delete ui;
}

void CListWidgetItem::setPointText(QPoint point)
{
    ui->x_Label->setText(" X "+QString::number(point.x()));
    ui->y_Label->setText("Y "+QString::number(point.y()));
}
