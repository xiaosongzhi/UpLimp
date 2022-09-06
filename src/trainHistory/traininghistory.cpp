#include "traininghistory.h"
#include "ui_traininghistory.h"
#include "cdatabaseinterface.h"
#include "currentuserdata.h"
#include <QScrollBar>
#include <QDebug>
#include "reportdialog.h"
#include <QMessageBox>
#include "controlmaskdialog.h"
#include <QPdfWriter>
#include <QPainter>
#include <QPrinter>
#include <QFileDialog>

TrainingHistory::TrainingHistory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrainingHistory),
    m_currentPage(1),
    m_perPageCount(5),
    m_maxRow(100),
    m_totalPage(0),
    m_totalReportNum(0),
    m_reportDialog(NULL)
{
    ui->setupUi(this);
    initTableWidget();
    QObjectList list = ui->buttons_GroupBox->children();
    foreach(QObject* object,list)
    {
        if(object->objectName().contains("view"))
            connect(object,SIGNAL(clicked()),this,SLOT(slotViewBtn()));
        else if(object->objectName().contains("import"))
            connect(object,SIGNAL(clicked()),this,SLOT(slotImportBtn()));
    }

    connect(CurrentUserData::getInstace(),SIGNAL(signalUserChanged()),this,SLOT(slotCurrentUserChanged()));

    m_reportDialog = new ReportDialog();
}

TrainingHistory::~TrainingHistory()
{
    if(m_reportDialog)
    {
        m_reportDialog->deleteLater();
        m_reportDialog = NULL;
    }
    delete ui;
}

void TrainingHistory::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    flushTableData();
}

//刷新表格
void TrainingHistory::flushTableData()
{
    int currentID = CurrentUserData::getInstace()->getCurrentPatientMsg().ID;
    QString orderWord("startTime");
    QString query(QString("select * from reporttable where ID = '%1' order by %2 DESC").arg(currentID).arg(orderWord));
    CDatabaseInterface::getInstance()->exec(query);
    int count = CDatabaseInterface::getInstance()->getValuesSize();

    //隐藏按钮
    setRowButtonHide(-1,true);
    //无数据
    if(-1 == count)
    {
        ui->page_Label->setText("1/1");
        //清除表格
        ui->tableWidget->clearContents();
        return;
    }

    if(count%m_perPageCount == 0)
        m_totalPage = count/m_perPageCount;
    else
        m_totalPage = count/m_perPageCount + 1;

    m_currentPage = 1;
    //设置当前页数
    ui->page_Label->setText(QString("1/%2").arg(m_totalPage));
    m_historyListMap = CDatabaseInterface::getInstance()->getValues(0,count < m_maxRow ? count:m_maxRow);
    m_totalReportNum = m_historyListMap.size();
    if(!m_historyListMap.isEmpty())
    {
        //先清理表格
        ui->tableWidget->clearContents();
        for(int i = 0;i <m_historyListMap.size();i++)
        {
            ST_TrainReport st_TrainReport = variantMapToTrainReport(m_historyListMap.at(i));
            fillTableWidget(st_TrainReport,i);
            setRowButtonHide(i,false);
        }
    }
}

void TrainingHistory::on_previous_Btn_clicked()
{
   if(m_currentPage <= 1)
        return;
    m_currentPage--;
    //设置当前页数
    ui->page_Label->setText(QString("%1/%2").arg(m_currentPage).arg(m_totalPage));
    //当前首个数据条下标
    int firstIndex = (m_currentPage-1)*m_perPageCount;
    //剩余的个数
    int surplus = m_totalReportNum - firstIndex;
    //本页要添加的个数
    int toAddNum = surplus > m_perPageCount ? m_perPageCount : surplus;

    setRowButtonHide(-1,true);
    if(toAddNum > 0)
    {
        //先清理表格
        ui->tableWidget->clearContents();
        for(int i = 0;i <toAddNum;i++)
        {
            ST_TrainReport st_TrainReport = variantMapToTrainReport(m_historyListMap.at(i+firstIndex));
            fillTableWidget(st_TrainReport,i);
            setRowButtonHide(i,false);
        }
    }
}

