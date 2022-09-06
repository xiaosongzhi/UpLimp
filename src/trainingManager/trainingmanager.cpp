#include "trainingmanager.h"
#include "ui_trainingmanager.h"
#include "ccommunicateapi.h"
#include "dataFormate.h"
#include "recipeselectdialog.h"
#include "reportdialog.h"
#include "currentuserdata.h"
#include <QDebug>
#include <QTimer>
#include <QtMath>
#include <QDateTime>
#include "simulategame.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QFile>
#include "readconfig.h"
#include "gameparamcontrol.h"
#include <QMessageBox>
#include <QDateTime>
#include "logger.h"
#include "cdatabaseinterface.h"
#include <QMapIterator>

LOG4QT_DECLARE_STATIC_LOGGER(logger, TrainingManager)

TrainingManager::TrainingManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrainingManager),
    m_recipeSelected(NULL),
    m_reportDialog(NULL),
    isTraining(false),
    m_heartBeatTimes(0),
    m_tarinMode(E_TRAINMODE_PASSIVE),    //默认为被动模式
    m_spasmTimes(0),
    m_gameSocket(NULL),
    m_Process(NULL),
    m_currentGamePath(""),
    m_emergencyWarningDialog(NULL),
    m_spasmWarningDialog(NULL),
    m_waitingDialog(NULL),
    m_clampDialog(NULL),
    m_resetHadleDialog(nullptr),
    m_bindTipDialog(nullptr),
    m_isSpasming(false),
    bells("./source/music/warning.wav")
{
    ui->setupUi(this);
    CCommunicateAPI *m_commuAPI = CCommunicateAPI::getInstance();
    m_recipeSelected = new RecipeSelectDialog();
    m_reportDialog = new ReportDialog();
    connect(m_commuAPI,SIGNAL(signalReadyRead(QByteArray)),this,SLOT(slotReceiveCommuAPIData(QByteArray)));
    for(int i = 0;i <ui->stackedWidget->count();i++)
    {
        connect(ui->stackedWidget->widget(i),SIGNAL(signalRecipeChecked(bool)),this,SLOT(slotRecipeShow(bool)));
    }

    initGameSocket();
    initButtonGroup();

    m_Process = new QProcess();
    connect(m_Process,SIGNAL(stateChanged(QProcess::ProcessState)),this,SLOT(slotStateChanged(QProcess::ProcessState)));
    connect(m_Process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(slotProcessFinished(int,QProcess::ExitStatus)));

    m_emergencyWarningDialog = new WarningDialog();
    m_spasmWarningDialog = new WarningDialog();
    //痉挛复位连接
    connect(m_spasmWarningDialog,SIGNAL(confirmSignal()),this,SLOT(slotSpasmRecover()));
    m_waitingDialog = new WaitingDialog();
    m_clampDialog = new WarningDialog();
    uploadGameBtn();

    ui->stackedWidget->setCurrentIndex(1);

    loadStyleSheetFile();

    setTestBtnHide(false);

    ui->debug_GroupBox->setVisible(false);

    ui->point_label->setVisible(false);

    ui->message_Label->setVisible(false);

    ui->message_Label->setVisible(false);

    ui->point_label->setVisible(false);

    //测试初始化使用
    m_st_TrainReport.score = 0;
    ui->msg_Label->setText("被动运动是指设备完全带动患者运动，患者不需要提供任何力。");

    //设置报警音无线循环
    bells.setLoops(-1);

    m_resetHadleDialog = new ResetHandleDialog();

    m_bindTipDialog = new BindTipDialog();
}

TrainingManager::~TrainingManager()
{
    if(m_recipeSelected)
    {
        m_recipeSelected->deleteLater();
        m_recipeSelected = NULL;
    }
    if(m_reportDialog)
    {
        m_reportDialog->deleteLater();
        m_reportDialog = NULL;
    }
    if(m_gameSocket)
    {
        m_gameSocket->deleteLater();
        m_gameSocket = NULL;
    }
    if(m_Process)
    {
        m_Process->kill();
        m_Process->deleteLater();
        m_Process = NULL;
    }
    if(m_emergencyWarningDialog)
    {
        m_emergencyWarningDialog->deleteLater();
        m_emergencyWarningDialog = NULL;
    }
    if(m_spasmWarningDialog)
    {
        delete  m_spasmWarningDialog;
        m_spasmWarningDialog = NULL;
    }
    if(m_waitingDialog)
    {
        m_waitingDialog->deleteLater();
        m_waitingDialog = NULL;
    }
    if(m_clampDialog)
    {
        delete m_clampDialog;
        m_clampDialog = NULL;
    }
    if(m_resetHadleDialog)
    {
        delete m_resetHadleDialog;
        m_resetHadleDialog = nullptr;
    }
    if(m_bindTipDialog)
    {
        delete  m_bindTipDialog;
        m_bindTipDialog = nullptr;
    }
    delete ui;
}


void TrainingManager::loadStyleSheetFile()
{
    //加载样式文件
    QFile qssFile("./QSS/trainmanager.qss");
    if(!qssFile.open(QIODevice::ReadOnly))
    {
        logger()->debug("trainmanager.qss文件打开失败!");
    }
    else
    {
        this->setStyleSheet(qssFile.readAll());
        qssFile.close();
    }
}

void TrainingManager::initGameSocket()
{
    m_gameSocket = new QUdpSocket();
    int16_t port;
    QString IP;
    ReadConfig::getInstance()->getGameSeverAddress(port,IP);
    if(m_gameSocket->bind(port))
    {
        logger()->debug(QString("游戏服务端口%1").arg(port));
    }
    connect(m_gameSocket,&QUdpSocket::readyRead,this,&TrainingManager::slotReceiveGameData);
}

