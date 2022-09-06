#include "edituser.h"
#include "ui_edituser.h"
#include <qvalidator.h>
#include "cdatabaseinterface.h"
#include <QDebug>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QSettings>
#include <QDir>
#include "currentuserdata.h"
#include "cpassworddialog.h"
#include "logger.h"

LOG4QT_DECLARE_STATIC_LOGGER(logger, EditUser)

EditUser::EditUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditUser)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_QuitOnClose,false);
    setLineEditFormat();
    initWidgetUI();
    loadStyleSheetFile();
}

EditUser::~EditUser()
{
    delete ui;
}

void EditUser::loadStyleSheetFile()
{
    //加载样式文件s
    QFile qssFile("./QSS/editUserfont.qss");
    if(!qssFile.open(QIODevice::ReadOnly))
        logger()->debug("editUserfont.qss文件打开失败!");
    else
    {
        this->setStyleSheet(qssFile.readAll());
        qssFile.close();
    }
}


//设置输入限制
void EditUser::setLineEditFormat()
{
    //待添加
//    QIntValidator ageLimit( 0, 150, this );
//    ui->age_LineEdit->setValidator(&ageLimit);
//    QIntValidator heightLimit( 0, 250, this );
//    ui->height_LineEdit->setValidator(&heightLimit);
//    QIntValidator weightLimit( 0, 250, this );
//    ui->weight_LineEdit->setValidator(&weightLimit);
/*
    // 整形 范围：[1, 99]
    QIntValidator *pIntValidator = new QIntValidator(this);
    pIntValidator->setRange(1, 99);

    // 浮点型 范围：[-360, 360] 精度：小数点后2位
    QDoubleValidator *pDoubleValidator = new QDoubleValidator(this);
    pDoubleValidator->setRange(0, 100);
    pDoubleValidator->setNotation(QDoubleValidator::StandardNotation);
    pDoubleValidator->setDecimals(2);

    // 字符和数字
    QRegExp reg("[a-zA-Z0-9]+$");
    QRegExpValidator *pValidator = new QRegExpValidator(this);
    pValidator->setRegExp(reg);

    ui->ID_LineEdit->setValidator(pValidator);
//    ui->name_LineEdit->setText();
*/
    //名字
    //ID号 由系统分配，全局唯一
    //年龄、身高、体重
    QIntValidator *pIntValidator = new QIntValidator(this);
    pIntValidator->setRange(1, 150);    //年龄
    ui->age_LineEdit->setValidator(pIntValidator);
    pIntValidator->setRange(1, 250);    //身高
    ui->height_LineEdit->setValidator(pIntValidator);
    pIntValidator->setRange(1, 300);
    ui->weight_LineEdit->setValidator(pIntValidator);
    //电话
    QRegExp reg("[0-9]+$");
    QRegExpValidator *pValidator = new QRegExpValidator(this);
    pValidator->setRegExp(reg);
    ui->patientPhone_LineEdit->setMaxLength(11);
    ui->guardian_LineEdit->setMaxLength(11);
    ui->patientPhone_LineEdit->setValidator(pValidator);
    ui->guardian_LineEdit->setValidator(pValidator);
    //家庭住址
    ui->address_LineEdit->setMaxLength(30);
    //备注信息
    ui->remark_LineEdit->setMaxLength(100);

}

void EditUser::switchPageType(QString ID,int type)
{
    switch(type)
    {
    case 0:     //添加新用户
        addUserPage();
        setTitile("新建用户");
        break;
    case 1:     //编辑用户
        editUserPage(ID);
        setTitile("编辑用户");
        break;
    default:
        break;
    }
}

void EditUser::setTitile(QString title)
{
    ui->title_Label->setText(title);
}

