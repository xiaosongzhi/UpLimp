#include "readconfig.h"
#include <QXmlStreamReader>
#include <QFile>
#include <QApplication>
#include <QDebug>
#include "logger.h"
LOG4QT_DECLARE_STATIC_LOGGER(logger, main)

ReadConfig *ReadConfig::m_pInstance = NULL;
QMutex ReadConfig::mutex;
ReadConfig::ReadConfig()
{
    isConnecting = false;
//    readConfigFile();
}
bool ReadConfig::readConfigFile()
{
    QString filePath = QApplication::applicationDirPath();
    QString fileName = filePath +"/configFile/config.xml";
    QFile configFile("./configFile/config.xml");
#ifdef ABSOLUTEPATH
    configFile.setFileName(fileName);
#endif
    if(!configFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    QXmlStreamReader reader(&configFile);
    while(!reader.atEnd())
    {
        QXmlStreamReader::TokenType nType = reader.readNext();
        switch(nType)
        {
        case QXmlStreamReader::StartDocument:   //开始元素
        {
            QString strVersion = reader.documentVersion().toString();
            QString strEncoding = reader.documentEncoding().toString();
            bool bAlone = reader.isStandaloneDocument();
            qDebug()<<strVersion<<strEncoding<<bAlone;
            break;
        }
        case QXmlStreamReader::Comment://注释
        {
            QString strComment = reader.text().toString();
            break;
        }
        case QXmlStreamReader::ProcessingInstruction://处理指令
        {
            QString strProInstr = reader.processingInstructionData().toString();
            break;
        }
        case QXmlStreamReader::DTD://DTD标识
        {
            QString strDTD = reader.text().toString();
            QXmlStreamNotationDeclarations notations = reader.notationDeclarations();
            QXmlStreamEntityDeclarations entity = reader.entityDeclarations();
            //DTD声明
            QString strDTDName = reader.dtdName().toString();
            QString strDTDPublicId = reader.dtdPublicId().toString();//DTD公开标识
            QString strDTDSystemId = reader.dtdSystemId().toString();//DTD系统标识

            break;
        }
        case QXmlStreamReader::StartElement://开始元素
        {
            QString strElementName = reader.name().toString();
            if(QString::compare(strElementName,"config") == 0)
            {
                QXmlStreamAttributes attributes = reader.attributes();

            }
            else if(QString::compare(strElementName,"tcpAddress") == 0)
            {
                QXmlStreamAttributes attributes = reader.attributes();
                if(attributes.hasAttribute("IP"))
                {
                    st_configData.tcpIP = attributes.value("IP").toString();
                }
                if(attributes.hasAttribute("port"))
                {
                    st_configData.tcpPort = attributes.value("port").toInt();
                }
            }
            else if(QString::compare(strElementName,"udpServerAddress") == 0)
            {
                QXmlStreamAttributes attributes = reader.attributes();
                if(attributes.hasAttribute("IP"))
                {
                     st_configData.serverAddress.IP = attributes.value("IP").toString();
                }
                if(attributes.hasAttribute("port"))
                {
                    st_configData.serverAddress.port = attributes.value("port").toInt();
                }
            }
            else if(QString::compare(strElementName,"udpClientAddress") == 0)
            {
                QXmlStreamAttributes attributes = reader.attributes();
                if(attributes.hasAttribute("IP"))
                {
                     st_configData.clientAddress.IP = attributes.value("IP").toString();
                }
                if(attributes.hasAttribute("port"))
                {
                    st_configData.clientAddress.port = attributes.value("port").toInt();
                }
            }
            else if(QString::compare(strElementName,"udpGameClientAddress") == 0)
            {
                QXmlStreamAttributes attributes = reader.attributes();
                if(attributes.hasAttribute("IP"))
                {
                     st_configData.gameClientAddress.IP = attributes.value("IP").toString();
                }
                if(attributes.hasAttribute("port"))
                {
                    st_configData.gameClientAddress.port = attributes.value("port").toInt();
                }
            }
            else if(QString::compare(strElementName,"udpGameServerAddress") == 0)
            {
                QXmlStreamAttributes attributes = reader.attributes();
                if(attributes.hasAttribute("IP"))
                {
                     st_configData.gameServerAddress.IP = attributes.value("IP").toString();
                }
                if(attributes.hasAttribute("port"))
                {
                    st_configData.gameServerAddress.port = attributes.value("port").toInt();
                }
            }
            else if(QString::compare(strElementName,"serialPort") == 0)
            {
                QXmlStreamAttributes attributes = reader.attributes();
                if(attributes.hasAttribute("portName"))
                {
                    st_configData.serialConfig.portName = attributes.value("portName").toString();
                }
                if(attributes.hasAttribute("baud"))
                {
                    st_configData.serialConfig.baud = attributes.value("baud").toInt();
                }
                if(attributes.hasAttribute("dataBits"))
                {
                    st_configData.serialConfig.dataBits = attributes.value("dataBits").toInt();
                }
                if(attributes.hasAttribute("parity"))
                {
                    st_configData.serialConfig.parity = attributes.value("parity").toInt();
                }
                if(attributes.hasAttribute("stopBit"))
                {
                    st_configData.serialConfig.stopBit = attributes.value("stopBit").toInt();
                }
                if(attributes.hasAttribute("flowControl"))
                {
                    st_configData.serialConfig.flowControl = attributes.value("flowControl").toInt();
                }

              }
            else if(QString::compare(strElementName,"dataBase") == 0)
            {
                QXmlStreamAttributes attributes = reader.attributes();
                if(attributes.hasAttribute("IP"))
                {
                    st_configData.dataBaseConfig.IP = attributes.value("IP").toString();
                }
                if(attributes.hasAttribute("port"))
                {
                    st_configData.dataBaseConfig.port = attributes.value("port").toInt();
                }
                if(attributes.hasAttribute("userName"))
                {
                    st_configData.dataBaseConfig.userName = attributes.value("userName").toString();
                }
                if(attributes.hasAttribute("password"))
                {
                    st_configData.dataBaseConfig.password = attributes.value("password").toString();
                }
            }
            else if(QString::compare(strElementName,"communicateType") == 0)
            {
                QXmlStreamAttributes attributes = reader.attributes();
                if(attributes.hasAttribute("type"))
                    st_configData.communicateType = attributes.value("type").toInt();
            }
            break;
        }
        case QXmlStreamReader::EndDocument://结束文档
        {
            break;
        }
        default:
            break;
        }
    }
    configFile.close();

    if(reader.hasError())
    {
        logger()->debug(QString::fromLocal8Bit("错误信息：%1 行号：%2 列号：%3 字符移位：%4").arg(reader.errorString())
                  .arg(reader.lineNumber()).arg(reader.columnNumber()).arg(reader.characterOffset()));
        return false;
    }

    return true;
}
//获取实例
ReadConfig* ReadConfig::getInstance()
{
    QMutexLocker mutexLocker(&mutex);
    if(!m_pInstance)
    {
        m_pInstance = new ReadConfig();
    }
    return m_pInstance;
}

//获取通信方式
int ReadConfig::getCommunicateType()
{
    return st_configData.communicateType;
}

//获取UDP地址信息
bool ReadConfig::getUdpServerAddress(int16_t &port,QString &IP)
{
    port = st_configData.serverAddress.port;
    IP = st_configData.serverAddress.IP;

    if(port != 0 && !IP.isEmpty())
        return true;
    else
        return false;
}

bool ReadConfig::getUdpClientAddress(int16_t &port,QString &IP)
{
    port = st_configData.clientAddress.port;
    IP = st_configData.clientAddress.IP;

    if(port != 0 && !IP.isEmpty())
        return true;
    else
        return false;
}

//获取TCP地址信息
bool ReadConfig::getTcpAddress(int16_t &port,QString &IP)
{
    port = st_configData.tcpPort;
    IP = st_configData.tcpIP;

    if(port != 0 && !IP.isEmpty())
        return true;
    else
        return false;
}
//获取串口信息
bool ReadConfig::getSerialPortConfig(ST_SerialPortConfig &serialConfig)
{
    serialConfig = st_configData.serialConfig;
    if(!serialConfig.portName.isEmpty() && serialConfig.baud != 0)
    {
        return true;
    }
    else
        return false;
}
//获取数据库配置
bool ReadConfig::getDataBaseConfig(ST_DataBaseConfig &databaseConfig)
{
    databaseConfig = st_configData.dataBaseConfig;
    if(!databaseConfig.IP.isEmpty())
        return true;
    else
        return false;

}

bool ReadConfig::getGameSeverAddress(int16_t &port,QString &IP)
{
    port = st_configData.gameServerAddress.port;
    IP = st_configData.gameServerAddress.IP;
    if(port != 0 && !IP.isEmpty())
        return true;
    else
        return false;
}
bool ReadConfig::getGameClientAddress(int16_t &port,QString &IP)
{
    port = st_configData.gameClientAddress.port;
    IP = st_configData.gameClientAddress.IP;
    if(port != 0 && !IP.isEmpty())
        return true;
    else
        return false;
}

//设置连接状态
void ReadConfig::setConnectState(bool state)
{
    isConnecting = state;
}
//获取连接状态
bool ReadConfig::getConnectState()
{
    return isConnecting;
}
