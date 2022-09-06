#include "userdataformate.h"
#include <QVariantMap>
#include <QUuid>

extern ST_PatientMsg variantMapToPatientMsg(QVariantMap vMap)
{
    ST_PatientMsg st_PatientMsg;
    if(vMap.contains("ID"))
        st_PatientMsg.ID = vMap.value("ID").toUInt();
    if(vMap.contains("name"))
        st_PatientMsg.name = vMap.value("name").toString();
    if(vMap.contains("age"))
        st_PatientMsg.age = vMap.value("age").toUInt();
    if(vMap.contains("sex"))
        st_PatientMsg.sex = vMap.value("sex").toUInt();
    if(vMap.contains("height"))
        st_PatientMsg.height = vMap.value("height").toUInt();
    if(vMap.contains("weight"))
        st_PatientMsg.weight = vMap.value("weight").toUInt();
    if(vMap.contains("phone"))
        st_PatientMsg.phone = vMap.value("phone").toString();
    if(vMap.contains("message"))
        st_PatientMsg.message = vMap.value("message").toString();
    if(vMap.contains("guardian"))
        st_PatientMsg.guardian = vMap.value("guardian").toString();
    if(vMap.contains("guardianPhone"))
        st_PatientMsg.guardianPhone = vMap.value("guardianPhone").toString();
    if(vMap.contains("address"))
        st_PatientMsg.address = vMap.value("address").toString();
    if(vMap.contains("remarkMessage"))
        st_PatientMsg.remarkMessage = vMap.value("remarkMessage").toString();
    if(vMap.contains("certificateType"))
        st_PatientMsg.certificateType = vMap.value("certificateType").toInt();
    if(vMap.contains("certificateNum"))
        st_PatientMsg.certificateNum = vMap.value("certificateNum").toString();
    return st_PatientMsg;
}

extern ST_TrackMsg variantMapToTrackMsg(QVariantMap vMap)
{
    ST_TrackMsg st_TrackMsg;
    if(vMap.contains("name"))
        st_TrackMsg.author = vMap.value("author").toString();
    if(vMap.contains("trackName"))
        st_TrackMsg.trackName = vMap.value("trackName").toString();
    if(vMap.contains("createTime"))
        st_TrackMsg.createTime = vMap.value("createTime").toDateTime();
    if(vMap.contains("trackImagePath"))
        st_TrackMsg.trackImagePath = vMap.value("trackImagePath").toString();
    if(vMap.contains("trackDataFile"))
        st_TrackMsg.trackDataFile = vMap.value("trackDataFile").toString();
    if(vMap.contains("gameTrackDataFile"))
        st_TrackMsg.trackDataFile = vMap.value("gameTrackDataFile").toString();
    return st_TrackMsg;
}

extern ST_TrainReport variantMapToTrainReport(QVariantMap vMap)
{
    ST_TrainReport st_TrainReport;
    if(vMap.contains("ID"))
        st_TrainReport.ID = vMap.value("ID").toInt();
    if(vMap.contains("certificateNum"))
        st_TrainReport.certificateNum = vMap.value("certificateNum").toString();
    if(vMap.contains("name"))
        st_TrainReport.name = vMap.value("name").toString();
    if(vMap.contains("level"))
        st_TrainReport.level = vMap.value("level").toInt();
    if(vMap.contains("totalTime"))
        st_TrainReport.totalTime = vMap.value("totalTime").toInt();
    if(vMap.contains("totalScore"))
        st_TrainReport.totalScore = vMap.value("totalScore").toInt();
    if(vMap.contains("totalLength"))
        st_TrainReport.totalLength = vMap.value("totalLength").toDouble();
    if(vMap.contains("trainLength"))
        st_TrainReport.trainLength = vMap.value("trainLength").toDouble();
    if(vMap.contains("startTime"))
         st_TrainReport.startTime = vMap.value("startTime").toDateTime();
    if(vMap.contains("endTime"))
        st_TrainReport.endTime = vMap.value("endTime").toDateTime();
    if(vMap.contains("timeString"))
        st_TrainReport.timeString = vMap.value("timeString").toString();
    if(vMap.contains("score"))
        st_TrainReport.score = vMap.value("score").toInt();
    if(vMap.contains("trainMode"))
        st_TrainReport.trainMode = (E_TRAINMODE)vMap.value("trainMode").toInt();
    if(vMap.contains("trainDuration"))
        st_TrainReport.trainDuration = vMap.value("trainDuration").toInt();
    if(vMap.contains("energy"))
        st_TrainReport.energy = vMap.value("energy").toFloat();
    if(vMap.contains("spasmTimes"))
        st_TrainReport.spasmTimes = vMap.value("spasmTimes").toInt();
    if(vMap.contains("trainSpeed"))
        st_TrainReport.trainSpeed = vMap.value("trainSpeed").toFloat();
    if(vMap.contains("trainScop"))
        st_TrainReport.trainScorp = vMap.value("trainScop").toInt();
    if(vMap.contains("trainTrack"))
        st_TrainReport.trainTrack = vMap.value("trainTrack").toString();
    if(vMap.contains("trainTrackImage"))
        st_TrainReport.trainTrackImage = vMap.value("trainTrackImage").toString();
    if(vMap.contains("markMsg"))
        st_TrainReport.markMsg = vMap.value("markMsg").toString();
    if(vMap.contains("activePercent"))
        st_TrainReport.activePercent = vMap.value("activePercent").toFloat();
    return st_TrainReport;
}