void EditUser::addUserPage()
{
    ui->delete_Btn->setVisible(false);
    ui->modify_Btn->setVisible(false);
    ui->confirm_Btn->setVisible(false);
    ui->save_Btn->setVisible(true);
    clearuserData();
    //读取当前用户ID
    readSetting();
    ui->ID_LineEdit->setText(QString::number(m_currentID+1));
    //默认男性用户
    on_male_RadioButton_clicked();

}
void EditUser::editUserPage(QString ID)
{
    ui->delete_Btn->setVisible(true);
    ui->modify_Btn->setVisible(true);
    ui->confirm_Btn->setVisible(true);
    ui->save_Btn->setVisible(false);

    QString query = QString("select * from patientmessagetable where ID = '%1'").arg(ID);

    if(CDatabaseInterface::getInstance()->exec(query))
    {
        if(int count = CDatabaseInterface::getInstance()->getValuesSize() > 0)
        {
            QList<QVariantMap>valueList = CDatabaseInterface::getInstance()->getValues(0,count);
            ST_PatientMsg st_patientMsg = variantMapToPatientMsg(valueList.at(0));
            setUserData(st_patientMsg);
        }
    }
    else
    {
        QString errorStr = CDatabaseInterface::getInstance()->getLastError();
        QMessageBox::warning(NULL,"警告",errorStr);
    }
}

void EditUser::on_delete_Btn_clicked()
{
    CPasswordDialog m_dialog;
    int result = m_dialog.exec();
    if(result != 3)
        return;

    //从数据库中将该患者信息删除
    QString ID = ui->ID_LineEdit->text();
    QString dataBaseName("patientmessagetable");
    if(CDatabaseInterface::getInstance()->deleteRowTable(dataBaseName,"ID",ID))
    {
        logger()->debug(QString("ID=%1的用户删除成功").arg(ID));
    }
    else
        logger()->debug(CDatabaseInterface::getInstance()->getLastError());
    this->close();
    //返回用户管理界面
    //从数据库中将该患者信息进行更新
    emit signalUpdateUserList();
    emit signalUserSelected(0);
}

void EditUser::on_modify_Btn_clicked()
{
    CPasswordDialog m_dialog;
    int result = m_dialog.exec();
    if(result != 3)
        return;
    //从数据库中将该患者信息进行更新
    QString tableName("patientmessagetable");
    QVariantMap vMap = patientMsgToVariantMap(getUserData());
    if(!CDatabaseInterface::getInstance()->updateRowTable(tableName,"ID",vMap))
    {
        logger()->debug(CDatabaseInterface::getInstance()->getLastError());
        //进入用户管理界面
        emit signalUserSelected(0);
    }
    else
    {
        emit signalUpdateUserList();
        this->close();
        //进入训练界面
        emit signalUserSelected(1);
    }
}

void EditUser::on_confirm_Btn_clicked()
{
    //直接退出该界面，设置该用户为当前用户
    this->close();
    //进入训练界面
    emit signalUserSelected(1);
    //设置当前用户
    CurrentUserData::getInstace()->setCurrentUserID(ui->ID_LineEdit->text().toUInt());
}

void EditUser::on_save_Btn_clicked()
{
    //将该用户添加到数据库中
    QString tableName("patientmessagetable");
    QVariantMap vMap = patientMsgToVariantMap(getUserData());

    if(!CDatabaseInterface::getInstance()->insertRowTable(tableName,vMap))
    {
        logger()->debug(CDatabaseInterface::getInstance()->getLastError());
        //进入用户管理界面
        emit signalUserSelected(0);
    }
    else
    {
        //数据插入成功
        emit signalUpdateUserList();
        //进入训练界面
        emit signalUserSelected(1);
        writeSetting();
    }
    this->close();
}

ST_PatientMsg EditUser::getUserData()
{
    ST_PatientMsg st_PatientMsg;
    //此处需要查询数据库ID
    st_PatientMsg.ID = ui->ID_LineEdit->text().toInt();
    st_PatientMsg.name = ui->name_LineEdit->text();
    st_PatientMsg.age = ui->age_LineEdit->text().toInt();
    if(ui->male_RadioButton->isChecked())
        st_PatientMsg.sex = 0;
    else if(ui->female_RadioButton->isChecked())
        st_PatientMsg.sex = 1;
    st_PatientMsg.height = ui->height_LineEdit->text().toInt();
    st_PatientMsg.weight = ui->weight_LineEdit->text().toInt();
    st_PatientMsg.phone = ui->patientPhone_LineEdit->text();
    st_PatientMsg.message = ui->userMsg_LineEdit->text();
    st_PatientMsg.guardianPhone = ui->guardian_LineEdit->text();
    st_PatientMsg.address = ui->address_LineEdit->text();
    st_PatientMsg.remarkMessage = ui->remark_LineEdit->text();

    return st_PatientMsg;
}


