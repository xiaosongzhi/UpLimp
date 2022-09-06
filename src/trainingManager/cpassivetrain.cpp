#include "cpassivetrain.h"
#include "ui_cpassivetrain.h"
#include "gameparamcontrol.h"
#include <QFile>
#include "logger.h"
#include <QDebug>

LOG4QT_DECLARE_STATIC_LOGGER(logger, ScopeManager)

CPassiveTrain::CPassiveTrain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CPassiveTrain),
    isRecipeSelected(false)
{
    ui->setupUi(this);
    ui->gameLevel_ListWidget->setVisible(false);
    loadStyleSheetFile();
    ui->track_StackedWidget->setCurrentIndex(1);
}

CPassiveTrain::~CPassiveTrain()
{
    delete ui;
}
void CPassiveTrain::loadStyleSheetFile()
{
    //加载样式文件s
    QFile qssFile("./QSS/trainPageFont.qss");
    if(!qssFile.open(QIODevice::ReadOnly))
        logger()->debug("trainPageFont.qss文件打开失败!");
    else
    {
        this->setStyleSheet(qssFile.readAll());
        qssFile.close();
    }
}

ST_PassiveParam CPassiveTrain::getPassiveGameParam()
{
    ST_PassiveParam st_Param;
    st_Param.gameName = m_currentText;
    st_Param.trainMode = E_TRAINMODE_PASSIVE;
    st_Param.gamePath = GameParamControl::getInstance()->getGameMsgByName(m_currentText).gamePath;

    //路径
    QString gameTrack(QApplication::applicationDirPath() + "/recipe/gameData/");
    QString gameTrackImage("./recipe/image/");
#ifdef ABSOLUTEPATH
    gameTrack = QApplication::applicationDirPath() + "/recipe/data/";
#endif
    if(ui->upDown_RadioButton->isChecked())
    {   //文件路径
        gameTrack.append("default/upDown.csv");
        gameTrackImage.append("upDown.png");
    }
    else if(ui->leftRight_radioButton->isChecked())
    {
        gameTrack.append("default/leftRight.csv");
        gameTrackImage.append("leftRight.png");
    }
    else if(ui->pie_RadioButton->isChecked())
    {
        gameTrack.append("default/pie.csv");
        gameTrackImage.append("pie.png");
    }
    else if(ui->circle_RadioButton->isChecked())
    {
        gameTrack.append("default/circle.csv");
        gameTrackImage.append("circle.png");
    }
    else if(ui->rect_RadioButton->isChecked())
    {
        gameTrack.append("default/rect.csv");
        gameTrackImage.append("rect.png");
    }
    else if(ui->s_RadioButton->isChecked())
    {
        gameTrack.append("default/s.csv");
        gameTrackImage.append("s.png");
    }
    else
        gameTrack.append("default/");

    st_Param.gameTrack = gameTrack;
    if("海底冒险" == m_currentText)
        st_Param.gameTrackImage = "random";
    else
        st_Param.gameTrackImage = gameTrackImage;

    if(ui->recipe_RadioButton->isChecked())
    {
        st_Param.gameTrackImage = "处方";
        isRecipeSelected = true;
    }
    else
        isRecipeSelected = false;

    //活动范围
    if(ui->small_RadioButton->isChecked())
    {
        st_Param.scorpSize = 1;
    }
    else if(ui->median_RadioButton->isChecked())
    {
        st_Param.scorpSize = 2;
    }
    else if(ui->big_radioButton->isChecked())
    {
        st_Param.scorpSize = 3;
    }

    //训练速度
    if(ui->lowSpeed_RadioButton->isChecked())
    {
        st_Param.speed = 1;
    }
    else if(ui->middleSpeed_RadioButton->isChecked())
    {
        st_Param.speed = 2;
    }
    else if(ui->highSpeed_RadioButton->isChecked())
    {
        st_Param.speed = 3;
    }
    //训练流程
    st_Param.trainProcessMap.insert(0,ui->warmUp_ComboBox->currentText().toInt());
    st_Param.trainProcessMap.insert(1,ui->train_ComboBox->currentText().toInt());
    st_Param.trainProcessMap.insert(2,ui->relax_ComboBox->currentText().toInt());
    return st_Param;
}

bool CPassiveTrain::recipeIsSelected()
{
    return isRecipeSelected;
}

void CPassiveTrain::setGameList(QList<ST_GameMsg>& gameList)
{
    ui->gameType_ListWidget->setSpacing(10);
    ui->gameType_ListWidget->setMovement(QListView::Static);
    ui->gameType_ListWidget->setViewMode(QListView::IconMode);
    ui->gameType_ListWidget->setIconSize(QSize(60,60));
    connect(ui->gameType_ListWidget,SIGNAL(currentTextChanged(QString)),this,SLOT(slotGetCurrentText(QString)));

    for(int i = 0;i < gameList.size();i++)
    {
        if(gameList.at(i).suitTypeList.contains("0"))
        {
            QString pixPath("./gameExecutable/gameIcon/");
#ifdef ABSOLUTEPATH
            pixPath = QApplication::applicationDirPath() + "/gameExecutable/gameIcon/";
#endif
            pixPath.append(gameList.at(i).iconPath);
            QListWidgetItem *addItem = new QListWidgetItem(QIcon(pixPath),gameList.at(i).gameName);
            ui->gameType_ListWidget->addItem(addItem);
        }
    }
    init();
}

void CPassiveTrain::slotGetCurrentText(QString text)
{
    m_currentText = text;

    if("地下钻探" == m_currentText)
    {
        ui->track_StackedWidget->setCurrentIndex(0);
        ui->scope_GroupBox->setVisible(false);
    }

    else if("海底冒险" == m_currentText)
    {
        ui->track_StackedWidget->setCurrentIndex(1);
        ui->scope_GroupBox->setVisible(true);
    }
}

void CPassiveTrain::init()
{
    ui->upDown_RadioButton->setChecked(true);
    ui->small_RadioButton->setChecked(true);
    ui->lowSpeed_RadioButton->setChecked(true);
    if(ui->gameType_ListWidget->count() != 0)
    {
        ui->gameType_ListWidget->item(0)->setSelected(true);
        m_currentText = ui->gameType_ListWidget->item(0)->text();
    }
}

void CPassiveTrain::on_recipe_RadioButton_toggled(bool checked)
{
    if(checked)
        emit signalRecipeChecked(true);
}
