#include "gamepaintdevice.h"
#include <QPainter>
#include <QDebug>
#include <QtMath>
#include <QRandomGenerator>

GamePaintDevice::GamePaintDevice(QWidget *parent) :
    QWidget(parent),
    m_currentPoint(10,10),
    m_destPoint(100,200),
    isClear(false)
{

}

void GamePaintDevice::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    QPen pen(Qt::red, 3, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawRect(rect());

    QPen pen1;
    pen1.setBrush(QBrush(Qt::red));
    painter.setPen(pen1);
    painter.setBrush(QBrush(Qt::red));
    painter.drawEllipse(m_destPoint,10,10);
    QPixmap pix("./source/+.png");
    painter.drawPixmap(m_currentPoint,pix);
    if(isClear)
    {
        painter.fillRect(rect(),Qt::white);

        isClear = false;
    }

}

void GamePaintDevice::setPoint(QPoint point)
{
    float x_percent = point.x()/630.f;
    float y_percent = point.y()/450.f;

    m_currentPoint.setX(m_currentRect.width()*x_percent);
    m_currentPoint.setY(m_currentRect.height()*(1-y_percent));

    if(qSqrt(qPow(m_currentPoint.x()-m_destPoint.x(),2) + qPow(m_currentPoint.y()-m_destPoint.y(),2)) < 20)
    {
        quint32 value = QRandomGenerator::global()->generate();
        m_destPoint.setX(value%m_currentRect.width());
        m_destPoint.setY(value%m_currentRect.height());
    }
    update();
}

void GamePaintDevice::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    m_currentRect = rect();

}

void GamePaintDevice::clearPaintDevice()
{
    isClear = true;
    update();
}
