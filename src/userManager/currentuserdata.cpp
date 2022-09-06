#include "currentuserdata.h"
#include "cdatabaseinterface.h"
#include <QDebug>
#include "logger.h"
#include <QMessageBox>

LOG4QT_DECLARE_STATIC_LOGGER(logger, CurrentUserData)

CurrentUserData* CurrentUserData::m_currentUserData = NULL;


CurrentUserData::CurrentUserData(QObject *parent) : QObject(parent)
{

}

CurrentUserData* CurrentUserData::getInstace()
{
    if(!m_currentUserData)
    {
        m_currentUserData = new CurrentUserData();
    }
    return m_currentUserData;
}

//设置当前用户ID,在选择用户的时候调用
void CurrentUserData::setCurrentUserID(uint32_t ID)
{
    m_CurrentUserID = ID;
    updateCurrentPatientMsg();
    emit signalUserChanged();
}

uint32_t CurrentUserData::getCurrentUserID()
{
    updateCurrentPatientMsg();
    return st_CurrentUserData.ID;
}

//获取患者信息
ST_PatientMsg CurrentUserData::getCurrentPatientMsg()
{
    return st_CurrentUserData;
}

//获取当前患者当前时间的训练报告
ST_TrainReport CurrentUserData::getCurrentTrainReport()
{
    updateCurrentTrainReport();
    return st_CurrentTrainReport;
}

void CurrentUserData::updateCurrentTrainReport()
{

}

void CurrentUserData::updateCurrentPatientMsg()
{
    //从数据库中查询数据
    QString queryStr= QString("select * from patientmessagetable where ID = '%1'").arg(QString::number(m_CurrentUserID));

    if(CDatabaseInterface::getInstance()->exec(queryStr))
    {
        if(CDatabaseInterface::getInstance()->getValuesSize() > 0)
        {
            QList<QVariantMap> valueMapList;
            valueMapList = CDatabaseInterface::getInstance()->getValues(0,1);
            st_CurrentUserData = variantMapToPatientMsg(valueMapList.at(0));
        }
        else
        {
            QMessageBox::warning(NULL,"提示","未查询到数据");
        }
    }
    else
    {
        qDebug()<<"update CurrentPatientMsg failed";
    }
}

void CurrentUserData::setCurrentTrainReport(const ST_TrainReport&st_TrainReport)
{
    st_CurrentTrainReport = st_TrainReport;
}
