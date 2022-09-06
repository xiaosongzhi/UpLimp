#ifndef READCONFIG_H
#define READCONFIG_H

#include <QObject>
#include "dataFormate.h"
#include <QMutex>
//饿汉式单例

class ReadConfig
{
private:
    ReadConfig();

    static QMutex mutex;
    static ReadConfig *m_pInstance;

    typedef struct
    {
        QString IP;
        int16_t port;
    }ST_UDPIPAddress;

    typedef struct ST_AllConfigData
    {
        ST_UDPIPAddress serverAddress;
        ST_UDPIPAddress clientAddress;
        ST_UDPIPAddress gameServerAddress;
        ST_UDPIPAddress gameClientAddress;
        QString tcpIP;
        int16_t tcpPort;
        ST_SerialPortConfig serialConfig;
        ST_DataBaseConfig dataBaseConfig;
        int communicateType = -1;
        ST_AllConfigData()
        {
            tcpIP.clear();
            tcpPort = 0;
        }

    }ST_AllConfigData;

    ST_AllConfigData st_configData;

    bool isConnecting;
public:
    //获取实例
    static ReadConfig* getInstance();
    //获取上下位机UDP地址信息
    bool getUdpServerAddress(int16_t &port,QString &IP);
    bool getUdpClientAddress(int16_t &port,QString &IP);
    //获取游戏上下位机地址
    bool getGameSeverAddress(int16_t &port,QString &IP);
    bool getGameClientAddress(int16_t &port,QString &IP);
    //获取TCP地址信息
    bool getTcpAddress(int16_t &port,QString &IP);
    //获取串口信息
    bool getSerialPortConfig(ST_SerialPortConfig &serialConfig);
    //获取数据库配置
    bool getDataBaseConfig(ST_DataBaseConfig &databaseConfig);
    //获取通信方式
    int getCommunicateType();

    bool readConfigFile();

    //设置连接状态
    void setConnectState(bool);
    //获取连接状态
    bool getConnectState();
};

#endif // READCONFIG_H
