#include "scopereport.h"
#include "ui_scopereport.h"
#include <QDebug>
#include  "cdatabaseinterface.h"
#include "currentuserdata.h"
#include <QPainter>

ScopeReport::ScopeReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScopeReport)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose,false);
    setWindowFlags(Qt::FramelessWindowHint /*| Qt::WindowStaysOnTopHint*/);    //在windows环境下需要打开
    setAttribute(Qt::WA_TranslucentBackground);

    ui->track_Widget->initChart(LineChart_E,"轨迹");
    ui->xAxis_Widget->initChart(BarChart_E,"X轴");
    ui->yAxis_Widget->initChart(BarChart_E,"Y轴");
}

ScopeReport::~ScopeReport()
{
    delete ui;
}

//绘制范围
void ScopeReport::paintScopeTrack(QList<QPoint> & listPoint)
{
    DataTable dataTable;
    DataList dataList;
    foreach(QPoint point,listPoint)
    {
        Data pointData;
        pointData.first = point;
        dataList.append(pointData);
    }
    dataTable.append(dataList);

    ui->track_Widget->setChartData(LineChart_E,dataTable);
    ui->track_Widget->setAxisValue(X_Axis,0,DEVICEWIDTH);
    ui->track_Widget->setAxisValue(Y_Axis,0,DEVICEHEIGHT);
}
//绘制当前用户的最大最小值
void ScopeReport::paintLimitValue()
{
    int currentID = CurrentUserData::getInstace()->getCurrentUserID();
    QString orderWord("testTime");
    QString query(QString("select * from scopetesttable where ID = '%1' order by %2 DESC").arg(currentID).arg(orderWord));
    CDatabaseInterface::getInstance()->exec(query);
    int count = CDatabaseInterface::getInstance()->getValuesSize();

    //显示小于 等于5组数据
   QList<QVariantMap>listMap = CDatabaseInterface::getInstance()->getValues(0,count < 5 ? count:5);

    createXAxisBarChart(listMap);
    createYAxisBarChart(listMap);
}

void ScopeReport::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
}
void ScopeReport::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    paintLimitValue();
}

//创建X轴柱状图
void ScopeReport::createXAxisBarChart(QList<QVariantMap>& listMap)
{
    QMap<QDateTime,int> scopeMap;

    for(int i = 0;i < listMap.size();i++)
    {
        int xLength = 0;
        QDateTime dateTime;
        QVariantMap vMap = listMap.at(i);
        if(vMap.contains("testTime"))
            dateTime = vMap.value("testTime").toDateTime();
        if(vMap.contains("xLength"))
            xLength = vMap.value("xLength").toInt();
        scopeMap.insert(dateTime,xLength);
    }

    DataTable dataTable;
    DataList dataList;
    int index = 1;
    QMapIterator<QDateTime, int> i(scopeMap);
    while (i.hasNext()) {
        i.next();
        Data pointData;
        pointData.first.setX(index);
        pointData.first.setY(i.value());
        dataList.append(pointData);
        index++;
    }
    dataTable.append(dataList);
    //生成柱状图
    ui->xAxis_Widget->setChartData(BarChart_E,dataTable);
}
//创建Y轴柱状图
void ScopeReport::createYAxisBarChart(QList<QVariantMap>& listMap)
{
    QMap<QDateTime,int> scopeMap;

    for(int i = 0;i < listMap.size();i++)
    {
        int yLength = 0;
        QDateTime dateTime;
        QVariantMap vMap = listMap.at(i);
        if(vMap.contains("testTime"))
            dateTime = vMap.value("testTime").toDateTime();
        if(vMap.contains("yLength"))
            yLength = vMap.value("yLength").toInt();
        scopeMap.insert(dateTime,yLength);
    }

    DataTable dataTable;
    DataList dataList;
    int index = 1;
    QMapIterator<QDateTime, int> i(scopeMap);
    while (i.hasNext()) {
        i.next();
        Data pointData;
        pointData.first.setX(index);
        pointData.first.setY(i.value());
        dataList.append(pointData);
        index++;
    }
    dataTable.append(dataList);
    //生成柱状图
    ui->yAxis_Widget->setChartData(BarChart_E,dataTable);
}

void ScopeReport::on_delete_Btn_clicked()
{
    this->close();
}

void ScopeReport::on_save_Btn_clicked()
{
    this->close();
}

void ScopeReport::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.fillRect(rect(),QColor(0,0,0,127));
}

void ScopeReport::on_close_Btn_clicked()
{
    this->close();
}
