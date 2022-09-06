#ifndef CPASSIVETRAIN_H
#define CPASSIVETRAIN_H

#include <QWidget>
//#include "ctrainwidget.h"
#include "dataFormate.h"
namespace Ui {
class CPassiveTrain;
}

class CPassiveTrain : public QWidget//,public CTrainWidget
{
    Q_OBJECT

public:
    explicit CPassiveTrain(QWidget *parent = nullptr);
    ~CPassiveTrain();

    ST_PassiveParam getPassiveGameParam();

    void setGameList(QList<ST_GameMsg>& gameList);

    void init();

    //是否选择自定义处方
    bool recipeIsSelected();
signals:
    void signalRecipeChecked(bool);

private slots:

    void slotGetCurrentText(QString);

    void on_recipe_RadioButton_toggled(bool checked);

private:
    void loadStyleSheetFile();

private:
    Ui::CPassiveTrain *ui;

    QString m_currentText;

    bool isRecipeSelected;
};

#endif // CPASSIVETRAIN_H
