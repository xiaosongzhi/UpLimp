#ifndef CBASEWIDGET_H
#define CBASEWIDGET_H

#include <QWidget>
#include "titleform.h"
QT_BEGIN_NAMESPACE
namespace Ui { class CBaseWidget; }
QT_END_NAMESPACE

enum WINDOWSIZE
{
  E_MAXWINDOW = 0,
  E_NORMALWINDOW
};

enum CURSEPOSITION
{
    LEFTTOP = 0,
    TOP,
    RIGHTTOP,
    LEFT,
    RIGHT,
    LEFTBOTTOM,
    BOTTOM,
    RIGHTBOTTOM
};

class CBaseWidget : public QWidget
{
    Q_OBJECT

public:
    CBaseWidget(QWidget *parent = nullptr);
    ~CBaseWidget();

protected:
    void paintEvent(QPaintEvent *event) override;

    bool eventFilter(QObject *obj, QEvent *ev) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
public slots:
    //移动窗体
    void slotWindowMove(QPoint);
    //窗口最大化和正常状态切换
    void slotSwitchWindowSize();
    //标题栏中的按钮事件
    void slotButtonEvent(E_BUTTONEVENT);
private:
    void installEventFilter();
    //设置鼠标的形状
    void setMouseCurse(QObject *obj);
private slots:
    void on_close_Btn_clicked();

private:
    Ui::CBaseWidget *ui;
    bool m_isMousePressed;
    CURSEPOSITION E_cursePosition;
    QPoint m_lastGlobalPoint;
    QPoint m_eventPoint;
    QRect m_originalRect;
};
#endif // CBASEWIDGET_H