extern ST_TrainRanking variantMapToTrainRanking(QVariantMap vMap)
{
    ST_TrainRanking st_TrainRanking;
    if(vMap.contains("ID"))
        st_TrainRanking.ID = vMap.value("ID").toInt();
    if(vMap.contains("ranking"))
        st_TrainRanking.ranking = vMap.value("ranking").toInt();
    if(vMap.contains("name"))
        st_TrainRanking.name = vMap.value("name").toString();
    if(vMap.contains("level"))
        st_TrainRanking.level = vMap.value("level").toInt();
    if(vMap.contains("totalTime"))
        st_TrainRanking.totalTime = vMap.value("totalTime").toInt();
    if(vMap.contains("totalScore"))
        st_TrainRanking.totalScore = vMap.value("totalScore").toInt();
    return st_TrainRanking;
}

extern ST_TrainRecord variantMapToTrainRecord(QVariantMap vMap)
{
    ST_TrainRecord st_TrainRecord;
    if(vMap.contains("ID"))
        st_TrainRecord.ID = vMap.value("ID").toInt();
    if(vMap.contains("trainMode"))
        st_TrainRecord.trainMode = (E_TRAINMODE)(vMap.value("trainMode").toInt());
    if(vMap.contains("gamePath"))
        st_TrainRecord.gamePath = vMap.value("gamePath").toString();
    if(vMap.contains("trackPath"))
        st_TrainRecord.trackPath = vMap.value("trackPath").toString();
    if(vMap.contains("trainScorp"))
        st_TrainRecord.trainScope = vMap.value("trainScope").toInt();
    if(vMap.contains("speed"))
        st_TrainRecord.speed = vMap.value("speed").toInt();
    if(vMap.contains("blockLength"))
        st_TrainRecord.blockLength = vMap.value("blockLength").toInt();
    if(vMap.contains("level"))
        st_TrainRecord.level = vMap.value("level").toInt();
    if(vMap.contains("process"))
        st_TrainRecord.process = vMap.value("process").toString();
    if(vMap.contains("gameTime"))
        st_TrainRecord.gameTime = vMap.value("gameTime").toInt();

    return st_TrainRecord;
}

extern ST_ScopeTest variantMapToScopeTest(QVariantMap vMap)
{
    ST_ScopeTest st_ScopeTest;
    if(vMap.contains("ID"))
        st_ScopeTest.ID = vMap.value("ID").toInt();
    if(vMap.contains("xLength"))
        st_ScopeTest.xLength = vMap.value("xLength").toInt();
    if(vMap.contains("yLength"))
        st_ScopeTest.yLength = vMap.value("yLength").toInt();
    if(vMap.contains("area"))
        st_ScopeTest.area = vMap.value("area").toFloat();
    if(vMap.contains("testTime"))
        st_ScopeTest.testTime = vMap.value("testTime").toDateTime();
    return st_ScopeTest;
}

extern QVariantMap patientMsgToVariantMap(ST_PatientMsg st_PatientMsg)
{
    QVariantMap vMap;
    vMap.insert("ID",st_PatientMsg.ID);
    vMap.insert("name",st_PatientMsg.name);
    vMap.insert("age",st_PatientMsg.age);
    vMap.insert("sex",st_PatientMsg.sex);
    vMap.insert("height",st_PatientMsg.height);
    vMap.insert("weight",st_PatientMsg.weight);
    vMap.insert("phone",st_PatientMsg.phone);
    vMap.insert("message",st_PatientMsg.message);
    vMap.insert("guardian",st_PatientMsg.guardian);
    vMap.insert("guardianPhone",st_PatientMsg.guardianPhone);
    vMap.insert("address",st_PatientMsg.address);
    vMap.insert("remarkMessage",st_PatientMsg.remarkMessage);
    vMap.insert("certificateType",st_PatientMsg.certificateType);
    vMap.insert("certificateNum",st_PatientMsg.certificateNum);
    return vMap;
}
extern QVariantMap trackMsgToVariantMap(ST_TrackMsg st_TrackMsg)
{
    QVariantMap vMap;
    vMap.insert("author",st_TrackMsg.author);
    vMap.insert("trackName",st_TrackMsg.trackName);
    vMap.insert("createTime",st_TrackMsg.createTime);
    vMap.insert("trackDataFile",st_TrackMsg.trackDataFile);
    vMap.insert("trackImagePath",st_TrackMsg.trackImagePath);
    vMap.insert("gameTrackDataFile",st_TrackMsg.gameTrackDataFile);
    return vMap;
}

