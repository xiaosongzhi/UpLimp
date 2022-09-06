#ifndef CACTIVETRAIN_H
#define CACTIVETRAIN_H

#include <QWidget>
#include "ctrainwidget.h"
#include "dataFormate.h"
namespace Ui {
class CActiveTrain;
}

class CActiveTrain : public QWidget//,public CTrainWidget
{
    Q_OBJECT

public:
    explicit CActiveTrain(QWidget *parent = nullptr);
    ~CActiveTrain();
    //载入游戏图标
    void setGameList(QList<ST_GameMsg>&);

    ST_ActiveParam getActiveGameParam();

    //是否选择自定义处方
    bool recipeIsSelected();
private slots:
    void slotGetCurrentText(QString);

private:
    void loadStyleSheetFile();
    void init();
signals:
    void signalRecipeChecked(bool);
private:
    Ui::CActiveTrain *ui;

    QString m_currentText;

    bool isRecipeSelected;
};

#endif // CACTIVETRAIN_H
