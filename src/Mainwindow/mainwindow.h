#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cmaskdialog.h"

class QLabel;
class CCommunicationInterface;
class QButtonGroup;
class UserManager;
class WarningDialog;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void enterUserManagerPage();
protected:
    virtual void closeEvent(QCloseEvent *event);

private slots:

    void on_training_Btn_clicked();

    void on_scope_Btn_clicked();

    void on_teach_Btn_clicked();

    void on_history_Btn_clicked();

    void on_ranking_Btn_clicked();

    void on_setting_Btn_clicked();

    void on_shutDown_Btn_clicked();

    void on_quickStart_Btn_clicked();

    void slotCurrentStackWidgetChanged(int);

    void slotBtnGroupClicked(int);

    /****切换界面********
     * 参数：@int index  0-trainPage  1-scorpPage 2-teachPage
     *      3-historyPage 4-rankingPage 5-settingPage
     * 返回值：void
     * ************/
    void slotSwitchPage(int index);
    void on_switchUser_Btn_clicked();

    //显示急停效果槽
    void slotShowEmergency(bool);


    void slotHeartBeatChecking();

    //闪烁效果
    void slotConnectBlink();
public slots:
    void slotReceiveRecipe(bool);

    void slotCurrentUserChanged();

    void slotReceiveCommuAPIData(QByteArray);
private:

    void initButtonGroup();

    void initWidgetUI();

    void setCurrentUser();

//    void showEmergency(bool);
protected:
    void hideEvent(QHideEvent *event);
private:
    Ui::MainWindow *ui;
    CmaskDialog m_maskDialog;
    QButtonGroup *m_buttonGroup;
    UserManager *m_userManager;
    QLabel *m_connectStateLabel;
    QTimer *m_blinkTimer;
    QTimer *m_heartBeatTimer;

    int m_timeOutTimes; //超时次数
    WarningDialog *m_connectStateDialog;

};
#endif // MAINWINDOW_H
