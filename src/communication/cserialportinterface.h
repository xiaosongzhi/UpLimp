#ifndef CSERIALPORTINTERFACE_H
#define CSERIALPORTINTERFACE_H

#include "ccommunicationinterface.h"
#include <QObject>
#include <QSerialPort>
#include <QTimer>
class CSerialportInterface : public CCommunicationInterface
{
public:
    CSerialportInterface();
    ~CSerialportInterface();
    //配置参数
    virtual bool setConfigParam();
    //发送数据接口
    virtual void sendDataInterface(QByteArray);

public slots:
    //接收数据接口
   void receiveDataInterface();
   //通信状态变化
   void deviceStateChanged(int);
   //设备错误
   void displayError(QSerialPort::SerialPortError error);
   //串口检测函数
   void slotCheckTimer();
private:
    QSerialPort *m_serialPort;
    QByteArray receiveArray;
    QTimer *checkTimer;
};

#endif // CSERIALPORTINTERFACE_H
