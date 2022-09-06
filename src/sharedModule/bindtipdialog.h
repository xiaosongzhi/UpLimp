#ifndef BINDTIPDIALOG_H
#define BINDTIPDIALOG_H

#include <QDialog>
class QTimer;
namespace Ui {
class BindTipDialog;
}

class BindTipDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BindTipDialog(QWidget *parent = nullptr);
    ~BindTipDialog();
signals:
    void signalCloseCode(int);

private slots:
    void slotcloseTimer();
protected:
    void paintEvent(QPaintEvent *event);

    void showEvent(QShowEvent *event);

    void closeEvent(QCloseEvent *event);
private slots:

    void on_confirm_Btn_clicked();

private:
    Ui::BindTipDialog *ui;
    QTimer *m_timer;
    int Countdown;  //倒计时数
};

#endif // BINDTIPDIALOG_H
