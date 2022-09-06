#include "movementrange.h"
#include "ui_movementrange.h"

MovementRange::MovementRange(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MovementRange)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose,false);
}

MovementRange::~MovementRange()
{
    delete ui;
}