void TrainingManager::initButtonGroup()
{
    m_buttonGroup = new QButtonGroup();
    m_buttonGroup->addButton(ui->active_Btn,0);
    m_buttonGroup->addButton(ui->boost_Btn,1);
    m_buttonGroup->addButton(ui->passive_Btn,2);
    m_buttonGroup->addButton(ui->against_Btn,3);
    m_buttonGroup->setExclusive(true);
    connect(m_buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotBtnGroupClicked(int)));
}

/*********************与游戏交互***********************/
void TrainingManager::slotReceiveGameData()
{
    while(m_gameSocket->hasPendingDatagrams())
    {
        QByteArray buf;
        buf.resize(m_gameSocket->pendingDatagramSize());
        m_gameSocket->readDatagram(buf.data(),buf.size());
        parseGameMsg(buf);
    }
}

//解析游戏传输的坐标点数据
ST_ControlData TrainingManager::parseGamePoint(QJsonObject& object)
{
    ST_ControlData st_ControlData;
    if(object.contains("m_xv"))
    {
        QJsonObject xObject = object.value("m_xv").toObject();
        int current = 0;
        int max = 1;
        if(xObject.contains("current"))
            current = xObject.value("current").toInt();
        if(xObject.contains("max"))
            max = xObject.value("max").toInt();

        st_ControlData.st_EndPort.x = (current/(float)max) * DEVICEWIDTH;
    }
    if(object.contains("m_yv"))
    {
        QJsonObject yObject = object.value("m_yv").toObject();
        int current = 0;
        int max = 1;
        if(yObject.contains("current"))
            current = yObject.value("current").toInt();
        if(yObject.contains("max"))
            max = yObject.value("max").toInt();

        st_ControlData.st_EndPort.y = (current/(float)max) * DEVICEHEIGHT;
    }
    if(object.contains("m_startBarrierPoint"))
    {
        QJsonObject startBarrierPoint = object.value("m_startBarrierPoint").toObject();
        if(startBarrierPoint.contains("xPoint"))
        {
            QJsonObject startObject = startBarrierPoint.value("xPoint").toObject();
            int current = 0;
            int max = 1;
            if(startObject.contains("current"))
                current = startObject.value("current").toInt();
            if(startObject.contains("max"))
                max = startObject.value("max").toInt();
            st_ControlData.st_LeftTopPoint.x = (current/(float)max) * DEVICEWIDTH;
        }
        if(startBarrierPoint.contains("yPoint"))
        {
            QJsonObject endObject = startBarrierPoint.value("yPoint").toObject();
            int current = 0;
            int max = 1;
            if(endObject.contains("current"))
                current = endObject.value("current").toInt();
            if(endObject.contains("max"))
                max = endObject.value("max").toInt();
            st_ControlData.st_LeftTopPoint.y = (current/(float)max) * DEVICEHEIGHT;
        }
    }
    if(object.contains("m_endBarrierPoint"))
    {
        QJsonObject endBarrierPoint = object.value("m_endBarrierPoint").toObject();
        if(endBarrierPoint.contains("xPoint"))
        {
            QJsonObject startObject = endBarrierPoint.value("xPoint").toObject();
            int current = 0;
            int max = 1;
            if(startObject.contains("current"))
                current = startObject.value("current").toInt();
            if(startObject.contains("max"))
                max = startObject.value("max").toInt();
            st_ControlData.st_RightBottomPoint.x = (current/(float)max) * DEVICEWIDTH;
        }
        if(endBarrierPoint.contains("yPoint"))
        {
            QJsonObject endObject = endBarrierPoint.value("yPoint").toObject();
            int current = 0;
            int max = 1;
            if(endObject.contains("current"))
                current = endObject.value("current").toInt();
            if(endObject.contains("max"))
                max = endObject.value("max").toInt();
            st_ControlData.st_RightBottomPoint.y = (current/(float)max) * DEVICEHEIGHT;
        }
    }

    return st_ControlData;
}

