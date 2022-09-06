#include "teachpaint.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include "cgraphicsscene.h"
#include <QGraphicsView>
#include <QGridLayout>
#include <QDebug>
#include <QResizeEvent>
#include "link.h"
#include "node.h"
#include "cdatabaseinterface.h"
#include <QMessageBox>
#include <QThread>
TeachPaint::TeachPaint(QWidget *parent) :
    QWidget(parent),
    nodePoint(10,10),
    seqNumber(0),
    minZ(0),
    maxZ(0)
{
    m_scene = new CGraphicsScene(0,0,rect().width(),rect().height());
    m_view = new QGraphicsView();
    m_view->setScene(m_scene);
    m_view->setDragMode(QGraphicsView::RubberBandDrag);
    m_view->setRenderHints(QPainter::Antialiasing
                           | QPainter::TextAntialiasing);
    m_view->setContextMenuPolicy(Qt::ActionsContextMenu);
    QLayout *layout = new QGridLayout();
    layout->setMargin(0);
    layout->addWidget(m_view);
    setLayout(layout);

    connect(m_scene,&CGraphicsScene::signalSendDestPos,this,&TeachPaint::slotReceiveDestPos);
    connect(m_scene,&CGraphicsScene::signalFinishedEdit,this,&TeachPaint::slotFinishedEdit);
}

//接收来自场景中待添加点的位置
void TeachPaint::slotReceiveDestPos(QPointF destPoint)
{
    nodePoint = destPoint;
    //由场景坐标转换成视图坐标
    viewCoordinate = m_view->mapFromScene(destPoint);
    //将视图坐标转换成物理坐标
    physicalCoordinate = viewToPhysical(viewCoordinate);
    //添加点
    addNode();
}

void TeachPaint::addNode()
{
    Node *node = new Node;
    node->setText(tr("%1").arg(seqNumber + 1));
    //此处需要将场景坐标转换成物理坐标
    emit signalAddNewPoint(node->text(),sceneToPhysical(nodePoint.toPoint()));
    connect(node,SIGNAL(signalPositionChanged(QString,QPointF)),this,SLOT(slotNodeChanged(QString,QPointF)));
    setupNode(node);
}

void TeachPaint::slotNodeChanged(QString text,QPointF point)
{
    //此处将场景坐标转化成了物理坐标
    emit signalNodeChanged(text,sceneToPhysical(point.toPoint()));
}

//接收场景中结束编辑的信号
void TeachPaint::slotFinishedEdit()
{
    emit signalAddNewNodeFinished();
}

//是否开始添加
void TeachPaint::startAdd(bool isAdding)
{
    seqNumber = 0;
    minZ = 0;
    maxZ = 0;
    m_scene->setEditState(isAdding);
}

void TeachPaint::setupNode(Node *node)
{
    node->setPos(nodePoint);
    m_scene->addItem(node);
    ++seqNumber;

    m_scene->clearSelection();//清除当前选中的选项
    node->setSelected(true);
    bringToFront();//先将最后一个点放在最上层

    QList<QGraphicsItem *> items = m_scene->items();
    //取出倒数第二个item,使其处于选中状态
    if(items.size() >= 2)
    {
        int nodeIndex = 1;//Items是属于从头部插入型的，所以当数量大于2时，倒数第二个插入的下标为1
        Node *penultNode = dynamic_cast<Node *>(items.at(nodeIndex));
        penultNode->setSelected(true);
        addLink();
    }
}

void TeachPaint::bringToFront()
{
    ++maxZ;
    setZValue(maxZ);
}

void TeachPaint::setZValue(int z)
{
    Node *node = selectedNode();
    if (node)
        node->setZValue(z);
}

void TeachPaint::addLink()
{
    NodePair nodes = selectedNodePair();
    if (nodes == NodePair())
        return;

    Link *link = new Link(nodes.first, nodes.second);
    m_scene->addItem(link);
}

