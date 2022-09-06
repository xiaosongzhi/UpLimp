#include "simulategame.h"
#include "ui_simulategame.h"

SimulateGame::SimulateGame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimulateGame),
    isStartPaint(false)
{
    ui->setupUi(this);
}

SimulateGame::~SimulateGame()
{
    delete ui;
}

void SimulateGame::on_start_Btn_clicked()
{
    isStartPaint = true;
}

void SimulateGame::on_stop_Btn_clicked()
{
    isStartPaint = false;
}

void SimulateGame::on_clear_Btn_clicked()
{
    ui->widget->clearPaintDevice();
}

void SimulateGame::setPoint(QPoint point)
{
    if(isStartPaint)
    {
        ui->widget->setPoint(point);
        ui->x_Label->setText(QString::number(point.x()));
        ui->y_Label->setText(QString::number(point.y()));
    }

}