void TrainingManager::parseGameMsg(QByteArray jsonArray)
{
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(jsonArray, &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        if(doucment.isObject())
        {
            QJsonObject object = doucment.object();  // 转化为对象
            if(object.contains("msgID"))
            {
                int msgID = object.value("msgID").toInt();
                switch(msgID)
                {
                case 1: //游戏开始信号
                {
                    m_spasmTimes = 0;
                    isTraining = true;
                    //退出加载游戏的动画
                    m_waitingDialog->setDialogCloseState(true);
                    m_st_TrainReport.startTime = QDateTime::currentDateTime();

                    if(E_TRAINMODE_ACTIVE == m_tarinMode)
                    {
                        m_currentControlData.E_Mode = m_tarinMode;
                        sendControlData(m_currentControlData);
                    }
                }
                    break;
                case 2: //游戏给出的目标位置
                {
                    //有障碍物的情况下
                    if(E_TRAINMODE_ACTIVE == m_tarinMode)
                    {
                        ST_ControlData tempCurrentControlData = parseGamePoint(object);
                        m_currentControlData.st_EndPort = tempCurrentControlData.st_EndPort;
                        m_currentControlData.st_LeftTopPoint = tempCurrentControlData.st_LeftTopPoint;
                        m_currentControlData.st_RightBottomPoint = tempCurrentControlData.st_RightBottomPoint;
                    }
                    else
                    {
                        if(object.contains("m_xv"))
                        {
                            QJsonObject xObject = object.value("m_xv").toObject();
                            int current = 0;
                            int max = 1;
                            if(xObject.contains("current"))
                                current = xObject.value("current").toInt();
                            if(xObject.contains("max"))
                                max = xObject.value("max").toInt();
                            m_currentControlData.st_EndPort.x = (current/(float)max) * DEVICEWIDTH;
                        }
                        if(object.contains("m_yv"))
                        {
                            QJsonObject yObject = object.value("m_yv").toObject();
                            int current = 0;
                            int max = 1;
                            if(yObject.contains("current"))
                                current = yObject.value("current").toInt();
                            if(yObject.contains("max"))
                                max = yObject.value("max").toInt();

                            m_currentControlData.st_EndPort.y = (current/(float)max) * DEVICEHEIGHT;
                        }

                    }
                    //非痉挛状态下发送给下位机
                    if(!m_isSpasming)
                    {
                        //在被动和抗阻模式下热身和放松起作用
                        if(E_TRAINMODE_PASSIVE == m_currentControlData.E_Mode)
                        {
                            //热身阶段
                            if(QDateTime::currentDateTime() < m_st_TrainReport.startTime.addSecs(m_trainProcessMap.value(0)*60))
                            {
                                m_currentControlData.speed = getSpeedByLevel(1);
                            }//训练阶段
                            else if(QDateTime::currentDateTime() < m_st_TrainReport.startTime.addSecs((m_trainProcessMap.value(0)+m_trainProcessMap.value(1))*60))
                            {
                                m_currentControlData.speed = getSpeedByLevel(m_passiveSpeed);
                            }
                            else
                                m_currentControlData.speed = getSpeedByLevel(1);
                            //放松阶段
                        }
                        else if(E_TRAINMODE_AGAINST == m_currentControlData.E_Mode)
                        {

                        }

                        sendControlData(m_currentControlData);
                    }

                }
                    break;
                case 3: //结束信息
                {
                    isTraining = false;
                    if(object.contains("quitType"))
                    {
                        //告知下位机训练完成
                        sendStopCmd();
                        m_quitType = object.value("quitType").toInt();
                        //正常退出
                        if(1 == m_quitType)
                        {
                            stopTraining();
                            logger()->debug("游戏运行完，正常退出");
                            m_st_TrainReport.endTime = QDateTime::currentDateTime();
                            if(object.contains("totalTime"))
                                m_st_TrainReport.trainDuration = object.value("totalTime").toInt();
                            if(object.contains("totalScore"))
                                m_st_TrainReport.score = object.value("totalScore").toInt();
                            //生成训练报告
                            createTrainReport();
                        }
                        //手动退出
                        else if(-1 == m_quitType)
                        {
                            stopTraining();
                            logger()->debug("游戏未运行完，被手动退出");
                            m_st_TrainReport.endTime = QDateTime::currentDateTime();
                            if(object.contains("totalTime"))
                                m_st_TrainReport.trainDuration = object.value("totalTime").toInt();
                            if(object.contains("totalScore"))
                                m_st_TrainReport.score = object.value("totalScore").toInt();
                            //生成训练报告
                            createTrainReport();
                        }
                        //急停退出不生成报告
                        else if(-2 == m_quitType)
                        {
                            qDebug()<<"设备急停，退出游戏";
                        }
                    }
                }
                    break;
                }
            }
        }
    }
    else
    {
        logger()->debug(jsonError.errorString());
        //此处不该在此处停止心跳
        stopTraining();
    }
}

void TrainingManager::sendCurrentPointToGame(QPointF pointF)
{
    ui->point_label->setText("x:"+QString::number(pointF.x()) + " y:"+QString::number(pointF.y()));
    QString toGamePoint = "device x:"+QString::number(pointF.x()) + " y:"+QString::number(pointF.y());
    QJsonObject Xobject;
    Xobject.insert("current",pointF.x());
    Xobject.insert("min",0);
    Xobject.insert("max",DEVICEWIDTH);
    QJsonObject Yobject;
    Yobject.insert("current",pointF.y());
    Yobject.insert("min",0);
    Yobject.insert("max",DEVICEHEIGHT);
    QJsonObject object;
    object.insert("msgID",2);
    object.insert("TypeID",1);
    object.insert("m_xv",Xobject);
    object.insert("m_yv",Yobject);
    QJsonDocument document;
    document.setObject(object);
    QByteArray sendArray = document.toJson();
    QString ip;
    int16_t port;
    ReadConfig::getInstance()->getGameClientAddress(port,ip);
    m_gameSocket->writeDatagram(sendArray,QHostAddress(ip),port);
}

//上位机控制退出游戏
void TrainingManager::stopGame()
{
    QJsonObject object;
    object.insert("msgID",4);
    object.insert("TypeID",-2);
    QJsonDocument document;
    document.setObject(object);
    QByteArray sendArray = document.toJson();
    int16_t port;
    QString ip;
    ReadConfig::getInstance()->getGameClientAddress(port,ip);
    m_gameSocket->writeDatagram(sendArray,QHostAddress(ip),port);
}

