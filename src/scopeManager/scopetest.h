#ifndef SCOPETEST_H
#define SCOPETEST_H

#include <QWidget>
#include "dataFormate.h"
#include "cbasedialog.h"
#include "scopereport.h"

class QPushButton;
class QTimer;
namespace Ui {
class SCopeTest;
}

class SCopeTest : public CBaseDialog
{
    Q_OBJECT

public:
    explicit SCopeTest(QWidget *parent = nullptr);
    ~SCopeTest();
    //显示当前手柄所在位置 x,y都为百分比
    void moveTipLabel(float x,float y);

protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    //接收下位机数据
    void slotReceiveCommuAPIData(QByteArray);

    void on_confirm_Btn_clicked();

    void on_start_Btn_clicked();

    void on_clear_Btn_clicked();

private:

    //获取两个点之间的距离
    float lengthOfPoints(QPoint start,QPoint end);

    void dealwithTrainData(const ST_DeviceParam& st_DeviceParam);

    //心跳开关
    void switchSendHeartBeat(bool isOpen);

    void loadStyleSheetFile();

    //初始化边界值
    void initLimitData();
    //设定边界值
    void setLimitData(QPoint);
    //解锁控制
    void sendStopCmd();
private:
    Ui::SCopeTest *ui;
    QList<QPoint> m_paintList;
    bool isTesting; //是否开始了测试
    ScopeReport *m_scorpReport;
    struct
    {
        int leftLimit;
        int rightLimit;
        int upLimit;
        int downLimit;
    }ST_LimitPoint;
};

#endif // SCOPETEST_H
