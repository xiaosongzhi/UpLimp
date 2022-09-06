#include "ccommunicateapi.h"
#include "readconfig.h"
#include "ccommunicationinterface.h"
#include "cserialportinterface.h"
#include "cudpinterface.h"
#include "ctcpsocketinterface.h"
#include "dataFormate.h"
#include <QDebug>

CCommunicateAPI *CCommunicateAPI::m_API = NULL;

CCommunicateAPI::CCommunicateAPI(QObject *parent) :
    QObject(parent),
    m_interface(NULL)
{
    init();
}
CCommunicateAPI::~CCommunicateAPI()
{
    if(m_interface)
    {
        delete m_interface;
        m_interface = NULL;
    }
}

CCommunicateAPI *CCommunicateAPI::getInstance()
{
    if(!m_API)
    {
        m_API = new CCommunicateAPI();
    }
    return m_API;
}

void CCommunicateAPI::init()
{
    commuType = ReadConfig::getInstance()->getCommunicateType();
    //根据通信方式生成不同的对象
    if(0 == commuType) //串口
    {
        m_interface = new CSerialportInterface();
    }
    else if(1 == commuType) //udp
    {
        m_interface = new CUdpInterface();
    }
    else if(2 == commuType) //tcp
    {
        m_interface = new CTcpSocketInterface();
    }
    else if(3 == commuType) //can
    {
        //待添加
    }
    if(m_interface)
    {
        connect(m_interface,SIGNAL(signalReadyRead(QByteArray)),this,SIGNAL(signalReadyRead(QByteArray)));
    }
}


void CCommunicateAPI::sendData(QByteArray sendArray)
{
    sendArray.prepend(PACKHEAD);

    //添加校验
    uint8_t CRC_H = 0;
    uint8_t CRC_L = 0;
    Pressure_CheckCRC((uint8_t*)(sendArray.data()),sendArray.length(),&CRC_H,&CRC_L);
    sendArray.append(CRC_L);
    sendArray.append(CRC_H);
    m_interface->sendDataInterface(sendArray);
}

void CCommunicateAPI::sendData(uint8_t seq,uint8_t id,uint8_t cmdID,QByteArray arrayData)
{
    QByteArray fixedArray;
    fixedArray.resize(4);
    fixedArray[0] = arrayData.length();
    fixedArray[1] = seq;
    fixedArray[2] = id;
    fixedArray[3] = cmdID;

    fixedArray.append(arrayData);

    sendData(fixedArray);
}

void CCommunicateAPI::sendPureDataPure(QByteArray sendArray)
{
    if(m_interface)
    m_interface->sendDataInterface(sendArray);
}
