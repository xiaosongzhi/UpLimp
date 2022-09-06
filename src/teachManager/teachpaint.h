#ifndef TEACHPAINT_H
#define TEACHPAINT_H

#include <QWidget>
#include "userdataformate.h"

class CGraphicsScene;
class QGraphicsView;
class Node;
class Link;
typedef QPair<Node *, Node *> NodePair;
class TeachPaint : public QWidget
{
    Q_OBJECT
public:
    explicit TeachPaint(QWidget *parent = nullptr);

    void addNode();
    //是否开始添加
    void startAdd(bool isAdding);
    //清除画板
    void clearPaintDevice();
    //保存轨迹
    bool saveTrackData(QString recipeName,QString author);
    //更新轨迹
    bool updateTrackData(QString recipeName,QString author);
    //删除轨迹
    bool deleteTrackData(QString recipeName,QString author);
    //根据外部给的list点绘制图
    void paintPointList(QList<QPoint>);
protected:

    virtual void
    resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event);

signals:
    //添加完成
    void signalAddFinished();
    //告知teachedit添加了新的点
    void signalAddNewPoint(QString index,QPoint point);
    //在编辑拖拽时，左侧的位置始终显示右侧点的位置
    void signalNodeChanged(QString text,QPoint point);
    //新点添加结束
    void signalAddNewNodeFinished();
public slots:
    //接收来自场景中待添加点的位置
    void slotReceiveDestPos(QPointF);
    //接收场景中结束编辑的信号
    void slotFinishedEdit();
    //Item发生位置变化
    void slotNodeChanged(QString text,QPointF point);
private:
//    void addNode();
    //视图和物理坐标转换
    QPoint viewToPhysical(QPoint);
    QPoint physicalToView(QPoint);
    QPoint sceneToPhysical(QPointF);
    QPoint sceneToGame(QPointF);

    void setupNode(Node *node);
    void bringToFront();
    void setZValue(int z);
    void addLink();
    Node *selectedNode() const;
    Link *selectedLink() const;
    NodePair selectedNodePair() const;
    //将处方添加到数据库
    bool addRecipeToDatabase(ST_TrackMsg);
private:
    CGraphicsScene *m_scene;
    QGraphicsView *m_view;
    QPoint viewCoordinate;      //视图坐标
    QPoint physicalCoordinate;  //物理坐标
    //视图坐标与物理坐标转换的系数
    double xCoefficient,yCoefficient;

    QPointF nodePoint;
    int seqNumber;//节点的序号
    int minZ;
    int maxZ;
};

#endif // TEACHPAINT_H
