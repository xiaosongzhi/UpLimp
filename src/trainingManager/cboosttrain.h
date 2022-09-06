#ifndef CBOOSTTRAIN_H
#define CBOOSTTRAIN_H

#include <QWidget>
//#include "ctrainwidget.h"
#include "dataFormate.h"

namespace Ui {
class CBoostTrain;
}

class CBoostTrain : public QWidget//,public CTrainWidget
{
    Q_OBJECT

public:
    explicit CBoostTrain(QWidget *parent = nullptr);
    ~CBoostTrain();
    ST_BoostParam getBoostGameParam();

    void setGameList(QList<ST_GameMsg>& gameList);

    void init();

    //是否选择自定义处方
    bool recipeIsSelected();
signals:
    void signalRecipeChecked(bool);
private slots:
    void slotGetCurrentText(QString);

private:
    void loadStyleSheetFile();

private:
    Ui::CBoostTrain *ui;

    QString m_currentText;

    bool isRecipeSelected;
};

#endif // CBOOSTTRAIN_H
