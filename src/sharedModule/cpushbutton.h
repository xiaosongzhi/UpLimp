#ifndef CPUSHBUTTON_H
#define CPUSHBUTTON_H

#include <QPushButton>
#include <QWidget>
#include <QRect>
class QLabel;
class CPushButton : public QPushButton
{
    Q_OBJECT
public:
//    CPushButton();
    CPushButton(const QString& iconPath,const QString& upper,const QString& lower);
    ~CPushButton();

    QString getId();
    QString getName();

protected:
//    void paintEvent(QPaintEvent* event);

private:
    QString m_upper;
    QString m_lower;
    QPixmap* m_icon;
#if 0
    //使用painter自己绘制
    QRect m_IconRect;
    QRect m_upperTextRect;
    QRect m_lowerTextRect;
}
#endif
    QLabel *pixLabel;
    QLabel *upperTextLabel; //姓名
    QLabel *lowerTextLabel; //ID

};

#endif // CPUSHBUTTON_H