Node *TeachPaint::selectedNode() const
{
    QList<QGraphicsItem *> items = m_scene->selectedItems();
    if (items.count() == 1) {
        return dynamic_cast<Node *>(items.first());
    } else {
        return 0;
    }
}
NodePair TeachPaint::selectedNodePair() const
{
    QList<QGraphicsItem *> items = m_scene->selectedItems();
    if (items.count() == 2) {
        Node *first = dynamic_cast<Node *>(items.first());
        Node *second = dynamic_cast<Node *>(items.last());
        if (first && second)
            return NodePair(first, second);
    }
    return NodePair();
}

QPoint TeachPaint::viewToPhysical(QPoint viewPoint)
{
    //将原点从左上角移动到左下角
    int tempY = height()-viewPoint.y();
    //将视图转换成物理
    QPoint tempPoint;
    tempPoint.setX(viewPoint.x()/xCoefficient);
    tempPoint.setY(tempY/yCoefficient);
    return tempPoint;
}
QPoint TeachPaint::physicalToView(QPoint phyPoint)
{
    QPoint tempPoint;
    tempPoint.setX(phyPoint.x()*xCoefficient);
    tempPoint.setY(height()-phyPoint.y()*yCoefficient);
    return tempPoint;
}
//由场景坐标直接转到物理坐标
QPoint TeachPaint::sceneToPhysical(QPointF scenePoint)
{
    return viewToPhysical(m_view->mapFromScene(scenePoint));
}

//由场景坐标转换成游戏坐标
QPoint TeachPaint::sceneToGame(QPointF scenePoint)
{
    QPoint tempPoint = sceneToPhysical(scenePoint);
    QPoint gamePoint;
    gamePoint.setX((float)(tempPoint.x()*1800)/DEVICEWIDTH);
    gamePoint.setY((float)(tempPoint.y()*900)/DEVICEHEIGHT);
    return gamePoint;
}

void TeachPaint::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    //根据大小实时调整
    //xCoefficient(1.36507)  //860px--630mm
    //yCoefficient(1.46667)   //660px--450mm
    xCoefficient = ((float)width())/DEVICEWIDTH;
    yCoefficient = ((float)height())/DEVICEHEIGHT;
}

