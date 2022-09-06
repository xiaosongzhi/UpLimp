#ifndef GAMEPARAMCONTROL_H
#define GAMEPARAMCONTROL_H

#include "dataFormate.h"
#include <QObject>
#include <QDomDocument>
class GameParamControl : public QObject
{
    Q_OBJECT
public:
    //更新游戏参数，通过该文件与游戏进行通信，游戏在启动时读取该文件
    void setGamParam(ST_GameParam&);
    //通过配置表获取所有游戏的信息
    QList<ST_GameMsg> getGameMsgs();
    //根据游戏名获得游戏信息
    ST_GameMsg getGameMsgByName(QString name);

    //获取实例
    static GameParamControl* getInstance();
private:
    explicit GameParamControl(QObject *parent = nullptr);

    void writeXml();

    void readXml();

    void updateXml(ST_GameParam&);

    void addXml();

    void removeXml();

private:

    static GameParamControl* m_GameParamControl;

    QMap<QString,ST_GameMsg> m_mapGameName;
signals:

};

#endif // GAMEPARAMCONTROL_H
