#ifndef DATAFORMATE_H
#define DATAFORMATE_H
#include <QWidget>
#include <QMap>
#define DEVICEWIDTH 633
#define DEVICEHEIGHT 463

#define PACKHEAD 0xFE
typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;

extern quint8 g_isTesting;

typedef enum
{
    EMERGENCY_E = 0,    //急停
    SPASM_E,            //痉挛
    CONNECTLOST_E,      //连接断开
    CLAMP_E,            //夹手警告
    GAMETIPS_E,         //绑好绑带
    DISCONNECT_E,       //设备断连
    RESET_HANDLE_E      //复位手柄
}E_WarningType;

typedef enum
{
    INITWAITING,    //初始化等待
    LOADINGWITING,  //复位中

}E_WaitingType;


typedef enum
{
    AreaChart_E = 0,
    BarChart_E,
    PieChart_E,
    LineChart_E,
    SplineChart_E,
    ScatterChart_E
}E_ChartType;
typedef enum
{
    X_Axis = 0,
    Y_Axis
}E_AxisType;
typedef enum
{
    UnconnectedState_E = 0,
    HostLookupState_E,
    ConnectingState_E,
    ConnectedState_E,
    BoundState_E,
    ClosingState_E,
    ListeningState_E

}E_DeviceState;

typedef enum
{
    HEARTBEAT = 0X00,           //心跳
    EMERGENCY = 0X01,           //急停报警
    CHECKRESULT = 0X02,         //自检结果
    DEVICEERROR = 0X03,         //设备报警
    SPASMWARNING = 0X04,        //痉挛报警
    REALTIMEDATA = 0x05,        //下位机实时回传的数据
    ERRORRESET = 0X06,          //故障复位
    GOHOME = 0X07,              //回原点
    SENSORCALIBRATION = 0X08,   //传感器校准
    CONTROLDATA = 0X09,         //控制数据
    SETSPASM = 0X0A,            //设置痉挛力度
    STOPTRAINING = 0X0B,        //停止训练
    READYCMD = 0x0C,            //下位机准备好，可以开始训练

    SPEEDTEST = 0x0D,           //速度测试
    XBOOSTTEST = 0X0E,          //x方向助力测试
    YBOOSTTEST = 0X0F,          //y方向助力测试
    XAGAINSTTEST = 0X10,        //x方向阻力测试
    YAGAINSTTEST = 0X11,         //y方向阻力测试
    GOBACKTESTPOINT = 0x12,       //回测试远点
    DEVICEISON = 0x13            //检测下位机是否在线

}E_CMD;

//训练模式
typedef enum
{
    E_TRAINMODE_PASSIVE = 0,	//被动
    E_TRAINMODE_ACTIVE,         //主动
    E_TRAINMODE_BOOST,          //助力
    E_TRAINMODE_AGAINST         //抗阻
}E_TRAINMODE;

typedef struct ST_SerialPortConfig
{
    QString portName;   //端口名
    int32_t baud;       //波特率
    int8_t dataBits;    //数据位
    int8_t parity;      //奇偶校验
    int8_t stopBit;     //停止位
    int8_t flowControl; //流控
    ST_SerialPortConfig()
    {
        portName = "";
        baud = 0;
        dataBits = 0;
        parity = 0;
        stopBit = 0;
        flowControl = 0;
    }

}ST_SerialPortConfig;

typedef struct ST_DataBaseConfig
{
    QString IP;
    int16_t port;
    QString userName;   //用户名
    QString password;   //密码
    ST_DataBaseConfig()
    {
        IP = "";
        port = 0;
        userName = "";
        password = "";
    }

}ST_DataBaseConfig;

#pragma pack(push) //保存对齐状态
#pragma pack(1)//设定为1字节对齐

typedef struct
{
    int x;
    int y;
}ST_Point;

typedef struct
{
    float x;
    float y;
}ST_CurrentPoint;

//设备实时参数（设备-->上位机）
typedef struct
{
    ST_CurrentPoint st_currentPoint;
    int xSpeed;
    int ySpeed;
}ST_DeviceParam;

