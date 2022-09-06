#include "resethandledialog.h"
#include "ui_resethandledialog.h"
#include <QPainter>

ResetHandleDialog::ResetHandleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResetHandleDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_QuitOnClose,false);
    setAttribute(Qt::WA_TranslucentBackground,true);

    QPixmap pixmap("./source/image/dialog/move.png");
    ui->image_Label->setPixmap(pixmap);

    connect(this,&ResetHandleDialog::signalCloseCode,this,&ResetHandleDialog::done);
}

ResetHandleDialog::~ResetHandleDialog()
{
    delete ui;
}

void ResetHandleDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.fillRect(rect(),QColor(0,0,0,100));
}

void ResetHandleDialog::on_confirm_Btn_clicked()
{
    emit signalCloseCode(1);
    this->close();
}

void ResetHandleDialog::on_close_Btn_clicked()
{
    emit signalCloseCode(0);
    this->close();
}
