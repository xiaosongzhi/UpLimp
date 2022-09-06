#ifndef TITLEFORM_H
#define TITLEFORM_H

#include <QWidget>

namespace Ui {
class TitleForm;
}

enum E_BUTTONEVENT
{
    E_MINEVENT = 0,
    E_MAXEVENT,
    E_HELPEVENT,
    E_QUITEVENT
};


class TitleForm : public QWidget
{
    Q_OBJECT

public:
    explicit TitleForm(QWidget *parent = nullptr);
    ~TitleForm();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseDoubleClickEvent(QMouseEvent *event);

signals:
    void signalWindowMove(QPoint destPoint);
    void signalSwitchWindowSize();
    void signalButtonEvent(E_BUTTONEVENT);
private slots:
    void on_min_Btn_clicked();

    void on_max_Btn_clicked();

    void on_close_Btn_clicked();

    void on_help_Btn_clicked();

private:
    Ui::TitleForm *ui;
    bool m_isPressed;
    QPoint m_pressdPoint;
};

#endif // TITLEFORM_H
