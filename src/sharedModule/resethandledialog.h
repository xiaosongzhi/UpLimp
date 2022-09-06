#ifndef RESETHANDLEDIALOG_H
#define RESETHANDLEDIALOG_H

#include <QDialog>

namespace Ui {
class ResetHandleDialog;
}

class ResetHandleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResetHandleDialog(QWidget *parent = nullptr);
    ~ResetHandleDialog();

signals:
    void signalCloseCode(int);

protected:

    void paintEvent(QPaintEvent *event);
private slots:
    void on_confirm_Btn_clicked();

    void on_close_Btn_clicked();

private:
    Ui::ResetHandleDialog *ui;
};

#endif // RESETHANDLEDIALOG_H