//上位机控制暂停游戏（痉挛使用）
void TrainingManager::pauseGame()
{
    QJsonObject object;
    object.insert("msgID",5);
    object.insert("TypeID",0);
    QJsonDocument document;
    document.setObject(object);
    QByteArray sendArray = document.toJson();
    int16_t port;
    QString ip;
    ReadConfig::getInstance()->getGameClientAddress(port,ip);
    m_gameSocket->writeDatagram(sendArray,QHostAddress(ip),port);
}
//上位机控制暂停后的继续
void TrainingManager::resumeGame()
{
    QJsonObject object;
    object.insert("msgID",5);
    object.insert("TypeID",1);
    QJsonDocument document;
    document.setObject(object);
    QByteArray sendArray = document.toJson();
    int16_t port;
    QString ip;
    ReadConfig::getInstance()->getGameClientAddress(port,ip);
    m_gameSocket->writeDatagram(sendArray,QHostAddress(ip),port);
    Sleep(100);
    m_gameSocket->writeDatagram(sendArray,QHostAddress(ip),port);

}

//退出游戏指令
void TrainingManager::quitGameCmd()
{
    QJsonObject object;
    object.insert("msgID",4);
    QJsonDocument document;
    document.setObject(object);
    QByteArray sendArray = document.toJson();
    QString ip;
    int16_t port;
    ReadConfig::getInstance()->getGameClientAddress(port,ip);
    m_gameSocket->writeDatagram(sendArray,QHostAddress(ip),port);
}

void TrainingManager::slotBtnGroupClicked(int btnId)
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

void TrainingManager::slotRecipeShow(bool checked)
{
    if(checked)
    {
        m_recipeSelected->show();
    }
}

void TrainingManager::slotReceiveCommuAPIData(QByteArray array)
{
    int length = array[1];
    QByteArray data = array.mid(5,length);

    switch(array[4])
    {
    case HEARTBEAT:  //心跳
    {
        ST_DeviceParam st_DeviceParam;
        memcpy(&st_DeviceParam,data.data(),sizeof(ST_DeviceParam));
        dealwithTrainData(st_DeviceParam);
    }
        break;
    case EMERGENCY:  //急停报警
        switch(array[5])
        {
        case 0://急停复位
        {
            emit signalShowEmergency(false);
            m_emergencyWarningDialog->setWarningType(EMERGENCY_E);
            m_emergencyWarningDialog->hide();
        }
            break;
        case 1://开启急停弹框
        {
            setEmergency();
        }
            break;
        }
        break;
    case DEVICEERROR://设备报警
        setDeviceError(data[0]);
        break;
    case SPASMWARNING://痉挛报警
        setSapsmWarning();
        break;
    case READYCMD:     //下位机回到了原点
    {
        if(m_bindTipDialog->exec() != 1)
            return;
        //弹出设备复位中的弹框
        m_waitingDialog->setWaitingType(LOADINGWITING);
        m_waitingDialog->setDialogCloseState(false);
        //启动游戏
        startGame(m_currentGamePath);
        //将获取式改为开关式
        switchSendHeartBeat(true);
    }
        break;
    default:
        break;
    }
}

//处理下位机实时数据
void TrainingManager::dealwithTrainData(const ST_DeviceParam& st_DeviceParam)
{
    static QPoint lastPoint(0,0);
    //正在训练中--记录其运动轨迹,完成后需要将运动轨迹绘制在报告中
    if(isTraining)
    {
        QPoint point(st_DeviceParam.st_currentPoint.x,st_DeviceParam.st_currentPoint.y);
        QPointF pointF(st_DeviceParam.st_currentPoint.x,st_DeviceParam.st_currentPoint.y);
        //正在训练将其发送给游戏
        sendCurrentPointToGame(pointF);
        //将非重复数据记录到文件中
        if(lastPoint != point)
        {
            m_paintList.append(point);
        }
        lastPoint.setX(st_DeviceParam.st_currentPoint.x);
        lastPoint.setY(st_DeviceParam.st_currentPoint.y);
    }
}

//心跳开关
void TrainingManager::switchSendHeartBeat(bool isOn)
{
    QByteArray sendArray(1,0);
    if(isOn)
        sendArray[0] = 1;
    else
        sendArray[0] = 0;
    CCommunicateAPI::getInstance()->sendData(0,2,HEARTBEAT,sendArray);
}

//设置急停效果(急停直接退出游戏)
void TrainingManager::setEmergency()
{
    //收到急停后，退出游戏，不生成报告
//    stopGame();
    //先停止设备运动
    stopTraining();
    //弹出急停提示框
//    emit signalShowEmergency(true);
    m_emergencyWarningDialog->setWarningType(EMERGENCY_E);
    m_emergencyWarningDialog->show();
}

//故障报警
void TrainingManager::setDeviceError(int errorType)
{
    //设备报警弹框
    switch(errorType)
    {
    case 0: //夹手复位
        logger()->debug("夹手恢复！");
        m_clampDialog->hide();
        break;
    case 1:
        logger()->debug("夹手警告！");
        m_clampDialog->setWarningType(CLAMP_E);
        m_clampDialog->show();
        break;//夹手
    }
}

//生成报告
void TrainingManager::createTrainReport()
{
    //外部拉力
    float F = 1;
    //获取报告所需要的数据
    CurrentUserData *currentUser = CurrentUserData::getInstace();
    ST_PatientMsg st_PatientMsg = currentUser->getCurrentPatientMsg();
    m_st_TrainReport.level = 1; //用户等级
    m_st_TrainReport.ID = st_PatientMsg.ID;
    m_st_TrainReport.name = st_PatientMsg.name;
    m_st_TrainReport.trainLength = calculateTotalLength(m_paintList);
    //消耗掉的能量
    m_st_TrainReport.energy = m_st_TrainReport.trainLength/1000 * F;
    m_st_TrainReport.trainMode = m_tarinMode;
    m_st_TrainReport.trainDuration = m_st_TrainReport.startTime.secsTo(m_st_TrainReport.endTime);
    m_st_TrainReport.spasmTimes = m_spasmTimes;
    if(m_st_TrainReport.trainDuration != 0)
    {
        m_st_TrainReport.trainSpeed = m_st_TrainReport.trainLength/10.f/(m_st_TrainReport.trainDuration-m_st_TrainReport.score*0.4);
        //1239/10/(300-490*0.4)
    }
    else
        m_st_TrainReport.trainSpeed = 0;
    //生成报告
    m_reportDialog->setTrainReport(m_st_TrainReport,m_paintList,0);
    m_paintList.clear();
}

