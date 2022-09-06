#include "rankinglist.h"
#include "ui_rankinglist.h"
#include <QDebug>
#include <QScrollBar>
#include "cdatabaseinterface.h"
#include <QTableWidgetItem>
#include "currentuserdata.h"

RankingList::RankingList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RankingList),
    m_currentPage(1),
    m_perPageCount(5),
    m_currentUserRanking(-1),
    m_maxRow(100),
    m_totalRowCount(0)
{
    ui->setupUi(this);
    initTableWidget();
}

RankingList::~RankingList()
{
    delete ui;
}

//根据字段排行 0-训练时长 1-总得分
void RankingList::setSortByField(int field)
{

}

void RankingList::on_previous_Btn_clicked()
{
    if(m_currentPage <= 1)
         return;
     m_currentPage--;
     //设置当前页数
     ui->page_Label->setText(QString("%1/%2").arg(m_currentPage).arg(m_totalPage));
     //当前首个数据条下标
     int firstIndex = (m_currentPage-1)*m_perPageCount;
     //剩余的个数
     int surplus = m_totalRowCount - firstIndex;
     //本页要添加的个数
     int toAddNum = surplus > m_perPageCount ? m_perPageCount : surplus;

     qDebug()<<"previous toAddNum"<<toAddNum;
     if(toAddNum > 0)
     {
         //先清理表格
         ui->tableWidget->clearContents();
         for(int i = 0;i <toAddNum;i++)
         {
             ST_TrainRanking st_TrainRanking = variantMapToTrainRanking(m_rankingListMap.at(i+firstIndex));
             fillTableWidget(st_TrainRanking,i);
         }
     }

}

void RankingList::on_next_Btn_clicked()
{
    if(m_currentPage < m_totalPage)
        m_currentPage++;
    else
        return;
     //设置当前页数
     ui->page_Label->setText(QString("%1/%2").arg(m_currentPage).arg(m_totalPage));
     //当前首个数据条下标
     qDebug()<<"m_currentPage"<<m_currentPage;
     int firstIndex = (m_currentPage-1)*m_perPageCount;
     //剩余的个数
     int surplus = m_totalRowCount - firstIndex;
     //本页要添加的个数
     int toAddNum = surplus > m_perPageCount ? m_perPageCount : surplus;

     qDebug()<<"next toAddNum"<<toAddNum;
     if(toAddNum > 0)
     {
         //先清理表格
         ui->tableWidget->clearContents();
         for(int i = 0;i <toAddNum;i++)
         {
             ST_TrainRanking st_TrainRanking = variantMapToTrainRanking(m_rankingListMap.at(i+firstIndex));
             fillTableWidget(st_TrainRanking,i);
         }
     }
}

void RankingList::initTableWidget()
{
    ui->tableWidget->horizontalHeader()->hide();
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(5);
    for(int i = 0;i <ui->tableWidget->columnCount();i++)
        ui->tableWidget->setColumnWidth(i,ui->tableWidget->width()/ui->tableWidget->columnCount()-1);
    for(int i = 0;i < ui->tableWidget->rowCount();i++)
        ui->tableWidget->setRowHeight(i,ui->tableWidget->height()/ui->tableWidget->rowCount()-1);
    ui->tableWidget->horizontalScrollBar()->hide();
    ui->tableWidget->verticalScrollBar()->hide();
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->setEnabled(false);
}

