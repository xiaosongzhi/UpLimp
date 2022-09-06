#ifndef CLISTWIDGETITEM_H
#define CLISTWIDGETITEM_H

#include <QWidget>

namespace Ui {
class CListWidgetItem;
}

class CListWidgetItem : public QWidget
{
    Q_OBJECT

public:
    explicit CListWidgetItem(QWidget *parent = nullptr);
    CListWidgetItem(QString text,QPoint point);
    ~CListWidgetItem();

public slots:


    void setPointText(QPoint point);

private:
    Ui::CListWidgetItem *ui;
    QString m_text;
    QPoint m_point;
};

#endif // CLISTWIDGETITEM_H