//实时控制参数
typedef struct ST_ControlData
{
    E_TRAINMODE E_Mode;                 //运行模式 0:被动; 1:主动; 2:助力; 3:抗阻
    ST_Point    st_EndPort;             //终点位置
    ST_Point    st_LeftTopPoint;        //障碍物左上角位置
    ST_Point    st_RightBottomPoint;    //障碍物右下角位置
    int force;                          //力量大小
    int speed;                          //速度大小
    ST_ControlData()
    {
        E_Mode = E_TRAINMODE_PASSIVE;
        st_EndPort.x = 0;
        st_EndPort.y = 0;
        st_LeftTopPoint.x = 0;
        st_LeftTopPoint.y = 0;
        st_RightBottomPoint.x = 0;
        st_RightBottomPoint.y = 0;
        force = 0;
        speed = 0;
    }
}ST_ControlData;

#pragma pack(pop)//恢复对齐状态

//主动模式游戏参数
typedef struct
{
    QString gameName;               //游戏名称
    QString gamePath;               //游戏路径
    QString gameTrack;              //运动轨迹
    QString gameTrackImage;         //轨迹截图
    int scorpSize;                  //运动范围大小 1-小 2-中 3-大
    int blockLength;                //障碍物长度 1-短 2-中 3-长
    QMap<int,int> trainProcessMap;  //训练流程 QMap<训练方式，时间> 训练方式为0,1,2
    E_TRAINMODE trainMode;          //训练模式
}ST_ActiveParam;

/***********与游戏交互**************/
//被动模式游戏参数
typedef struct
{
    QString gameName;               //游戏名称
    QString gamePath;               //游戏路径
    QString gameTrack;              //运动轨迹
    QString gameTrackImage;         //轨迹截图
    int scorpSize;                  //运动范围大小 1-小 2-中 3-大
    QMap<int,int> trainProcessMap;  //训练流程
    int speed;                      //速度大小 1-低速 2-中速 3-高速
    E_TRAINMODE trainMode;          //训练模式
}ST_PassiveParam;

//助力模式游戏参数
typedef struct
{
    QString gameName;               //游戏名称
    QString gamePath;               //游戏路径
    QString gameTrack;              //运动轨迹
    QString gameTrackImage;         //轨迹截图
    int scorpSize;                  //运动范围大小 1-小 2-中 3-大
    int  level;                     //助力等级
    QMap<int,int> trainProcessMap;  //训练流程
    E_TRAINMODE trainMode;          //训练模式
}ST_BoostParam;

//阻抗模式游戏参数
typedef struct
{
    QString gameName;               //游戏名称
    QString gamePath;               //游戏路径
    QString gameTrack;              //运动轨迹
    QString gameTrackImage;         //轨迹截图
    int scorpSize;                  //运动范围大小 1-小 2-中 3-大
    int level;                      //阻力等级
    QMap<int,int> trainProcessMap;  //训练流程
    E_TRAINMODE trainMode;          //训练模式
}ST_AgainstParam;

//更改游戏配置文件的参数(通信文件)
typedef  struct ST_GameParam
{
    int hardLevel;          //难度等级 1-3(此处的难度等级对应范围大小)
    int gameTime;           //游戏运行时间 单位/s
    int speed;              //运行速度 1-5
    int trainingMode;       //训练类型
    QString trainingTrack;  //游戏路径文件
    bool readPosTable;      //是否去读轨迹
    ST_GameParam()
    {
        hardLevel = 1;
        gameTime = 0;
        speed = 3;
        trainingMode = 0;
        trainingTrack = "";
        readPosTable = false;
    }
}ST_GameParam;

//该结构体用于读取游戏列表
typedef struct
{
    int gameID;             //游戏ID
    QString gameName;       //游戏名称
    QString gamePath;       //游戏路径
    QString iconPath;       //游戏图标路径
    QStringList suitTypeList;//适合的游戏类型
}ST_GameMsg;
extern void Sleep(int msec);
extern void  Pressure_CheckCRC(uint8_t*buf,int len,uint8_t* CRC_H,uint8_t* CRC_L);
#endif // DATAFORMATE_H