//在表中填充数据
void RankingList::fillTableWidget(ST_TrainRanking& st_TrainRanking,int row)
{
    QTableWidgetItem *rankingItem = new QTableWidgetItem();
    rankingItem->setTextAlignment(Qt::AlignCenter);
    //第一列显示排序
    if(1 == m_currentPage)//第一页
    {
        rankingItem->setText(QString::number(row+1));
    }
    else
    {
        int index = (m_currentPage-1)*5 + row+1;
        rankingItem->setText(QString::number(index));
    }
    ui->tableWidget->setItem(row,0,rankingItem);
    //姓名
    QTableWidgetItem *nameItem = new QTableWidgetItem();
    nameItem->setTextAlignment(Qt::AlignCenter);
    nameItem->setText(st_TrainRanking.name);
    ui->tableWidget->setItem(row,1,nameItem);
    //等级
    QTableWidgetItem *levelItem = new QTableWidgetItem();
    levelItem->setTextAlignment(Qt::AlignCenter);
    levelItem->setText(QString::number(st_TrainRanking.level));
    ui->tableWidget->setItem(row,2,levelItem);
    //总时长
    QTableWidgetItem *totalTimeItem = new QTableWidgetItem();
    totalTimeItem->setTextAlignment(Qt::AlignCenter);
    totalTimeItem->setText(QString::number(st_TrainRanking.totalTime/60));
    ui->tableWidget->setItem(row,3,totalTimeItem);
    //累计总得分
    QTableWidgetItem *totalScoreItem = new QTableWidgetItem();
    totalScoreItem->setTextAlignment(Qt::AlignCenter);
    totalScoreItem->setText(QString::number(st_TrainRanking.totalScore));
    ui->tableWidget->setItem(row,4,totalScoreItem);
}

//刷新表格
void RankingList::flushTableData()
{
    int currentID = CurrentUserData::getInstace()->getCurrentPatientMsg().ID;
    //首先查询当前用户的数据 存放在第一行
    QString table("rankingtable");
    QString query(QString("select * from rankingtable where ID = '%1'").arg(currentID));
    CDatabaseInterface::getInstance()->exec(query);
    QList<QVariantMap> vMapList = CDatabaseInterface::getInstance()->getValues(0,1);
    if(!vMapList.isEmpty())
    {
        ST_TrainRanking st_TrainRanking = variantMapToTrainRanking(vMapList.at(0));

        ui->name_Label->setText(st_TrainRanking.name);
        ui->level_Label->setText(QString::number(st_TrainRanking.level));
        ui->totalTime_Label->setText(QString::number(st_TrainRanking.totalTime/60));
        ui->totoalScore_Label->setText(QString::number(st_TrainRanking.totalScore));
    }

    //填充表格控件tableWidget
    QString orderWord("totalScore");
    QString query1(QString("select * from rankingtable order by %1 DESC").arg(orderWord));
    CDatabaseInterface::getInstance()->exec(query1);
    int count = CDatabaseInterface::getInstance()->getValuesSize();
    //无数据
    if(-1 == count)
    {
        ui->page_Label->setText("1/1");
        //清除表格
        ui->tableWidget->clearContents();
        return;
    }
    m_totalPage = count/m_perPageCount + 1;
    m_currentPage = 1;
    //设置当前页数
    ui->page_Label->setText(QString("1/%1").arg(m_totalPage));
    //参与排行的所有数据
    m_rankingListMap = CDatabaseInterface::getInstance()->getValues(0,count < m_maxRow ? count:m_maxRow);
    m_totalRowCount =m_rankingListMap.size();
    //求当前用户排名
    QList<QVariantMap> tempMapList = CDatabaseInterface::getInstance()->getValues(0,count);
    for(int i = 0;i < count;i++)
    {
        QVariantMap map = tempMapList.at(i);
        if(map.value("ID").toInt() == currentID)
        {
            m_currentUserRanking = i+1;
            ui->ranking_Label->setText(QString::number(m_currentUserRanking));
            break;
        }
    }

    if(!m_rankingListMap.isEmpty())
    {
        //先清理表格
        ui->tableWidget->clearContents();
        for(int i = 0;i <m_rankingListMap.size();i++)
        {
            ST_TrainRanking st_TrainRanking = variantMapToTrainRanking(m_rankingListMap.at(i));
            fillTableWidget(st_TrainRanking,i);
        }
    }
}

void RankingList::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    flushTableData();
}
