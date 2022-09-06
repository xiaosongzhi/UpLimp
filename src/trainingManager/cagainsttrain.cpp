#include "cagainsttrain.h"
#include "ui_cagainsttrain.h"
#include "gameparamcontrol.h"
#include <QFile>
#include "logger.h"
#include <QDebug>
LOG4QT_DECLARE_STATIC_LOGGER(logger, ScopeManager)

CAgainstTrain::CAgainstTrain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CAgainstTrain),
    isRecipeSelected(false)
{
    ui->setupUi(this);
    ui->gameLevel_ListWidget->setVisible(false);
    loadStyleSheetFile();

    ui->warmUp_ComboBox->setEnabled(false);
    ui->relax_ComboBox->setEnabled(false);
}

CAgainstTrain::~CAgainstTrain()
{
    delete ui;
}

void CAgainstTrain::loadStyleSheetFile()
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

void CAgainstTrain::init()
{
    /*
    ui->upDown_RadioButton->setChecked(true);
    ui->small_RadioButton->setChecked(true);
    ui->level1_RadioButton->setChecked(true);
    */
    if(ui->gameType_ListWidget->count() != 0)
    {
        ui->gameType_ListWidget->item(0)->setSelected(true);
        m_currentText = ui->gameType_ListWidget->item(0)->text();
    }
}


ST_AgainstParam CAgainstTrain::getAgainstGameParam()
{
    ST_AgainstParam st_Param;
    st_Param.gameName = m_currentText;
    st_Param.gamePath = GameParamControl::getInstance()->getGameMsgByName(m_currentText).gamePath;
    st_Param.trainMode = E_TRAINMODE_AGAINST;
    //运动轨迹
    QString gameTrack("./recipe/data/");
    QString gameTrackImage("./recipe/image/random");
#ifdef ABSOLUTEPATH
    gameTrack = QApplication::applicationDirPath() + "/recipe/data/";
#endif
    /*
    if(ui->upDown_RadioButton->isChecked())
    {   //文件路径
        gameTrack.append("upDown.csv");
        gameTrackImage.append("upDown.png");
    }
    else if(ui->leftRight_radioButton->isChecked())
    {
        gameTrack.append("leftRight.csv");
        gameTrackImage.append("leftRight.png");
    }
    else if(ui->pie_RadioButton->isChecked())
    {
        gameTrack.append("pie.csv");
        gameTrackImage.append("pie.png");
    }
    else if(ui->circle_RadioButton->isChecked())
    {
        gameTrack.append("circle.csv");
        gameTrackImage.append("circle.png");
    }
    else if(ui->rect_RadioButton->isChecked())
    {
        gameTrack.append("rect.csv");
        gameTrackImage.append("rect.png");
    }
    else if(ui->s_RadioButton->isChecked())
    {
        gameTrack.append("s.csv");
        gameTrackImage.append("s.png");
    }
    if(ui->recipe_RadioButton->isChecked())
        isRecipeSelected = true;
    else
        isRecipeSelected = false;
    */
    isRecipeSelected = false;
    st_Param.gameTrack = gameTrack;
    st_Param.gameTrackImage = gameTrackImage;
    //活动范围
    if(ui->small_RadioButton->isChecked())
        st_Param.scorpSize = 1;
    else if(ui->median_RadioButton->isChecked())
        st_Param.scorpSize = 2;
    else if(ui->big_radioButton->isChecked())
        st_Param.scorpSize = 3;

    //模拟阻力
    if(ui->level1_RadioButton->isChecked())
        st_Param.level = 1;
    else if(ui->level2_RadioButton->isChecked())
        st_Param.level = 2;
    else if(ui->level3_RadioButton->isChecked())
        st_Param.level = 3;
    else if(ui->level4_RadioButton->isChecked())
        st_Param.level = 4;
    else if(ui->level5_RadioButton->isChecked())
        st_Param.level = 5;

    //训练流程
    st_Param.trainProcessMap.insert(0,ui->warmUp_ComboBox->currentText().toInt());
    st_Param.trainProcessMap.insert(1,ui->train_ComboBox->currentText().toInt());
    st_Param.trainProcessMap.insert(2,ui->relax_ComboBox->currentText().toInt());

    return st_Param;
}

bool CAgainstTrain::recipeIsSelected()
{
    return isRecipeSelected;
}

void CAgainstTrain::setGameList(QList<ST_GameMsg>& gameList)
{
    ui->gameType_ListWidget->setSpacing(10);
    ui->gameType_ListWidget->setMovement(QListView::Static);
    ui->gameType_ListWidget->setViewMode(QListView::IconMode);
    ui->gameType_ListWidget->setIconSize(QSize(200,150));

    connect(ui->gameType_ListWidget,SIGNAL(currentTextChanged(QString)),this,SLOT(slotGetCurrentText(QString)));
    for(int i = 0;i < gameList.size();i++)
    {
        if(gameList.at(i).suitTypeList.contains("3"))
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

void CAgainstTrain::slotGetCurrentText(QString text)
{
    m_currentText = text;
}
