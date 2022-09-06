#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include "logger.h"
#include "cudpinterface.h"
#include "ccommunicationinterface.h"
#include "reportdialog.h"
#include <QButtonGroup>
#include <QAbstractButton>
#include "usermanager.h"
#include <QGraphicsDropShadowEffect>
#include "currentuserdata.h"
#include "cpassworddialog.h"
#include <QLabel>
#include <QPixmap>
#include <QTimer>
#include "ccommunicateapi.h"
#include "warningdialog.h"
#include "readconfig.h"

LOG4QT_DECLARE_STATIC_LOGGER(logger, MainWindow)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_buttonGroup(NULL)
    , m_userManager(NULL)
    , m_blinkTimer(NULL)
    , m_heartBeatTimer(NULL)
    , m_timeOutTimes(5)
    , m_connectStateDialog(NULL)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose,true);
    setWindowFlags(Qt::FramelessWindowHint);
    connect(ui->stackedWidget,SIGNAL(currentChanged(int)),this,SLOT(slotCurrentStackWidgetChanged(int)));
    //管理用户初始化
    m_userManager = new UserManager();
    connect(m_userManager,&UserManager::signalSwitchStackPage,this,&MainWindow::slotSwitchPage);
    connect(ui->train_Widget,SIGNAL(signalShowEmergency(bool)),this,SLOT(slotShowEmergency(bool)));

    //初始化按钮组
    initButtonGroup();
    initWidgetUI();

    ui->stackedWidget->setCurrentIndex(0);
    connect(CurrentUserData::getInstace(),SIGNAL(signalUserChanged()),this,SLOT(slotCurrentUserChanged()));

    m_connectStateLabel = new QLabel(this);
    m_connectStateLabel->move(1550,70);
    m_connectStateLabel->resize(120,120);
    m_connectStateLabel->setWindowFlags(Qt::FramelessWindowHint);
    QPixmap pixmap("./source/image/dialog/disconnect.png");
    m_connectStateLabel->setPixmap(pixmap);
    m_connectStateLabel->hide();

    m_blinkTimer = new QTimer();
    m_blinkTimer->setSingleShot(true);
    m_blinkTimer->setInterval(500);
    connect(m_blinkTimer,SIGNAL(timeout()),this,SLOT(slotConnectBlink()));

    //心跳检测
    connect(CCommunicateAPI::getInstance(),SIGNAL(signalReadyRead(QByteArray)),this,SLOT(slotReceiveCommuAPIData(QByteArray)));
    m_heartBeatTimer = new QTimer();
    m_heartBeatTimer->setInterval(1000);
    connect(m_heartBeatTimer,SIGNAL(timeout()),this,SLOT(slotHeartBeatChecking()));
    m_heartBeatTimer->start(1000);

    m_connectStateDialog = new WarningDialog();
    m_connectStateDialog->setWarningType(DISCONNECT_E);
}

MainWindow::~MainWindow()
{
    m_maskDialog.close();
    if(m_blinkTimer)
    {
        m_blinkTimer->stop();
        delete  m_blinkTimer;
        m_blinkTimer = NULL;
    }
    if(m_userManager)
    {
        m_userManager->close();
        delete m_userManager;
    }
    if(m_connectStateLabel)
        delete m_connectStateLabel;
    if(m_connectStateDialog)
        delete m_connectStateDialog;

    delete ui;
}

void MainWindow::initButtonGroup()
{
    m_buttonGroup = new QButtonGroup();
    //设置互斥开启
    m_buttonGroup->setExclusive(true);
    m_buttonGroup->addButton(ui->training_Btn,0);

    m_buttonGroup->addButton(ui->scope_Btn,1);
    m_buttonGroup->addButton(ui->teach_Btn,2);
    m_buttonGroup->addButton(ui->history_Btn,3);
    m_buttonGroup->addButton(ui->ranking_Btn,4);
    m_buttonGroup->addButton(ui->setting_Btn,5);
    m_buttonGroup->addButton(ui->quickStart_Btn,7);
    m_buttonGroup->addButton(ui->shutDown_Btn,8);

    connect(m_buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotBtnGroupClicked(int)));

    ui->training_Btn->setChecked(true);

}

void MainWindow::initWidgetUI()
{
    QGraphicsDropShadowEffect * shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setYOffset(2);
    shadowEffect->setBlurRadius(12);
    shadowEffect->setColor(QColor(0,0,0,51));
    ui->title_Widget->setGraphicsEffect(shadowEffect);
    //提升层级 提升控件的Z值
    ui->title_Widget->raise();
}