//设置用户信息
void EditUser::setUserData(ST_PatientMsg st_PatientMsg)
{
    ui->ID_LineEdit->setText(QString::number(st_PatientMsg.ID));
    ui->name_LineEdit->setText(st_PatientMsg.name);
    ui->age_LineEdit->setText(QString::number(st_PatientMsg.age));

    QString pixPath;

    if(0 == st_PatientMsg.sex)
    {
        ui->male_RadioButton->setChecked(true);
        ui->female_RadioButton->setChecked(false);
        pixPath = ":/new/prefix1/release/source/image/male.png";
    }
    else
    {
        ui->male_RadioButton->setChecked(false);
        ui->female_RadioButton->setChecked(true);
        pixPath = ":/new/prefix1/release/source/image/female.png";
    }
    ui->portrait_Label->setPixmap(pixPath);

    ui->height_LineEdit->setText(QString::number(st_PatientMsg.height));
    ui->weight_LineEdit->setText(QString::number(st_PatientMsg.weight));
    ui->patientPhone_LineEdit->setText(st_PatientMsg.phone);
    ui->userMsg_LineEdit->setText(st_PatientMsg.message);
    ui->guardian_LineEdit->setText(st_PatientMsg.guardian);
    ui->address_LineEdit->setText(st_PatientMsg.address);
    ui->remark_LineEdit->setText(st_PatientMsg.remarkMessage);

}

void EditUser::clearuserData()
{
    ui->ID_LineEdit->clear();
    ui->name_LineEdit->clear();
    ui->age_LineEdit->clear();

    ui->male_RadioButton->setChecked(true);
    ui->female_RadioButton->setChecked(false);

    ui->height_LineEdit->clear();
    ui->weight_LineEdit->clear();
    ui->patientPhone_LineEdit->clear();
    ui->userMsg_LineEdit->clear();
    ui->guardian_LineEdit->clear();
    ui->address_LineEdit->clear();
    ui->remark_LineEdit->clear();

}

void EditUser::on_leftLimp_Btn_clicked()
{
    ui->userMsg_LineEdit->setText("左侧上肢障碍");
}

void EditUser::on_rightLimp_Btn_clicked()
{
    ui->userMsg_LineEdit->setText("右侧上肢障碍");
}

void EditUser::on_allLimp_Btn_clicked()
{
    ui->userMsg_LineEdit->setText("双侧上肢障碍");
}

void EditUser::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    emit signalUserSelected(0);
}

void EditUser::initWidgetUI()
{
    QGraphicsDropShadowEffect * shadowEffect = new QGraphicsDropShadowEffect();
    //向四周均匀扩展阴影
    shadowEffect->setOffset(0,0);
    shadowEffect->setBlurRadius(10);
    shadowEffect->setColor(QColor("#BBBBBB"));
    ui->groupBox->setGraphicsEffect(shadowEffect);
}

//写配置
void EditUser::writeSetting()
{
    m_currentID++;
    QString dirPath("./conf/");
#ifdef ABSOLUTEPATH
    dirPath = QApplication::applicationDirPath() + "/conf/";
#endif
    QDir confdir(dirPath);
    if(!confdir.exists())
        confdir.mkdir(dirPath);
    QString confFile(dirPath + "IDconf.ini");
    QSettings iniSetting(confFile, QSettings::IniFormat);

    iniSetting.setValue("number",m_currentID);

}
//读配置
void EditUser::readSetting()
{
    QString dirPath("./conf/");
#ifdef ABSOLUTEPATH
    dirPath = QApplication::applicationDirPath() + "/conf/";
#endif
    QDir confdir(dirPath);
    if(!confdir.exists())
        confdir.mkdir(dirPath);
    QString confFile(dirPath + "IDconf.ini");
    QSettings iniSetting(confFile, QSettings::IniFormat);
    m_currentID = iniSetting.value("number").toUInt();
}

void EditUser::on_male_RadioButton_clicked()
{
    QString pixPath = ":/new/prefix1/release/source/image/male.png";
    ui->portrait_Label->setPixmap(pixPath);
}

void EditUser::on_female_RadioButton_clicked()
{
    QString pixPath = ":/new/prefix1/release/source/image/female.png";
    ui->portrait_Label->setPixmap(pixPath);
}

void EditUser::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.fillRect(rect(),QColor(0,0,0,127));
}

void EditUser::on_close_Btn_clicked()
{
    this->close();
}
