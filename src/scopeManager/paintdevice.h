#ifndef PAINTDEVICE_H
#define PAINTDEVICE_H

#include <QWidget>
//#include <QPaintEvent>
class PaintDevice : public QWidget
{
    Q_OBJECT
public:
    explicit PaintDevice(QWidget *parent = nullptr);

    void addPoint(QPoint);

    void setPointList(QList<QPoint>&);

    void clearPath();

    float getArea();

    QList<QPoint> getPaintList();
signals:

protected:
    void paintEvent(QPaintEvent *event);

private:
    float getPaintArea(QList<QPoint>list);
    float getDestArea(QList<QPoint>);
    QList<QPoint> getLowerPointList(QList<QPoint>);
private:
    QList<QPoint> pathList;
    QList<QPoint> realPointList;
};

#endif // PAINTDEVICE_H