void MainWindow::slotBtnGroupClicked(int btnId)
{
    //设置当前按下的按钮为选中状态，其他的为未选中状态
    QPushButton *selectedBtn = qobject_cast<QPushButton*>(m_buttonGroup->button(btnId));
    selectedBtn->setStyleSheet("color: white;background: #0D9DDB;");
    foreach(QAbstractButton* button,m_buttonGroup->buttons())
    {
        QPushButton *tempBtn =  qobject_cast<QPushButton*>(button);
        if(tempBtn != selectedBtn)
        {
            tempBtn->setStyleSheet("QPushButton:hover{color: black;background: #E4F4FC;}"
                                   "QPushButton{color: black;background: white;}");
        }
    }
}

void MainWindow::slotCurrentStackWidgetChanged(int index)
{
    ui->scope_Widget->stopMovie();
    if(1 == index)
    {
        ui->scope_Widget->startMovie();
    }
}

void MainWindow::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event)
    m_maskDialog.close();
}

void MainWindow::on_training_Btn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->train_page);
}

void MainWindow::on_scope_Btn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->scorp_page);
    ui->scope_Widget->startMovie();
}

void MainWindow::on_teach_Btn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->teach_page);
}

void MainWindow::on_history_Btn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->history_page);
}

void MainWindow::on_ranking_Btn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ranking_page);
}

void MainWindow::on_setting_Btn_clicked()
{
    CPasswordDialog m_dialog;
    int result = m_dialog.exec();
    if(3 == result)
        ui->stackedWidget->setCurrentWidget(ui->setting_page);
    else
        return;
}

void MainWindow::on_shutDown_Btn_clicked()
{
    this->close();
}

void MainWindow::slotReceiveRecipe(bool checked)
{
    if(checked)
    {
        ui->stackedWidget->setCurrentWidget(ui->teach_page);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)

}

void MainWindow::on_quickStart_Btn_clicked()
{
    //直接进入游戏
    ui->train_Widget->quickIntoTrain();
}

void MainWindow::slotSwitchPage(int index)
{
    switch(index)
    {
    case 0: //trainPage
        ui->stackedWidget->setCurrentWidget(ui->train_page);
        break;
    case 1: //scorpPage
        ui->stackedWidget->setCurrentWidget(ui->scorp_page);
        break;
    case 2: //teachPage
        ui->stackedWidget->setCurrentWidget(ui->teach_page);
        break;
    case 3: //historyPage
        ui->stackedWidget->setCurrentWidget(ui->history_page);
        break;
    case 4: //rankingPage
        ui->stackedWidget->setCurrentWidget(ui->ranking_page);
        break;
    case 5: //settingPage
        ui->stackedWidget->setCurrentWidget(ui->setting_page);
        break;
    case 6: //settingPage
        ui->stackedWidget->setCurrentWidget(ui->user_page);
        break;
    }

    slotBtnGroupClicked(index); 
}

//切换到用户管理界面
void MainWindow::on_switchUser_Btn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->train_page);
    m_userManager->show();
}

void MainWindow::slotCurrentUserChanged()
{
    setCurrentUser();
}

//更新当前用户信息
void MainWindow::setCurrentUser()
{
    ST_PatientMsg st_PatientMsg = CurrentUserData::getInstace()->getCurrentPatientMsg();
    QString pixPath;
    if(0 == st_PatientMsg.sex)//male
        pixPath = ":/new/prefix1/release/source/image/male.png";
    else if(1 == st_PatientMsg.sex)//female
        pixPath = ":/new/prefix1/release/source/image/female.png";
    QPixmap pix(pixPath);
    ui->portrait_Label->setPixmap(pix);

    ui->ID_Label->setText(QString("ID号:%1").arg(st_PatientMsg.ID));
    ui->name_Label->setText(QString("姓名:%1").arg(st_PatientMsg.name));
}

void MainWindow::enterUserManagerPage()
{
    on_switchUser_Btn_clicked();
}

//显示急停效果槽
void MainWindow::slotShowEmergency(bool isShow)
{
//    showEmergency(isShow);
}

void MainWindow::slotReceiveCommuAPIData(QByteArray array)
{
    switch(array[4])
    {
    case DEVICEISON:
        m_timeOutTimes = 0;
        break;
    default:
        break;
    }
}

void MainWindow::slotHeartBeatChecking()
{
    //当超时次数大于5时则弹出警告框
    if(m_timeOutTimes > 3)
    {
        ReadConfig::getInstance()->setConnectState(false);
        m_timeOutTimes = 10;
        m_connectStateLabel->show();
        m_blinkTimer->start();
    }
    else //隐藏图标
    {
        ReadConfig::getInstance()->setConnectState(true);
        if(m_connectStateLabel->isVisible())
            m_connectStateLabel->hide();
    }
    m_timeOutTimes++;
}

void MainWindow::slotConnectBlink()
{
    m_blinkTimer->stop();
    m_connectStateLabel->hide();
}
