#include "warningdialog.h"
#include "ui_warningdialog.h"
#include <QPixmap>
#include <QApplication>
#include "controlmaskdialog.h"
#include <QTimer>
#include <QFile>
#include "logger.h"
#include <QPainter>

LOG4QT_DECLARE_STATIC_LOGGER(logger, WarningDialog)
WarningDialog::WarningDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WarningDialog),
    m_closeTimer(NULL)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_QuitOnClose,false);
    setAttribute(Qt::WA_TranslucentBackground,true);
    m_closeTimer = new QTimer();
    connect(m_closeTimer,&QTimer::timeout,this,&WarningDialog::slotCloseEvent);
    m_closeTimer->setSingleShot(true);
    loadStyleSheetFile();
}

WarningDialog::~WarningDialog()
{
    delete ui;
    if(m_closeTimer)
    {
        delete  m_closeTimer;
        m_closeTimer = NULL;
    }
}

void WarningDialog::loadStyleSheetFile()
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

void WarningDialog::setImageAndText(QString path,QString text)
{
    ui->text_Label->setText(text);
    QPixmap pixmap(path);
    if(m_warnType != GAMETIPS_E)
    {
        ui->image_Label->setPixmap(pixmap);
        ui->image_Label->setVisible(true);
        ui->gameTips_Label->setVisible(false);
    }
    else if(m_warnType == GAMETIPS_E)
    {
        ui->image_Label->setPixmap(pixmap);
        ui->image_Label->setVisible(false);
        ui->gameTips_Label->setVisible(true);
    }
}

void WarningDialog::setWarningType(E_WarningType type,int spasmNum )
{
    m_warnType = type;
    switch(type)
    {
    case EMERGENCY_E:
        showEmergencyDialog();
        break;
    case SPASM_E:
        showSpasmDialog(spasmNum);
        break;
    case CONNECTLOST_E:
        break;
    case CLAMP_E:
        showClampDialog();
        break;
    case GAMETIPS_E:
        showGameTipsDialog();
        break;
    case DISCONNECT_E:
        showDeviceDisconnect();
        break;
    case RESET_HANDLE_E:
        break;
    }
}

void WarningDialog::showEmergencyDialog()
{
    QString rootPath = QApplication::applicationDirPath();
    QString text("急停按钮被按下，请重启设备以及电脑软件");
    QString path("./source/image/dialog/emergency.png");
#ifdef ABSOLUTEPATH
    path = rootPath + "/source/image/dialog/emergency.png";
#endif
    setImageAndText(path,text);
}

void WarningDialog::showSpasmDialog(int spasmNum)
{
    QString rootPath = QApplication::applicationDirPath();
    QString text("请注意痉挛");
    if(spasmNum >= 3)
        text = tr("已发生三次痉挛，请退出游戏");
    QString path("./source/image/dialog/spasm.png");
#ifdef ABSOLUTEPATH
    path = rootPath + "/source/image/dialog/spasm.png";
#endif
    setImageAndText(path,text);
}

//夹手警告
void WarningDialog::showClampDialog()
{
    QString rootPath = QApplication::applicationDirPath();
    QString text("设备存在夹手风险，请移开手指");
    QString path("./source/image/dialog/clamp.png");
#ifdef ABSOLUTEPATH
    path = rootPath + "/source/image/dialog/clamp.png";
#endif
    setImageAndText(path,text);
}

void WarningDialog::showGameTipsDialog()
{
    QString rootPath = QApplication::applicationDirPath();
    QString text("请将手放置于手托上并绑好绑带");
    QString path("./source/image/dialog/gameTips.png");
#ifdef ABSOLUTEPATH
    path = rootPath + "/source/image/dialog/gameTips.png";
#endif
    setImageAndText(path,text);
}
void WarningDialog::showDeviceDisconnect()
{
    QString rootPath = QApplication::applicationDirPath();
    QString text("设备已失联，请检查设备");
    QString path("./source/image/dialog/disconnect.png");
#ifdef ABSOLUTEPATH
    path = rootPath + "/source/image/dialog/clamp.png";
#endif
    setImageAndText(path,text);
}

//复位手柄【待测试更新】
void WarningDialog::showResetHandleDialog()
{
    QString rootPath = QApplication::applicationDirPath();
    QString text("设备即将复位，请将手移开");
    QString path("./source/image/dialog/disconnect.png");
#ifdef ABSOLUTEPATH
    path = rootPath + "/source/image/dialog/clamp.png";
#endif
    setImageAndText(path,text);
}

void WarningDialog::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
}

void WarningDialog::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    emit confirmSignal();//主要用于痉挛复位
}

void WarningDialog::setCloseTime(int second)
{
    m_closeTimer->start(second*1000);
}

void WarningDialog::slotCloseEvent()
{
    this->close();
}

void WarningDialog::setDialogOpenState(bool isShow)
{
    if(isShow)
    {
        this->show();
    }
    else
        this->close();
}

void WarningDialog::on_confirm_Btn_clicked()
{
    this->close();
}

void WarningDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.fillRect(rect(),QColor(0,0,0,100));
}

void WarningDialog::on_close_Btn_clicked()
{
    this->close();
}
