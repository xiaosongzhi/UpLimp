#ifndef CAGAINSTTRAIN_H
#define CAGAINSTTRAIN_H
#include "dataFormate.h"
#include <QWidget>
#include "ctrainwidget.h"
namespace Ui {
class CAgainstTrain;
}

class CAgainstTrain : public QWidget//,public CTrainWidget
{
    Q_OBJECT

public:
    explicit CAgainstTrain(QWidget *parent = nullptr);
    ~CAgainstTrain();
    ST_AgainstParam getAgainstGameParam();

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
    Ui::CAgainstTrain *ui;
    QString m_currentText;
    bool isRecipeSelected;
};

#endif // CAGAINSTTRAIN_H