//清除画板
void TeachPaint::clearPaintDevice()
{
    seqNumber = 0;
    minZ = 0;
    maxZ = 0;
    QList<QGraphicsItem *> items = m_scene->items();

    QMutableListIterator<QGraphicsItem *> i(items);
    while (i.hasNext()) {
        Link *link = dynamic_cast<Link *>(i.next());

        if (link) {
            delete link;
            i.remove();
        }
    }
    qDeleteAll(items);
}
//保存轨迹
bool TeachPaint::saveTrackData(QString recipeName,QString author)
{
    QString recipePath("./recipe/");
#ifdef ABSOLUTEPATH
    recipePath = QApplication::applicationDirPath() + "/recipe/";
#endif
    QDir recipeDir(recipePath);
    if(!recipeDir.exists())
        recipeDir.mkdir(recipePath);
    //数据保存位置
    QDir trackDataDir(recipePath+"data/");
    if(!trackDataDir.exists())
        trackDataDir.mkdir(recipePath+"data/");
    //截图保存位置
    QDir trackImageDir(recipePath+"image/");
    if(!trackImageDir.exists())
        trackImageDir.mkdir(recipePath+"image/");
    //游戏数据保存位置
    QDir gameTrackDataDir(recipePath+"gameData/default/");
    if(!gameTrackDataDir.exists())
        gameTrackDataDir.mkdir(recipePath+"gameData/default/");

    //处方图片保存位置
    QPixmap pixmap = m_view->grab();
    QString recipeImgSaveName = trackImageDir.absolutePath()+"/"+recipeName+".png";
    if(!pixmap.save(recipeImgSaveName))
    {
        qDebug()<<"pixmap save failed";
        return false;
    }
    //处方数据保存位置
    QString recipeDataSaveName = trackDataDir.absolutePath()+"/"+recipeName+".txt";
    QFile file(recipeDataSaveName);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        QList<QGraphicsItem *> items = m_scene->items();
        //通过type进行区分Node和Link的类型
        //此处得到的都是场景坐标list
        for(int i = 0;i < items.size();i++)
        {
            //Node 节点Item
            if(20 == items.at(i)->type())
            {
                //item获取的坐标为场景坐标
                QPointF tempPoint = items.at(i)->pos();
                out<<"["<<tempPoint.x()<<","<<tempPoint.y()<<"]"<<endl;
            }
        }
    }
    else
    {
        qDebug()<<"文件打开失败";
        return false;
    }
    file.close();

    //游戏数据保存位置
    QString gameDataSaveName = gameTrackDataDir.absolutePath()+"/"+recipeName+".csv";

    QFile gameFile(gameDataSaveName);

    if(gameFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&gameFile);

        QTextStream csvPut(&gameFile);
        QString title = "ID,posX,posY\n";
        csvPut << title.toUtf8();

        QList<QGraphicsItem *> items = m_scene->items();
        //通过type进行区分Node和Link的类型
        //此处得到的都是场景坐标list
        int index = 1;
        for(int i = items.size()-1;i > -1;i--)
        {
            //Node 节点Item
            if(20 == items.at(i)->type())
            {   //item获取的坐标为场景坐标，坐标中心为笛卡尔指教坐标系，所以出现的坐标有负数
                //此处要将场景坐标进行转换成游戏坐标
                QPoint tempPoint = sceneToGame(items.at(i)->pos());
                QString pointStr = QString::number(index) + "," + QString::number(tempPoint.x()) + "," + QString::number(tempPoint.y()) + "\n";
                out<<pointStr.toUtf8();
                index++;
            }
        }
    }
    else
    {
        qDebug()<<"csv文件打开失败";
        return false;
    }
    gameFile.close();

    ST_TrackMsg st_TrackMsg;
    st_TrackMsg.author =  author;
    st_TrackMsg.trackDataFile = recipeDataSaveName;
    st_TrackMsg.trackImagePath = recipeImgSaveName;
    st_TrackMsg.trackName = recipeName;
    st_TrackMsg.gameTrackDataFile = gameDataSaveName;
    st_TrackMsg.createTime = QDateTime::currentDateTime();
    if(addRecipeToDatabase(st_TrackMsg))
    {
        QMessageBox::warning(NULL,"提示","处方添加成功",QMessageBox::Ok);
        return true;
    }
    return false;

}
//更新轨迹
bool TeachPaint::updateTrackData(QString recipeName,QString author)
{
    Q_UNUSED(author)
    //从数据库中查到存放本地文件的路径
    QString query = QString("select * from tracktable where trackName = '%1'").arg(recipeName);
    CDatabaseInterface::getInstance()->exec(query);
    int resultSize = CDatabaseInterface::getInstance()->getValuesSize();
    if(resultSize > 0)
    {
        //更新本地文件
        //获取本地文件的路径
        QList<QVariantMap> listMap = CDatabaseInterface::getInstance()->getValues(0,1);
        QString pixPath = listMap.at(0).value("trackImagePath").toString();
        QString filePath = listMap.at(0).value("trackDataFile").toString();
        QString gameFilePath = listMap.at(0).value("gameTrackDataFile").toString();

        QPixmap pixmap = m_view->grab();
        //处方图片保存位置
        if(!pixmap.save(pixPath))
        {
            qDebug()<<"save failed";
        }
        //处方数据保存位置
        QFile file(filePath);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            QList<QGraphicsItem *> items = m_scene->items();
            //通过type进行区分Node和Link的类型
            //此处得到的都是场景坐标list
            for(int i = 0;i < items.size();i++)
            {
                //Node 节点Item
                if(20 == items.at(i)->type())
                {
                    out<<"["<<items.at(i)->pos().x()<<","<<items.at(i)->pos().y()<<"]"<<endl;
                }
            }
        }
        file.close();
        //游戏坐标数据保存位置
        QFile gamefile(gameFilePath);
        if(gamefile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&gamefile);
            QList<QGraphicsItem *> items = m_scene->items();
            //通过type进行区分Node和Link的类型
            //此处得到的都是场景坐标list
            for(int i = 0;i < items.size();i++)
            {
                //Node 节点Item
                if(20 == items.at(i)->type())
                {
                    //item获取的坐标为场景坐标，坐标中心为笛卡尔指教坐标系，所以出现的坐标有负数
                    //此处要将场景坐标进行转换成游戏坐标
                    QPoint tempPoint = sceneToGame(items.at(i)->pos());
                    out<<"["<<tempPoint.x()<<","<<tempPoint.y()<<"]"<<endl;
                }
            }
        }
        gamefile.close();
        return true;
    }
    else
        QMessageBox::warning(NULL,"WARNING","数据库中没有该数据",QMessageBox::Ok);
    return false;
}
//删除轨迹
bool TeachPaint::deleteTrackData(QString recipeName,QString author)
{
    //查询数据库
    Q_UNUSED(author)
    //从数据库中查到存放本地文件的路径
    QString query = QString("select * from tracktable where trackName = '%1'").arg(recipeName);
    CDatabaseInterface::getInstance()->exec(query);
    int resultSize = CDatabaseInterface::getInstance()->getValuesSize();
    if(resultSize > 0)
    {
        //更新本地文件
        //获取本地文件的路径
        QList<QVariantMap> listMap = CDatabaseInterface::getInstance()->getValues(0,1);
        QString pixPath = listMap.at(0).value("trackImagePath").toString();
        QString filePath = listMap.at(0).value("trackDataFile").toString();
        QString gameFilePath = listMap.at(0).value("gameTrackDataFile").toString();

        //删除数据库
        if(!CDatabaseInterface::getInstance()->deleteRowTable("tracktable","trackName",recipeName))
        {
            qDebug()<<recipeName + "删除失败";
            return false;
        }
        //删除本地
        if(!pixPath.isEmpty())
        {
            bool ok = QFile::remove(pixPath);
            if(ok)
            {
                qDebug()<<pixPath + "删除成功";
            }
            else
                return false;
        }
        if(!filePath.isEmpty())
        {
            bool ok = QFile::remove(filePath);
            if(ok)
            {
                qDebug()<<filePath + "删除成功";
            }
            else
                return false;
        }
        if(!gameFilePath.isEmpty())
        {
            bool ok = QFile::remove(gameFilePath);
            if(ok)
            {
                qDebug()<<gameFilePath + "删除成功";
            }
            else
                return false;
        }

        return true;
    }
    else
    {
        qDebug()<<"未找到要删除的数据";
        return false;
    }
}

