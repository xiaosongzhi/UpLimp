#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QWidget>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QWidget
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();

private slots:
    void on_speedTest_Btn_clicked();

    void on_boostTest_Btn_clicked();

    void on_againstTest_Btn_clicked();

    void on_goBack_Btn_clicked();

    void on_switchLock_Btn_clicked();

    void on_stop_Btn_clicked();

    void on_startTest_Btn_clicked();

private:
    Ui::SettingDialog *ui;
};

#endif // SETTINGDIALOG_H
