#include "gameparamcontrol.h"
#include <QFile>
#include <QDebug>
#include "logger.h"
#include <QTextStream>
LOG4QT_DECLARE_STATIC_LOGGER(logger, GameParamControl)
GameParamControl* GameParamControl::m_GameParamControl = NULL;

GameParamControl::GameParamControl(QObject *parent) : QObject(parent)
{

}
GameParamControl* GameParamControl::getInstance()
{
    if(!m_GameParamControl)
    {
        m_GameParamControl = new GameParamControl();
    }
    return m_GameParamControl;
}
void GameParamControl::writeXml()
{

}

void GameParamControl::readXml()
{

}

void GameParamControl::updateXml(ST_GameParam& st_GameParam)
{
    QFile file("./gameExecutable/gameConfig.xml");
    if(!file.open(QFile::ReadOnly))
        return;
    QDomDocument doc;

    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root = doc.documentElement();

    if(1)
    {
        QDomNodeList timeList = root.elementsByTagName("GameTime");
        QDomNode timeNode = timeList.at(0);
        QDomNode oldnode=timeNode.firstChild();
        timeNode.firstChild().setNodeValue(QString::number(st_GameParam.gameTime));
        QDomNode newNode = timeNode.firstChild();
        timeNode.replaceChild(newNode,oldnode);
    }
    if(1)
    {
        QDomNodeList levelList = root.elementsByTagName("Level");
        QDomNode levelNode = levelList.at(0);
        QDomNode oldLevelnode=levelNode.firstChild();
        levelNode.firstChild().setNodeValue(QString::number(st_GameParam.hardLevel));
        QDomNode newLevelNode = levelNode.firstChild();
        levelNode.replaceChild(newLevelNode,oldLevelnode);
    }
    if(1)
    {
        QDomNodeList List = root.elementsByTagName("Speed");
        QDomNode node = List.at(0);
        QDomNode oldLevelnode=node.firstChild();
        node.firstChild().setNodeValue(QString::number(st_GameParam.speed));
        QDomNode newLevelNode = node.firstChild();
        node.replaceChild(newLevelNode,oldLevelnode);
    }
    if(1)
    {
        QDomNodeList List = root.elementsByTagName("TrainingType");
        QDomNode node = List.at(0);
        QDomNode oldLevelnode=node.firstChild();
        node.firstChild().setNodeValue(QString::number(st_GameParam.trainingMode));
        QDomNode newLevelNode = node.firstChild();
        node.replaceChild(newLevelNode,oldLevelnode);
    }
    if(1)
    {
        QDomNodeList List = root.elementsByTagName("ReadPosTable");
        QDomNode node = List.at(0);
        QDomNode oldLevelnode=node.firstChild();
        QString ok("false");
        if(st_GameParam.readPosTable)
            ok = "true";
        else
            ok = "false";
        node.firstChild().setNodeValue(ok);
        QDomNode newLevelNode = node.firstChild();
        node.replaceChild(newLevelNode,oldLevelnode);
    }
    if(1)
    {
        QDomNodeList List = root.elementsByTagName("TrainingTrackPath");
        QDomNode node = List.at(0);
        QDomNode oldLevelnode=node.firstChild();
        node.firstChild().setNodeValue(st_GameParam.trainingTrack);
        QDomNode newLevelNode = node.firstChild();
        node.replaceChild(newLevelNode,oldLevelnode);
    }

    /*
    QDomNode n = root.firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement();
        if(!e.isNull())
        {
            if(e.tagName() == "TrainingTrack")
            {
                e.setAttribute("path",st_GameParam.trainingTrack);
            }
        }
        n = n.nextSibling();
    }
    */

    if(!file.open(QFile::WriteOnly|QFile::Truncate))
        return;
    //输出到文件
    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    file.close();
}

void GameParamControl::addXml()
{

}

void GameParamControl::removeXml()
{

}

