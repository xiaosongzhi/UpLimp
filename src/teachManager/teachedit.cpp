#include "teachedit.h"
#include "ui_teachedit.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include "cdatabaseinterface.h"
#include <QMessageBox>
#include "clistwidgetitem.h"
#include "controlmaskdialog.h"

TeachEdit::TeachEdit(QWidget *parent) :
    CBaseDialog(parent),
    ui(new Ui::TeachEdit)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::FramelessWindowHint);
    connect(ui->paint_Widget,SIGNAL(signalAddNewPoint(QString,QPoint)),this,SLOT(slotAddNewPoint(QString,QPoint)));
    connect(ui->paint_Widget,SIGNAL(signalNodeChanged(QString,QPoint)),this,SLOT(slotNodeChanged(QString,QPoint)));
    connect(ui->paint_Widget,SIGNAL(signalAddNewNodeFinished()),this,SLOT(slotNodeAddFinished()));
    init();
    this->move(150,150);
}

TeachEdit::~TeachEdit()
{
    delete ui;
}

void TeachEdit::clearWidgetList()
{
    while (!widgetList.isEmpty())
    {
        delete widgetList.takeFirst();
    }
    while(!itemList.isEmpty())
    {
        delete itemList.takeFirst();
    }
}

void TeachEdit::switchType(E_SHOWTYPE type)
{
    clearWidgetList();
    //添加按钮使能
    ui->add_Btn->setEnabled(true);
    switch(type)
    {
    case ADDTYPE:
        setTitile("添加处方");
        setBtnVisble(true);
        ui->paint_Widget->setEnabled(true);
        ui->author_LineEdit->clear();
        ui->recipeName_LineEdit->clear();
        break;
    case EDITTYPE:
        setTitile("编辑处方");
        setBtnVisble(false);
        ui->paint_Widget->setEnabled(false);
        break;
    default:
        break;
    }
}

void TeachEdit::setBtnVisble(bool ok)
{
    ui->add_Btn->setVisible(ok);
    ui->clear_Btn->setVisible(ok);
    ui->save_Btn->setVisible(ok);
    ui->edit_Btn->setVisible(!ok);
    ui->delete_Btn->setVisible(!ok);
    ui->confirm_Btn->setVisible(!ok);
    ui->author_LineEdit->setEnabled(ok);
    ui->recipeName_LineEdit->setEnabled(ok);
}

void TeachEdit::init()
{
    ui->paint_Widget->clearPaintDevice();

    clearWidgetList();
}
void TeachEdit::on_add_Btn_clicked()
{
    bool isAdding = true;
    ui->paint_Widget->startAdd(isAdding);
}

void TeachEdit::on_clear_Btn_clicked()
{
    ui->paint_Widget->clearPaintDevice();
    ui->add_Btn->setEnabled(true);
    clearWidgetList();
}

void TeachEdit::on_save_Btn_clicked()
{
    if(ui->recipeName_LineEdit->text().isEmpty())
    {
        QMessageBox::warning(NULL,"提示","处方名不能为空");
        return;
    }
    if(ui->author_LineEdit->text().isEmpty())
    {
        QMessageBox::warning(NULL,"提示","创建者不能为空");
        return;
    }
    if(ui->paint_Widget->saveTrackData(ui->recipeName_LineEdit->text(),ui->author_LineEdit->text()))
    {
        emit signalUpdateDatabase();
        QMessageBox::warning(NULL,"提示","保存成功");
        this->close();
        ControlMaskDialog::getInstace()->setMaskDialogHide(true);
    }
}

void TeachEdit::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    ui->paint_Widget->clearPaintDevice();
    ui->author_LineEdit->clear();
    ui->recipeName_LineEdit->clear();
    init();
}

void TeachEdit::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)

}

void TeachEdit::on_edit_Btn_clicked()
{
    ui->paint_Widget->setEnabled(true);
}
//编辑状态
void TeachEdit::setRecipe(QString trackName)
{
    QString query = QString("select * from tracktable where trackName = '%1'").arg(trackName);

    CDatabaseInterface::getInstance()->exec(query);
    if(CDatabaseInterface::getInstance()->getValuesSize() > 0)
    {
        QList<QVariantMap> listMap = CDatabaseInterface::getInstance()->getValues(0,1);
        QString pixPath = listMap.at(0).value("trackImagePath").toString();
        QString filePath = listMap.at(0).value("trackDataFile").toString();
        QString author = listMap.at(0).value("author").toString();
        QString trackName = listMap.at(0).value("trackName").toString();
        ui->author_LineEdit->setText(author);
        ui->recipeName_LineEdit->setText(trackName);

        QFile file(filePath);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            QList<QPoint> pointList;
            pointList.clear();
            while(!in.atEnd())
            {
                QString str = in.readLine();
                if(str.contains("["))
                    str.remove("[");
                if(str.contains("]"))
                    str.remove("]");
                QStringList list = str.split(',');
                QPoint point(list.at(0).toInt(),list.at(1).toInt());

                pointList.append(point);
            }
            if(!pointList.isEmpty())
            {
                ui->paint_Widget->paintPointList(pointList);
            }
        }
        else
        {
            qDebug()<<"文件打开失败";
        }
    }
}

void TeachEdit::on_delete_Btn_clicked()
{
    if(ui->paint_Widget->deleteTrackData(ui->recipeName_LineEdit->text(),ui->author_LineEdit->text()))
    {
        emit signalUpdateDatabase();
        QMessageBox::warning(NULL,"提示","删除成功");
    }
    this->close();
    ControlMaskDialog::getInstace()->setMaskDialogHide(true);
}

void TeachEdit::on_confirm_Btn_clicked()
{
    if(ui->recipeName_LineEdit->text().isEmpty())
    {
        QMessageBox::warning(NULL,"提示","处方名不能为空");
        return;
    }
    if(ui->author_LineEdit->text().isEmpty())
    {
        QMessageBox::warning(NULL,"提示","创建者不能为空");
        return;
    }
    if(ui->paint_Widget->updateTrackData(ui->recipeName_LineEdit->text(),ui->author_LineEdit->text()))
    {
        emit signalUpdateDatabase();
        QMessageBox::warning(NULL,"提示","更新成功");
        this->close();
        ControlMaskDialog::getInstace()->setMaskDialogHide(true);
    }
}

void TeachEdit::addListWidgetItem(QString seq,QPoint point)
{
    ui->listWidget->setSpacing(10);

    QListWidgetItem *item = new QListWidgetItem();
    itemList.append(item);
    item->setSizeHint(QSize(300,60));
    ui->listWidget->setStyleSheet("QListWidget::Item{border:none;}");
    ui->listWidget->setStyleSheet("QListWidget::Item{border-image: url(:/new/prefix1/release/source/image/teachItem.png);}");
    ui->listWidget->addItem(item);
    CListWidgetItem *widget = new CListWidgetItem(seq,point);
    widgetList.append(widget);
    ui->listWidget->setItemWidget(item,widget);
}

void TeachEdit::slotAddNewPoint(QString index,QPoint point)
{
    addListWidgetItem(index,point);
}

//拖拽右侧点，左侧列表中位置进行更新
void TeachEdit::slotNodeChanged(QString text,QPoint point)
{
    int index = text.toInt()-1;
    if(widgetList.size() > index)
        widgetList.at(index)->setPointText(point);
}

void TeachEdit::slotNodeAddFinished()
{
    ui->add_Btn->setEnabled(false);
}

void TeachEdit::on_close_Btn_clicked()
{
    ControlMaskDialog::getInstace()->setMaskDialogHide(true);
    this->close();
}