//痉挛报警(痉挛只是暂停游戏并不停止游戏)
void TrainingManager::setSapsmWarning()
{
    //开启报警音
    playBell();
    //暂停游戏
    pauseGame();
    //痉挛状态置位
    m_isSpasming = true;
    ++m_spasmTimes;
    //弹出痉挛报警框
    m_spasmWarningDialog->setWarningType(SPASM_E,m_spasmTimes);
    m_spasmWarningDialog->show();
}

float TrainingManager::calculateTotalLength(QList<QPoint>& pointList)
{
    float length = 0;
    for(int i = 0; i < pointList.size()-1;i++)
    {
        QPoint leftPoint = pointList.at(i);
        QPoint rightPoint = pointList.at(i+1);
        length += qSqrt(qPow(rightPoint.x()-leftPoint.x(),2) + qPow(rightPoint.y()-leftPoint.y(),2));
    }
    return length;
}

void TrainingManager::on_passive_Btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    m_tarinMode = E_TRAINMODE_PASSIVE;
    setTipMessage(E_TRAINMODE_PASSIVE);
}

void TrainingManager::on_active_Btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    m_tarinMode = E_TRAINMODE_ACTIVE;
    setTipMessage(E_TRAINMODE_ACTIVE);
}

void TrainingManager::on_boost_Btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    m_tarinMode = E_TRAINMODE_BOOST;
    setTipMessage(E_TRAINMODE_BOOST);
}

void TrainingManager::on_against_Btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    m_tarinMode = E_TRAINMODE_AGAINST;
    setTipMessage(E_TRAINMODE_AGAINST);
}