bool TeachPaint::addRecipeToDatabase(ST_TrackMsg st_TrackMsg)
{
    QVariantMap vMap = trackMsgToVariantMap(st_TrackMsg);
    //首先查询当前表获取个数

    QString query("select * from tracktable");
    CDatabaseInterface::getInstance()->exec(query);
    int count = CDatabaseInterface::getInstance()->getValuesSize();
    if(count == 0)
    {
        vMap.insert("recipeID",0);
    }
    QString queryId("select max(recipeID) from tracktable");
    CDatabaseInterface::getInstance()->exec(queryId);
    QList<QVariantMap> listMap = CDatabaseInterface::getInstance()->getValues(0,1);
    if(listMap.size() > 0)
    {
        int id = listMap.at(0).value("max(recipeID)").toInt();
        vMap.insert("recipeID",id+1);
    }

    QString tableName("tracktable");
    bool ok = CDatabaseInterface::getInstance()->insertRowTable(tableName,vMap);
    if(!ok)
    {
        qDebug()<<CDatabaseInterface::getInstance()->getLastError();
    }
    return ok;
}

void TeachPaint::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
}

//根据外部给的list点绘制图
void TeachPaint::paintPointList(QList<QPoint> list)
{
    startAdd(true);
#if 1
    //清理画板
    clearPaintDevice();
    //开启绘制

    for(int i = 0;i < list.size();i++)
    {
        nodePoint = list.at(i);

        addNode();
    }
    //添加结束操作
    m_scene->setEditState(false);
#endif

}
