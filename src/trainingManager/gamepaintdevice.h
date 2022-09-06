#ifndef GAMEPAINTDEVICE_H
#define GAMEPAINTDEVICE_H

#include <QWidget>

class GamePaintDevice : public QWidget
{
    Q_OBJECT
public:
    explicit GamePaintDevice(QWidget *parent = nullptr);

    void setPoint(QPoint);

    void clearPaintDevice();
signals:
protected:
    void paintEvent(QPaintEvent *event);

    void resizeEvent(QResizeEvent *event);
private:
    QPoint m_currentPoint;
    QPoint m_destPoint;
    QRect m_currentRect;
    bool isClear;
};

#endif // GAMEPAINTDEVICE_H
