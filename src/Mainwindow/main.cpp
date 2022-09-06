#include "mainwindow.h"
#include <QApplication>
#include <log4qt/Logger>
#include <log4qt/TTCCLayout>
#include <log4qt/ConsoleAppender>
#include <log4qt/LoggerRepository>
#include "basicconfigurator.h"
#include "logmanager.h"
#include <QDebug>
#include "log4j/helper/log4qt_init_helper_by_config.h"
#include "log4qt/dailyfileappender.h"
#include "mysqldatabase.h"
#include <QRandomGenerator>
#include "readconfig.h"
#include "cdatabaseinterface.h"
#include "cpassworddialog.h"
#include "cbasedialog.h"
#include <QFile>
#include <QSplashScreen>
#include <QPixmap>
#include <QMessageBox>
LOG4QT_DECLARE_STATIC_LOGGER(logger, main)
//在类的cpp文件中，使用此静态方法声明logger（此方法比较通用）
//第二个参数写类名字，因此，输出的log条目中包含其对应的类名
quint8 g_isTesting;
int main(int argc, char *argv[])
{
//    qputenv("QT_IM_MODULE",QByteArray("Qt5Input"));
    QApplication a(argc, argv);
    //①开启日志选项
#ifdef LOG4QT
    //使用config 配置Log4Qt
    //把源代码目录中的"log4qt.properties"文件复制到编译好的可执行文件所在的目录
    QString configFileAbsPath  = QCoreApplication::applicationDirPath() +"/configFile/"+ QStringLiteral("log4qt.properties");//配置文件不包括应用程序名称
    qDebug()<<"configFileAbsPath"<<configFileAbsPath;
    SetupLog4QtByConfigWithConfigFileAbsPath(configFileAbsPath);
#endif
    //②读取配置文件
    bool ok =ReadConfig::getInstance()->readConfigFile();
    if(!ok)
    {
        logger()->error("read config file failed");
        qDebug()<<"read config file failed";
        return -1;
    }

    //③连接数据库
    if(!CDatabaseInterface::getInstance()->openDB("uplimp.db","QSQLITE"))
    {
        QString errorString = CDatabaseInterface::getInstance()->getLastError();
        QMessageBox::warning(NULL,"TIPS","数据库打开失败" + errorString);
    }
    /*
    //加载样式文件
    QFile qssFile("./QSS/app.qss");
    if(!qssFile.open(QIODevice::ReadOnly))
        qDebug()<<"app.qss文件打开失败!";
    else
    {
        qApp->setStyleSheet(qssFile.readAll());
        qssFile.close();
    }
    */

    QPixmap startPix("./source/startpicture.png");
    QSplashScreen screen(startPix);
    screen.show();
    a.processEvents();
    //④进入主程序
    MainWindow w;
    w.show();
    screen.finish(&w);
    w.enterUserManagerPage();

    int ret = a.exec();
#ifdef LOG4QT
    //exec()执行完成后，才关闭logger
    ShutDownLog4QtByConfig();
#endif
    return ret;
}
