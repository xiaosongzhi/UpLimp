#include "scopemanager.h"
#include "ui_scopemanager.h"
#include "scopetest.h"
#include <QMovie>
#include <QFile>
#include "logger.h"
#include  "cdatabaseinterface.h"
#include "currentuserdata.h"
#include <QDebug>
#include <QShowEvent>
LOG4QT_DECLARE_STATIC_LOGGER(logger, ScopeManager)
ScopeManager::ScopeManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScopeManager)
{
    ui->setupUi(this);
    m_scopeTest = new SCopeTest();
    QString moviePath("./source/scorp.gif");
#ifdef ABSOLUTEPATH
    moviePath = QApplication::applicationDirPath() + "/source/scorp.gif";
#endif
    m_scopeMovie = new QMovie(moviePath);
    connect(m_scopeMovie,&QMovie::finished,this,&ScopeManager::slotMovieFinished);
    ui->gif_Label->setMovie(m_scopeMovie);

    loadStyleSheetFile();
}

ScopeManager::~ScopeManager()
{
    delete ui;
}

void ScopeManager::loadStyleSheetFile()
{
    //加载样式文件s
    QFile qssFile("./QSS/scorpManager.qss");
    if(!qssFile.open(QIODevice::ReadOnly))
        logger()->debug("scorpManager.qss文件打开失败!");
    else
    {
        this->setStyleSheet(qssFile.readAll());
        qssFile.close();
    }
}

void ScopeManager::slotMovieFinished()
{
    m_scopeMovie->start();
}

void ScopeManager::on_startTest_Btn_clicked()
{
    if(m_scopeTest)
    {
        m_scopeTest->show();
    }
}

void ScopeManager::startMovie()
{
    m_scopeMovie->start();
    QString areaText = QString::number(getScopeArea(),'f',2);
    ui->area_Label->setText(areaText);
}
void ScopeManager::stopMovie()
{
    m_scopeMovie->stop();
}


float ScopeManager::getScopeArea()
{
    float area = 0;
    int currentID = CurrentUserData::getInstace()->getCurrentUserID();
    QString orderWord("testTime");
    //前加引号 后不加引号
    QString query(QString("select * from scopetesttable where ID = '%1' order by %2 DESC").arg(currentID).arg(orderWord));

    CDatabaseInterface::getInstance()->exec(query);
    int count = CDatabaseInterface::getInstance()->getValuesSize();
    if(count > 0)
    {
        QList<QVariantMap>listMap = CDatabaseInterface::getInstance()->getValues(0,1);
        ST_ScopeTest st_scopeTest =  variantMapToScopeTest(listMap.at(0));
        area = st_scopeTest.area;
    }
    return  area/100.f;
}