void TrainingManager::on_startTrain_Btn_clicked()
{
    //在设备复位之前先给出提示
    int ret = m_resetHadleDialog->exec();
    if(ret == 0)
        return;
    else if(1 == ret)
    {
        m_heartBeatTimes = 0;
        m_isSpasming = false;
        ST_GameParam st_GameParam;
        //记录训练参数
        ST_TrainRecord st_TrainRecord;
        st_TrainRecord.ID = CurrentUserData::getInstace()->getCurrentUserID();
        st_TrainRecord.trainMode = m_tarinMode;
        switch (m_tarinMode)
        {
        case E_TRAINMODE_PASSIVE:
        {
            ST_PassiveParam st_PassiveParam = ui->passive_Page->getPassiveGameParam();
            st_TrainRecord.gamePath = st_PassiveParam.gamePath;
            st_TrainRecord.trackPath = st_PassiveParam.gameTrack;
            st_TrainRecord.trainScope = st_PassiveParam.scorpSize;
            st_TrainRecord.speed = st_PassiveParam.speed;
            st_TrainRecord.process = transProcess(st_PassiveParam.trainProcessMap);
            st_TrainRecord.blockLength = 0;
            st_TrainRecord.level = 1;
            m_passiveSpeed = st_PassiveParam.speed;
            m_currentGamePath = st_PassiveParam.gamePath;
            m_st_TrainReport.trainScorp = st_PassiveParam.scorpSize;
            m_st_TrainReport.trainTrackImage = st_PassiveParam.gameTrackImage;
            st_GameParam.gameTime = getTotalTrainingTime(st_PassiveParam.trainProcessMap);
            m_trainProcessMap = st_PassiveParam.trainProcessMap;

            if("海底冒险" == st_PassiveParam.gameName)
                st_GameParam.readPosTable = false;
            else if("地下钻探" == st_PassiveParam.gameName)
                st_GameParam.readPosTable = true;
            st_GameParam.hardLevel = st_PassiveParam.scorpSize;
            st_GameParam.speed = st_PassiveParam.speed;
            st_GameParam.trainingMode = st_PassiveParam.trainMode;

            if(ui->passive_Page->recipeIsSelected())
            {
                int index = st_PassiveParam.gameTrack.lastIndexOf("/");
                QString path = st_PassiveParam.gameTrack.left(index+1);
                st_GameParam.trainingTrack = path+m_recipeSelected->getSlectedRecipe()+".csv";

                st_TrainRecord.blockLength = 1; //读取轨迹文件
            }
            else
                st_GameParam.trainingTrack = st_PassiveParam.gameTrack;

            st_TrainRecord.trackPath = st_GameParam.trainingTrack;

            m_currentControlData.E_Mode = st_PassiveParam.trainMode;
            m_currentControlData.speed = getSpeedByLevel(st_PassiveParam.speed);
        }
            break;
        case E_TRAINMODE_ACTIVE:
        {
            ST_ActiveParam st_ActiveParam = ui->active_Page->getActiveGameParam();

            st_TrainRecord.gamePath = st_ActiveParam.gamePath;
    //        st_TrainRecord.trackPath = st_ActiveParam.gameTrack;
            st_TrainRecord.trainScope = st_ActiveParam.scorpSize;
            st_TrainRecord.blockLength = 0;
            st_TrainRecord.process = transProcess(st_ActiveParam.trainProcessMap);
            st_TrainRecord.level = 1;
            m_currentGamePath = st_ActiveParam.gamePath;

            m_st_TrainReport.trainScorp = st_ActiveParam.scorpSize;
            m_st_TrainReport.trainTrackImage = st_ActiveParam.gameTrackImage;
            st_GameParam.gameTime = getTotalTrainingTime(st_ActiveParam.trainProcessMap);
            st_GameParam.hardLevel = st_ActiveParam.scorpSize;
            st_GameParam.trainingMode = st_ActiveParam.trainMode;
            st_GameParam.readPosTable = false;
            if(ui->active_Page->recipeIsSelected())
                st_GameParam.trainingTrack = m_recipeSelected->getSlectedRecipe();

            else
                st_GameParam.trainingTrack = st_ActiveParam.gameTrack;

            st_TrainRecord.trackPath = st_GameParam.trainingTrack;

            m_trainProcessMap = st_ActiveParam.trainProcessMap;
            m_currentControlData.E_Mode = st_ActiveParam.trainMode;
            m_currentControlData.force = 0;
            m_currentControlData.speed = 0;
        }
            break;
        case E_TRAINMODE_BOOST:
        {
            ST_BoostParam st_BoostParam = ui->boost_Page->getBoostGameParam();

            st_TrainRecord.gamePath = st_BoostParam.gamePath;
            st_TrainRecord.trackPath = st_BoostParam.gameTrack;
            st_TrainRecord.trainScope = st_BoostParam.scorpSize;
            st_TrainRecord.level = st_BoostParam.level;
            st_TrainRecord.process = transProcess(st_BoostParam.trainProcessMap);
            st_TrainRecord.blockLength = 0;

            m_currentGamePath = st_BoostParam.gamePath;
            m_trainProcessMap = st_BoostParam.trainProcessMap;
            m_st_TrainReport.trainScorp = st_BoostParam.scorpSize;
            m_st_TrainReport.trainTrackImage = st_BoostParam.gameTrackImage;
            st_GameParam.gameTime = getTotalTrainingTime(st_BoostParam.trainProcessMap);
            st_GameParam.hardLevel = st_BoostParam.scorpSize;
            st_GameParam.trainingMode = st_BoostParam.trainMode;
            st_GameParam.trainingTrack = st_BoostParam.gameTrack;
            st_GameParam.readPosTable = false;
            if(ui->boost_Page->recipeIsSelected())
                st_GameParam.trainingTrack = m_recipeSelected->getSlectedRecipe();
            else
                st_GameParam.trainingTrack = st_BoostParam.gameTrack;


            m_currentControlData.E_Mode = st_BoostParam.trainMode;
            //        m_currentControlData.force = getBoostForceByLevel(st_BoostParam.level);
            m_currentControlData.force = st_BoostParam.level;
            m_currentControlData.speed = 0;
        }
            break;
        case E_TRAINMODE_AGAINST:
        {
            ST_AgainstParam st_AgainstParam = ui->against_Page->getAgainstGameParam();

            st_TrainRecord.gamePath = st_AgainstParam.gamePath;
            st_TrainRecord.trackPath = st_AgainstParam.gameTrack;
            st_TrainRecord.trainScope = st_AgainstParam.scorpSize;
            st_TrainRecord.level = st_AgainstParam.level;
            st_TrainRecord.process = transProcess(st_AgainstParam.trainProcessMap);
            st_TrainRecord.blockLength = 0;
            m_againstLevel = st_AgainstParam.level;
            m_currentGamePath = st_AgainstParam.gamePath;
            m_st_TrainReport.trainScorp = st_AgainstParam.scorpSize;
            m_st_TrainReport.trainTrackImage = st_AgainstParam.gameTrackImage;

            m_trainProcessMap = st_AgainstParam.trainProcessMap;
            st_GameParam.gameTime = getTotalTrainingTime(st_AgainstParam.trainProcessMap);
            st_GameParam.hardLevel = st_AgainstParam.scorpSize;
            st_GameParam.trainingMode = st_AgainstParam.trainMode;
            st_GameParam.trainingTrack = st_AgainstParam.gameTrack;
            if(ui->against_Page->recipeIsSelected())
                st_GameParam.trainingTrack = m_recipeSelected->getSlectedRecipe();
            else
                st_GameParam.trainingTrack = st_AgainstParam.gameTrack;

            st_GameParam.readPosTable = false;

            m_currentControlData.E_Mode = st_AgainstParam.trainMode;
            //        m_currentControlData.force = getResistanceByLevel(st_AgainstParam.level);
            m_currentControlData.force = st_AgainstParam.level;
            m_currentControlData.speed = 0;
        }
            break;
        }

        //是否读取文件中的轨迹（此处的参数待定）

        st_TrainRecord.gameTime = st_GameParam.gameTime;
        //将游戏训练参数添加到数据库中
        recordCurrentUserTrainParam(st_TrainRecord);

        GameParamControl::getInstance()->setGamParam(st_GameParam);


        //告知下位机复位
        setGoHome();

    }

}

void TrainingManager::uploadGameBtn()
{
    //读取游戏配置文件，1有些ID 2路径 3图标 4游戏名
    QList<ST_GameMsg> gameList = GameParamControl::getInstance()->getGameMsgs();
    //根据游戏图标创建游戏按钮
    ui->active_Page->setGameList(gameList);
    ui->boost_Page->setGameList(gameList);
    ui->against_Page->setGameList(gameList);
    ui->passive_Page->setGameList(gameList);
    //将ID和按钮进行绑定
}