void TrainingHistory::on_next_Btn_clicked()
{
    if(m_currentPage < m_totalPage)
        m_currentPage++;
    else
        return;
    //设置当前页数
    ui->page_Label->setText(QString("%1/%2").arg(m_currentPage).arg(m_totalPage));
    //当前首个数据条下标
    int firstIndex = (m_currentPage-1)*m_perPageCount;
    //剩余的个数
    int surplus = m_totalReportNum - firstIndex;
    //本页要添加的个数
    int toAddNum = surplus > m_perPageCount ? m_perPageCount : surplus;
    //还有下一页数据
    setRowButtonHide(-1,true);
    if(toAddNum > 0)
    {
        //先清理表格
        ui->tableWidget->clearContents();
        for(int i = 0;i < toAddNum;i++)
        {
            ST_TrainReport st_TrainReport = variantMapToTrainReport(m_historyListMap.at(i+firstIndex));
            fillTableWidget(st_TrainReport,i);
            setRowButtonHide(i,false);
        }
    }
}

void TrainingHistory::initTableWidget()
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

void TrainingHistory::slotViewBtn()
{
    //"view0_Btn"
    QString buttonName = sender()->objectName();
    int index = buttonName.indexOf('_');
    int indexText = buttonName.mid(index-1,1).toInt();

    QTableWidgetItem *dateItem = ui->tableWidget->item(indexText,0);
    QTableWidgetItem *timeItem = ui->tableWidget->item(indexText,1);

    QString dateTimeText = dateItem->text()+"-"+timeItem->text();

    QDateTime dateTime = QDateTime::fromString(dateTimeText,"yyyyMMdd-hh:mm:ss");
    QString queryDateTime = dateTime.addSecs(1).toString("yyyy-MM-ddThh:mm:ss");

    int currentID = CurrentUserData::getInstace()->getCurrentPatientMsg().ID;

    QString queryTest(QString("select * from reporttable where ID = '%1' and startTime <= '%2'").arg(currentID).arg(queryDateTime));

    if(CDatabaseInterface::getInstance()->exec(queryTest))
    {
        qDebug()<<"查询成功";
        qDebug()<<CDatabaseInterface::getInstance()->getValuesSize();
    }

    QString query(QString("select * from reporttable where timeString = '%1'").arg(dateTimeText));
    //查询当前信息并生成报告
    CDatabaseInterface::getInstance()->exec(query);
    int count = CDatabaseInterface::getInstance()->getValuesSize();
    if(-1 == count)
    {
        QMessageBox::warning(NULL,"提示","未查询到结果！");
        return;
    }

    QList<QVariantMap> vMapList = CDatabaseInterface::getInstance()->getValues(0,1);
    if(!vMapList.isEmpty())
    {
        QVariantMap vMap = vMapList.at(0);
        ST_TrainReport st_TrainReport = variantMapToTrainReport(vMap);
        //回看报告就不需要添加本次了
        m_reportDialog->setTrainReport(st_TrainReport,1);
    }
}

void TrainingHistory::slotImportBtn()
{
    //"import1_Btn"
    QString buttonName = sender()->objectName();
    int index = buttonName.indexOf('_');
    int indexText = buttonName.mid(index-1,1).toInt();

    QTableWidgetItem *dateItem = ui->tableWidget->item(indexText,0);
    QTableWidgetItem *timeItem = ui->tableWidget->item(indexText,1);

    QString dateTimeText = dateItem->text()+"-"+timeItem->text();

    QString query(QString("select * from reporttable where timeString = '%1'").arg(dateTimeText));
    //查询当前信息并生成报告
    CDatabaseInterface::getInstance()->exec(query);
    int count = CDatabaseInterface::getInstance()->getValuesSize();
    if(-1 == count)
    {
        QMessageBox::warning(NULL,"提示","未查询到结果！");
        return;
    }
    QList<QVariantMap> vMapList = CDatabaseInterface::getInstance()->getValues(0,1);
    if(!vMapList.isEmpty())
    {
        QVariantMap vMap = vMapList.at(0);
        ST_TrainReport st_TrainReport = variantMapToTrainReport(vMap);
        QString defaultName = st_TrainReport.name + st_TrainReport.startTime.toString("yyyyMMddhhmmss") +".pdf";
        //回看报告
        m_reportDialog->setTrainReport(st_TrainReport,1);
        ControlMaskDialog::getInstace()->setMaskDialogHide(true);
        QPixmap pix = m_reportDialog->grab();
        pix.save("./reportImage.png");
        m_reportDialog->hide();
        //导出报告到pdf
        pixmapImportPDF(pix,defaultName);
    }

}

