#include "paintdevice.h"
#include <QPainter>
#include <QPen>
#include <QDebug>
#include "dataFormate.h"

PaintDevice::PaintDevice(QWidget *parent) : QWidget(parent)
{

}

void PaintDevice::setPointList(QList<QPoint> &pointList)
{
    //先进行坐标转换
    for(int i = 0;i <pointList.size();i++)
    {
        pathList.append(QPoint(pointList.at(i).x(),height()-pointList.at(i).y()));
    }
    update();
}

void PaintDevice::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.eraseRect(0,0,width(),height());
    painter.fillRect(0,0,width(),height(),Qt::white);
    QPen pen;

    pen.setWidth(3);
    pen.setColor("#FE9C3B");
    painter.setPen(pen);

    QPainterPath path;
    if(pathList.isEmpty())
        return;
    path.moveTo(pathList.first());
    for(int i = 1;i < pathList.size();i++)
    {
        path.lineTo(pathList.at(i));
    }
    painter.drawPath(path);
    QPointF point;
    point.setX(pathList.last().x()-20);
    point.setY(pathList.last().y() -16);
    QRectF target(point,QSize(40,32));
    QRectF source(0,0,40,32);
    QPixmap pixmap("./source/hand.png");
    painter.drawPixmap(target,pixmap,source);
}

float PaintDevice::getPaintArea(QList<QPoint>list)
{
    if(list.isEmpty())
        return 0;
    QPoint leftPoint = list.first();
    float area = 0;
    for(int i = 1;i < list.size();i++)
    {
        QPoint rightPoint = list.at(i);
        //矩形面积
        int l = qAbs(rightPoint.x()-leftPoint.x());
        area += (l*(rightPoint.y() > leftPoint.y() ? leftPoint.y():rightPoint.y()));
        //三角形的面积
        area += (0.5*l*qAbs(rightPoint.y()-leftPoint.y()));
    }
    return area;
}

void PaintDevice::addPoint(QPoint point)
{
    //将物理坐标转换成画布坐标
    realPointList.append(point);
    QPoint tempPoint;
    tempPoint.setX((float)point.x()/DEVICEWIDTH * (width()));
    tempPoint.setY((float)(DEVICEHEIGHT- point.y())/DEVICEHEIGHT *height());
    pathList.append(tempPoint);
    update();
}

float PaintDevice::getDestArea(QList<QPoint> list)
{
    QList<QPoint> upperList;
    QList<QPoint> lowerList;
    QPoint leftPoint,rightPoint;
    int leftIndex = 200,rightIndex = 0;
    int left = 200,right = 0;
    //先找到两侧的值
    for(int i = 0;i < list.size();i++)
    {
        int temp = list.at(i).x();
        if(left > temp)
            left = temp;
        if(right < temp)
            right = temp;
    }
    for(int i = 0;i <list.size();i++)
    {
        if(list.at(i).x() == left)
        {
            leftIndex = i;
        }
        if(list.at(i).x() == right)
        {
            rightIndex = i;
        }
    }
    if(leftIndex > rightIndex)
    {
        int temp = leftIndex;
        leftIndex = rightIndex;
        rightIndex = temp;
    }
    //上半部分曲线
    QList<QPoint> preList;
    for(int i = 0;i < list.size();i++)
    {
        if(i <= leftIndex)
            preList.append(list.at(i));

        if(i >= leftIndex && i <= rightIndex)
        {
            upperList.append(list.at(i));
        }
        if(i >= rightIndex)
            lowerList.append(list.at(i));
    }
    lowerList.append(preList);

    float area1 = getPaintArea(lowerList);
    float area2 = getPaintArea(upperList);

    return qAbs(area1-area2);
}

void PaintDevice::clearPath()
{
    realPointList.clear();
    pathList.clear();
    update();
}

float PaintDevice::getArea()
{
    return getDestArea(realPointList);
}

QList<QPoint> PaintDevice::getPaintList()
{
    return realPointList;
}