//下位机回原点指令
void TrainingManager::setGoHome()
{
    QByteArray sendArray(1,0);
    CCommunicateAPI::getInstance()->sendData(0,2,GOHOME,sendArray);
}
//将目标位置发送给下位机
void TrainingManager::sendControlData(ST_ControlData& st_ControlData)
{
    QByteArray sendArray;
    sendArray.resize(sizeof(ST_ControlData));
    memcpy(sendArray.data(),&st_ControlData,sendArray.length());
    CCommunicateAPI::getInstance()->sendData(0,2,CONTROLDATA,sendArray);
}

void TrainingManager::sendStopCmd()
{
    QByteArray sendArray(0);
    CCommunicateAPI::getInstance()->sendData(0,2,STOPTRAINING,sendArray);
}

void TrainingManager::stopTraining()
{
    switchSendHeartBeat(false);
    Sleep(100);
    sendStopCmd();
}

void TrainingManager::startGame(QString path)
{
    logger()->debug("当前启动游戏的路径" + path);
    //1.开启游戏进程
    if(path.isEmpty())
        return;
    QString hardDisk = path.mid(0,2);
    hardDisk.append("\n\r");

    QString gameName = path.mid(path.lastIndexOf('/')+1);
    gameName.prepend("start ");
    gameName.append("\n\r");
    QString gamePath = path.mid(0,path.lastIndexOf('/'));
    gamePath.prepend("cd ");
    gamePath.append("\n\r");
    m_Process->start("cmd.exe");
    //切换盘符
    m_Process->write(hardDisk.toLatin1());
    //进入文件夹
    m_Process->write(gamePath.toLatin1());
    //开启进程
    m_Process->write(gameName.toLatin1());
    m_Process->write("exit\n\r");
    m_Process->waitForFinished();
    m_Process->close();

}

void TrainingManager::terminateGame(QString gameName)
{
    if(gameName.isEmpty())
        return;
    m_Process->start("cmd.exe");
    QString stopGameCmd = QString("taskkill /im %1 /f").arg(gameName);
    m_Process->write(stopGameCmd.toLatin1());

    m_Process->write("\n\r");
}


void TrainingManager::on_goHome_Btn_clicked()
{
    setGoHome();
}

void TrainingManager::on_stop_Btn_clicked()
{
    sendStopCmd();
}

void TrainingManager::slotStateChanged(QProcess::ProcessState state)
{
    qDebug()<<"state"<<state;
}

void TrainingManager::slotProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug()<<"exitCode="<<exitCode << "exitStatus"<<exitStatus;
}

int TrainingManager::getTotalTrainingTime(QMap<int,int>&trainProcessMap)
{
    int totalTime = 0;

    QMapIterator<int, int> i(trainProcessMap);
    while (i.hasNext()) {
        i.next();
        totalTime += i.value();
    }
    return totalTime*60;
}

//根据阻力等级获取阻力大小
int TrainingManager::getResistanceByLevel(int level)
{
    int force = 0;
    switch(level)
    {
    case 1:
        force = -10;
        break;
    case 2:
        force = -20;
        break;
    case 3:
        force = -30;
        break;
    case 4:
        force = -50;
        break;
    case 5:
        force = -60;
        break;
    }
    return force;

}
//根据助力等级获取助力大小
int TrainingManager::getBoostForceByLevel(int level)
{
    int force = 0;
    switch(level)
    {
    case 1:
        force = 30;
        break;
    case 2:
        force = 38;
        break;
    case 3:
        force = 46;
        break;
    case 4:
        force = 52;
        break;
    case 5:
        force = 60;
        break;
    }
    return force;
}

//根据速度等级获得实际速度
int TrainingManager::getSpeedByLevel(int level)
{
    int speed;
    speed = level*50;
    return speed;
}

//设置提示信息
void TrainingManager::setTipMessage(E_TRAINMODE mode)
{
    switch(mode)
    {
    case E_TRAINMODE_PASSIVE:
        ui->title_Label->setText("被动训练");
        ui->msg_Label->setText("被动运动是指设备完全带动患者运动，患者不需要提供任何力。");
        break;
    case E_TRAINMODE_ACTIVE:
        ui->title_Label->setText("主动训练");
        ui->msg_Label->setText("患者完全自主运动 机器不提供辅助力，只提供机械阻力，用户在活动范围内需要自己完成正确运动控制训练。");
        break;
    case E_TRAINMODE_BOOST:
        ui->title_Label->setText("助力训练");
        ui->msg_Label->setText("按需要提供辅助力，机器提供可设置大小的辅助力，帮助用户完成正确的运动动作。并且抑制用户偏离运动方向的异常运动，促进分离运动的出现。");
        break;
    case E_TRAINMODE_AGAINST:
        ui->title_Label->setText("抗阻训练");
        ui->msg_Label->setText("需患者克服阻力，机器提供可设置大小的不同阻抗力，用户需要克服阻力并完成 正确的运动动作，强化肌力和运动训练。");
        break;
    }
}