//在表中填充数据
void TrainingHistory::fillTableWidget(ST_TrainReport& st_TrainReport,int row)
{
    //date
    QList<QString> strList = st_TrainReport.timeString.split("-");
    QTableWidgetItem *dateItem = new QTableWidgetItem();
    dateItem->setTextAlignment(Qt::AlignCenter);
    if(!strList.isEmpty())
        dateItem->setText(strList.first());
    ui->tableWidget->setItem(row,0,dateItem);
    //time
    QTableWidgetItem *timeItem = new QTableWidgetItem();
    timeItem->setTextAlignment(Qt::AlignCenter);
    if(strList.size() > 1)
        timeItem->setText(strList.at(1));
    ui->tableWidget->setItem(row,1,timeItem);
    //时长
    QTableWidgetItem *durationItem = new QTableWidgetItem();
    durationItem->setTextAlignment(Qt::AlignCenter);
    if(st_TrainReport.trainDuration/60 >1)
        durationItem->setText(QString::number(st_TrainReport.trainDuration/60));
    else
        durationItem->setText("1");
    ui->tableWidget->setItem(row,2,durationItem);
    //得分
    QTableWidgetItem *scoreItem = new QTableWidgetItem();
    scoreItem->setTextAlignment(Qt::AlignCenter);
    scoreItem->setText(QString::number(st_TrainReport.score));
    ui->tableWidget->setItem(row,3,scoreItem);
    //运动模式
    QTableWidgetItem *modeItem = new QTableWidgetItem();
    modeItem->setTextAlignment(Qt::AlignCenter);
    if(E_TRAINMODE_PASSIVE == st_TrainReport.trainMode)
        modeItem->setText("被动模式");
    if(E_TRAINMODE_BOOST == st_TrainReport.trainMode)
        modeItem->setText("助力模式");
    if(E_TRAINMODE_ACTIVE == st_TrainReport.trainMode)
        modeItem->setText("主动模式");
    if(E_TRAINMODE_AGAINST == st_TrainReport.trainMode)
        modeItem->setText("抗阻模式");
    ui->tableWidget->setItem(row,4,modeItem);

}

void TrainingHistory::setRowButtonHide(int row,bool hide)
{
    QObjectList list = ui->buttons_GroupBox->children();

    if(-1 == row)
    {
        foreach(QObject* object,list)
        {
            if(object->objectName().contains("view") ||object->objectName().contains("import") )
            {
                QPushButton* button = dynamic_cast<QPushButton*>(object);
                button->setVisible(!hide);
            }

        }
    }
    else
    {
        foreach(QObject* object,list)
        {
            if(object->objectName().contains(QString("view%1").arg(row)) || object->objectName().contains(QString("import%1").arg(row)))
            {
                QPushButton* button = dynamic_cast<QPushButton*>(object);
                button->setVisible(!hide);
            }
        }
    }
}

void TrainingHistory::slotCurrentUserChanged()
{
    flushTableData();
}

void TrainingHistory::pixmapImportPDF(const QPixmap &pixmap,QString fileName)
{
    QString filePath;

    filePath = QFileDialog::getSaveFileName(NULL,"保存文件",fileName,tr("*.pdf"));

    QFile pdfFile(filePath);
    if(!pdfFile.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(NULL,"导出文件","文件打开失败");
        return;
    }
    QPdfWriter *pdfWriter = new QPdfWriter(&pdfFile);
    QPainter *pic_painter = new QPainter(pdfWriter);
    pdfWriter->setPageSize(QPagedPaintDevice::A4);
    pdfWriter->setResolution(QPrinter::ScreenResolution);

    QRect rect = pic_painter->viewport();
    int factor = rect.width()/pixmap.width();
    pic_painter->scale(factor,factor);
    pic_painter->drawPixmap(10,10,pixmap);

    delete pic_painter;
    delete pdfWriter;

    pdfFile.close();
}
