#include "bindtipdialog.h"
#include "ui_bindtipdialog.h"
#include <QPainter>
#include <QTimer>
#include <QDebug>
#define OriginalNum 90

BindTipDialog::BindTipDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BindTipDialog),
    m_timer(nullptr),
    Countdown(OriginalNum)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_QuitOnClose,false);
    setAttribute(Qt::WA_TranslucentBackground,true);

    QPixmap pixmap("./source/image/dialog/gameTips.png");
    ui->image_Label->setPixmap(pixmap);
    connect(this,&BindTipDialog::signalCloseCode,this,&BindTipDialog::done);

    m_timer = new QTimer();
    m_timer->setInterval(1000);
    connect(m_timer,&QTimer::timeout,this,&BindTipDialog::slotcloseTimer);
}

BindTipDialog::~BindTipDialog()
{
    delete ui;
}

void BindTipDialog::on_confirm_Btn_clicked()
{
    emit signalCloseCode(1);
    this->close();
}

void BindTipDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.fillRect(rect(),QColor(0,0,0,100));
}

void BindTipDialog::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    Countdown = OriginalNum;
    m_timer->start();
}

void BindTipDialog::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    m_timer->stop();
    ui->confirm_Btn->setText(tr("确认(90s)"));
}

void BindTipDialog::slotcloseTimer()
{
    ui->confirm_Btn->setText(tr("确认(")  + QString::number(--Countdown) + "s)");
    if(0 == Countdown)
        on_confirm_Btn_clicked();
}
