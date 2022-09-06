#include "teachmanagerdialog.h"
#include "ui_teachmanagerdialog.h"
#include <QListWidgetItem>
#include <QFont>
#include "cdatabaseinterface.h"
#include <QDebug>
#include "userdataformate.h"
#include "controlmaskdialog.h"
#include <QKeyEvent>
#include <QMessageBox>
#include "logger.h"
#include <QPainter>
LOG4QT_DECLARE_STATIC_LOGGER(logger, TeachManager)

TeachManagerDialog::TeachManagerDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeachManagerDialog),
    m_currentPage(1),
    m_totalPage(0),
    m_totalUserNum(0)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose,false);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    loadStyleSheetFile();

    connect(ui->listWidget,&QListWidget::itemDoubleClicked,this,&TeachManagerDialog::slotSelectItemDoubleClicked);
}

TeachManagerDialog::~TeachManagerDialog()
{
    delete ui;
}

void TeachManagerDialog::updateRecipeList()
{
    QString query("select * from tracktable");
    CDatabaseInterface::getInstance()->exec(query);
    int count = CDatabaseInterface::getInstance()->getValuesSize();
    m_totalUserNum = count;
    m_recipeValueList.clear();
    m_recipeValueList = CDatabaseInterface::getInstance()->getValues(0,count);
    //总共要放的页数
    m_totalPage = count/PERCOUNT + 1;
    ui->page_Label->setText(QString("1/%1").arg(m_totalPage));
    //显示首页数据
    showUserData(1);
}

void TeachManagerDialog::slotSelectItemDoubleClicked(QListWidgetItem *item)
{

    QString itemText = item->text();
    if(itemText.contains("\r"))
    {
        int index = itemText.indexOf("\r");
        itemText.remove(index,itemText.length()-index);
    }
    this->close();
}

void TeachManagerDialog::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
//    updateRecipeList();
}

void TeachManagerDialog::on_left_Btn_clicked()
{
    if(m_currentPage < 2)
        return;
    m_currentPage--;
    showUserData(m_currentPage);
}

void TeachManagerDialog::on_right_Btn_clicked()
{
    if(m_currentPage < m_totalPage)
    {
        m_currentPage++;
        showUserData(m_currentPage);
    }
}

void TeachManagerDialog::slotUpdateDatabase()
{
    updateRecipeList();
}

void TeachManagerDialog::on_search_Btn_clicked()
{
    //首先清理界面
    ui->listWidget->clear();
    //检索的情况下不显示添加按钮，只显示符合检索条件的内容
    QString msg = ui->search_LineEdit->text();
    QString query(QString("select * from tracktable where author = '%1' or trackName = '%2'").arg(msg).arg(msg));
    QList<QVariantMap> valueMapList;
    if(CDatabaseInterface::getInstance()->exec(query))
        valueMapList = CDatabaseInterface::getInstance()->getValues(0,8);
    if(0 == valueMapList.count())
    {
        QMessageBox::warning(NULL,"提示","未找到合适的数据");
        return;
    }

    QListWidget *listWidget = ui->listWidget;

    listWidget->setViewMode(QListView::IconMode);
    listWidget->setMovement(QListView::Static);
    listWidget->setIconSize(QSize(130,90));
    listWidget->setSpacing(20);
    QFont font;
    font.setFamily("黑体");
    font.setPointSize(20);
    //要添加的个数
    int addNum = qMin(valueMapList.count(),PERCOUNT);
    for(int i = 0;i < addNum;i++)
    {
        QVariantMap vMap = m_recipeValueList.at(i);
        //图片路径
        QString pixPath = vMap.value("trackImagePath").toString();
        //处方名称
        QString trackName = vMap.value("trackName").toString();
        //创建人
        QString author = vMap.value("author").toString();
        QString text = trackName+"\r\n"+author;
        QListWidgetItem *addItem = new QListWidgetItem(QIcon(pixPath),text);
        addItem->setFont(font);
        ui->listWidget->addItem(addItem);
    }
}

void TeachManagerDialog::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case 16777220:
        on_search_Btn_clicked();
        break;
    }
}

void TeachManagerDialog::showUserData(uint16_t pageIndex)
{
    ui->listWidget->clear();
    connect(ui->listWidget,&QListWidget::itemDoubleClicked,this,&TeachManagerDialog::slotSelectItemDoubleClicked);
    ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->setMovement(QListView::Static);
    ui->listWidget->setIconSize(QSize(130,90));
    ui->listWidget->setSpacing(20);
    QFont font;
    font.setFamily("黑体");
    font.setPointSize(20);
    //第一页
    if(1 == pageIndex)
    {
        //添加 “添加”Item
        QListWidgetItem *addItem = new QListWidgetItem(QIcon(":/new/prefix1/release/source/image/add2.png"),"添加处方");

        addItem->setFont(font);
        ui->listWidget->addItem(addItem);
        //添加其他患者,第一页最多添加七个
        int addNum = qMin(m_totalUserNum,PERCOUNT-1);
        if(m_recipeValueList.size() >= addNum)
            for(int i = 0;i < addNum;i++)
            {
                QVariantMap vMap = m_recipeValueList.at(i);
                //图片路径
                QString pixPath = vMap.value("trackImagePath").toString();
                //处方名称
                QString trackName = vMap.value("trackName").toString();
                //创建人
                QString author = vMap.value("author").toString();
                QString text = trackName+"\r\n"+author;
                QListWidgetItem *addItem = new QListWidgetItem(QIcon(pixPath),text);
                addItem->setFont(font);
                ui->listWidget->addItem(addItem);
            }
    }
    //其它页
    else
    {
        //根据页数计算要添加的内容
        int currentUserIndex = (pageIndex-2)*PERCOUNT + (PERCOUNT-1);
        int tobeAdd = m_totalUserNum - (pageIndex-2)*PERCOUNT - (PERCOUNT-1);

        tobeAdd = qMin(tobeAdd,PERCOUNT);

        for(int i = 0;i < tobeAdd;i++)
        {
            if(m_recipeValueList.size() >i+currentUserIndex)
            {
                QVariantMap vMap = m_recipeValueList.at(i+currentUserIndex);
                //图片路径
                QString pixPath = vMap.value("trackImagePath").toString();
                //处方名称
                QString trackName = vMap.value("trackName").toString();
                //创建人
                QString author = vMap.value("author").toString();
                QString text = trackName+"\r\n"+author;
                QListWidgetItem *addItem = new QListWidgetItem(QIcon(pixPath),text);
                addItem->setFont(font);
                ui->listWidget->addItem(addItem);
            }
        }
    }
}

void TeachManagerDialog::on_search_LineEdit_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
    {
        updateRecipeList();
    }
}

void TeachManagerDialog::loadStyleSheetFile()
{
    //加载样式文件
    QFile qssFile("./QSS/teachmanager.qss");
    if(!qssFile.open(QIODevice::ReadOnly))
        logger()->debug("teachmanager.qss文件打开失败!");
    else
    {
        this->setStyleSheet(qssFile.readAll());
        qssFile.close();
    }
}

void TeachManagerDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.fillRect(rect(),QColor(0,0,0,200));
}