void GameParamControl::setGamParam(ST_GameParam& st_GameParam)
{
    /*
    //如果是被动游戏则根据难度改变范围
    if(st_GameParam.trainingMode == E_TRAINMODE_PASSIVE)
    {
        //缩放比例0.8  1.0  1.2
        float scale = (st_GameParam.hardLevel-2)*2/10.f +1;
        //原始文件位置
        QFile srcFile(st_GameParam.trainingTrack);
        //目标位置为default文件夹之外
        QString destFileName = st_GameParam.trainingTrack.remove("/default");

        QFile destFile(destFileName);

        if(srcFile.open(QIODevice::ReadOnly) && destFile.open(QIODevice::WriteOnly))
        {
            QTextStream cin(&srcFile);
            QTextStream cout(&destFile);
            QString title = "ID,posX,posY\n";
            cout << title.toUtf8();
            QStringList list;
            int lineIndex = 0;
            while(!cin.atEnd())
            {
                QString lineStr = cin.readLine();
                list = lineStr.split(",",QString::SkipEmptyParts);
                if(lineIndex > 0)
                {
                    QPointF tempPoint;
                    QPointF srcPoint;
                    QPointF destPoint;
                    //原数据
                    srcPoint.setX(list.at(1).toFloat());
                    srcPoint.setY(list.at(2).toFloat());
                    //坐标系变换，将原点从左下角变换至中心点
                    tempPoint.setX(srcPoint.x() - 900);
                    tempPoint.setY(srcPoint.y() - 450);
                    //坐标缩放
                    tempPoint.setX(tempPoint.x()*scale);
                    tempPoint.setY(tempPoint.y()*scale);
                    //再将坐标原点变换至左下角
                    destPoint.setX(tempPoint.x() + 900);
                    destPoint.setY(tempPoint.y() + 450);

                    QString pointStr = QString::number(lineIndex) + "," + QString::number(destPoint.x()) + "," + QString::number(destPoint.y()) + "\n";
                    cout<<pointStr.toUtf8();
                }
                lineIndex++;
            }
        }
        else
            qDebug()<<"原始csv文件打开失败";

        srcFile.close();
        destFile.close();
    }
    */

    updateXml(st_GameParam);
}

QList<ST_GameMsg> GameParamControl::getGameMsgs()
{
    QList<ST_GameMsg> gameMsgList;
    QFile file("./configFile/gameListConfig.xml");

    if(!file.open(QIODevice::ReadOnly))
    {
        logger()->debug("read gameListConfig failed");
        return gameMsgList;
    }
    QDomDocument doc;
    QString error;
    int row,colum;
    if(!doc.setContent(&file,&error,&row,&colum))
    {
        logger()->debug(error);
        file.close();
        return gameMsgList;
    }
    file.close();

    QDomElement root=doc.documentElement(); //返回根节点

    QDomNode node=root.firstChild(); //获得第一个子节点

    while(!node.isNull())
    {
        if(node.isElement()) //如果节点是元素
        {
            ST_GameMsg st_GameMsg;
            QDomElement e=node.toElement();
            st_GameMsg.gameID = e.attribute("ID").toInt();
            st_GameMsg.gameName = e.attribute("name");
            st_GameMsg.gamePath = e.attribute("path");
            st_GameMsg.iconPath = e.attribute("iconPath");
            QString suitType = e.attribute("suitType");
            st_GameMsg.suitTypeList = suitType.split(',');
            gameMsgList.append(st_GameMsg);
            m_mapGameName.insert(st_GameMsg.gameName,st_GameMsg);
        }
        node = node.nextSibling();
    }
    return gameMsgList;
}

ST_GameMsg GameParamControl::getGameMsgByName(QString name)
{
    ST_GameMsg st_gameMsg;

    if(m_mapGameName.contains(name))
    {
        st_gameMsg = m_mapGameName.value(name);
    }
    return st_gameMsg;
}