void TrainingManager::quickIntoTrain()
{
    //快速进入  进入被动模式
    ST_TrainRecord st_lastTrainRecord;
    int currentID = CurrentUserData::getInstace()->getCurrentUserID();
    if(getCurrentUserTrainParam(currentID,st_lastTrainRecord))
    {
        //在设备复位之前先给出提示
        int ret = m_resetHadleDialog->exec();
        if(ret == 0)
            return;
        //获取到之前的训练参数
        switchSendHeartBeat(true);
        m_isSpasming = false;
        ST_GameParam st_GameParam;
        st_GameParam.gameTime = st_lastTrainRecord.gameTime;
        //难度相当于是训练范围
        st_GameParam.hardLevel = st_lastTrainRecord.trainScope;
        st_GameParam.speed = st_lastTrainRecord.speed;
        st_GameParam.trainingMode = st_lastTrainRecord.trainMode;
        st_GameParam.trainingTrack = st_lastTrainRecord.trackPath;
        st_GameParam.readPosTable = st_lastTrainRecord.blockLength;

        m_currentGamePath = st_lastTrainRecord.gamePath;
        GameParamControl::getInstance()->setGamParam(st_GameParam);

        m_currentControlData.E_Mode = st_lastTrainRecord.trainMode;
        m_currentControlData.speed = getSpeedByLevel(st_lastTrainRecord.speed);
        m_currentControlData.force = st_lastTrainRecord.level;


        //告知下位机复位
        setGoHome();
        //弹出设备复位中的弹框
        m_waitingDialog->setWaitingType(LOADINGWITING);
        m_waitingDialog->setDialogCloseState(false);
    }
    else
    {
        //未获取到之前的训练参数
        qDebug()<<"未获取到之前的训练参数";
        QMessageBox::information(NULL,"提示","该用户为新用户，未找到上次训练参数");
        return;
    }
}

//记录当前用户训练参数(保存到数据库中)
void TrainingManager::recordCurrentUserTrainParam(ST_TrainRecord st_TrainRecord)
{
    ST_TrainRecord lastRecord;
    if(getCurrentUserTrainParam(st_TrainRecord.ID,lastRecord))
    {
        if(CDatabaseInterface::getInstance()->updateRowTable("trainrecord","ID",trainRecordToVariantMap(st_TrainRecord)))
            qDebug()<<"训练记录更新成功";
    }
    else
    {
        CDatabaseInterface::getInstance()->insertRowTable("trainrecord",trainRecordToVariantMap(st_TrainRecord));
        qDebug()<<"训练记录插入成功";
    }
}

//获取当前用户上一次训练参数
bool TrainingManager::getCurrentUserTrainParam(uint32_t ID,ST_TrainRecord& st_lastTrainRecord)
{
    QString query = QString("select * from trainrecord where ID = '%1'").arg(ID);
    CDatabaseInterface::getInstance()->exec(query);
    int count = CDatabaseInterface::getInstance()->getValuesSize();
    if(count > 0)
    {
        QVariantMap vMap = CDatabaseInterface::getInstance()->getValues(0,count).at(0);
        st_lastTrainRecord = variantMapToTrainRecord(vMap);
        return true;
    }
    else
        return false;
}

//训练过程的转换
QString TrainingManager::transProcess(QMap<int,int> processMap)
{
    QString process;
    QMapIterator<int,int> iter(processMap);

    while(iter.hasNext())
    {
        iter.next();
        process.append(QString::number(iter.value()));
        if(iter.hasNext())
            process.append(",");
    }
    return process;
}

QMap<int,int> TrainingManager::transProcess(QString process)
{
    QMap<int,int> processMap;
    QStringList list = process.split(',');
    for(int i = 0;i < list.size();i++)
        processMap.insert(i,list.at(i).toInt());
    return processMap;
}

//设置测试按钮的隐藏于显示
void TrainingManager::setTestBtnHide(bool hide)
{
    ui->heartBeat_Btn->setVisible(!hide);
    ui->pushButton->setVisible(!hide);
    ui->goHome_Btn->setVisible(!hide);
    ui->stop_Btn->setVisible(!hide);
}

//痉挛恢复
void TrainingManager::slotSpasmRecover()
{
    //关闭痉挛报警音
    stopPlayBell();
    if(m_spasmTimes < 3)
    {
        //重新给下位机发送当前的训练参数
        m_isSpasming = false;
        sendControlData(m_currentControlData);
        resumeGame();
    }
    //痉挛次数达到3次则退出当前训练
    else if(m_spasmTimes >=3)
    {
        //告知下位机停止训练
        stopTraining();
        //停止游戏
        stopGame();
    }
}

void TrainingManager::on_pause_Btn_clicked()
{
    pauseGame();
}

void TrainingManager::on_resume_Btn_clicked()
{
     resumeGame();
}

void TrainingManager::on_quit_Btn_clicked()
{
    stopGame();
}

void TrainingManager::on_report_Btn_clicked()
{
    //分数赋值
    m_st_TrainReport.score += 50;

    m_st_TrainReport.startTime = QDateTime::currentDateTime();
    m_st_TrainReport.endTime = m_st_TrainReport.startTime.addSecs(60);


    createTrainReport();
}

void TrainingManager::on_warning_Btn_clicked()
{
    m_emergencyWarningDialog->setWarningType(EMERGENCY_E);
    m_emergencyWarningDialog->show();
}

void TrainingManager::on_play_Btn_clicked()
{
    playBell();
}

//痉挛报警音控制
void TrainingManager::playBell()
{
    bells.play();
}
//停止报警音
void TrainingManager::stopPlayBell()
{
    if(bells.loopsRemaining())
        bells.stop();
}

//
void TrainingManager::on_resetHandle_Btn_clicked()
{
    int ret = m_bindTipDialog->exec();
    if(ret == 0)
    {
        qDebug()<<"ret up"<<ret;
        return;
    }
}
