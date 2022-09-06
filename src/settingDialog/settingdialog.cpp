#include "settingdialog.h"
#include "ui_settingdialog.h"
#include "ccommunicateapi.h"
#include "dataFormate.h"
#include <QDebug>
SettingDialog::SettingDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    ui->groupBox->setEnabled(false);
}

SettingDialog::~SettingDialog()
{
    delete ui;
}


void SettingDialog::on_speedTest_Btn_clicked()
{
    ST_ControlData   st_ControlData;
    int speed = ui->speed_LineEdit->text().toInt();
    //    int index = ui->axis_ComboBox->currentIndex();
    int mode = ui->mode_ComboBox->currentIndex();
    st_ControlData.E_Mode = (E_TRAINMODE)mode;
    st_ControlData.st_EndPort.x = ui->x_LineEdit->text().toUInt();
    st_ControlData.st_EndPort.y = ui->y_LineEdit->text().toUInt();
    st_ControlData.speed = speed;
    QByteArray sendArray;
    sendArray.resize(sizeof(ST_ControlData));
    memcpy(sendArray.data(),&st_ControlData,sendArray.length());
    CCommunicateAPI::getInstance()->sendData(0,2,SPEEDTEST,sendArray);
}

void SettingDialog::on_boostTest_Btn_clicked()
{
    ST_ControlData   st_ControlData;
    int speed = ui->speed_LineEdit->text().toInt();
    int index = ui->axis_ComboBox->currentIndex();
    switch(index)
    {
    case 0:
        index = XBOOSTTEST;
        break;
    case 1:
        index = YBOOSTTEST;
        break;
    }
    int mode = ui->mode_ComboBox->currentIndex();
    st_ControlData.E_Mode = (E_TRAINMODE)mode;
    st_ControlData.st_EndPort.x = ui->x_LineEdit->text().toUInt();
    st_ControlData.st_EndPort.y = ui->y_LineEdit->text().toUInt();
    st_ControlData.speed = speed;
    st_ControlData.force = ui->strength_LineEdit->text().toInt();
    QByteArray sendArray;
    sendArray.resize(sizeof(ST_ControlData));
    memcpy(sendArray.data(),&st_ControlData,sendArray.length());
    CCommunicateAPI::getInstance()->sendData(0,2,index,sendArray);
    qDebug()<<"index"<<index;
}

void SettingDialog::on_againstTest_Btn_clicked()
{
    ST_ControlData   st_ControlData;
    int speed = ui->speed_LineEdit->text().toInt();
    int index = ui->axis_ComboBox->currentIndex();
    switch(index)
    {
    case 0:
        index = XAGAINSTTEST;
        break;
    case 1:
        index = YAGAINSTTEST;
        break;
    }

    int mode = ui->mode_ComboBox->currentIndex();
    st_ControlData.E_Mode = (E_TRAINMODE)mode;
    st_ControlData.st_EndPort.x = ui->x_LineEdit->text().toUInt();
    st_ControlData.st_EndPort.y = ui->y_LineEdit->text().toUInt();
    st_ControlData.force = ui->strength_LineEdit->text().toInt();
    st_ControlData.speed = speed;
    QByteArray sendArray;
    sendArray.resize(sizeof(ST_ControlData));
    memcpy(sendArray.data(),&st_ControlData,sendArray.length());
    CCommunicateAPI::getInstance()->sendData(0,2,index,sendArray);
    qDebug()<<"index"<<index;
}

void SettingDialog::on_goBack_Btn_clicked()
{
    QByteArray sendArray(1,0);
    CCommunicateAPI::getInstance()->sendData(0,2,GOBACKTESTPOINT,sendArray);
}

void SettingDialog::on_switchLock_Btn_clicked()
{

}

void SettingDialog::on_stop_Btn_clicked()
{
    QByteArray sendArray(0);
    CCommunicateAPI::getInstance()->sendData(0,2,STOPTRAINING,sendArray);
}

void SettingDialog::on_startTest_Btn_clicked()
{
    if(ui->startTest_Btn->text() == "开始测试")
    {
        ui->startTest_Btn->setText("停止测试");
        g_isTesting = 1;
        ui->groupBox->setEnabled(true);
    }
    else
    {
        ui->startTest_Btn->setText("开始测试");
        g_isTesting = 0;
        ui->groupBox->setEnabled(false);
    }
}
