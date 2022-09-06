#ifndef CURRENTUSERDATA_H
#define CURRENTUSERDATA_H

#include <QObject>
#include "userdataformate.h"
class CurrentUserData : public QObject
{
    Q_OBJECT
public:
    static CurrentUserData* getInstace();

    //设置当前用户ID,在选择用户的时候调用
    void setCurrentUserID(uint32_t ID);

    //获取患者信息
    ST_PatientMsg getCurrentPatientMsg();

    //获取当前患者当前时间的训练报告
    ST_TrainReport getCurrentTrainReport();
    void setCurrentTrainReport(const ST_TrainReport&);

    //获取当前用户的ID
    uint32_t getCurrentUserID();
signals:
    void signalUserChanged();
private:
    void updateCurrentPatientMsg();
    void updateCurrentTrainReport();

private:
    explicit CurrentUserData(QObject *parent = nullptr);

    static CurrentUserData* m_currentUserData;
    //当前用户ID
    uint32_t m_CurrentUserID;
    //当前用户certificateNum
    QString certificateNum;
    //当前用户信息
    ST_PatientMsg st_CurrentUserData;
    //当前选择的训练处方
    ST_TrackMsg st_CurrentTrackMsg;
    //当前用户当前时间的训练报告
    ST_TrainReport st_CurrentTrainReport;
};

#endif // CURRENTUSERDATA_H
