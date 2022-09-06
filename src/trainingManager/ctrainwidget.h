#ifndef CTRAINWIDGET_H
#define CTRAINWIDGET_H

#include <QObject>

class CTrainWidget : public QObject
{
    Q_OBJECT
public:
    explicit CTrainWidget(QObject *parent = nullptr);

signals:
    void signalRecipeChecked(bool);
};

#endif // CTRAINWIDGET_H
