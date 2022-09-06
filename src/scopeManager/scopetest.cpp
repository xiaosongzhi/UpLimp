#include "scopetest.h"
#include "ui_scopetest.h"
#include <QDebug>
#include <QTimer>
#include <QPoint>
#include <QtMath>
#include <QRandomGenerator>
#include "ccommunicateapi.h"
#include "controlmaskdialog.h"
#include <QFile>
#include "logger.h"
#include  "cdatabaseinterface.h"
#include <QDateTime>
#include "userdataformate.h"
#include "currentuserdata.h"
LOG4QT_DECLARE_STATIC_LOGGER(logger, SCopeTest)
SCopeTest::SCopeTest(QWidget *parent) :
    CBaseDialog(parent),
    ui(new Ui::SCopeTest),
    isTesting(false),
    m_scorpReport(NULL)
{
    ui->setupUi(this);

    m_scorpReport = new ScopeReport();

    loadStyleSheetFile();
    initLimitData();
}

SCopeTest::~SCopeTest()
{
    delete ui;
}

void SCopeTest::loadStyleSheetFile()
{
    //加载样式文件s
    QFile qssFile("./QSS/scorptest.qss");
    if(!qssFile.open(QIODevice::ReadOnly))
        logger()->debug("scorptest.qss文件打开失败!");
    else
    {
        this->setStyleSheet(qssFile.readAll());
        qssFile.close();
    }
}

void SCopeTest::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    ControlMaskDialog::getInstace()->setMaskDialogHide(false);
    CCommunicateAPI *m_commuAPI = CCommunicateAPI::getInstance();
    connect(m_commuAPI,SIGNAL(signalReadyRead(QByteArray)),this,SLOT(slotReceiveCommuAPIData(QByteArray)));
}

void SCopeTest::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    ControlMaskDialog::getInstace()->setMaskDialogHide(true);
    CCommunicateAPI *m_commuAPI = CCommunicateAPI::getInstance();
    disconnect(m_commuAPI,SIGNAL(signalReadyRead(QByteArray)),this,SLOT(slotReceiveCommuAPIData(QByteArray)));
}

void SCopeTest::slotReceiveCommuAPIData(QByteArray byteArray)
{
    int length = byteArray[1];
    QByteArray data = byteArray.mid(5,length);
    switch(byteArray[4])
    {
    case HEARTBEAT:  //心跳
    {
        ST_DeviceParam st_DeviceParam;
        memcpy(&st_DeviceParam,data.data(),sizeof(ST_DeviceParam));
        dealwithTrainData(st_DeviceParam);
    }
    }
}

void SCopeTest::dealwithTrainData(const ST_DeviceParam& st_DeviceParam)
{
    static QPoint lastPoint(0,0);
    //正在训练中--记录其运动轨迹,完成后需要将运动轨迹绘制在报告中
    if(isTesting)
    {
        QPoint point(st_DeviceParam.st_currentPoint.x,st_DeviceParam.st_currentPoint.y);
        if(lastPoint != point)
        {
            setLimitData(point);
            m_paintList.append(point);
            ui->paint_Widget->addPoint(point);
        }
        lastPoint.setX(st_DeviceParam.st_currentPoint.x);
        lastPoint.setY(st_DeviceParam.st_currentPoint.y);
    }
}

void SCopeTest::switchSendHeartBeat(bool isOpen)
{
    QByteArray sendArray(1,0);
    if(isOpen)
        sendArray[0] = 1;
    else
        sendArray[0] = 0;
    CCommunicateAPI::getInstance()->sendData(0,2,HEARTBEAT,sendArray);
}

void SCopeTest::sendStopCmd()
{
    QByteArray sendArray(0);
    CCommunicateAPI::getInstance()->sendData(0,2,STOPTRAINING,sendArray);
}

//获取两个点之间的距离
float SCopeTest::lengthOfPoints(QPoint start,QPoint end)
{
    int x1 = start.x();
    int y1 = start.y();
    int x2 = end.x();
    int y2 = end.y();
    float length =qSqrt(qPow((x2-x1),2) + qPow((y2-y1),2));
    return length;
}

void SCopeTest::on_confirm_Btn_clicked()
{
    this->close();
    ST_ScopeTest st_ScopeTest;
    st_ScopeTest.xLength = ST_LimitPoint.rightLimit - ST_LimitPoint.leftLimit;
    st_ScopeTest.yLength = ST_LimitPoint.upLimit - ST_LimitPoint.downLimit;
    st_ScopeTest.ID =CurrentUserData::getInstace()->getCurrentUserID();
    st_ScopeTest.testTime = QDateTime::currentDateTime();
    st_ScopeTest.area = st_ScopeTest.xLength * st_ScopeTest.yLength;
    //将该数据保存至数据库中
    QString table("scopetesttable");
    if(!CDatabaseInterface::getInstance()->insertRowTable(table,ScopeTestToVariantMap(st_ScopeTest)))
    {
        qDebug()<<CDatabaseInterface::getInstance()->getLastError();
        qDebug()<<"范围测试数据插入失败,请重试";
    }
    else
    {
        QList<QPoint> pointList = ui->paint_Widget->getPaintList();
        m_scorpReport->show();
        m_scorpReport->paintScopeTrack(pointList);
    }
    ui->paint_Widget->clearPath();
}

void SCopeTest::on_start_Btn_clicked()
{
    if(ui->start_Btn->text() == "开始")
    {
        sendStopCmd();
        isTesting = true;
        //将申请式改为开关式
        switchSendHeartBeat(true);
        ui->start_Btn->setText("完成");
        initLimitData();
    }
    else if(ui->start_Btn->text() == "完成")
    {
        isTesting = false;
        //将申请式改为开关式
        switchSendHeartBeat(false);
        ui->start_Btn->setText("开始");
    }
}

void SCopeTest::on_clear_Btn_clicked()
{
//    isTesting = false;
//    ui->start_Btn->setText("开始");
    on_start_Btn_clicked();
    ui->paint_Widget->clearPath();
}

//初始化边界值
void SCopeTest::initLimitData()
{
    ST_LimitPoint.leftLimit = DEVICEWIDTH;
    ST_LimitPoint.rightLimit = 0;
    ST_LimitPoint.downLimit = DEVICEHEIGHT;
    ST_LimitPoint.upLimit = 0;
}

//设定边界值
void SCopeTest::setLimitData(QPoint point)
{
    if(ST_LimitPoint.leftLimit > point.x())
        ST_LimitPoint.leftLimit = point.x();
    if(ST_LimitPoint.rightLimit < point.x())
        ST_LimitPoint.rightLimit = point.x();
    if(ST_LimitPoint.downLimit > point.y())
        ST_LimitPoint.downLimit = point.y();
    if(ST_LimitPoint.upLimit < point.y())
        ST_LimitPoint.upLimit = point.y();
}
