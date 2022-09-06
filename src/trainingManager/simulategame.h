#ifndef SIMULATEGAME_H
#define SIMULATEGAME_H

#include <QWidget>

namespace Ui {
class SimulateGame;
}

class SimulateGame : public QWidget
{
    Q_OBJECT

public:
    explicit SimulateGame(QWidget *parent = nullptr);
    ~SimulateGame();

    void setPoint(QPoint);
private slots:
    void on_start_Btn_clicked();

    void on_stop_Btn_clicked();

    void on_clear_Btn_clicked();

private:
    Ui::SimulateGame *ui;
    bool isStartPaint;
};

#endif // SIMULATEGAME_H
