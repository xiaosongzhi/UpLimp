#ifndef TRAININGMANAGER_H
#define TRAININGMANAGER_H

/**********各种训练模式简单说明********
** 模式      热身         训练      放松
** 被动  *速度设为一档    正常档位  将速度降为1挡
** 助力  *不可用                     不可用
** 主动  *不可用                     不可用
** 抗阻  *将阻力设为一级  正常阻力  将阻力设为1级
**********************************/

#include <QWidget>
#include "dataFormate.h"
#include "userdataformate.h"
#include <QButtonGroup>
#include <QUdpSocket>
#include <QProcess>
#include "gameparamcontrol.h"
#include "warningdialog.h"
#include "waitingdialog.h"
#include "resethandledialog.h"
#include "bindtipdialog.h"
#include <QSound>
class SimulateGame;
class RecipeSelectDialog;
class CCommunicateAPI;
class ReportDialog;
class QTimer;
namespace Ui {
class TrainingManager;
}

class TrainingManager : public QWidget
{
    Q_OBJECT

public:
    explicit TrainingManager(QWidget *parent = nullptr);
    ~TrainingManager();

    void quickIntoTrain();

    //痉挛报警音控制
    void playBell();
    //停止报警音
    void stopPlayBell();

signals:
    void signalSendPoints(QPoint);

    void signalShowEmergency(bool);
private slots:

    void slotReceiveCommuAPIData(QByteArray);

    void on_passive_Btn_clicked();

    void on_active_Btn_clicked();

    void on_boost_Btn_clicked();

    void on_against_Btn_clicked();

    void on_startTrain_Btn_clicked();

    void slotBtnGroupClicked(int);

    //接收游戏数据
    void slotReceiveGameData();

    void on_goHome_Btn_clicked();

    void on_stop_Btn_clicked();

    //游戏管理
    void slotStateChanged(QProcess::ProcessState);

    void slotProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

    void on_pause_Btn_clicked();

    void on_resume_Btn_clicked();

    void on_quit_Btn_clicked();

    void on_report_Btn_clicked();

    void on_warning_Btn_clicked();

    void on_play_Btn_clicked();

    void on_resetHandle_Btn_clicked();

public slots:
    void slotRecipeShow(bool);

    //痉挛复位
    void slotSpasmRecover();


private:
    //加载样式表
    void loadStyleSheetFile();
    //设置急停效果
    void setEmergency();
    //痉挛报警
    void setSapsmWarning();
    //故障报警
    void setDeviceError(int errorType);
    //生成报告
    void createTrainReport();
    //处理下位机实时数据
    void dealwithTrainData(const ST_DeviceParam&);
    //计算总长度
    float calculateTotalLength(QList<QPoint>&);

    void initButtonGroup();

    void initGameSocket();

    void parseGameMsg(QByteArray jsonArray);

    //设置测试按钮的隐藏于显示
    void setTestBtnHide(bool hide);
    //从配置文件中读取游戏
    void uploadGameBtn();

    /**********发送数据给游戏************/
    //启动游戏程序
    void startGame(QString path);
    //给游戏发送实时坐标点
    void sendCurrentPointToGame(QPointF);
    //退出游戏指令
    void quitGameCmd();
    //退出游戏
    void stopGame();
    //暂停/继续游戏
    void pauseGame();
    void resumeGame();

    /**********发送数据给下位机************/
    //下位机复位指令
    void setGoHome();
    //将目标位置发送给下位机
    void sendControlData(ST_ControlData&);
    //停止训练命令
    void sendStopCmd();
    //心跳开关
    void switchSendHeartBeat(bool isOn);


    //强制关闭游戏进程
    void terminateGame(QString gameName);
    //获取总训练时长
    int getTotalTrainingTime(QMap<int,int>&);
    //根据阻力等级获取阻力大小
    int getResistanceByLevel(int level);
    //根据助力等级获取助力大小
    int getBoostForceByLevel(int level);
    //根据速度等级获得实际速度
    int getSpeedByLevel(int level);
    //设置提示信息
    void setTipMessage(E_TRAINMODE);
    //停止训练
    void stopTraining();
    //解析游戏传输的坐标点数据
    ST_ControlData parseGamePoint(QJsonObject& object);

   /*******训练记录相关*********/
    //记录当前用户训练参数(保存到数据库中)
    void recordCurrentUserTrainParam(ST_TrainRecord);
    //获取当前用户上一次训练参数
    bool getCurrentUserTrainParam(uint32_t,ST_TrainRecord&);
    //训练过程的转换
    QString transProcess(QMap<int,int>);
    QMap<int,int> transProcess(QString process);
private:
    Ui::TrainingManager *ui;

    RecipeSelectDialog *m_recipeSelected;
    ReportDialog *m_reportDialog;
    QList<QPoint> m_paintList;          //轨迹记录链表
    bool isTraining;
    int m_quitType;                     //游戏退出类型 1正常退出 -1异常退出
    int m_heartBeatTimes;               //心跳计数
    E_TRAINMODE m_tarinMode;            //训练模式
    int m_spasmTimes;                   //痉挛次数
    ST_TrainReport m_st_TrainReport;
    QButtonGroup *m_buttonGroup;
    QUdpSocket *m_gameSocket;
    ST_ControlData m_currentControlData;//对下位机的实时控制参数
    QProcess *m_Process;
    QString m_currentGamePath;
    WarningDialog *m_emergencyWarningDialog;//急停报警框
    WarningDialog *m_spasmWarningDialog;    //痉挛报警框
    WaitingDialog *m_waitingDialog;         //等待提示框
    WarningDialog *m_clampDialog;           //夹手报警框
    ResetHandleDialog *m_resetHadleDialog;  //设备返回原点提示
    BindTipDialog *m_bindTipDialog;         //绑带提示按钮
    bool m_isSpasming;                  //是否处于痉挛状态
    QMap<int,int> m_trainProcessMap;    //训练流程QMap<类型,时间> 类型0-热身 1-训练 2-放松
    int m_passiveSpeed;                 //被动训练速度
    int m_againstLevel;                 //抗阻等级
    QSound bells;                       //铃声对象
};

#endif // TRAININGMANAGER_H
