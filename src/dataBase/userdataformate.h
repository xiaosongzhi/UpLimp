#ifndef USERDATAFORMATE_H
#define USERDATAFORMATE_H

#include <QWidget>
#include <QDateTime>
#include "dataFormate.h"
#define PERCOUNT 8  //每页放8个处方
typedef struct PatientMsg
{
    uint32_t ID;            //自增性ID，由系统分配控制
    QString name;           //患者姓名 必填
    int age;                //患者年龄
    int sex;                //0-男性 1-女性
    int height;             //患者身高
    int weight;             //患者体重
    QString phone;          //患者电话
    QString message;        //患者康复信息
    QString guardian;       //患者监护人姓名
    QString guardianPhone;  //监护人电话
    QString address;        //家庭住址
    int certificateType;    //0-身份证 1-护照  3-军官证
    QString certificateNum; //证件号码
    QString remarkMessage;  //备注信息

}ST_PatientMsg;

typedef struct TrackMsg
{
    QString author;             //创建人
    QString trackName;          //处方名
    QDateTime createTime;       //创建时间
    QString trackImagePath;     //轨迹截图名（本地路径）
    QString trackDataFile;      //轨迹文件(场景坐标)
    QString gameTrackDataFile;  //轨迹文件(游戏坐标)
}ST_TrackMsg;

typedef struct TrainReport
{
    QString UUID;           //唯一主键
    int ID;                 //系统ID
    QString  certificateNum;//身份证号
    QString name;           //患者姓名
    int level;              //当前等级
    int totalTime;          //训练总时长 单位min 在排行中使用
    int totalScore;         //累计总得分 在排行中使用
    double trainLength;     //本次长度
    double totalLength;     //运动总长度
    QDateTime startTime;    //开始时间
    QDateTime endTime;      //结束时间
    QString timeString;     //起始时间戳，用于数据库索引
    int score;              //得分
    E_TRAINMODE trainMode;  //训练模式 0-被动 1-主动 2-助力 3-抗阻
    int trainDuration;      //本次训练时长 单位 秒
    float energy;           //能量消耗  该怎样计算？
    int spasmTimes;         //痉挛次数
    double trainSpeed;      //平均速度
    int trainScorp;         //运动范围 1-小 2-中 3-大
    QString trainTrack;     //运动轨迹路径
    QString trainTrackImage;//轨迹截图
    QString markMsg;        //备注
    float activePercent;    //主动占比

}ST_TrainReport;

typedef struct TrainRanking
{
    int ID;             //系统ID
    int ranking;        //排名
    QString name;       //患者姓名
    int level;          //等级
    int totalTime;      //训练总时长
    int totalScore;     //总得分
}ST_TrainRanking;

//训练记录，用于快速启动使用，存放于数据库中，每个用户只存储一次
typedef struct TrainRecord
{
    int ID;                 //系统ID
    E_TRAINMODE trainMode;  //训练模式
    QString gamePath;       //游戏路径
    QString trackPath;      //运动轨迹
    int trainScope;         //运动范围 1-小 2-中 3-大
    int speed;              //训练速度--被动训练
    int blockLength;        //障碍物长度--主动训练中（代替是否读取轨迹文件0-不读  1-读取）
    int level;              //助力或者阻力等级
    QString process;        //训练流程
    int gameTime;           //训练时间
}ST_TrainRecord;

//范围测试
typedef struct ScopeTest
{
    QString UUID;       //唯一主键
    int ID;             //系统ID
    int xLength;        //x轴向长度
    int yLength;        //y轴向长度
    float area;         //绘制面积
    QDateTime testTime; //测试时间
}ST_ScopeTest;

extern ST_PatientMsg variantMapToPatientMsg(QVariantMap vMap);
extern ST_TrackMsg variantMapToTrackMsg(QVariantMap vMap);
extern ST_TrainReport variantMapToTrainReport(QVariantMap vMap);
extern ST_TrainRanking variantMapToTrainRanking(QVariantMap vMap);
extern ST_TrainRecord variantMapToTrainRecord(QVariantMap vMap);
extern ST_ScopeTest variantMapToScopeTest(QVariantMap vMap);

extern QVariantMap patientMsgToVariantMap(ST_PatientMsg);
extern QVariantMap trackMsgToVariantMap(ST_TrackMsg);
extern QVariantMap trainReportToVariantMap(ST_TrainReport);
extern QVariantMap trainRankingToVariantMap(ST_TrainRanking);
extern QVariantMap trainRecordToVariantMap(ST_TrainRecord);
extern QVariantMap ScopeTestToVariantMap(ST_ScopeTest);

#endif // USERDATAFORMATE_H
