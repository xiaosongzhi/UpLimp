#include "waitingdialog.h"
#include "ui_waitingdialog.h"
#include <QMovie>
#include <QApplication>
#include <QDebug>
#include <QFile>
#include "logger.h"
LOG4QT_DECLARE_STATIC_LOGGER(logger, WarningDialog)

WaitingDialog::WaitingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitingDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose,false);
    setWindowFlags(Qt::FramelessWindowHint);
    initGif();
//    loadStyleSheetFile();
}

WaitingDialog::~WaitingDialog()
{
    delete ui;
}

void WaitingDialog::loadStyleSheetFile()
{
    //加载样式文件s
    QFile qssFile("./QSS/waitingDialog.qss");
    if(!qssFile.open(QIODevice::ReadOnly))
        logger()->debug("waitingDialog.qss文件打开失败!");
    else
    {
        this->setStyleSheet(qssFile.readAll());
        qssFile.close();
    }
}

void WaitingDialog::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    m_movie->start();

}
void WaitingDialog::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    m_movie->stop();
}

void WaitingDialog::initGif()
{
    QString gifPath("./source/waiting.gif");
#ifdef ABSOLUTEPATH
    gifPath = QApplication::applicationDirPath() + "/source/waiting.gif";
#endif
    m_movie = new QMovie();
    m_movie->setScaledSize(ui->gif_Label->size());
    m_movie->setFileName(gifPath);
    ui->gif_Label->setMovie(m_movie);
}

void WaitingDialog::setWaitingType(E_WaitingType type)
{
    switch(type)
    {
    case INITWAITING:
    {
        ui->text_Label->setText("加载中...");
    }
        break;
    case LOADINGWITING:
    {
        ui->text_Label->setText("游戏加载中...");
    }
        break;
    }
}

void WaitingDialog::setDialogCloseState(bool isClose)
{
    if(!isClose)
    {
        this->show();
    }
    else
        this->close();
}
