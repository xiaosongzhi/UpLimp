#include "cserialportinterface.h"
#include <QApplication>
#include "readconfig.h"
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>
#include "logger.h"
LOG4QT_DECLARE_STATIC_LOGGER(logger, TrainingManager)
CSerialportInterface::CSerialportInterface():m_serialPort(NULL)
{
    receiveArray.clear();
    if(!m_serialPort)
    {
        m_serialPort = new QSerialPort();
        connect(m_serialPort,&QSerialPort::readyRead,this,&CSerialportInterface::receiveDataInterface);
        connect(m_serialPort,&QSerialPort::errorOccurred,this,&CSerialportInterface::displayError);
        setConfigParam();
    }
    checkTimer = new QTimer();
    connect(checkTimer,&QTimer::timeout,this,&CSerialportInterface::slotCheckTimer);
    checkTimer->start(3000);

}
CSerialportInterface::~CSerialportInterface()
{
    if(m_serialPort)
    {
        delete m_serialPort;
        m_serialPort = NULL;
    }
}


void CSerialportInterface::slotCheckTimer()
{
    if(!ReadConfig::getInstance()->getConnectState())
        setConfigParam();
}

//配置参数
bool CSerialportInterface::setConfigParam()
{
    ST_SerialPortConfig st_SerialConfig;
    if(!ReadConfig::getInstance()->getSerialPortConfig(st_SerialConfig))
    {
        logger()->debug("获取串口配置失败");
        return false;
    }

    /*
    QSerialPortInfo m_SerialPortInfo;
    QStringList serialPortNames;
    foreach(m_SerialPortInfo,QSerialPortInfo::availablePorts())
    {
        QSerialPort serialport;
        serialport.setPort(m_SerialPortInfo);

        if(serialport.open(QIODevice::ReadWrite))
        {
            serialPortNames.append(m_SerialPortInfo.portName());
            serialport.close();
        }
    }
    if(serialPortNames.isEmpty())
    {
        logger()->debug("无可用串口");
        return false;
    }
    */

    if(m_serialPort)
    {
        m_serialPort->close();
        m_serialPort->setPortName(st_SerialConfig.portName);
        m_serialPort->setReadBufferSize(200);
        if(m_serialPort->open(QIODevice::ReadWrite))
        {
            m_serialPort->setBaudRate(st_SerialConfig.baud);
            m_serialPort->setDataBits(QSerialPort::Data8);
            m_serialPort->setParity(QSerialPort::NoParity);
            m_serialPort->setStopBits(QSerialPort::OneStop);
            m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
        }
        else
        {
            logger()->debug("串口打开失败");
            return false;
        }
    }

    return true;
}
//发送数据接口
void CSerialportInterface::sendDataInterface(QByteArray sendArray)
{
    if(m_serialPort)
    {
        m_serialPort->write(sendArray);
    }
}
//接收数据接口
void CSerialportInterface::receiveDataInterface()
{
    QByteArray buf;
    buf = m_serialPort->readAll();
    receiveArray.append(buf);

    while(!receiveArray.isEmpty())
    {
        if(receiveArray[0] != (char)PACKHEAD)
        {
            receiveArray.remove(0,1);
        }
        else
        {
            //获取有效数据长度
            uint8_t datalen = 0;
            memcpy(&datalen,receiveArray.constData()+1,sizeof(uint8_t));

            if(receiveArray.length() >= datalen + 7)
            {
                uint8_t CRC_H = 0;
                uint8_t CRC_L = 0;
                Pressure_CheckCRC((uint8_t*)receiveArray.constData(),datalen+5,&CRC_H,&CRC_L);
                //校验成功
                if((CRC_L == (uint8_t)receiveArray[5+datalen]) && (CRC_H == (uint8_t)receiveArray[6+datalen]))
                {
                    emit  signalReadyRead(receiveArray.mid(0,datalen + 7));
                    receiveArray.remove(0,datalen + 7);
                }
                else //校验失败
                {
                    //方式1 丢弃本包
#if 1
                    receiveArray.remove(0,datalen + 7);
#else
                    //方式2 丢掉一个字节
                    receiveArray.remove(0,1);
#endif
                }
            }
            else    //数据不够，直接退出继续接收
                break;
        }
    }

}

void CSerialportInterface::displayError(QSerialPort::SerialPortError error)
{
    QString lastError("");
    switch(error)
    {
    case QSerialPort::DeviceNotFoundError:
        lastError = "DeviceNotFoundError";
        break;
    case QSerialPort::PermissionError:
        lastError = "PermissionError";
        break;
    case QSerialPort::OpenError:
        lastError = "OpenError";
        break;
    case QSerialPort::NotOpenError:
        lastError = "NotOpenError";
        break;
    case QSerialPort::WriteError:
        lastError = "WriteError";
        break;
    case QSerialPort::ReadError:
        lastError = "ReadError";
        break;
    case QSerialPort::UnknownError:
        lastError = "UnknownError";
        break;
    default:
        break;
    }

    qDebug()<<lastError;
    emit signalDisplayError(lastError);
}