extern QVariantMap trainReportToVariantMap(ST_TrainReport st_TrainReport)
{
    QVariantMap vMap;
    //插入UUID
    QString uuid = QUuid::createUuid().toString();
    uuid.remove(0,1);
    uuid.remove(uuid.length()-1,1);
    vMap.insert("UUID",uuid);
    vMap.insert("ID",st_TrainReport.ID);
    vMap.insert("certificateNum",st_TrainReport.certificateNum);
    vMap.insert("name",st_TrainReport.name);
    vMap.insert("level",st_TrainReport.level);
    vMap.insert("totalTime",st_TrainReport.totalTime);
    vMap.insert("totalScore",st_TrainReport.totalScore);
    vMap.insert("totalLength",st_TrainReport.totalLength);
    vMap.insert("trainLength",st_TrainReport.trainLength);
    vMap.insert("startTime",st_TrainReport.startTime);
    vMap.insert("endTime",st_TrainReport.endTime);
    vMap.insert("timeString",st_TrainReport.startTime.toString("yyyyMMdd-hh:mm:ss"));
    vMap.insert("score",st_TrainReport.score);
    vMap.insert("trainMode",st_TrainReport.trainMode);
    vMap.insert("trainDuration",st_TrainReport.trainDuration);
    vMap.insert("energy",st_TrainReport.energy);
    vMap.insert("spasmTimes",st_TrainReport.spasmTimes);
    vMap.insert("trainSpeed",st_TrainReport.trainSpeed);
    vMap.insert("trainScop",st_TrainReport.trainScorp);
    vMap.insert("trainTrack",st_TrainReport.trainTrack);
    vMap.insert("trainTrackImage",st_TrainReport.trainTrackImage);
    vMap.insert("markMsg",st_TrainReport.markMsg);
    vMap.insert("activePercent",st_TrainReport.activePercent);
    return vMap;
}

extern QVariantMap trainRankingToVariantMap(ST_TrainRanking st_TrainRanking)
{
    QVariantMap vMap;
    vMap.insert("ID",st_TrainRanking.ID);
    vMap.insert("ranking",st_TrainRanking.ranking);
    vMap.insert("name",st_TrainRanking.name);
    vMap.insert("level",st_TrainRanking.level);
    vMap.insert("totalTime",st_TrainRanking.totalTime);
    vMap.insert("totalScore",st_TrainRanking.totalScore);

    return vMap;
}

extern QVariantMap trainRecordToVariantMap(ST_TrainRecord st_TrainRecord)
{
    QVariantMap vMap;
    vMap.insert("ID",st_TrainRecord.ID);
    vMap.insert("trainMode",st_TrainRecord.trainMode);
    vMap.insert("gamePath",st_TrainRecord.gamePath);
    vMap.insert("trackPath",st_TrainRecord.trackPath);
    vMap.insert("trainScope",st_TrainRecord.trainScope);
    vMap.insert("speed",st_TrainRecord.speed);
    vMap.insert("blockLength",st_TrainRecord.blockLength);
    vMap.insert("level",st_TrainRecord.level);
    vMap.insert("process",st_TrainRecord.process);
    vMap.insert("gameTime",st_TrainRecord.gameTime);

    return vMap;
}

extern QVariantMap ScopeTestToVariantMap(ST_ScopeTest st_ScopeTest)
{
    QVariantMap vMap;
    //插入UUID
    QString uuid = QUuid::createUuid().toString();
    uuid.remove(0,1);
    uuid.remove(uuid.length()-1,1);
    vMap.insert("UUID",uuid);
    vMap.insert("ID",st_ScopeTest.ID);
    vMap.insert("xLength",st_ScopeTest.xLength);
    vMap.insert("yLength",st_ScopeTest.yLength);
    vMap.insert("area",st_ScopeTest.area);
    vMap.insert("testTime",st_ScopeTest.testTime);
    return vMap;
}
