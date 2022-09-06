#ifndef WARNINGDIALOG_H
#define WARNINGDIALOG_H

#include <QWidget>
#include "dataFormate.h"
#include "cbasedialog.h"

class QTimer;

namespace Ui {
class WarningDialog;
}

class WarningDialog : public QWidget
{
    Q_OBJECT

public:
    explicit WarningDialog(QWidget *parent = nullptr);
    ~WarningDialog();

    void setWarningType(E_WarningType,int spasmNum = 0);
    //设置倒计时关闭按钮
    void setCloseTime(int second);

    void setDialogOpenState(bool);

signals:
    //主要痉挛使用
    void confirmSignal();

private:
    void setImageAndText(QString path,QString text);

    //急停警告
    void showEmergencyDialog();
    //痉挛警告
    void showSpasmDialog(int spasmNum);
    //夹手警告
    void showClampDialog();
    //绑带绑定提示
    void showGameTipsDialog();
    //设备断连提示
    void showDeviceDisconnect();
    //复位手柄
    void showResetHandleDialog();

    void loadStyleSheetFile();
private slots:
    void slotCloseEvent();
    void on_confirm_Btn_clicked();

    void on_close_Btn_clicked();

protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    Ui::WarningDialog *ui;
    QTimer *m_closeTimer;
    E_WarningType m_warnType;
};

#endif // WARNINGDIALOG_H
